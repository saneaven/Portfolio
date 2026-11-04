# Jamo Tokenization Translator

*A Korean BPE tokenizer that decomposes syllables into jamo before merging, tested end to end on a Korean-to-English Seq2Seq Transformer.*

## Overview

Standard subword tokenizers (BPE, WordPiece, SentencePiece) treat the character or byte as the smallest unit. Korean breaks that assumption: a Hangul syllable is itself a combination of an initial consonant, a medial vowel, and an optional final consonant, and morpheme boundaries often fall *inside* a syllable. In 노랫말 the interfix ㅅ sits inside the syllable 랫, and in 먹었어요 the past-tense ㅆ is fused into 었. A character-level tokenizer can never split those apart.

This project tests a simple hypothesis: decompose each Hangul syllable into its jamo *first*, then let BPE merge. Sub-syllable structure becomes visible, so the tokenizer can land on more morpheme-aligned pieces and represent the language with fewer tokens, especially at small vocabularies and on out-of-domain text. To measure the effect, an identical Korean-to-English Transformer is trained on each tokenizer, isolating the tokenizer as the only variable.

## Role

A two-person research project at DigiPen (2026). I was the main researcher and implementer; Ju-ve Chankasemporn was the co-author. The work spans the tokenizer design, the translation model and training, the quantitative and qualitative evaluation, and the live demo.

## Highlights

- **Reversible jamo decomposition.** Hangul syllables (U+AC00-U+D7A3) decompose deterministically via `S = ((init × 21) + med) × 28 + fin + 0xAC00`. Using Kss's `h2hcj` / `hcj2h` conversions, text round-trips losslessly between syllables and compatibility jamo, so any tokenization is fully reversible: the jamo round-trip is invisible to anything downstream.
- **Two tokenizer implementations, one idea.** A from-scratch pure-Python BPE (`JamoBPE`) written for conceptual clarity over the whole train-merge loop, and a production `HFJamoBPE` built on the HuggingFace `tokenizers` Rust backend with an `</w>` end-of-word suffix, the version used for the real experiments at 32k and 16k vocabularies.
- **Identical Transformer per tokenizer.** An encoder-decoder Transformer (d_model 512, 8 heads, 4 + 4 layers, FFN 1024, sinusoidal positional encoding, causal masking, greedy decoding) is trained twice, once per Korean tokenizer, so any difference is attributable to tokenization alone.
- **Token-count statistics.** A stats harness measures mean / median / stdev / min / p95 / max token length over a chunk of the CC-100 Korean corpus, comparing the jamo and character tokenizers against a tiktoken `o200k_base` baseline.
- **Live demo.** A Streamlit app lets you pick a dataset (small / large / large-16k) and a mode (jamo / full character), type Korean, and see the tokenization and token count side by side, with live Korean-to-English translation on the large models.

## How it works

**Tokenization.** When jamo mode is on, the preprocessing step runs `h2hcj` to expand each syllable into compatibility jamo before training or encoding. Compatibility jamo (U+3131-U+318E) are used so that initial and final consonants share the same glyph, avoiding wasted vocabulary. BPE then proceeds as usual: build word-frequency tuples (each word suffixed with `</w>`), count adjacent pair statistics, merge the most frequent pair, and repeat until the vocabulary target or a minimum-frequency floor is hit. Decoding rejoins tokens, turns `</w>` back into spaces, and optionally reconstructs Hangul with `hcj2h`.

The same `jamo_break` flag drives the character baseline: with it off, no decomposition happens and BPE operates on raw syllables, giving a clean A/B comparison.

**Translation.** The Korean side is encoded with `HFJamoBPE` (jamo or character); the English side uses the pretrained GPT-2 tokenizer. Both feed a `Seq2SeqTransformer` wrapping PyTorch's `nn.Transformer`: scaled embeddings, sinusoidal positional encoding, a causal target mask, and a linear generator over the English vocabulary. Training uses AdamW, an inverse-square-root learning-rate schedule with a 1k-step warmup, label smoothing of 0.1, gradient clipping, and bfloat16 autocast on CUDA. Inference is greedy decoding. The Korean-to-English pairs come from a domain-split JSON dataset (tech and life splits).

