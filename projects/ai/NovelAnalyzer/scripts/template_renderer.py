#!/usr/bin/env python3
"""
Novel Buds — Training Dataset Renderer

Renders Jinja2 scenario templates against every refined novel project in
./data/refined_novel/ and emits a JSONL training dataset.

Usage:
    python render_dataset.py \
        --templates ./templates \
        --projects ./data/refined_novel \
        --tools ./tools.json \
        --output ./data/dataset

Each template is rendered once per project. Empty/whitespace-only renders are
treated as "skip this project for this template" (used for precondition guards).
Each successful render is validated against the expected sample schema and
written to the output JSONL on its own line.
"""

from __future__ import annotations

import argparse
import json
import logging
import random
import re
import sys
import uuid
from dataclasses import dataclass, field

import numpy as np
from datetime import datetime, timezone
from pathlib import Path
from typing import Any

from jinja2 import (
    Environment,
    FileSystemLoader,
    StrictUndefined,
    TemplateError,
    select_autoescape,
)

# ---------------------------------------------------------------------------
# Logging
# ---------------------------------------------------------------------------

log = logging.getLogger("novel_buds.renderer")


def setup_logging(verbose: bool) -> None:
    level = logging.DEBUG if verbose else logging.INFO
    logging.basicConfig(
        level=level,
        format="%(asctime)s [%(levelname)s] %(name)s: %(message)s",
        datefmt="%H:%M:%S",
    )


# ---------------------------------------------------------------------------
# Stats
# ---------------------------------------------------------------------------


@dataclass
class RenderStats:
    templates_seen: int = 0
    projects_seen: int = 0
    attempts: int = 0
    skipped_empty: int = 0
    failed_render: int = 0
    failed_validation: int = 0
    succeeded: int = 0
    per_template: dict[str, dict[str, int]] = field(default_factory=dict)
    per_category: dict[str, int] = field(default_factory=dict)

    def bump_template(self, template_name: str, key: str) -> None:
        bucket = self.per_template.setdefault(
            template_name,
            {"attempts": 0, "succeeded": 0, "skipped": 0, "failed": 0},
        )
        bucket[key] = bucket.get(key, 0) + 1

    def bump_category(self, category: str) -> None:
        self.per_category[category] = self.per_category.get(category, 0) + 1

    def report(self) -> str:
        lines = [
            "=" * 64,
            "Renderer summary",
            "=" * 64,
            f"  Templates seen     : {self.templates_seen}",
            f"  Projects seen      : {self.projects_seen}",
            f"  Total attempts     : {self.attempts}",
            f"  Succeeded          : {self.succeeded}",
            f"  Skipped (empty)    : {self.skipped_empty}",
            f"  Failed (render)    : {self.failed_render}",
            f"  Failed (validate)  : {self.failed_validation}",
            "",
            "Per category:",
        ]
        for cat, count in sorted(self.per_category.items()):
            lines.append(f"  {cat:>4} : {count}")
        lines.append("")
        lines.append("Per template:")
        for tmpl, b in sorted(self.per_template.items()):
            lines.append(
                f"  {tmpl}: attempts={b['attempts']} ok={b['succeeded']} "
                f"skipped={b['skipped']} failed={b['failed']}"
            )
        return "\n".join(lines)


# ---------------------------------------------------------------------------
# Tool catalog
# ---------------------------------------------------------------------------


