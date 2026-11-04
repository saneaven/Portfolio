from dotenv import load_dotenv
import os
import time
from typing import List, Optional, Dict

import pandas as pd
from google import genai
from google.genai import types
from google.genai import errors as genai_errors


# =========================
#  환경 설정
# =========================

load_dotenv()

API_KEY = os.getenv("GEMINI_API_KEY")

client = genai.Client(api_key=API_KEY)

EMBEDDING_MODEL = "models/gemini-embedding-001"
EMBED_TASK_TYPE = "CLASSIFICATION"

INPUT_FILE = "./data_news_pipeline/step3_clean/AAPL_content_cleaned.csv"
OUTPUT_FILE = "./data_news_pipeline/step4_embedding/AAPL_embedded.csv"

CHUNK_SIZE = 250      # 한 번에 몇 개 문서를 하나의 batch job으로 보낼지
POLL_INTERVAL = 60    # 모든 job 상태를 확인하는 주기(초) = 1분
SLEEP_INTERVAL = 45   # job 제출 시 각 job 사이에 대기 시간(초)
MAX_429_RETRIES = 5   # 429가 계속 나올 때 최대 몇 번까지 재시도할지


# =========================
#  공통: 429 나오면 1분 뒤 재시도하는 래퍼
# =========================

def call_with_429_retry(func, *args, max_retries: int = MAX_429_RETRIES, **kwargs):
    """
    Google GenAI 호출에서 429(RESOURCE_EXHAUSTED)가 나올 경우
    1분(60초) 쉬었다가 같은 요청을 다시 시도한다.
    """
    attempt = 0
    while True:
        try:
            return func(*args, **kwargs)

        # google-genai 전용 에러
        except genai_errors.ClientError as e:
            status = getattr(e, "status_code", None)
            msg = str(e)

            # 429 또는 RESOURCE_EXHAUSTED 텍스트가 보이면 재시도
            if status == 429 or "429" in msg or "RESOURCE_EXHAUSTED" in msg:
                attempt += 1
                if attempt > max_retries:
                    print(f"⚠️ 429가 {max_retries}번 연속 발생해서 더 이상 재시도하지 않습니다.")
                    raise

                print(f"⚠️ 429(RESOURCE_EXHAUSTED) 발생. {attempt}번째 재시도 전 60초 대기...")
                time.sleep(60)
                continue

            # 다른 에러면 그대로 내보냄
            raise


# =========================
#  유틸 함수
# =========================

def load_text_df(csv_path: str) -> pd.DataFrame:
    df = pd.read_csv(csv_path)

    if "FullText" not in df.columns:
        raise ValueError("CSV 파일에 'FullText' 컬럼이 없습니다.")

    df = df.dropna(subset=["FullText"])
    df["FullText"] = df["FullText"].astype(str).str.strip()
    df = df[df["FullText"] != ""].reset_index(drop=True)

    return df


def iter_chunks(df: pd.DataFrame, chunk_size: int):
    """DataFrame 을 chunk_size 단위로 잘라 (start, end, texts 리스트)를 yield."""
    n = len(df)
    for start in range(0, n, chunk_size):
        end = min(start + chunk_size, n)
        texts = df.loc[start:end - 1, "FullText"].tolist()
        yield start, end, texts


# =========================
#  Batch Embedding 관련 함수
# =========================

def submit_embed_job(texts: List[str], display_name: str):
    """
    inlined_requests 방식으로 Embeddings 배치 잡 생성.
    EmbeddingsBatchJobSource(inlined_requests=EmbedContentBatch(...)) 구조를 그대로 사용.
    """

    # EmbedContentBatch: config + contents
    embed_batch = types.EmbedContentBatch(
        config=types.EmbedContentConfig(
            task_type=EMBED_TASK_TYPE,
            # 필요시 차원 축소:
            # output_dimensionality=256,
        ),
        contents=[
            types.Content(
                parts=[types.Part.from_text(text=t)]
            )
            for t in texts
        ],
    )

    src = types.EmbeddingsBatchJobSource(
        inlined_requests=embed_batch
    )

    # 🔴 여기서 429 나면 1분 뒤 재시도
    batch_job = call_with_429_retry(
        client.batches.create_embeddings,
        model=EMBEDDING_MODEL,
        src=src,
        config=types.CreateEmbeddingsBatchJobConfig(
            display_name=display_name,
        ),
    )

    return batch_job


