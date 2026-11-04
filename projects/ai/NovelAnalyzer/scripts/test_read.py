#!/usr/bin/env python3
import json
with open('/run/media/Saneaven/Data/Projects/Personal/MiniProj/NovelAnalyzer/data/3_rendered/shard_0014.jsonl') as f:
    for i, line in enumerate(f):
        obj = json.loads(line)
        print(f"Line {i}: {obj['scenario_id']}")