class ToolCatalog:
    """
    Holds the canonical JSON Schema definitions for every Novel Buds tool.

    The catalog file may be either:

    1. A JSON object mapping tool name to schema:
           { "create_story_entity": { ... }, ... }

    2. A JSON array of tool schema objects:
           [ { "name": "create_story_entity", ... }, ... ]

    Both forms are accepted. The array form matches the upstream Novel Buds
    tool definitions file format, so it can be used as-is without conversion.

    On load, the catalog automatically:

    - Expands the placeholder entry `call_{agent_name}` into one concrete
      tool per default sub-agent (see DEFAULT_SUB_AGENTS below). Templates
      should reference the concrete names like `call_character_planner`.

    Translation tools (`translate_*`, `patch_translation_*`) are kept in the
    catalog as distractors. Templates may include them in the `tools` array
    so the model sees them as available, but templates must NEVER actually
    call them — the validator enforces this. The point is to teach the model
    to ignore translation tools during this training run.
    """

    # The seven default sub-agents that ship with Novel Buds. Users can
    # enable, disable, or add their own per project, but these are the
    # canonical set the renderer expands the placeholder into.
    DEFAULT_SUB_AGENTS: list[dict[str, str]] = [
        {
            "agent_name": "character_planner",
            "display_name": "Character Planner",
            "description": (
                "Plan characters: motivations, arcs, relationships, voice, "
                "backstory, and role in the plot."
            ),
        },
        {
            "agent_name": "critic",
            "display_name": "Critic",
            "description": (
                "Reviews novel and lore content and points out problems: "
                "continuity errors, weak prose, inconsistent characterization, "
                "pacing issues."
            ),
        },
        {
            "agent_name": "lore_explorer",
            "display_name": "Lore Explorer",
            "description": (
                "Explore and retrieve existing lore, facts, and references "
                "from the project using read/search tools."
            ),
        },
        {
            "agent_name": "novel_planner",
            "display_name": "Novel Planner",
            "description": (
                "Plan scenes and chapters: goals, conflict, emotional beats, "
                "POV, hooks, and continuity."
            ),
        },
        {
            "agent_name": "novel_writer",
            "display_name": "Novel Writer",
            "description": (
                "Write and revise prose for chapters. Uses manuscript edit "
                "tools when appropriate."
            ),
        },
        {
            "agent_name": "outline_planner",
            "display_name": "Outline Planner",
            "description": (
                "Plan and analyze story structure: outlines, acts, chapters, "
                "beats, pacing, and missing links."
            ),
        },
        {
            "agent_name": "world_planner",
            "display_name": "World Planner",
            "description": (
                "Plan worldbuilding: setting rules, factions, cultures, "
                "technology/magic systems, and consistency checks."
            ),
        },
    ]

    SUB_AGENT_PLACEHOLDER_NAME = "call_{agent_name}"

    def __init__(self, path: Path):
        if not path.exists():
            raise FileNotFoundError(f"Tool catalog not found at {path}")
        with path.open("r", encoding="utf-8") as f:
            data = json.load(f)

        tools: dict[str, dict[str, Any]] = {}

        if isinstance(data, dict):
            # Already in the {name: schema} form.
            for name, schema in data.items():
                if not isinstance(schema, dict):
                    raise ValueError(
                        f"Catalog entry {name!r} must be an object"
                    )
                tools[name] = schema
        elif isinstance(data, list):
            # Array form: each element must have a "name" key.
            for i, schema in enumerate(data):
                if not isinstance(schema, dict):
                    raise ValueError(
                        f"Catalog entry at index {i} must be an object"
                    )
                name = schema.get("name")
                if not name:
                    raise ValueError(
                        f"Catalog entry at index {i} has no 'name' field"
                    )
                tools[name] = schema
        else:
            raise ValueError("Tool catalog must be a JSON object or array")

        # Expand the sub-agent placeholder, if present, into one concrete
        # tool per default sub-agent.
        if self.SUB_AGENT_PLACEHOLDER_NAME in tools:
            placeholder = tools.pop(self.SUB_AGENT_PLACEHOLDER_NAME)
            for agent in self.DEFAULT_SUB_AGENTS:
                tool_name = f"call_{agent['agent_name']}"
                expanded = json.loads(json.dumps(placeholder))  # deep copy
                expanded["name"] = tool_name
                expanded["description"] = (
                    f"Call Sub Agent \"{agent['display_name']}\". "
                    f"{agent['description']}"
                )
                tools[tool_name] = expanded
            log.info(
                "Expanded %s into %d concrete sub-agent tools",
                self.SUB_AGENT_PLACEHOLDER_NAME,
                len(self.DEFAULT_SUB_AGENTS),
            )

        self._tools: dict[str, dict[str, Any]] = tools
        log.info("Loaded %d tools from %s", len(self._tools), path)

    def get(self, name: str) -> dict[str, Any]:
        if name not in self._tools:
            raise KeyError(f"Unknown tool: {name!r}")
        return self._tools[name]

    def names(self) -> set[str]:
        return set(self._tools.keys())

    def validate_arguments(self, name: str, args: dict[str, Any]) -> list[str]:
        """Cheap, schema-light validation of a tool call's arguments object.

        Checks: required fields present, enum values valid, no unknown fields
        when additionalProperties is False. Does not enforce deep type checks.
        """
        errors: list[str] = []
        schema = self.get(name).get("parameters", {})
        props: dict[str, Any] = schema.get("properties", {}) or {}
        required: list[str] = schema.get("required", []) or []
        additional = schema.get("additionalProperties", True)

        for req in required:
            if req not in args:
                errors.append(f"missing required field {req!r}")

        if additional is False:
            for key in args:
                if key not in props:
                    errors.append(f"unexpected field {key!r}")

        for key, value in args.items():
            spec = props.get(key)
            if not spec:
                continue
            if "enum" in spec and value not in spec["enum"]:
                # Content placeholders are allowed in any string field.
                if isinstance(value, str) and _is_content_placeholder(value):
                    continue
                errors.append(
                    f"field {key!r}={value!r} not in enum {spec['enum']}"
                )
        return errors


# ---------------------------------------------------------------------------
# Project loading
# ---------------------------------------------------------------------------


def load_project(project_dir: Path) -> dict[str, Any] | None:
    """Load a single refined novel project from disk.

    Returns None if the directory is not a valid project.
    """
    project_json = project_dir / "project.json"
    if not project_json.exists():
        log.debug("Skipping %s — no project.json", project_dir.name)
        return None

    try:
        with project_json.open("r", encoding="utf-8") as f:
            project = json.load(f)
    except json.JSONDecodeError as e:
        log.warning("Project %s has invalid JSON: %s", project_dir.name, e)
        return None

    # Pre-load manuscript text into each chapter so templates can reference it
    manuscripts_dir = project_dir / "manuscripts"
    if manuscripts_dir.exists():
        for outline in project.get("outlines", []):
            for act in outline.get("acts", []):
                for chapter in act.get("chapters", []):
                    rel = chapter.get("manuscript")
                    if not rel:
                        continue
                    manuscript_path = project_dir / rel
                    if manuscript_path.exists():
                        try:
                            chapter["manuscript_text"] = manuscript_path.read_text(
                                encoding="utf-8"
                            )
                        except OSError as e:
                            log.warning(
                                "Could not read manuscript %s: %s", manuscript_path, e
                            )
                            chapter["manuscript_text"] = ""
                    else:
                        chapter["manuscript_text"] = ""

    # Stamp every entity with a stable id if missing.
    # In production these come from the database, but for refined corpus
    # files we generate deterministic ids per project so renders are stable.
    _stamp_ids(project, project_dir.name)

    project["_project_dir"] = str(project_dir)
    project["_project_slug"] = project_dir.name
    return project


