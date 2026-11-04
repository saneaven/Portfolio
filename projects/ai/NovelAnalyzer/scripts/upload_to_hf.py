"""
data/6_hf_gemma4의 JSONL 데이터를 HuggingFace에 업로드하는 스크립트.
대상 repo: saneaven/novelbuds-agent-data-v0
"""

from datasets import load_dataset
from pathlib import Path

DATA_DIR = Path(__file__).resolve().parent.parent / "data" / "6_hf_gemma4"
REPO_ID = "saneaven/novelbuds-agent-data-v0"


def main():
    data_files = {
        "train": str(DATA_DIR / "train.jsonl"),
        "validation": str(DATA_DIR / "validation.jsonl"),
        "test": str(DATA_DIR / "test.jsonl"),
    }

    print(f"Loading data from {DATA_DIR} ...")
    dataset = load_dataset("json", data_files=data_files)
    print(dataset)

    print(f"\nUploading to {REPO_ID} ...")
    dataset.push_to_hub(REPO_ID)
    print("Done!")


if __name__ == "__main__":
    main()
