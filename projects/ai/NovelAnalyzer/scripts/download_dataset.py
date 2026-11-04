from datasets import load_dataset

ds = load_dataset("Despina/project_gutenberg", "fiction_books")
ds.save_to_disk("data/project_gutenberg")
print("Done! Dataset saved to data/project_gutenberg/")