**Findings (from the report).** At equal vocabulary size the jamo tokenizer produces fewer tokens on average than the character tokenizer, with the gap widening on small or domain-mismatched corpora and at smaller vocabularies. Qualitatively, jamo segmentation exposes morpheme boundaries inside syllables that character BPE structurally cannot reach. The honest caveat, stated plainly as a limitation: at this model scale the jamo and character training curves were nearly identical, so the tokenization efficiency gain did not translate into measurably better translation.

## Tech stack

- **Language:** Python 3.12
- **Tokenization:** `kss` (jamo decomposition / recomposition), HuggingFace `tokenizers`, `tiktoken` (baseline)
- **Modeling:** PyTorch (`nn.Transformer`), bfloat16 autocast
- **Data:** HuggingFace `datasets`, CC-100 Korean corpus, a Korean-English parallel dataset
- **Demo:** Streamlit
- **Tooling:** `uv` (lockfile + PyTorch CUDA index), `tqdm`, `matplotlib`

## Build & run

The project is managed with `uv` (see `Jamo-basedKoreanTokenization/pyproject.toml` and `uv.lock`); `requirements.txt` is also provided. From the inner `Jamo-basedKoreanTokenization/` directory:

```bash
# install dependencies (uv pulls the CUDA build of torch on Linux/Windows)
uv sync

# launch the Streamlit demo (tokenize + translate live)
uv run streamlit run src/main.py
```

Other entry points (run from the inner directory):

```bash
# train the HF tokenizer (edit the config constants at the top of the script)
python -m src.scripts.train_hf_tokenizer

# token-count statistics vs. the tiktoken baseline
python -m src.scripts.tokenizer_stats

# train a translator on one tokenizer
python -m src.translation.train --kr-tokenizer hf_jamo   # or hf_full

# translate a single sentence from a checkpoint
python -m src.translation.translate --checkpoint <path/to/best.pt> "안녕하세요"
```

The demo's translation panel requires trained checkpoints in `models/` (`jamo_based.pt`, `character_based.pt`); without them the tokenization view still works and the translation panel reports the missing checkpoint.

## Project structure

```
Jamo-basedKoreanTokenization/
├── src/
│   ├── main.py                     # Streamlit demo (tokenize + translate)
│   ├── tokenizer/
│   │   ├── jamo_bpe.py             # from-scratch pure-Python BPE
│   │   ├── hf_jamo_bpe.py          # HuggingFace tokenizers backend
│   │   └── config.py               # default vocab size / min frequency
│   ├── translation/
│   │   ├── model.py                # Seq2SeqTransformer + positional encoding
│   │   ├── train.py                # training loop (AdamW, warmup, bf16)
│   │   ├── dataset.py              # Korean-English pair dataset & collate
│   │   └── translate.py            # checkpoint inference CLI
│   └── scripts/
│       ├── train_hf_tokenizer.py   # train HF tokenizer over CC-100 chunks
│       ├── tokenizer_stats.py      # token-length stats vs. tiktoken
│       └── plot_loss.py            # loss-curve plotting
├── dicts/                          # trained tokenizers (jamo / full, 32k / 16k)
├── datas/                          # corpora and parallel data (sampled)
├── ProjectReport.tex               # full technical write-up
├── pyproject.toml / uv.lock
└── requirements.txt
```

The full technical write-up lives in `Jamo-basedKoreanTokenization/ProjectReport.tex`, with milestone slide decks under `powerpoints/`.

## Links

- [Source on GitHub](https://github.com/saneaven/Portfolio/tree/main/projects/ai/JamoBased%20Tokenization%20Translator)
