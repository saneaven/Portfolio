# BuzzaMaker

*A reinforcement-learning portfolio generator that reads price history and news with a dual-stream Transformer, then lets a PPO agent decide how much of each stock, and cash, to hold every step.*

## Overview

The stock market carries too many variables and too many unforeseeable shocks to script a profitable strategy by hand. BuzzaMaker reframes the problem as one a model can learn instead of one a human has to solve outright: understand the recent past as a time series, take an action, observe the outcome, and improve.

A Transformer reads each stock's price window together with the news that surrounds it (both the stock's own headlines and the broad macro backdrop), and a PPO agent turns that understanding into portfolio weights. The reward is the realised log return net of trading cost, so the policy is pushed toward risk-adjusted gains rather than raw bets. The whole thing runs on a real data pipeline that scrapes prices, crawls news, and embeds it from scratch.

This is a solo project: I designed and built it end to end: the scraping and embedding pipeline, the multi-modal Transformer encoder, and the reinforcement-learning portfolio policy.

## Highlights

- **Dual-stream Transformer encoder.** Each `StockNewsTransformerLayer` runs self-attention over the price series and self-attention over the news embeddings separately, then a stock-to-news cross-attention lets price queries attend to the surrounding headlines. Stock and news streams each get their own 4× GELU feed-forward block.
- **Learnable news gate.** The cross-attention contribution is scaled by a learnable gate parameter initialised to zero, so the model starts out ignoring news and gradually learns how much to trust it during training: a built-in curriculum that keeps early training stable.
- **Portfolio-level aggregation.** `PortfolioMaker` encodes each stock with the per-stock backbone, builds a macro token from global news, then runs self-attention over `[macro, stock₁ … stockₙ]` so cross-asset and macro context shape every stock's representation before the policy head reads it. The macro token is reference-only and dropped before the weights are produced.
- **No future leakage.** Causal masking on the attention and learnable positional embeddings over the price window keep the encoder strictly backward-looking, so it can only ever attend to information it would actually have had at decision time.
- **PPO agent allocates capital every step.** The policy outputs logits over `num_stocks + 1` assets (including cash); a softmax turns them into portfolio weights, and the reward is `log(1 + net_return)` after a turnover-based transaction cost.
- **End-to-end data pipeline.** Real OHLCV history, real news crawled and parsed, and Gemini embeddings: all scraped from scratch with resumable, fault-tolerant collection.

## How it works

### Model architecture

The encoder is implemented in `source/stock_news_backbone.py`:

- `StockNewsTransformerLayer`: one block that does stock self-attention, news self-attention, gated stock-to-news cross-attention, and separate feed-forward networks for each stream. The gate is `nn.Parameter(torch.tensor(0.0))`, added as `stock + gate * cross_attn(...)`.
- `StockNewsAgent`: projects the OHLCV features and the 768-d news embeddings into the model dimension, adds a learnable positional embedding, stacks the Transformer layers, and reads out the final ("today") time-step token per stock.
- `PortfolioMaker`: runs `StockNewsAgent` over every stock in the batch, projects the latest macro-news vector into a single macro token, concatenates `[macro, stock₁ … stockₙ]`, applies multi-head self-attention with a residual norm, then drops the macro token and returns the per-stock features.

`source/portfolio_feature_extractor.py` wraps `PortfolioMaker` as a Stable-Baselines3 `BaseFeaturesExtractor`, so the Transformer becomes the backbone of a PPO `MultiInputPolicy`. It flattens the per-stock features to the `N × d_model` vector SB3 expects.

### The trading environment

`source/stock_portfolio_env.py` defines `PortfolioEnv`, a Gymnasium environment:

- **Observation** is a `Dict` space, produced by sliding a window over the data, with three streams: `stock` (`N × window × F`), `stock_news` (`N × window × 768`), and `global_news` (`window × 768`).
- **Action** is a continuous `Box` of size `num_stocks + 1` (the extra slot is cash); the environment applies a numerically-stable softmax to turn the raw logits into weights.
- **Reward** is the log return: it computes per-asset returns from `P_{t+1}/P_t`, subtracts a transaction cost proportional to portfolio turnover (`Σ|w_t - w_{t-1}|`), then returns `log(1 + net_return)`. Wealth is tracked compounding across the episode, which ends just before the data runs out.

### Training

The training notebook (`buzzamaker_eng.ipynb`) builds the env, configures the PPO policy with the custom feature extractor (`d_model=256`, `n_head=4`, `agent_layers=2`, simple `[64]` policy/value heads, `n_steps=2048`, fixed seed), and calls `model.learn(...)`. Trained checkpoints are saved under `models/` (`portfolio_ppo_v1`…`v4`) and SB3 progress logs under `logs/`. The notebook iterates across versions, varying the transaction-cost assumption (1% for the early runs, 0.1% for the later ones), and evaluates each by replaying the held-out test slice deterministically and reporting final wealth.

### Data pipeline

Built from scratch under `source/data_manager/` and feeding the staged folders in `data_news_pipeline/`:

