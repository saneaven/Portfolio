from dotenv import load_dotenv
import pandas as pd
from google import genai
from google.genai import types
from google.genai import errors as genai_errors
import os
import time

load_dotenv()

GOOGLE_API_KEY = os.getenv("GEMINI_API_KEY")
client = genai.Client(api_key=GOOGLE_API_KEY)

INPUT_FILE = "./main_news/main_news_zipped.csv"
OUTPUT_FILE = "./main_news/main_news_zipped_embedded.csv"

MAX_429_RETRIES = 5      # How many consecutive 429 errors to give up on
RETRY_SLEEP_SEC = 60     # How many seconds to wait for each 429 error

EMBEDDING_COLUMN = "embedding"
TEXT_COLUMN = "Title"


def call_with_429_retry(func, *args, **kwargs):
    """Wrapper that retries after waiting for a certain amount of time if 429 (RESOURCE_EXHAUSTED) occurs."""
    attempt = 0
    while True:
        try:
            return func(*args, **kwargs)
        except genai_errors.ClientError as e:
            status = getattr(e, "status_code", None)
            msg = str(e)

            # Retry if status is 429 or message contains 429 / RESOURCE_EXHAUSTED
            if status == 429 or "429" in msg or "RESOURCE_EXHAUSTED" in msg:
                attempt += 1
                if attempt > MAX_429_RETRIES:
                    print(f" 429 occurred {MAX_429_RETRIES} times in a row, stopping retries.")
                    raise

                print(f"429(RESOURCE_EXHAUSTED) occurred. Waiting {RETRY_SLEEP_SEC} seconds before retry {attempt}...")
                time.sleep(RETRY_SLEEP_SEC)
                continue

            # Raise other errors as is
            raise

def embed_csv_in_chunks(
    input_csv: str,
    output_csv: str,
    text_column: str = "FullText",
    batch_size: int = 64,
    model_name: str = "gemini-embedding-001",
):
    """
    - If output_csv already exists: resume from there (skip already embedded rows)
    - Embed only rows not yet embedded in chunks of batch_size, saving CSV after each batch
    """
    # ===== 1. Load Data =====
    if os.path.exists(output_csv):
        print(f"Found existing result file '{output_csv}' -> Resuming.")
        df = pd.read_csv(output_csv)
    else:
        print(f"Loading '{input_csv}'... (Starting new)")
        df = pd.read_csv(input_csv)

        # ----- Preprocessing -----
        if text_column not in df.columns:
            raise ValueError(f"Column '{text_column}' not found.")

        df = df.dropna(subset=[text_column])
        df = df[df[text_column].astype(str).str.strip() != ""]
        df = df.reset_index(drop=True)

        # Initialize embedding column
        df[EMBEDDING_COLUMN] = None

        # Initial save of OUTPUT directory/file
        os.makedirs(os.path.dirname(output_csv), exist_ok=True)
        df.to_csv(output_csv, index=False, encoding="utf-8-sig")
        print(f"Initial preprocessing done, saved to '{output_csv}'.")

    # ===== 2. Find rows not yet embedded =====
    if EMBEDDING_COLUMN not in df.columns:
        # If existing file but no embedding column
        df[EMBEDDING_COLUMN] = None

    # Treat None, NaN, empty strings as "not yet done"
    mask_no_emb = df[EMBEDDING_COLUMN].isna() | (
        df[EMBEDDING_COLUMN].astype(str).str.strip().isin(["", "nan", "None"])
    )
    indices_to_embed = df[mask_no_emb].index.tolist()

    total_to_embed = len(indices_to_embed)
    print(f"Total rows: {len(df)}")
    print(f"Rows not yet embedded: {total_to_embed}")

    if total_to_embed == 0:
        print("Every row is already embedded.")
        return

    # ===== 3. Embed only remaining rows in batches =====
    for batch_start in range(0, total_to_embed, batch_size):
        batch_indices = indices_to_embed[batch_start : batch_start + batch_size]
        batch_texts = df.loc[batch_indices, text_column].astype(str).tolist()

        print(
            f"{batch_start} ~ {batch_start + len(batch_indices) - 1} "
            f"(Actual row index: {batch_indices[0]} ~ {batch_indices[-1]}) "
            f"Requesting embedding... (Total {len(batch_texts)})"
        )

        response = call_with_429_retry(
            client.models.embed_content,
            model=model_name,
            contents=batch_texts,
            config=types.EmbedContentConfig(
                task_type="CLASSIFICATION",
                output_dimensionality=768,
            ),
        )

        # Record results in the corresponding row
        for i, emb in enumerate(response.embeddings):
            row_idx = batch_indices[i]
            df.at[row_idx, EMBEDDING_COLUMN] = emb.values

        # ===== 4. Save immediately after each batch =====
        df.to_csv(output_csv, index=False, encoding="utf-8-sig")
        print(
            f"{len(batch_indices)} rows embedded, saved to '{output_csv}' "
            f"(Cumulative: {batch_start + len(batch_indices)} / {total_to_embed})"
        )
        time.sleep(0.1)

    print(f"\nJob complete! Saved to '{output_csv}'.")



if __name__ == "__main__":
    # Adjust batch_size as needed
    embed_csv_in_chunks(
        INPUT_FILE,
        OUTPUT_FILE,
        text_column=TEXT_COLUMN,
        batch_size=100,
        model_name="gemini-embedding-001",
    )
