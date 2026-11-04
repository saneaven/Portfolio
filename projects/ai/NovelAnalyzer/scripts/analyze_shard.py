#!/usr/bin/env python3
"""Analyze placeholders in a shard jsonl"""
import json
import re
import sys

shard = sys.argv[1] if len(sys.argv) > 1 else 'shard_0011'
with open(f'/run/media/Saneaven/Data/Projects/Personal/MiniProj/NovelAnalyzer/data/3_rendered/{shard}.jsonl') as f:
    for i, line in enumerate(f):
        obj = json.loads(line)
        scenario = obj.get('scenario_id', '')
        cat = obj.get('scenario_category', '')
        system = obj.get('system', '')
        title_match = re.search(r'novel\s+"([^"]+)"', system) or re.search(r'novel\s+\\?"([^"\\]+)', system)
        title = title_match.group(1) if title_match else 'unknown'

        thinking = re.findall(r'\[THINKING_PLACEHOLDER_\d+\]', line)
        gen_content = re.findall(r'\[GENERATED_CONTENT\]', line)
        asst_resp = re.findall(r'\[ASSISTANT_RESPONSE_\d+\]', line)
        gen_prose = re.findall(r'\[GENERATED_PROSE\]', line)
        subagent = re.findall(r'\[SUBAGENT_RESPONSE\]', line)

        print(f'L{i}: {scenario} ({cat}) "{title[:50]}" | T:{len(thinking)} GC:{len(gen_content)} AR:{len(asst_resp)} GP:{len(gen_prose)} SA:{len(subagent)}')
