from transformers import AutoTokenizer
from datasets import load_dataset
import numpy as np

# 모델 weight 로드 없음 — tokenizer 설정 파일만 받아옴 (~10MB)
tokenizer = AutoTokenizer.from_pretrained("google/gemma-4-31B-it")

ds = load_dataset("saneaven/novelbuds-agent-data-v0", split="train")

# 전체 돌려도 CPU로 몇 분이면 충분
lens = [len(tokenizer(x["text"]).input_ids) for x in ds]

print(f"평균:   {np.mean(lens):>6.0f}")
print(f"중앙값: {np.median(lens):>6.0f}")
print(f"P90:    {np.percentile(lens, 90):>6.0f}")
print(f"P95:    {np.percentile(lens, 95):>6.0f}")
print(f"P99:    {np.percentile(lens, 99):>6.0f}")
print(f"최대:   {max(lens):>6}")