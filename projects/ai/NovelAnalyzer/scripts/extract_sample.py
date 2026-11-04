#!/usr/bin/env python3
"""Extract a single sample from a shard, showing its structure and placeholders."""
import json, sys, re

shard_path = sys.argv[1]
line_num = int(sys.argv[2])  # 0-indexed

with open(shard_path) as f:
    for i, line in enumerate(f):
        if i == line_num:
            obj = json.loads(line)
            break

# Print system context (truncated)
print("=== SYSTEM (first 500 chars) ===")
print(obj['system'][:500])
print("\n=== SCENARIO ===")
print(f"ID: {obj['scenario_id']}, Category: {obj['scenario_category']}")

# Print messages with placeholder locations
print(f"\n=== MESSAGES ({len(obj['messages'])} total) ===")
placeholders_found = []
for i, msg in enumerate(obj['messages']):
    content = msg.get('content', '')
    if isinstance(content, str):
        phs = re.findall(r'\[(?:THINKING_PLACEHOLDER_\d+|ASSISTANT_RESPONSE_\d+|GENERATED_CONTENT|GENERATED_PROSE|SUBAGENT_RESPONSE)\]', content)
        print(f"  [{i}] {msg['role']} ({len(content)} chars) placeholders: {phs}")
        placeholders_found.extend(phs)
    else:
        print(f"  [{i}] {msg['role']} (non-string content)")

print(f"\n=== TOTAL PLACEHOLDERS: {len(placeholders_found)} ===")
for ph in placeholders_found:
    print(f"  {ph}")