def _stamp_ids(project: dict[str, Any], seed: str) -> None:
    """Assign deterministic UUIDs to entities lacking ids.

    Uses uuid5 with the project slug as a namespace seed so the same project
    always produces the same ids across renders. This makes training data
    reproducible.
    """
    namespace = uuid.uuid5(uuid.NAMESPACE_URL, f"novelbuds://{seed}")

    def mkid(scope: str, index: int) -> str:
        return str(uuid.uuid5(namespace, f"{scope}/{index}"))

    for i, ent in enumerate(project.get("story_entities", []) or []):
        ent.setdefault("id", mkid("story_entity", i))

    for i, outline in enumerate(project.get("outlines", []) or []):
        outline.setdefault("id", mkid(f"outline/{i}", 0))
        for j, act in enumerate(outline.get("acts", []) or []):
            act.setdefault("id", mkid(f"outline/{i}/act", j))
            for k, chapter in enumerate(act.get("chapters", []) or []):
                chapter.setdefault("id", mkid(f"outline/{i}/act/{j}/chapter", k))
                chapter.setdefault(
                    "manuscript_id", mkid(f"outline/{i}/act/{j}/manuscript", k)
                )

    timeline = project.get("timeline") or {}
    for i, track in enumerate(timeline.get("tracks", []) or []):
        track.setdefault("id", mkid("timeline_track", i))
        for j, event in enumerate(track.get("events", []) or []):
            event.setdefault("id", mkid(f"timeline_track/{i}/event", j))


# ---------------------------------------------------------------------------
# Jinja environment
# ---------------------------------------------------------------------------


