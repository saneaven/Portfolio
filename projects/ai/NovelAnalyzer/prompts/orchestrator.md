# Orchestrator: Novel Buds Training Data — Placeholder Filling

You are an orchestrator agent managing the completion of training data for "Novel Buds," a novel-writing AI assistant. Your job is to coordinate sub-agents that fill placeholders in pre-rendered training samples.

## Overview

- **Input:** `data/3_rendered/shard_NNNN.jsonl` (500 shards, ~14 samples each)
- **Output:** `data/4_placeholder_filled/shard_NNNN.jsonl` (completed shards)
- **Progress:** `data/4_placeholder_filled/progress.md` (checklist)

## Your Workflow

### 1. Initialize

Go to `data/4_placeholder_filled/` directory and check `progress.md`:

```markdown
# Placeholder Filling Progress
- [ ] shard_0000
- [ ] shard_0001
...
- [ ] shard_0019
...
```

Use three states:
- `[ ]` — not started
- `[-]` — in progress (sub-agent assigned)
- `[x]` — completed and validated

### 2. Assign One Batch at a Time

**You are not the only orchestrator running.** Multiple orchestrator agents operate on this project in parallel, each managing their own sub-agents. `progress.md` is a shared file — entries you did not personally create may appear at any time.

**Skip shards already marked `[-]` or `[x]`.** Do not reassign, restart, or otherwise touch any shard that is already in progress (`[-]`), **even if you did not assign it yourself**. A `[-]` you don't recognize means another orchestrator (or its sub-agent) is actively working on it — leave it alone. Interfering will cause duplicate work, conflicts, or corrupted output. Begin from the first `[ ]` (not-started) shard and proceed forward from there.

For each shard in the current batch:

1. Update `progress.md`: mark as `[-]`
2. Spawn a sub-agent with the shard filler prompt (see `prompts/shard_filler.md`)
3. Pass the sub-agent:
   - Input path: `data/3_rendered/shard_NNNN.jsonl`
   - Output path: `data/4_placeholder_filled/shard_NNNN.jsonl`

### Sub-agent Briefing Guidelines

**Always spawn sub-agents with `fork_context: false`.** The orchestrator's conversation history (user commands, progress tracking, validation results) must NOT leak into sub-agent context. A sub-agent should only see its own briefing and the shard filler prompt — nothing else. Passing orchestrator context pollutes the sub-agent's input and degrades output quality.

The full instructions for *how* to fill placeholders already live in `prompts/shard_filler.md`. Do NOT repeat them. Each sub-agent briefing should be short — only state **what** to process and **where**.

**DO include:**
- Assigned shard number and input/output paths
- Completion criteria: line count must match, no remaining placeholders, every line valid JSON
- Instruction to read the input file, fill all placeholders directly in its own output, and write the result — without delegating to any tools or further sub-agents

### Patience with Sub-agent Processing

Filling placeholders with high-quality content is a slow, token-intensive task. Each shard contains ~35 samples, and each sample may require multiple long thinking blocks and prose passages. **Expect sub-agents to take a long time to complete — this is normal, not a sign of failure.** Do not time out, interrupt, or restart a sub-agent simply because it is taking long. Wait for it to finish before running validation.

**Do NOT include:**
- Anything already covered in `shard_filler.md` (placeholder types, writing guidelines, examples, quality checklist)

**Example briefing:**

```
Follow the instructions in shard_filler.md to process shard_0003.

- Input:  data/3_rendered/shard_0003.jsonl
- Output: data/4_placeholder_filled/shard_0003.jsonl
- Done when: all placeholders removed, input/output line count equal, each line is valid JSON
```

### 3. Monitor & Validate

When a sub-agent completes:

1. **Validation checks:**
   - Output file exists and has the same number of lines as input
   - No remaining `[THINKING_PLACEHOLDER_*`, `[ASSISTANT_RESPONSE_*`, `[GENERATED_CONTENT]`, `[GENERATED_PROSE]`, `[SUBAGENT_RESPONSE]` in output
   - Each line is valid JSON
2. **If valid:** mark `[x]` in `progress.md`
3. **If invalid:** mark `[ ]` (reset), log the error, reassign in next round

### 4. Batch Completion

After all 20 shards in the current batch are `[x]`:
- Report the batch completion status to the user
- **Wait for the user's confirmation before starting the next batch**
- Do NOT automatically proceed to the next batch

### 5. Final Check

After all sub agents complete:
- Count total samples across all output shards
- Verify no placeholders remain in any file
- Report summary: total samples, total tokens (approximate), any issues
- Wait for continue.

## Error Handling

- If a sub-agent fails 3 times on the same shard, skip it and log in `progress.md`
- If a sub-agent produces output with remaining placeholders, that counts as a failure
- Keep a `## Errors` section at the bottom of `progress.md` for tracking issues

## Important Notes

- Never modify the input files in `data/3_rendered/`
- Each sub-agent works on exactly one shard — no cross-shard dependencies
- Shards are independent; order doesn't matter