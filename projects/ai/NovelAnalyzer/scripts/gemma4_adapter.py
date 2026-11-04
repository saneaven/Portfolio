#!/usr/bin/env python3
"""Adapt 5_merge_and_split JSONL → Gemma 4 training text.

Produces JSONL where each line has:
  - text: ready-to-train Gemma 4 string (feed to SFTTrainer with
          dataset_text_field="text")
  - messages / tools: normalized structured form (for debugging)

The `text` field is rendered directly rather than via apply_chat_template
because neither Unsloth's `gemma-4-thinking` nor Google's official template
handles `thinking` + `tool_calls` + `tool_responses` correctly for training.

Usage:
    python scripts/gemma4_adapter.py
    python scripts/gemma4_adapter.py --input-dir data/5_merge_and_split --output-dir data/6_hf_gemma4
"""
import argparse
import copy
import json
import os
import re
import sys

INPUT_DIR = "data/5_merge_and_split"
OUTPUT_DIR = "data/6_hf_gemma4"
SPLITS = ["train", "validation", "test"]

SYSTEM_THINK_PREFIX_RE = re.compile(r"^<\|think\|>\n")
THOUGHT_BLOCK_RE = re.compile(
    r"^<\|channel>thought\n(?P<body>.*?)<channel\|>",
    re.DOTALL,
)
THOUGHT_UNCLOSED_RE = re.compile(
    r"^<\|channel>thought\n(?P<body>.+)$",
    re.DOTALL,
)


# ─── JSON repair (unchanged) ───────────────────────────────────────────────

def repair_json(s):
    """Parse JSON string, repairing unescaped quotes inside values if needed."""
    try:
        return json.loads(s, strict=False)
    except json.JSONDecodeError:
        pass

    buf = list(s)
    for _ in range(10):
        try:
            return json.loads("".join(buf), strict=False)
        except json.JSONDecodeError as exc:
            idx = exc.pos - 1
            while idx >= 0 and buf[idx] != '"':
                idx -= 1
            if idx < 0:
                break
            buf[idx] = '\\"'
    del buf

    KEY_BOUNDARY_RE = re.compile(r'"([^"]+)"\s*:\s*"')
    spans = list(KEY_BOUNDARY_RE.finditer(s))
    if not spans:
        raise ValueError(f"no JSON keys found: {s[:200]}")

    parts = []
    for i, m in enumerate(spans):
        if i == 0:
            parts.append(s[: m.start()])
        parts.append(m.group(0))
        val_start = m.end()
        if i + 1 < len(spans):
            next_start = spans[i + 1].start()
            tail = s[val_start:next_start]
            sep = tail.rfind('", ')
            if sep == -1:
                sep = tail.rfind('",')
            if sep == -1:
                sep = tail.rfind('"')
            val_body = tail[:sep]
            val_tail = tail[sep:]
        else:
            tail = s[val_start:]
            sep = tail.rfind('"}')
            if sep == -1:
                sep = tail.rfind('"')
            val_body = tail[:sep]
            val_tail = tail[sep:]

        val_body = val_body.replace('\\"', '\x00')
        val_body = val_body.replace('"', '\\"')
        val_body = val_body.replace('\x00', '\\"')
        parts.append(val_body)
        parts.append(val_tail)

    repaired = "".join(parts)
    try:
        return json.loads(repaired, strict=False)
    except json.JSONDecodeError:
        raise ValueError(f"structural repair failed: {s[:200]}")


# ─── Structural normalization (mostly unchanged) ───────────────────────────

def adapt_tool(t):
    if "function" in t and t.get("type") == "function":
        return t
    if "name" in t and "parameters" in t:
        return {
            "type": "function",
            "function": {
                "name": t["name"],
                "description": t.get("description", ""),
                "parameters": t["parameters"],
            },
        }
    raise ValueError(f"unrecognized tool schema: {t}")


def adapt_tool_call(tc):
    fn = tc["function"]
    args = fn["arguments"]
    if isinstance(args, str):
        args = repair_json(args)
    return {
        "type": "function",
        "function": {"name": fn["name"], "arguments": args},
    }


def adapt_assistant(msg):
    """Split inlined <|channel>thought block out of content into `thinking` field."""
    content = msg.get("content", "") or ""
    m = THOUGHT_BLOCK_RE.match(content)
    if m:
        thinking = m.group("body").strip()
        visible = content[m.end():].lstrip("\n")
    else:
        m2 = THOUGHT_UNCLOSED_RE.match(content)
        if m2:
            thinking = m2.group("body").strip()
            visible = ""
        else:
            thinking = None
            visible = content

    out = {"role": "assistant", "content": visible}
    if thinking is not None:
        out["thinking"] = thinking
    if msg.get("tool_calls"):
        out["tool_calls"] = [adapt_tool_call(tc) for tc in msg["tool_calls"]]
    return out