def build_jinja_env(
    templates_dir: Path,
    catalog: ToolCatalog,
    embeddings_index: dict[str, Any] | None = None,
) -> Environment:
    env = Environment(
        loader=FileSystemLoader(str(templates_dir)),
        undefined=StrictUndefined,
        autoescape=select_autoescape(disabled_extensions=("j2",), default=False),
        trim_blocks=True,
        lstrip_blocks=True,
        keep_trailing_newline=False,
    )

    # ----- ID helpers -----------------------------------------------------

    def uuid4_helper() -> str:
        return str(uuid.uuid4())

    env.globals["uuid4"] = uuid4_helper

    # ----- Time helpers ---------------------------------------------------

    def now_iso() -> str:
        return datetime.now(timezone.utc).strftime("%Y-%m-%dT%H:%M:%SZ")

    def iso_to_date_dict(iso_string: str | None) -> dict[str, int] | None:
        if not iso_string:
            return None
        try:
            dt = datetime.fromisoformat(iso_string)
        except ValueError:
            log.warning("iso_to_date_dict: could not parse %r", iso_string)
            return None
        return {"year": dt.year, "month": dt.month, "day": dt.day}

    def date_dict_to_iso(d: dict[str, int] | None) -> str | None:
        if not d:
            return None
        year = int(d.get("year", 1))
        month = int(d.get("month", 1))
        day = int(d.get("day", 1))
        return f"{year:04d}-{month:02d}-{day:02d}T00:00:00"

    env.globals["now_iso"] = now_iso
    env.globals["iso_to_date_dict"] = iso_to_date_dict
    env.globals["date_dict_to_iso"] = date_dict_to_iso

    # ----- Tag normalization ---------------------------------------------

    _ws_re = re.compile(r"\s+")

    def normalize_tag(s: str | None) -> str:
        if s is None:
            return ""
        return _ws_re.sub(" ", str(s).strip().lower())

    def normalize_tags(items: list[str] | None) -> list[str]:
        if not items:
            return []
        seen: set[str] = set()
        out: list[str] = []
        for item in items:
            n = normalize_tag(item)
            if n and n not in seen:
                seen.add(n)
                out.append(n)
        return out

    env.globals["normalize_tag"] = normalize_tag
    env.globals["normalize_tags"] = normalize_tags
    env.filters["normalize_tag"] = normalize_tag
    env.filters["normalize_tags"] = normalize_tags

    # ----- Tool catalog helper -------------------------------------------

    def tool_helper(name: str) -> str:
        """Inject a tool's schema into the rendered output as a JSON literal.

        Returns a JSON string (not a Python dict) so it can be dropped
        directly into a JSON array context inside the template.
        """
        schema = catalog.get(name)
        return json.dumps(schema, ensure_ascii=False)

    env.globals["tool"] = tool_helper

    # ----- Tool list builder ----------------------------------------------

    ALL_TOOLS: list[str] = sorted(catalog.names())

    def build_tools(
        required: list[str],
        distractor_prob: float = 0.5,
    ) -> str:
        """Build a JSON array of tool schemas mixing required + random distractors.

        - ``required``: tool names that MUST appear (the ones actually called).
        - ``distractor_prob``: probability (0-1) that each non-required tool
          is included as a distractor.

        Returns a ready-to-embed JSON array string, e.g. ``[{...}, {...}]``.
        """
        names: set[str] = set(required)
        for name in ALL_TOOLS:
            if name not in names and random.random() < distractor_prob:
                names.add(name)
        ordered = list(required)  # required first, in given order
        ordered += sorted(names - set(required))
        schemas = [json.loads(tool_helper(n)) for n in ordered]
        return json.dumps(schemas, ensure_ascii=False)

    env.globals["build_tools"] = build_tools
    env.globals["ALL_TOOLS"] = ALL_TOOLS

    # ----- Text helpers for fabricating realistic tool results ---------------

    def find_snippet(text: str, term: str, radius: int = 60) -> str | None:
        """Find ``term`` in ``text`` and return a snippet with surrounding context.

        Returns None if ``term`` is not found (case-insensitive search).
        """
        if not text or not term:
            return None
        idx = text.lower().find(term.lower())
        if idx < 0:
            return None
        start = max(0, idx - radius)
        end = min(len(text), idx + len(term) + radius)
        prefix = "..." if start > 0 else ""
        suffix = "..." if end < len(text) else ""
        return prefix + text[start:end] + suffix

    def find_all_snippets(
        text: str, term: str, radius: int = 60, max_results: int = 5
    ) -> list[str]:
        """Find all occurrences of ``term`` in ``text`` and return snippets."""
        if not text or not term:
            return []
        lower_text = text.lower()
        lower_term = term.lower()
        results: list[str] = []
        start_pos = 0
        while len(results) < max_results:
            idx = lower_text.find(lower_term, start_pos)
            if idx < 0:
                break
            s = max(0, idx - radius)
            e = min(len(text), idx + len(lower_term) + radius)
            prefix = "..." if s > 0 else ""
            suffix = "..." if e < len(text) else ""
            results.append(prefix + text[s:e] + suffix)
            start_pos = idx + len(lower_term)
        return results

    def truncate(text: str, length: int = 300) -> str:
        """Truncate text to ``length`` characters, adding '...' if truncated."""
        if not text:
            return ""
        if len(text) <= length:
            return text
        return text[:length] + "..."

    def _keyword_search(
        project: dict[str, Any],
        queries: list[str],
        max_results: int = 5,
    ) -> list[dict[str, Any]]:
        """Fallback keyword-overlap search when embeddings are unavailable."""
        query_words = set()
        for q in queries:
            query_words.update(w.lower() for w in q.split() if len(w) > 2)

        candidates: list[tuple[float, dict[str, Any]]] = []

        for ent in project.get("story_entities", []):
            text = (ent.get("name", "") + " " +
                    ent.get("description", "") + " " +
                    ent.get("content", "")).lower()
            score = sum(1 for w in query_words if w in text) / max(len(query_words), 1)
            if score > 0:
                candidates.append((score, {
                    "type": "story_entity",
                    "id": ent.get("id", ""),
                    "name": ent.get("name", ""),
                    "kind": ent.get("kind", ""),
                    "excerpt": truncate(ent.get("description", ""), 150),
                    "score": round(score, 3),
                }))

        for outline in project.get("outlines", []):
            for act in outline.get("acts", []):
                for ch in act.get("chapters", []):
                    ms_text = ch.get("manuscript_text", "")
                    if not ms_text:
                        continue
                    text_lower = ms_text[:2000].lower()
                    score = sum(1 for w in query_words if w in text_lower) / max(len(query_words), 1)
                    if score > 0:
                        best_word = max(query_words, key=lambda w: text_lower.count(w))
                        snippet = find_snippet(ms_text, best_word, 80) or truncate(ms_text, 150)
                        candidates.append((score, {
                            "type": "manuscript",
                            "id": ch.get("manuscript_id", ""),
                            "name": ch.get("name", ""),
                            "location": ch.get("name", ""),
                            "excerpt": snippet,
                            "score": round(score, 3),
                        }))

        candidates.sort(key=lambda x: x[0], reverse=True)
        return [c[1] for c in candidates[:max_results]]

    def _vector_search(
        project: dict[str, Any],
        queries: list[str],
        max_results: int = 5,
    ) -> list[dict[str, Any]]:
        """Embedding-based semantic search using pre-built index."""
        slug = project.get("_project_slug", "")
        idx = embeddings_index.get(slug) if embeddings_index else None
        if idx is None:
            return _keyword_search(project, queries, max_results)

        vectors = idx["vectors"]     # (N, D) float16, pre-normalized
        meta = idx["meta"]           # list of dicts
        texts = idx["texts"]         # list of strings
        encode_fn = idx["encode_fn"]  # query encoder

        # Encode query
        query_text = " ".join(queries)
        query_vec = encode_fn(query_text)  # (D,) normalized

        # Cosine similarity = dot product (both normalized)
        sims = vectors.astype(np.float32) @ query_vec.astype(np.float32)

        # Top N indices
        top_k = min(max_results, len(sims))
        top_indices = np.argpartition(-sims, top_k)[:top_k]
        top_indices = top_indices[np.argsort(-sims[top_indices])]

        results = []
        for i in top_indices:
            score = float(sims[i])
            if score <= 0:
                continue
            m = meta[i]
            result = {
                "type": m.get("type", ""),
                "id": m.get("id", ""),
                "name": m.get("name", ""),
                "excerpt": truncate(texts[i], 150),
                "score": round(score, 3),
            }
            if "kind" in m and m["kind"]:
                result["kind"] = m["kind"]
            if "location" in m and m["location"]:
                result["location"] = m["location"]
            results.append(result)

        return results

    def semantic_search(
        project: dict[str, Any],
        queries: list[str],
        max_results: int = 5,
    ) -> list[dict[str, Any]]:
        """Semantic search with vector fallback to keyword."""
        if embeddings_index:
            return _vector_search(project, queries, max_results)
        return _keyword_search(project, queries, max_results)

    env.globals["semantic_search"] = semantic_search
    env.globals["find_snippet"] = find_snippet
    env.globals["find_all_snippets"] = find_all_snippets
    env.globals["truncate"] = truncate
    env.filters["truncate_text"] = truncate

    # ----- Misc convenience ----------------------------------------------

    env.globals["random_choice"] = random.choice
    env.globals["random_sample"] = random.sample
    env.globals["random_int"] = random.randint

    # tojson is built-in but we expose `to_json_string` as a clearer alias
    env.filters["to_json_string"] = lambda x: json.dumps(x, ensure_ascii=False)

    return env


# ---------------------------------------------------------------------------
# Sample validation
# ---------------------------------------------------------------------------

THOUGHT_BLOCK_RE = re.compile(
    r"^<\|channel>thought\n\[THINKING_PLACEHOLDER_(\d+)\]<channel\|>"
)
SYSTEM_THINK_RE = re.compile(r"^<\|think\|>\n")
VALID_ROLES = {"system", "user", "assistant", "tool"}

