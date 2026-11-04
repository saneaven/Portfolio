# Generative AI

*A self-contained tour of the modern generative-AI stack, built module by module: from sampling on a raw language model to a full synthesise -> fine-tune -> deploy pipeline.*

## Overview

This is an eight-module run across the generative-AI stack. Rather than skim the field, each module isolates one capability and builds something working around it: a decoder whose knobs you can feel, a typed API client, an agent that actually calls tools, a diffusion canvas, a local quantised model, and a fine-tuned checkpoint shipped to the Hub. The modules stack into a single arc (understand generation, drive it through APIs, give it tools, give it other modalities, run it locally, then train your own) and close with a written analysis of where humans should stay in the loop.

All eight modules are my own work; this is solo coursework completed at DigiPen.

## Modules

### 1: Decoding strategies on a raw language model

Loads GPT-2 Medium from HuggingFace Transformers and builds an autocomplete around it, then makes the decoding strategy tunable: greedy versus sampling with temperature, top-k and top-p. Running the same prompt through every setting (and across English and Korean continuations) turns the abstract sampling parameters into intuition: where greedy collapses into repetition loops, where high temperature tips into noise, and where nucleus sampling lands in between.

### 2: Multi-model comparison with structured output

A Gradio UI fans a single prompt out to seven production models through the OpenRouter API and shows the answers side by side, making model choice an A/B you can see. The module also builds a travel-itinerary planner: a system prompt pins down ambiguous requests (so "Plan a trip to France" doesn't just bounce back asking for more detail), and Pydantic models force the model to return valid structured JSON the app can consume directly.

### 3: A multi-agent campus assistant

A campus assistant built on the OpenAI Agents SDK routes between specialised sub-agents (buildings, courses, handbook, cafés and an internship agent), handing off to whichever fits the question. Agents combine a FileSearch tool over a vector store with a WebSearch tool for live information; today's date is passed in so time-sensitive answers (internship listings, daily café menus) stay current. Streaming events and tool calls are surfaced in the Gradio UI so you can watch the agent reason.

### 4: Drawing with diffusion (ControlNet)

A ControlNet Scribble model on Stable Diffusion 1.5, via the `diffusers` library, turns a rough sketch on a Gradio canvas plus a text prompt into a guided image: spatial control over the generation rather than prompt-only. The module documents the real dependency friction of trying newer base models and settling on SD 1.5 as the stable path.

### 5: Local quantised inference and streaming tool calls

A Gemma 3 GGUF model runs locally through `llama-cpp-python` with streaming chat completion, so generation happens on-device with no API round-trip. Because the small model emits tool calls as plain content rather than structured fields, the module parses tool calls out of the live token stream with `partial-json-parser` and applies text patches as they arrive, a small novel-editing simulation that previews the larger fine-tuning project. Swapping in a newer model surfaced a genuine `llama.cpp` / binding version mismatch, noted rather than hidden.

### 6: Synthesising a training dataset

The data half of the capstone. It takes the [Wright American Fiction](https://github.com/iulibdcs/Wright-American-Fiction) corpus (roughly 2,880 public-domain novels from 1851-1875 in XML), extracts the prose, and uses several LLMs (the OpenAI and Google Gemini APIs) behind typed Pydantic schemas to generate a lorebook and outline per novel. A set of Jinja2 templates then renders those records into chat-format JSONL training examples that teach a model to write fiction by making tool calls against a structured project database (create characters, build outlines, write and patch manuscript text, search). The rendered examples are split into train / validation / test sets.

The `Module6/templates/` directory holds 25 templates plus a shared macros file, each mapping a novel record to a specific writing task: world-building, outlining, chapter writing, revision passes, continuity checks, POV rewrites, foreshadowing insertion and more. `Module6/template_prompt.md` is the full specification those templates were generated from.

### 7: QLoRA fine-tuning and an honest observation log

The training half of the capstone. A Qwen3-1.7B base model is loaded in 4-bit nf4 (BitsAndBytes, double-quant, bf16 compute), adapted with LoRA (rank 16, alpha 32, dropout 0.05 on the attention and MLP projections: `q`/`k`/`v`/`o` and `gate`/`up`/`down`) and trained with TRL's `SFTTrainer` under Wandb tracking. Only the adapters update; the base weights stay frozen. After training, the adapter is merged back into the base (`merge_and_unload`) and the merged checkpoint is pushed to the HuggingFace Hub.

The accompanying report is deliberately a post-mortem, not a victory lap. It diagnoses why a clean-looking loss curve was misleading (the train/validation split was made across mixed examples rather than across whole novels, so the model was memorising specific novels rather than learning *how* to write one) and why the model learned to fire tool calls indiscriminately. The failed and overfitted runs are kept as part of the record. The two raw transcript logs in `Module7/` show exactly what those failure modes looked like.

### 8: Where the human stays in the loop

A written analysis that steps back from the code to automation bias and human oversight: why people over-trust automated output under time pressure and stress, and how human-in-the-loop versus human-on-the-loop architectures change that. It moves from an everyday context (developers waving through AI-generated code) to an extreme one (military decision-making), draws on cognitive-psychology literature and the 1983 Petrov incident, and argues that "a human makes the final decision" is theoretically sound but a weak safeguard in practice. A deliberate counterweight to the rest of the work.

## How the capstone fits together

```
Wright American Fiction (XML, ~2,880 novels, 1851-1875)
        |  extract prose
        v
  LLMs + Pydantic schemas  -->  lorebook + outline per novel
        |
        v
  Jinja2 templates  -->  chat-format JSONL  -->  train / val / test split
        |
        v
  Qwen3-1.7B (frozen, 4-bit nf4)
   + LoRA r=16 alpha=32 on attention + MLP projections   -->  SFTTrainer (TRL), Wandb-tracked
        |  merge_and_unload
        v
  Merged checkpoint  -->  HuggingFace Hub
```

Only the LoRA adapters train; the quantised base model is frozen throughout. The data pipeline (Module 6) produces the JSONL that the training pipeline (Module 7) consumes.

## Tech stack

- **Language models:** HuggingFace Transformers, GPT-2, Qwen3, Gemma 3
- **Fine-tuning:** TRL `SFTTrainer`, PEFT / LoRA, BitsAndBytes (4-bit nf4), Wandb
- **APIs & agents:** OpenAI Agents SDK, OpenRouter, FileSearch, WebSearch
- **Other modalities:** `diffusers`, ControlNet, Stable Diffusion 1.5, `llama-cpp-python`
- **Infra & UI:** Python, Gradio, Pydantic, Jinja2, `partial-json-parser`, HuggingFace Hub

## Running the modules

Each module is a self-contained Jupyter notebook (`Module{n}/*.ipynb`), built to run on Google Colab or locally with a CUDA GPU. The API-backed modules expect their keys as environment variables / secrets: an OpenRouter key (Module 2), an OpenAI key (Module 3), an OpenAI key plus a Gemini key (`OPENAI_API_KEY` / `GEMINI_API_KEY`, Module 6), and a HuggingFace token plus a Wandb key (Module 7). The local-inference module (5) was run under WSL because no compiled `llama-cpp-python` wheel was available for the needed build on Windows.

Key training hyperparameters for the capstone (Module 7) are set at the top of `train-cuda.ipynb`: base model `Qwen/Qwen3-1.7B`, 4-bit nf4 quantisation, LoRA rank 16 / alpha 32 / dropout 0.05, max sequence length 512, learning rate 2e-4 (cosine schedule, 0.1 warmup), 3 epochs, batch size 4 with 4 gradient-accumulation steps, `paged_adamw_8bit`.

## Project structure

```
GenAI-Learning/
├── Module1/   Decoding strategies on GPT-2 (notebook)
├── Module2/   Multi-model comparison + structured output (notebook)
├── Module3/   Multi-agent campus assistant (notebook)
├── Module4/   ControlNet Scribble on SD 1.5 (notebook)
├── Module5/   Local GGUF inference + streaming tool calls
│   ├── *.ipynb
│   └── test_manuscript.txt
├── Module6/   Dataset synthesis (notebook)
│   ├── template_prompt.md          # spec for the template set
│   └── templates/                  # 25 Jinja2 templates + macros + README
├── Module7/   QLoRA fine-tuning
│   ├── *.docx / *.pdf              # observation log / report
│   ├── *.txt                       # raw failed/overfitted run transcripts
│   └── Hands_on/
│       ├── create-dataset.ipynb    # JSONL -> HuggingFace dataset
│       └── train-cuda.ipynb        # QLoRA training + merge + push
└── Module8/   Human-in-the-loop analysis (report)
```

## Links

- [Source on GitHub](https://github.com/saneaven/Portfolio/tree/main/projects/ai/GenAI-Learning)