def extract_embeddings_from_job(job) -> List[Optional[List[float]]]:
    """
    BatchJob.dest.inlined_embed_content_responses 에서 임베딩 벡터 리스트 추출.
    [벡터, 벡터, ...] 형태로 반환 (실패한 건 None).
    """
    dest = job.dest
    if not dest or not dest.inlined_embed_content_responses:
        raise RuntimeError(
            "dest.inlined_embed_content_responses 가 없습니다. "
            "BatchJobDestination 설정 또는 SDK 버전을 확인하세요."
        )

    # 이건 이미 리스트
    inline_responses = dest.inlined_embed_content_responses
    embeddings: List[Optional[List[float]]] = []

    for idx, inline_res in enumerate(inline_responses):
        if inline_res.error:
            print(f"   ⚠️ 인덱스 {idx} 에러: {inline_res.error}")
            embeddings.append(None)
            continue

        resp = inline_res.response
        if not resp or not resp.embeddings:
            embeddings.append(None)
            continue

        vec = list(resp.embeddings[0].values)
        embeddings.append(vec)

    return embeddings


# =========================
#  메인 파이프라인 (비동기 느낌 버전)
# =========================

def embed_with_batch_api_async_style(
    input_csv: str,
    output_csv: str,
    chunk_size: int = CHUNK_SIZE,
):
    df = load_text_df(input_csv)
    total = len(df)
    print(f"총 {total} 개 row 로드 완료")

    # 최종 결과를 담을 배열
    all_embeddings: List[Optional[List[float]]] = [None] * total

    # 1단계: 모든 청크에 대해 job 제출 (기다리지 않고)
    jobs: Dict[str, dict] = {}  # job_name -> info(start, end, size)

    chunk_idx = 0
    for start, end, texts in iter_chunks(df, chunk_size):
        print(f"\n=== submit chunk {chunk_idx} | row {start} ~ {end - 1} | {len(texts)} 개 ===")

        job = submit_embed_job(
            texts,
            display_name=f"embed_chunk_{chunk_idx}",
        )
        print(f"   ➤ 제출된 job.name = {job.name}")

        jobs[job.name] = {
            "start": start,
            "end": end,
            "size": len(texts),
            "done": False,
        }

        chunk_idx += 1
        time.sleep(SLEEP_INTERVAL)

    print(f"\n총 {len(jobs)} 개 batch job 제출 완료.")
    if not jobs:
        print("보낼 텍스트가 없습니다.")
        return

    # 2단계: 1분마다 모든 job 상태를 확인하면서, 끝난 것만 결과 수집
    remaining = set(jobs.keys())
    print(f"\n⏳ 모든 job 완료 대기 시작 (interval = {POLL_INTERVAL}초) ...")

    while remaining:
        print(f"\n--- 상태 체크, 남은 job 수: {len(remaining)} ---")

        finished_now = []

        for job_name in list(remaining):
            # 🔴 상태 조회에서도 429 나올 수 있으니 여기에도 리트라이
            job = call_with_429_retry(client.batches.get, name=job_name)
            state = job.state
            print(f"   {job_name}: {state}")

            if state == "JOB_STATE_SUCCEEDED":
                info = jobs[job_name]
                start = info["start"]
                end = info["end"]

                # 이 job 결과에서 임베딩 추출
                chunk_embeddings = extract_embeddings_from_job(job)

                if len(chunk_embeddings) != info["size"]:
                    print(
                        f"   ⚠️ 경고: 응답 개수({len(chunk_embeddings)})와"
                        f" 청크 크기({info['size']}) 불일치"
                    )

                # all_embeddings 에 채우기
                for offset, emb in enumerate(chunk_embeddings):
                    idx = start + offset
                    if idx < total:
                        all_embeddings[idx] = emb

                jobs[job_name]["done"] = True
                finished_now.append(job_name)

            elif state in ("JOB_STATE_FAILED", "JOB_STATE_CANCELLED"):
                print(f"   ❌ job 실패 또는 취소: {job_name}, state={state}, error={job.error}")
                jobs[job_name]["done"] = True
                finished_now.append(job_name)

            # 나머지 상태(PENDING, RUNNING 등)는 다음 라운드에서 다시 확인

        # 이번 라운드에서 끝난 job 제거
        for job_name in finished_now:
            remaining.discard(job_name)

        if remaining:
            print(f"잠깐 쉼... {POLL_INTERVAL}초 후 다시 상태 확인")
            time.sleep(POLL_INTERVAL)

    print("\n✅ 모든 batch job 완료, DataFrame에 임베딩 채우는 중...")

    df["embedding"] = all_embeddings
    df.to_csv(output_csv, index=False, encoding="utf-8-sig")

    print(f"\n🎉 임베딩 완료! 결과 파일: {output_csv}")


if __name__ == "__main__":
    embed_with_batch_api_async_style(
        input_csv=INPUT_FILE,
        output_csv=OUTPUT_FILE,
        chunk_size=CHUNK_SIZE,
    )