# Placeholder patterns that are allowed to appear in rendered output.
# These represent content that would be generated at inference time.
_CONTENT_PLACEHOLDER_RE = re.compile(
    r"\[GENERATED_CONTENT\]"
    r"|\[GENERATED_PROSE\]"
    r"|\[SUBAGENT_RESPONSE\]"
    r"|\[ASSISTANT_RESPONSE_\d+\]"
)


def _is_content_placeholder(value: str) -> bool:
    """Return True if *value* is (or contains) an allowed content placeholder."""
    return bool(isinstance(value, str) and _CONTENT_PLACEHOLDER_RE.search(value))


@dataclass
class ValidationResult:
    ok: bool
    errors: list[str] = field(default_factory=list)

    def fail(self, msg: str) -> None:
        self.ok = False
        self.errors.append(msg)


def validate_sample(
    sample: dict[str, Any],
    catalog: ToolCatalog,
    *,
    template_name: str,
) -> ValidationResult:
    """Strict structural validation of a rendered sample.

    Mirrors the rules described in the agent system prompt: Gemma 4 thinking
    format, tool-call ↔ tool-result pairing, schema-light tool argument check,
    UUID id consistency, and the multi-turn thinking placeholder convention.
    """
    result = ValidationResult(ok=True)

    # ----- Top-level shape -----------------------------------------------

    for key in ("scenario_id", "scenario_category", "system", "tools", "messages"):
        if key not in sample:
            result.fail(f"missing top-level key {key!r}")
    if not result.ok:
        return result

    if not isinstance(sample["scenario_id"], str) or not sample["scenario_id"]:
        result.fail("scenario_id must be a non-empty string")
    if not isinstance(sample["scenario_category"], str) or not sample[
        "scenario_category"
    ]:
        result.fail("scenario_category must be a non-empty string")

    # ----- System prompt --------------------------------------------------

    system = sample.get("system", "")
    if not isinstance(system, str):
        result.fail("system must be a string")
    elif not SYSTEM_THINK_RE.match(system):
        result.fail("system prompt must start with '<|think|>\\n'")

    # ----- Tools ----------------------------------------------------------

    tools = sample.get("tools")
    if not isinstance(tools, list):
        result.fail("tools must be an array")
        tools = []

    declared_tool_names: set[str] = set()
    for i, t in enumerate(tools):
        if not isinstance(t, dict):
            result.fail(f"tools[{i}] is not an object")
            continue
        tname = t.get("name") or (t.get("function") or {}).get("name")
        if not tname:
            result.fail(f"tools[{i}] has no name")
            continue
        if tname not in catalog.names():
            result.fail(f"tools[{i}] references unknown tool {tname!r}")
        # Translation tools ARE allowed in the declared tools list — they
        # act as distractors that the model must learn to ignore. They are
        # only blocked when actually called (see _validate_tool_call_conventions).
        declared_tool_names.add(tname)

    # ----- Messages -------------------------------------------------------

    messages = sample.get("messages")
    if not isinstance(messages, list) or not messages:
        result.fail("messages must be a non-empty array")
        return result

    assistant_turn_index = 0
    seen_placeholders: set[int] = set()
    pending_tool_calls: list[dict[str, Any]] = []
    fabricated_ids: dict[str, str] = {}  # id -> first context where seen

    for idx, msg in enumerate(messages):
        if not isinstance(msg, dict):
            result.fail(f"messages[{idx}] is not an object")
            continue

        role = msg.get("role")
        if role not in VALID_ROLES:
            result.fail(f"messages[{idx}].role={role!r} is invalid")
            continue

        # Pending tool calls must be answered before any non-tool message
        if pending_tool_calls and role != "tool":
            result.fail(
                f"messages[{idx}]: expected tool result for "
                f"{pending_tool_calls[0]['function']['name']!r}, got role={role!r}"
            )
            pending_tool_calls.clear()

        if role == "user":
            content = msg.get("content")
            if not isinstance(content, str) or not content.strip():
                result.fail(f"messages[{idx}] (user) has empty content")

        elif role == "assistant":
            content = msg.get("content")
            if not isinstance(content, str):
                result.fail(f"messages[{idx}] (assistant) content must be a string")
                content = ""

            # Thought block must be present and indexed
            m = THOUGHT_BLOCK_RE.match(content)
            if not m:
                result.fail(
                    f"messages[{idx}] (assistant turn {assistant_turn_index}): "
                    f"missing or malformed <|channel>thought ... <channel|> block"
                )
            else:
                placeholder_index = int(m.group(1))
                if placeholder_index != assistant_turn_index:
                    result.fail(
                        f"messages[{idx}]: thought placeholder index "
                        f"{placeholder_index} != expected assistant turn "
                        f"{assistant_turn_index}"
                    )
                if placeholder_index in seen_placeholders:
                    result.fail(
                        f"messages[{idx}]: duplicate thought placeholder "
                        f"index {placeholder_index}"
                    )
                seen_placeholders.add(placeholder_index)

            tool_calls = msg.get("tool_calls") or []
            visible_text = (
                content[m.end():] if m else content
            ).strip() if isinstance(content, str) else ""

            if tool_calls:
                # Tool-calling turn: visible text after the thought should be empty
                if visible_text:
                    result.fail(
                        f"messages[{idx}] (assistant turn {assistant_turn_index}): "
                        f"tool-calling turn must not have visible text after the "
                        f"thought block"
                    )
                for tc_i, tc in enumerate(tool_calls):
                    err = _validate_tool_call(tc, catalog, declared_tool_names)
                    for e in err:
                        result.fail(
                            f"messages[{idx}].tool_calls[{tc_i}]: {e}"
                        )
                    pending_tool_calls.append(tc)
                    _collect_ids(tc, fabricated_ids, f"messages[{idx}]")
            else:
                # Final answer turn: visible text required
                if not visible_text:
                    result.fail(
                        f"messages[{idx}] (assistant turn {assistant_turn_index}): "
                        f"non-tool-calling turn must have visible text after the "
                        f"thought block"
                    )

            assistant_turn_index += 1

        elif role == "tool":
            if not pending_tool_calls:
                result.fail(
                    f"messages[{idx}] (tool): no pending tool call to respond to"
                )
                continue
            expected = pending_tool_calls.pop(0)
            expected_name = expected["function"]["name"]
            tname = msg.get("name")
            if tname != expected_name:
                result.fail(
                    f"messages[{idx}] (tool): name {tname!r} does not match "
                    f"pending tool call {expected_name!r}"
                )
            content = msg.get("content")
            if not isinstance(content, str):
                result.fail(f"messages[{idx}] (tool) content must be a string")
            else:
                # Tool result content should be JSON-parseable in our convention
                try:
                    parsed = json.loads(content)
                    _collect_ids_from_value(
                        parsed, fabricated_ids, f"messages[{idx}]"
                    )
                except json.JSONDecodeError:
                    # Non-JSON tool results are allowed but discouraged
                    log.debug(
                        "messages[%d] tool result is not JSON: %r",
                        idx, content[:60]
                    )

    if pending_tool_calls:
        result.fail(
            f"unanswered tool call(s) at end of conversation: "
            f"{[tc['function']['name'] for tc in pending_tool_calls]}"
        )

    # ----- UUID format check ---------------------------------------------

    for fid in fabricated_ids:
        if not _looks_like_uuid(fid):
            result.fail(
                f"id {fid!r} is not a UUID v4 (first seen in "
                f"{fabricated_ids[fid]})"
            )

    return result


