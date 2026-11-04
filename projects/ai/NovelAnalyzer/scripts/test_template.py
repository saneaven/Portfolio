from transformers import AutoTokenizer
import json
import pathlib

print("test")

PROJECT_DIR = pathlib.Path(__file__).parent.parent

tok = AutoTokenizer.from_pretrained("google/gemma-4-E2B-it")
sample = json.loads(open(PROJECT_DIR / "data/6_hf_gemma4_embedded_only/test.jsonl").readline())

rendered = tok.apply_chat_template(
    sample["messages"], tools=sample["tools"],
    tokenize=False, enable_thinking=True,
)

print(rendered)