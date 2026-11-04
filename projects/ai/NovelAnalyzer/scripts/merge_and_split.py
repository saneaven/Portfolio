#!/usr/bin/env python3
"""Merge all placeholder-filled shards, split by novel into train/val/test, upload to HF."""
import json
import glob
import random
import os
from collections import defaultdict

SHARD_DIR = "data/4_placeholder_filled"
OUTPUT_DIR = "data/5_merged_and_split"
SEED = 42

# 1. Load all records, group by novel
print("Loading shards...")
novel_records = defaultdict(list)
for path in sorted(glob.glob(f"{SHARD_DIR}/shard_*.jsonl")):
    with open(path, encoding="utf-8") as f:
        for line in f:
            rec = json.loads(line)
            project = rec.get("_meta", {}).get("project", "unknown")
            novel_records[project].append(rec)

total = sum(len(v) for v in novel_records.values())
print(f"Loaded {total} records from {len(novel_records)} novels")

# 2. Split novels into train/val/test (90%/9%/1%) by greedy assignment
novels = list(novel_records.keys())
random.seed(SEED)
random.shuffle(novels)

train_novels, val_novels, test_novels = [], [], []
train_count = val_count = test_count = 0

for novel in novels:
    n = len(novel_records[novel])
    train_gap = 0.90 * total - train_count
    val_gap = 0.09 * total - val_count
    test_gap = 0.01 * total - test_count

    best = max(
        [("train", train_gap), ("val", val_gap), ("test", test_gap)],
        key=lambda x: x[1],
    )
    if best[0] == "train":
        train_novels.append(novel)
        train_count += n
    elif best[0] == "val":
        val_novels.append(novel)
        val_count += n
    else:
        test_novels.append(novel)
        test_count += n


# 3. Collect and shuffle records per split
def collect(novel_list):
    recs = []
    for novel in novel_list:
        recs.extend(novel_records[novel])
    random.shuffle(recs)
    return recs


train_data = collect(train_novels)
val_data = collect(val_novels)
test_data = collect(test_novels)

print(f"Train: {len(train_data):,} records ({len(train_novels)} novels) — {len(train_data)/total:.1%}")
print(f"Val:   {len(val_data):,} records ({len(val_novels)} novels) — {len(val_data)/total:.1%}")
print(f"Test:  {len(test_data):,} records ({len(test_novels)} novels) — {len(test_data)/total:.1%}")

# 4. Save JSONL files
os.makedirs(OUTPUT_DIR, exist_ok=True)
for name, data in [("train", train_data), ("validation", val_data), ("test", test_data)]:
    path = os.path.join(OUTPUT_DIR, f"{name}.jsonl")
    with open(path, "w", encoding="utf-8") as f:
        for rec in data:
            f.write(json.dumps(rec, ensure_ascii=False) + "\n")
    print(f"Saved {path} ({len(data)} records)")