def _validate_tool_call(
    tc: dict[str, Any],
    catalog: ToolCatalog,
    declared_tool_names: set[str],
) -> list[str]:
    errors: list[str] = []
    if tc.get("type") != "function":
        errors.append("tool_call.type must be 'function'")
    fn = tc.get("function")
    if not isinstance(fn, dict):
        errors.append("tool_call.function must be an object")
        return errors

    name = fn.get("name")
    if not name:
        errors.append("tool_call.function.name missing")
        return errors
    if name not in catalog.names():
        errors.append(f"tool_call references unknown tool {name!r}")
        return errors
    if name not in declared_tool_names:
        errors.append(
            f"tool_call calls {name!r} which is not in the declared tools list"
        )

    args_str = fn.get("arguments")
    if not isinstance(args_str, str):
        errors.append("tool_call.function.arguments must be a JSON string")
        return errors

    try:
        args = json.loads(args_str)
    except json.JSONDecodeError as e:
        errors.append(f"tool_call.function.arguments is not valid JSON: {e}")
        return errors

    if not isinstance(args, dict):
        errors.append("tool_call.function.arguments must decode to an object")
        return errors

    schema_errors = catalog.validate_arguments(name, args)
    errors.extend(schema_errors)
    errors.extend(_validate_tool_call_conventions(name, args))
    return errors


def _validate_tool_call_conventions(
    name: str, args: dict[str, Any]
) -> list[str]:
    """Project-specific conventions on top of the JSON schema check.

    - Translation tools may appear as distractors but must never be called.
    - Optional fields must be omitted, not set to null/empty.
    - tags / genres must be normalized (lowercased, trimmed, internal
      whitespace collapsed).
    - position must be a non-negative integer when present.
    - read_manuscript must omit `offset`.
    - Timeline date dicts must include at least `year`.
    - color must be a #RRGGBB hex string.
    - ratio must match A:B (two positive integers).
    - search_regex accepts only the `case_sensitive` flag (plus pattern/page).
    """
    errors: list[str] = []

    # Translation tools are distractors only — they may appear in the
    # declared tools list but must never actually be called in this
    # training run. The model needs to learn to ignore them.
    if name.startswith("translate_") or name.startswith("patch_translation_"):
        errors.append(
            f"tool {name!r} is a translation tool and must not be called; "
            f"translation tools are present as distractors only"
        )

    # No null values for any field — the convention is to omit instead.
    for key, value in args.items():
        if value is None:
            errors.append(
                f"field {key!r} is null; omit optional fields instead of "
                f"passing null"
            )
        elif isinstance(value, dict) and not value and key in {"endDate", "offset"}:
            errors.append(
                f"field {key!r} is an empty object; omit it instead"
            )

    # Tag / genre normalization.
    for tag_field in ("tags", "genres"):
        values = args.get(tag_field)
        if isinstance(values, list):
            for v in values:
                if not isinstance(v, str):
                    errors.append(
                        f"{tag_field}[*] must be strings, got {type(v).__name__}"
                    )
                    continue
                if _is_content_placeholder(v):
                    continue
                normalized = re.sub(r"\s+", " ", v.strip().lower())
                if v != normalized:
                    errors.append(
                        f"{tag_field} contains un-normalized value {v!r} "
                        f"(expected {normalized!r})"
                    )

    # Position must be a non-negative int when present.
    if "position" in args:
        pos = args["position"]
        if not isinstance(pos, int) or isinstance(pos, bool) or pos < 0:
            errors.append(f"position must be a non-negative integer, got {pos!r}")

    # read_manuscript must not include offset.
    if name == "read_manuscript" and "offset" in args:
        errors.append(
            "read_manuscript: omit the 'offset' field; reads must be full "
            "manuscript only"
        )

    # Timeline date dicts: must include `year` if present, and must look like
    # date dicts not ISO strings.
    for date_field in ("startDate", "endDate"):
        if date_field not in args:
            continue
        d = args[date_field]
        if isinstance(d, str):
            errors.append(
                f"{date_field} must be a {{year, month, day}} dict, not an "
                f"ISO string {d!r}"
            )
            continue
        if not isinstance(d, dict):
            errors.append(f"{date_field} must be a dict, got {type(d).__name__}")
            continue
        if "year" not in d:
            errors.append(f"{date_field}.year is required")
        for k in ("year", "month", "day"):
            if k in d and not isinstance(d[k], int):
                errors.append(f"{date_field}.{k} must be an integer")

    # Color must be #RRGGBB hex.
    if "color" in args and args["color"] is not None:
        color = args["color"]
        if not _is_content_placeholder(color) and (
            not isinstance(color, str) or not _HEX_COLOR_RE.match(color)
        ):
            errors.append(
                f"color must be a hex string in the form '#RRGGBB', got {color!r}"
            )

    # Image ratio must be 'A:B' with positive integers.
    if "ratio" in args:
        ratio = args["ratio"]
        if not _is_content_placeholder(ratio) and (
            not isinstance(ratio, str) or not _RATIO_RE.match(ratio)
        ):
            errors.append(
                f"ratio must be in 'A:B' form with two positive integers, "
                f"got {ratio!r}"
            )

    # search_regex: only case_sensitive is allowed beyond pattern/page.
    if name == "search_regex":
        allowed = {"pattern", "page", "case_sensitive"}
        for key in args:
            if key not in allowed:
                errors.append(
                    f"search_regex does not accept field {key!r}; only "
                    f"pattern, page, and case_sensitive are supported"
                )

    return errors


