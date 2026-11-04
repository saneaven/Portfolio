# Artificial Intelligence

*My AI work: from data analysis and classical regression, through deep learning built from first principles, to reinforcement learning, transformers, and the modern generative-AI stack.*

## Overview

This category collects my machine-learning and AI projects. They span the full range of the field: a from-scratch neural network written by hand in NumPy, convolutional networks and transfer learning, reinforcement-learning agents, custom transformers and tokenizers, large-language-model systems, and pure exploratory data analysis with no model at all.

The recurring thread across these projects is building things from the ground up to understand *why* they work, and reporting honest findings, including where a clever idea did not move the needle. Most are solo work; two are research collaborations. The stack centres on Python, PyTorch and TensorFlow / Keras, scikit-learn, the HuggingFace ecosystem, and LLM APIs (Google Gemini, OpenAI).

## Projects

| Project | What it is | Key stack |
| --- | --- | --- |
| [BuzzaMaker](BuzzaMaker/README.md) | A reinforcement-learning portfolio generator: a dual-stream Transformer encodes stock price history and news, and a PPO agent allocates weights across stocks and cash each step to maximise risk-adjusted log return. | PyTorch, Stable-Baselines3 PPO, Gymnasium, Google Gemini embeddings, yfinance / GDELT data pipeline |
| [Deep Learning](DL-Learning/README.md) | Three deep-learning studies up the ladder: a feedforward net and backprop built by hand in NumPy, CNNs ending in ResNet50V2 transfer learning, and a Double DQN agent that cleans a grid world. | NumPy, TensorFlow / Keras, ResNet50V2, Double DQN, Pygame |
| [Generative AI](GenAI-Learning/README.md) | An eight-module, build-by-hand tour of the modern generative-AI stack: from decoding strategies on a raw language model to a full synthesise -> QLoRA-fine-tune -> deploy pipeline, closing with a written look at keeping humans in the loop. | HuggingFace Transformers, TRL / PEFT / LoRA, BitsAndBytes, diffusers / ControlNet, llama-cpp-python, Gradio, Pydantic |
| [Jamo Tokenization Translator](JamoBased%20Tokenization%20Translator/README.md) | A Korean BPE tokenizer that decomposes syllables into jamo before merging, evaluated end to end on an identical Korean->English Seq2Seq Transformer to isolate the tokenizer's effect. | Python, kss, HuggingFace tokenizers, PyTorch (`nn.Transformer`), Streamlit |
| [LLM Card Game](LLM%20Driven%20Card%20Game/README.md) | A turn-based card battler where one Google Gemini agent is the rules engine and a second is the opponent, kept consistent by Pydantic-typed JSON output. | Google Gemini (`google-genai`), Pydantic, Python state machine |
| [Movie Revenue Prediction](Movie%20Revenue%20Prediction/README.md) | A solo regression study predicting box-office revenue from movie metadata, comparing an MLP (with and without overview text embeddings) against a linear baseline on standard and outlier-heavy test sets. | scikit-learn (`MLPRegressor`, `LinearRegression`), sentence-transformers, pandas / numpy |
| [NYC Taxi Analysis](NY%20Taxi%20Analysis/README.md) | A solo exploratory data analysis joining and cleaning ~14.8M 2013 NYC yellow-taxi trip and fare records to read the city's geography, timing and economics, all in a single Jupyter notebook, no model required. | pandas, matplotlib, gmaps (Google Maps heatmaps), Jupyter |

## Common themes

- **Built from first principles.** Backpropagation by hand in NumPy, a from-scratch pure-Python BPE, a custom multi-stream Transformer, and hand-written RL environments all favour understanding the mechanics over reaching for a black box.
- **Transformers and attention** recur across BuzzaMaker's dual-stream encoder, the jamo tokenizer's Seq2Seq translator, and the generative-AI modules.
- **Reinforcement learning** appears in both the PPO portfolio agent and the Double DQN grid cleaner.
- **Large language models in the loop** (Gemini as a rules engine and opponent, multi-agent assistants, and a QLoRA-fine-tuned writing model) sit alongside classical ML and EDA.
- **Honest reporting.** Several projects lead with where the idea *didn't* win: dying ReLU, embeddings that overfit, a tokenizer gain that didn't reach translation quality, destabilised RL on randomised setups, and a misleading fine-tuning loss curve.
- **A shared toolchain** of Python, PyTorch / TensorFlow, scikit-learn, the HuggingFace ecosystem, and LLM APIs.

## Links

- [Portfolio source on GitHub](https://github.com/saneaven/Portfolio/tree/main/projects/ai)
