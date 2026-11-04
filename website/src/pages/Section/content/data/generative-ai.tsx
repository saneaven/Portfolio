import type { ContentBlocks } from '../contentblock/types'
import githubIcon from '../../../../assets/github-white-icon.svg'
import GenerativeAiArchDiagram from './generative-ai.diagram'

const generativeAi: ContentBlocks = [
  {
    type: 'lead',
    eyebrow: 'AI · Generative AI',
    title: 'Generative AI',
    subtitle:
      'A self-contained tour of modern generative AI built module by module: sampling and decoding on a raw language model, multi-model APIs with typed JSON output, tool-using agents, diffusion with spatial control, quantised local inference, and a complete pipeline that synthesises a dataset, fine-tunes a model with QLoRA and ships it to the Hub — closing with a written look at where humans should stay in the loop.',
    meta: [
      { label: 'Role', value: 'Solo — implemented all eight modules' },
      { label: 'Focus', value: 'LLMs · diffusion · agents · fine-tuning' },
      { label: 'Stack', value: 'PyTorch · HuggingFace · llama.cpp · Gradio · OpenRouter' },
    ],
  },
  {
    type: 'section',
    eyebrow: 'Overview',
    title: 'One running theme: build each layer of the stack by hand',
    blocks: [
      {
        type: 'text',
        paragraphs: [
          'Rather than skim the field, each module isolates one capability and builds something working around it — a decoder you can feel, a typed API client, an agent that actually calls tools, a diffusion canvas, a local model, a fine-tuned checkpoint. The modules stack into a single arc: understand generation, drive it through APIs, give it tools, give it other modalities, run it locally, then train your own.',
          'The back half is the part I care about most: an end-to-end loop that turns public-domain text into a synthetic training set, fine-tunes a small model on it with parameter-efficient methods, and publishes the result — the same shape a real product team would use, done solo at small scale.',
        ],
      },
    ],
  },
  {
    type: 'section',
    eyebrow: 'Foundations',
    title: 'Decoding strategies on a raw language model',
    blocks: [
      {
        type: 'text',
        paragraphs: [
          'The opening module loads GPT-2 Medium from HuggingFace Transformers and builds an autocomplete around it, then makes the decoding strategy tunable — greedy versus sampling with temperature, top-k and top-p — so the effect of each knob on coherence and diversity is something you watch happen rather than read about.',
          'Running the same prompt through every setting (and across English and Korean continuations) turns the abstract sampling parameters into intuition: where greedy collapses into loops, where temperature tips into noise, and where nucleus sampling lands in between.',
        ],
      },
    ],
  },
  {
    type: 'section',
    eyebrow: 'APIs & agents',
    title: 'Typed output, then tools and handoffs',
    blocks: [
      {
        type: 'cardGrid',
        cards: [
          {
            title: 'Multi-model comparison',
            body: 'A Gradio UI fans one prompt out to seven production models through the OpenRouter API and shows the answers side by side, making model choice an A/B you can see instead of a guess.',
          },
          {
            title: 'Structured output',
            body: 'A travel-itinerary planner uses a system prompt to pin down ambiguous requests and Pydantic models (Waypoints, TravelItinerary) to force valid JSON back, so the response is data the app can use directly.',
          },
          {
            title: 'Multi-agent system',
            body: 'A campus assistant on the OpenAI Agents SDK routes between five specialised sub-agents — buildings, courses, handbook, cafes, internships — handing off to whichever one fits the question.',
          },
          {
            title: 'Retrieval & web tools',
            body: 'The agents combine a FileSearch tool over a vector store with a WebSearch tool for live information, with streaming events and tool calls surfaced in the UI so you can watch the agent think.',
          },
        ],
      },
    ],
  },
  {
    type: 'section',
    eyebrow: 'Other modalities',
    title: 'Drawing with diffusion, and running a model locally',
    blocks: [
      {
        type: 'minis',
        items: [
          {
            title: 'ControlNet image generation',
            body: 'A ControlNet Scribble model on Stable Diffusion 1.5 (via the diffusers library) turns a rough sketch on a Gradio canvas plus a text prompt into a guided image — spatial control over the generation rather than prompt-only.',
          },
          {
            title: 'Local quantised inference',
            body: 'A Gemma 3 GGUF model runs locally through llama-cpp-python with streaming chat completion, so generation happens on-device with no API round-trip.',
          },
          {
            title: 'Streaming tool calls',
            body: 'A novel-editing agent parses tool calls out of the live token stream with partial-json-parser, applying text patches as the model emits them instead of waiting for a complete response.',
          },
          {
            title: 'Honest constraints',
            body: 'Swapping in a newer model surfaced a real llama.cpp / binding version mismatch — the kind of dependency friction that comes with running models yourself, noted rather than hidden.',
          },
        ],
      },
    ],
  },
  {
    type: 'section',
    eyebrow: 'Data → fine-tune → deploy',
    title: 'Synthesise a dataset, fine-tune a model, ship it',
    blocks: [
      {
        type: 'pipeline',
        title: 'QLoRA fine-tuning pipeline',
        body: 'The capstone loop takes ~2,880 public-domain novels (1851–1875) out of XML, uses several LLMs through typed Pydantic schemas to generate a lorebook and outline per novel, and renders novel-writing prompts through Jinja2 templates into a train/validation/test split. A Qwen3-1.7B base model is then loaded in 4-bit nf4 (BitsAndBytes), adapted with LoRA (rank 16, alpha 32) and trained with TRL\'s SFTTrainer under Wandb tracking — overfitted and failed runs included as part of the learning — before the adapter is merged back and the weights pushed to the HuggingFace Hub.',
        steps: [
          'Extract novels from XML',
          'LLM-generate lorebooks & outlines',
          'Template prompts → train/val/test split',
          'QLoRA fine-tune Qwen3-1.7B (4-bit)',
          'Merge adapter · push to HF Hub',
        ],
      },
      {
        type: 'custom',
        render: () => <GenerativeAiArchDiagram />,
      },
    ],
  },
  {
    type: 'section',
    eyebrow: 'Ethics',
    title: 'Where the human stays in the loop',
    blocks: [
      {
        type: 'text',
        paragraphs: [
          'The final module steps back from the code to a written analysis of automation bias and human oversight — why people over-trust automated output under stress, and how human-in-the-loop versus human-on-the-loop architectures change that. It is a deliberate counterweight to the rest of the work: a reminder that knowing how to build these systems is only half of knowing when and how they should be used.',
        ],
      },
    ],
  },
  {
    type: 'section',
    eyebrow: 'Stack',
    title: 'What it\'s built on',
    blocks: [
      {
        type: 'stack',
        rows: [
          { layer: 'Language models', items: ['HuggingFace Transformers', 'GPT-2', 'Qwen3', 'Gemma 3'] },
          { layer: 'Fine-tuning', items: ['TRL SFTTrainer', 'PEFT / LoRA', 'BitsAndBytes (4-bit)', 'Wandb'] },
          { layer: 'APIs & agents', items: ['OpenAI Agents SDK', 'OpenRouter', 'FileSearch', 'WebSearch'] },
          { layer: 'Other modalities', items: ['diffusers', 'ControlNet', 'Stable Diffusion 1.5', 'llama-cpp-python'] },
          { layer: 'Infra & UI', items: ['Python', 'Gradio', 'Pydantic', 'Jinja2', 'HuggingFace Hub'] },
        ],
      },
    ],
  },
  {
    type: 'links',
    links: [
      {
        label: 'Source on GitHub',
        href: 'https://github.com/saneaven/Portfolio/tree/main/projects/ai/GenAI-Learning',
        icon: githubIcon,
        hint: 'Code & technical docs',
      },
    ],
  },
  {
    type: 'closing',
    text: 'Eight solo modules across the generative-AI stack — decoding, typed APIs, tool-using agents, diffusion, local inference and a full synthesise → QLoRA-fine-tune → deploy pipeline, bookended by a written look at keeping humans in the loop.',
  },
]

export default generativeAi