_HEX_COLOR_RE = re.compile(r"^#[0-9a-fA-F]{6}$")
_RATIO_RE = re.compile(r"^[1-9][0-9]*:[1-9][0-9]*$")


UUID_RE = re.compile(
    r"^[0-9a-f]{8}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{12}$",
    re.IGNORECASE,
)


def _looks_like_uuid(s: str) -> bool:
    return bool(isinstance(s, str) and UUID_RE.match(s))


def _collect_ids(
    tool_call: dict[str, Any], bag: dict[str, str], context: str
) -> None:
    fn = tool_call.get("function") or {}
    args_str = fn.get("arguments")
    if not isinstance(args_str, str):
        return
    try:
        args = json.loads(args_str)
    except json.JSONDecodeError:
        return
    _collect_ids_from_value(args, bag, context)


def _collect_ids_from_value(value: Any, bag: dict[str, str], context: str) -> None:
    if isinstance(value, dict):
        for k, v in value.items():
            if k in {"id", "object_id", "objectId", "manuscript_id", "trackId",
                     "linkId", "image_id", "parentId", "folderId"} and isinstance(
                v, str
            ):
                # Content placeholders are not real IDs — skip them.
                if not _is_content_placeholder(v):
                    bag.setdefault(v, context)
            else:
                _collect_ids_from_value(v, bag, context)
    elif isinstance(value, list):
        for item in value:
            _collect_ids_from_value(item, bag, context)


# ---------------------------------------------------------------------------
# Rendering
# ---------------------------------------------------------------------------


def render_template(
    env: Environment,
    template_name: str,
    project: dict[str, Any],
    catalog: ToolCatalog,
) -> tuple[dict[str, Any] | None, str | None]:
    """Render one template against one project.

    Returns:
        (sample, None)            on success
        (None, "skip")            on empty render (precondition not met)
        (None, "render: <msg>")   on Jinja error
        (None, "parse: <msg>")    on JSON parse error
        (None, "validate: <msg>") on schema/format error
    """
    try:
        template = env.get_template(template_name)
        rendered = template.render(project=project)
    except TemplateError as e:
        return None, f"render: {e.__class__.__name__}: {e}"
    except Exception as e:  # noqa: BLE001
        return None, f"render: {e.__class__.__name__}: {e}"

    if not rendered or not rendered.strip():
        return None, "skip"

    try:
        sample = json.loads(rendered)
    except json.JSONDecodeError as e:
        snippet = _excerpt_around(rendered, e.pos)
        return None, f"parse: {e.msg} at line {e.lineno} col {e.colno}\n{snippet}"

    if not isinstance(sample, dict):
        return None, "parse: rendered output must be a JSON object"

    val = validate_sample(sample, catalog, template_name=template_name)
    if not val.ok:
        return None, "validate: " + "; ".join(val.errors)

    return sample, None


def _excerpt_around(text: str, pos: int, radius: int = 80) -> str:
    start = max(0, pos - radius)
    end = min(len(text), pos + radius)
    return f"  ...{text[start:end]}..."


# ---------------------------------------------------------------------------
# Main
# ---------------------------------------------------------------------------