def adapt_sample(sample):
    sample = copy.deepcopy(sample)
    system_text = SYSTEM_THINK_PREFIX_RE.sub("", sample.get("system", ""))
    tools = [adapt_tool(t) for t in sample.get("tools", [])]

    messages = []
    if system_text.strip():
        messages.append({"role": "system", "content": system_text})

    for msg in sample["messages"]:
        role = msg["role"]
        if role == "assistant":
            messages.append(adapt_assistant(msg))
        elif role == "tool":
            messages.append({
                "role": "tool",
                "name": msg["name"],
                "content": msg["content"],
            })
        elif role == "user":
            messages.append({"role": "user", "content": msg["content"]})
        else:
            messages.append(msg)

    return {"messages": messages, "tools": tools}


def expand_turns(sample):
    """Expand one sample into N samples, one per user turn.

    Past-turn thinking (before the target user turn) is stripped per Gemma 4 spec.
    """
    messages = sample["messages"]
    tools = sample["tools"]

    offset = 1 if messages and messages[0]["role"] == "system" else 0
    user_indices = [i for i in range(offset, len(messages)) if messages[i]["role"] == "user"]
    if not user_indices:
        return [sample]

    expanded = []
    for user_idx in user_indices:
        end = user_idx + 1
        while end < len(messages) and messages[end]["role"] != "user":
            end += 1

        truncated = []
        for i in range(end):
            msg = messages[i]
            if msg["role"] == "assistant" and "thinking" in msg and i < user_idx:
                msg = {k: v for k, v in msg.items() if k != "thinking"}
            truncated.append(msg)

        expanded.append({"messages": truncated, "tools": tools})

    return expanded


# ─── Gemma 4 direct rendering (NEW) ────────────────────────────────────────

def format_value(v, escape_keys=True):
    """Serialize a Python value to Gemma 4 structured token stream.

    Mirrors the official `format_argument` Jinja macro. Use escape_keys=True
    for tool declarations (parameters schema), escape_keys=False for tool_call
    argument bodies and tool_response bodies.
    """
    if isinstance(v, bool):
        return "true" if v else "false"
    if isinstance(v, str):
        return f'<|"|>{v}<|"|>'
    if isinstance(v, (int, float)):
        return str(v)
    if v is None:
        return "null"
    if isinstance(v, dict):
        pairs = []
        for k in sorted(v.keys()):
            key_str = f'<|"|>{k}<|"|>' if escape_keys else k
            pairs.append(f"{key_str}:{format_value(v[k], escape_keys)}")
        return "{" + ",".join(pairs) + "}"
    if isinstance(v, list):
        return "[" + ",".join(format_value(x, escape_keys) for x in v) + "]"
    return f'<|"|>{v}<|"|>'


def render_tool_declaration(tool):
    fn = tool["function"]
    desc = fn.get("description", "")
    params = fn.get("parameters", {"type": "object", "properties": {}})
    return (
        f'<|tool>declaration:{fn["name"]}'
        f'{{description:<|"|>{desc}<|"|>,'
        f'parameters:{format_value(params, escape_keys=True)}}}'
        f'<tool|>'
    )


def render_tool_call(tc):
    fn = tc["function"]
    args = fn["arguments"] if isinstance(fn["arguments"], dict) else {"value": fn["arguments"]}
    body = ",".join(
        f"{k}:{format_value(args[k], escape_keys=False)}"
        for k in sorted(args.keys())
    )
    return f"<|tool_call>call:{fn['name']}{{{body}}}<tool_call|>"


def render_tool_response(name, response):
    if isinstance(response, dict):
        body = ",".join(
            f"{k}:{format_value(response[k], escape_keys=False)}"
            for k in sorted(response.keys())
        )
    else:
        body = f"value:{format_value(response, escape_keys=False)}"
    return f"<|tool_response>response:{name}{{{body}}}<tool_response|>"