- **Stock prices**: OHLCV history pulled from Yahoo Finance via `yfinance`, with 2017-2024 used for training and early 2025 held out for testing. Given the cost of broad collection, the trained pipeline runs on Apple (AAPL); `company_list.csv` defines a wider candidate universe of large-cap tickers.
- **Stock-specific news** (`data_scaper.py`): GDELT DOC 2.0 queried in 7-day rolling windows by a producer thread, with article bodies fetched concurrently by a pool of consumer threads using the `newspaper` library. URLs are deduplicated, and a multi-stage on-disk format (step 1 URLs -> step 2 content) makes the crawl resumable and gap-filling after an interruption.
- **Macro news** (`data_scraper_main_news.py`): five themed keyword sets (politics/diplomacy, economy/market, technology/science, society/culture, environment/energy) scraped concurrently through a `ThreadPoolExecutor`, with a retry/backoff HTTP session and a processed-days log so a stopped run resumes where it left off.
- **Embeddings** (`make_embedding.py`): every article title embedded with Google Gemini (`gemini-embedding-001`) into 768-d vectors, batched and written back after each batch so a stopped run picks up where it left off. The embedder also handles `429`/rate-limit responses with bounded waited retries.

## Tech stack

- **Modeling**: PyTorch, multi-head attention, gated cross-attention, learnable positional embeddings.
- **Reinforcement learning**: Stable-Baselines3 PPO, Gymnasium, `MultiInputPolicy` with a custom feature extractor.
- **Data**: `yfinance`, GDELT DOC 2.0, the `newspaper` library, pandas / numpy.
- **Embeddings**: Google Gemini (`google-genai`), 768-d vectors, batched and resumable.
- **Infra**: Python, `python-dotenv`, `ThreadPoolExecutor` / threaded producer-consumer crawling.

## Build & run

This is a Python research project organised around a Jupyter notebook and a set of standalone pipeline scripts; there is no packaged entry point.

1. Use Python 3.12 in a virtual environment and install the dependencies imported across the code: `torch`, `stable-baselines3`, `gymnasium`, `yfinance`, `newspaper3k`, `requests`, `pandas`, `numpy`, `python-dotenv`, and `google-genai`.
2. Provide a Gemini API key in a `.env` file as `GEMINI_API_KEY=...` (read by the embedding script via `python-dotenv`).
3. Build the dataset by running the data-manager scripts: scrape prices and per-stock news (`source/data_manager/data_scaper.py`), scrape macro news (`source/data_manager/data_scraper_main_news.py`), then embed the assembled news/title CSVs with `source/data_manager/make_embedding.py` (point its input path at the per-stock news file and at the macro-news file in turn). All of these steps are resumable, so they can be stopped and restarted.
4. Open `buzzamaker_eng.ipynb` and run the cells to assemble the observation tensors, construct `PortfolioEnv`, train the PPO agent, and evaluate it against the held-out test slice. (The English notebook mirrors `buzzamaker.ipynb`.)

Pre-trained checkpoints are available from the [repository's Releases](https://github.com/saneaven/Portfolio/releases). Download them into `models/`, and evaluation cells can be run without re-training by loading e.g. `PPO.load("./models/portfolio_ppo_v3")`.

## Project structure

```
BuzzaMaker/
├── source/
│   ├── stock_news_backbone.py         # Dual-stream Transformer: layer, per-stock agent, PortfolioMaker
│   ├── stock_portfolio_env.py         # Gymnasium PortfolioEnv (Dict obs, softmax weights, log-return reward)
│   ├── portfolio_feature_extractor.py # SB3 feature extractor wrapping the Transformer backbone
│   └── data_manager/
│       ├── data_scaper.py             # Prices + per-stock GDELT news (threaded, resumable)
│       ├── data_scraper_main_news.py  # Themed macro-news crawler (ThreadPoolExecutor, resumable)
│       └── make_embedding.py          # Gemini title embeddings (batched, resumable)
├── buzzamaker_eng.ipynb               # Training & evaluation notebook (English)
├── buzzamaker.ipynb                   # Training & evaluation notebook
├── company_list.csv                   # Candidate ticker universe
├── data_news_pipeline/                # Staged crawl output: step1_urls -> step2_content -> step3_clean -> step4_embedding
├── data_stocks/                       # OHLCV history per ticker
├── main_news/                         # Macro-news CSVs (raw -> cleaned -> embedded)
├── models/                            # Trained PPO checkpoints (v1…v4), downloaded from Releases
├── logs/                              # SB3 training/progress logs
└── Minjae_Kyung_Project_Proposal.pdf  # Original project proposal
```

## Links

- [Source on GitHub](https://github.com/saneaven/Portfolio/tree/main/projects/ai/BuzzaMaker)

## Status

Solo project, built in 2025. Research prototype: the data pipeline, the multi-modal Transformer encoder, and the PPO portfolio policy are all working, with several trained checkpoints and evaluation runs against a held-out 2025 test window.