def main() -> int:
    parser = argparse.ArgumentParser(
        description="Render Novel Buds training samples from Jinja templates."
    )
    parser.add_argument(
        "--templates",
        type=Path,
        default=Path("./templates"),
        help="Directory containing .j2 template files (default: ./templates)",
    )
    parser.add_argument(
        "--projects",
        type=Path,
        default=Path("./data/2_refined_novel"),
        help="Directory containing per-novel project folders "
        "(default: ./data/2_refined_novel)",
    )
    parser.add_argument(
        "--tools",
        type=Path,
        default=Path("./tools.json"),
        help="Path to the tool catalog JSON file (default: ./tools.json)",
    )
    parser.add_argument(
        "--output",
        type=Path,
        default=Path("./data/3_rendered"),
        help="Output directory (default: ./data/3_rendered)",
    )
    parser.add_argument(
        "--num-shards",
        type=int,
        default=500,
        help="Number of output shards (default: 500)",
    )
    parser.add_argument(
        "--seed",
        type=int,
        default=3407,
        help="Random seed for stochastic Jinja helpers (default: 3407)",
    )
    parser.add_argument(
        "--filter-template",
        type=str,
        default=None,
        help="Render only templates whose filename matches this glob pattern",
    )
    parser.add_argument(
        "--filter-project",
        type=str,
        default=None,
        help="Render only projects whose directory name matches this glob",
    )
    parser.add_argument(
        "--embeddings",
        type=Path,
        default=None,
        help="Directory with pre-built embeddings (default: None, uses keyword fallback)",
    )
    parser.add_argument(
        "--strict",
        action="store_true",
        help="Exit with non-zero status if any sample fails validation",
    )
    parser.add_argument(
        "--verbose",
        "-v",
        action="store_true",
        help="Enable debug logging",
    )
    args = parser.parse_args()

    setup_logging(args.verbose)
    random.seed(args.seed)

    if not args.templates.is_dir():
        log.error("Templates dir not found: %s", args.templates)
        return 2
    if not args.projects.is_dir():
        log.error("Projects dir not found: %s", args.projects)
        return 2

    catalog = ToolCatalog(args.tools)

    # Load embeddings index if provided
    embeddings_index: dict[str, Any] | None = None
    if args.embeddings and args.embeddings.is_dir():
        log.info("Loading embeddings from %s", args.embeddings)
        from sentence_transformers import SentenceTransformer

        emb_model = SentenceTransformer("sentence-transformers/all-MiniLM-L6-v2")

        def _encode_query(text: str) -> np.ndarray:
            return emb_model.encode(
                text, normalize_embeddings=True, convert_to_numpy=True,
            )

        embeddings_index = {}
        for idx_dir in sorted(args.embeddings.iterdir()):
            if not idx_dir.is_dir():
                continue
            vec_path = idx_dir / "vectors.npy"
            meta_path = idx_dir / "chunks.json"
            texts_path = idx_dir / "texts.json"
            if not (vec_path.exists() and meta_path.exists() and texts_path.exists()):
                continue
            vectors = np.load(vec_path)
            with meta_path.open("r", encoding="utf-8") as f:
                meta = json.load(f)
            with texts_path.open("r", encoding="utf-8") as f:
                texts = json.load(f)
            embeddings_index[idx_dir.name] = {
                "vectors": vectors,
                "meta": meta,
                "texts": texts,
                "encode_fn": _encode_query,
            }
        log.info("Loaded embeddings for %d projects", len(embeddings_index))

    env = build_jinja_env(args.templates, catalog, embeddings_index)

    template_files = sorted(p for p in args.templates.glob("*.j2"))
    if args.filter_template:
        template_files = [
            p for p in template_files if p.match(args.filter_template)
        ]
    if not template_files:
        log.error("No templates matched in %s", args.templates)
        return 2
    log.info("Found %d template(s)", len(template_files))

    project_dirs = sorted(p for p in args.projects.iterdir() if p.is_dir())
    if args.filter_project:
        project_dirs = [
            p for p in project_dirs if p.match(args.filter_project)
        ]
    if not project_dirs:
        log.error("No projects matched in %s", args.projects)
        return 2
    log.info("Found %d project(s)", len(project_dirs))

    stats = RenderStats()
    stats.templates_seen = len(template_files)
    stats.projects_seen = len(project_dirs)

    # Pre-load all projects once
    projects: list[dict[str, Any]] = []
    for pdir in project_dirs:
        proj = load_project(pdir)
        if proj is not None:
            projects.append(proj)
    log.info("Loaded %d valid project(s)", len(projects))

    # Collect all samples
    all_samples: list[dict[str, Any]] = []

    for tpath in template_files:
        tname = tpath.name
        log.info("Rendering template: %s", tname)
        for project in projects:
            stats.attempts += 1
            stats.bump_template(tname, "attempts")

            sample, err = render_template(env, tname, project, catalog)

            if sample is None and err == "skip":
                stats.skipped_empty += 1
                stats.bump_template(tname, "skipped")
                continue

            if sample is None:
                if err and err.startswith("render:"):
                    stats.failed_render += 1
                else:
                    stats.failed_validation += 1
                stats.bump_template(tname, "failed")
                log.warning(
                    "FAIL %s × %s — %s",
                    tname,
                    project["_project_slug"],
                    err,
                )
                continue

            sample["_meta"] = {
                "template": tname,
                "project": project["_project_slug"],
                "rendered_at": datetime.now(timezone.utc).isoformat(),
            }
            stats.succeeded += 1
            stats.bump_template(tname, "succeeded")
            stats.bump_category(sample["scenario_category"])
            all_samples.append(sample)

    # Shuffle and write shards
    rng = random.Random(args.seed)
    rng.shuffle(all_samples)

    args.output.mkdir(parents=True, exist_ok=True)
    n_shards = max(1, min(args.num_shards, len(all_samples)))
    shard_size = len(all_samples) // n_shards
    remainder = len(all_samples) % n_shards

    offset = 0
    for i in range(n_shards):
        # Distribute remainder across first shards
        cur_size = shard_size + (1 if i < remainder else 0)
        shard = all_samples[offset : offset + cur_size]
        offset += cur_size

        fpath = args.output / f"shard_{i:04d}.jsonl"
        with fpath.open("w", encoding="utf-8") as f:
            for s in shard:
                f.write(json.dumps(s, ensure_ascii=False) + "\n")

    log.info(
        "Wrote %d samples across %d shards to %s (~%d samples/shard)",
        stats.succeeded,
        n_shards,
        args.output,
        shard_size,
    )
    print(stats.report())

    if args.strict and (stats.failed_render or stats.failed_validation):
        return 1
    return 0


if __name__ == "__main__":
    sys.exit(main())