def merge_agentic_turn(messages, start):
    """Merge OpenAI-style tool call chain into one Google-spec model turn.

    Accepts chains of (assistant(tool_calls) → tool* → assistant(...))+ until
    a terminal assistant with only content is reached. Returns a list of
    ordered events [("thought", text) | ("tool_call", tc) | ("tool_response",
    name, response) | ("content", text)] that should appear in order inside
    a single <|turn>model ... <turn|> block, plus the next index to consume.
    """
    m = messages[start]
    if not (m["role"] == "assistant" and m.get("tool_calls")):
        return None, None

    events = []
    if m.get("thinking"):
        events.append(("thought", m["thinking"]))
    if m.get("content", ""):
        events.append(("content", m["content"]))
    for tc in m["tool_calls"]:
        events.append(("tool_call", tc))

    i = start + 1
    while i < len(messages) and messages[i]["role"] == "tool":
        tm = messages[i]
        events.append(("tool_response", tm.get("name", "unknown"), tm["content"]))
        i += 1

    # Continue absorbing subsequent assistant turns in the same agentic chain
    while i < len(messages) and messages[i]["role"] == "assistant":
        asst = messages[i]
        if asst.get("thinking"):
            events.append(("thought", asst["thinking"]))

        if asst.get("tool_calls"):
            if asst.get("content", ""):
                events.append(("content", asst["content"]))
            for tc in asst["tool_calls"]:
                events.append(("tool_call", tc))
            i += 1
            while i < len(messages) and messages[i]["role"] == "tool":
                tm = messages[i]
                events.append(("tool_response", tm.get("name", "unknown"), tm["content"]))
                i += 1
        else:
            if asst.get("content", ""):
                events.append(("content", asst["content"]))
            i += 1
            break

    return events, i


def render_gemma4(sample):
    """Render normalized sample to Gemma 4 training text."""
    messages = sample["messages"]
    tools = sample.get("tools", [])

    parts = ["<bos>"]

    # System turn: always include <|think|> to enable thinking mode on 31B
    system_msg = None
    msg_start = 0
    if messages and messages[0]["role"] == "system":
        system_msg = messages[0]
        msg_start = 1

    if system_msg or tools:
        parts.append("<|turn>system\n")
        parts.append("<|think|>")
        if system_msg:
            parts.append(system_msg["content"].strip())
        for t in tools:
            parts.append(render_tool_declaration(t))
        parts.append("<turn|>\n")

    # Conversation
    remaining = messages[msg_start:]
    j = 0
    while j < len(remaining):
        m = remaining[j]
        role = m["role"]

        if role == "user":
            parts.append("<|turn>user\n")
            parts.append(m["content"].strip())
            parts.append("<turn|>\n")
            j += 1

        elif role == "assistant":
            events, next_j = merge_agentic_turn(remaining, j)

            parts.append("<|turn>model\n")
            if events is not None:
                for ev in events:
                    if ev[0] == "thought":
                        parts.append(f"<|channel>thought\n{ev[1].strip()}\n<channel|>")
                    elif ev[0] == "tool_call":
                        parts.append(render_tool_call(ev[1]))
                    elif ev[0] == "tool_response":
                        parts.append(render_tool_response(ev[1], ev[2]))
                    elif ev[0] == "content":
                        parts.append(ev[1])
                j = next_j
            else:
                # Non-agentic assistant turn (no tool_calls)
                if m.get("thinking"):
                    parts.append(f"<|channel>thought\n{m['thinking'].strip()}\n<channel|>")
                parts.append(m.get("content", "") or "")
                j += 1
            parts.append("<turn|>\n")

        else:
            # Orphan tool role — shouldn't happen after merge, but handle defensively
            j += 1

    return "".join(parts)


# ─── Pipeline ──────────────────────────────────────────────────────────────

def process_file(input_path, output_path):
    ok = errors = expanded_total = 0
    with open(input_path, encoding="utf-8") as fin, \
         open(output_path, "w", encoding="utf-8") as fout:
        for lineno, line in enumerate(fin, 1):
            line = line.strip()
            if not line:
                continue
            try:
                sample = json.loads(line, strict=False)
                adapted = adapt_sample(sample)
                samples = expand_turns(adapted)
                for s in samples:
                    text = render_gemma4(s)
                    fout.write(json.dumps({
                        "text": text,
                        "messages": s["messages"],
                        "tools": s["tools"],
                    }, ensure_ascii=False) + "\n")
                expanded_total += len(samples)
                ok += 1
            except Exception as exc:
                print(f"  ERROR line {lineno}: {exc}", file=sys.stderr)
                errors += 1
    return ok, errors, expanded_total


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--input-dir", default=INPUT_DIR)
    parser.add_argument("--output-dir", default=OUTPUT_DIR)
    parser.add_argument("--splits", default=",".join(SPLITS))
    args = parser.parse_args()

    splits = [s.strip() for s in args.splits.split(",")]
    os.makedirs(args.output_dir, exist_ok=True)

    for split in splits:
        input_path = os.path.join(args.input_dir, f"{split}.jsonl")
        output_path = os.path.join(args.output_dir, f"{split}.jsonl")
        if not os.path.exists(input_path):
            print(f"SKIP {split}: {input_path} not found")
            continue
        print(f"Processing {split}...")
        ok, errors, expanded = process_file(input_path, output_path)
        print(f"  {ok} samples → {expanded} expanded, {errors} errors → {output_path}")

    print("Done.")


if __name__ == "__main__":
    main()