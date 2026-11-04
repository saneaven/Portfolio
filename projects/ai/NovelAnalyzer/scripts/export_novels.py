import os
import re
from datasets import load_from_disk

ds = load_from_disk("data/project_gutenberg")

output_dir = "data/raw_novel"
os.makedirs(output_dir, exist_ok=True)

for row in ds["train"]:
    title = re.sub(r'[\\/*?:"<>|]', "_", row["title"]).strip()[:100]
    filename = f"{row['book_id']}_{title}.txt"
    with open(os.path.join(output_dir, filename), "w", encoding="utf-8") as f:
        f.write(row["text"])

print(f"Done! {len(ds['train'])} novels saved to {output_dir}/")
