import type { ContentBlocks } from '../contentblock/types'
import githubIcon from '../../../../assets/github-white-icon.svg'
import hfIcon from '../../../../assets/huggingface.svg'
import NovelBudsAgentDiagram from './novelbuds-agent-model.diagram'

const novelBudsAgentModel: ContentBlocks = [
  {
    type: 'lead',
    eyebrow: 'AI · LLM Fine-tuning',
    title: 'Novel Buds Agent',
    subtitle:
      'A QLoRA fine-tune of Google\'s Gemma 4 31B into a dedicated novel-writing agent for Novel Buds. It drives the same project tools — create characters, build outlines, draft and patch prose, search the project — and was trained end to end on a synthetic dataset distilled from public-domain novels by a multi-agent data pipeline.',
    meta: [
      { label: 'Role', value: 'Solo — data pipeline, dataset synthesis, training & deployment' },
      { label: 'Focus', value: 'LLM fine-tuning · agentic tool-use · synthetic data' },
      { label: 'Stack', value: 'Gemma 4 31B · Unsloth · QLoRA · RunPod' },
    ],
  },
  {
    type: 'section',
    eyebrow: 'Overview',
    title: 'Built for fiction, not for STEM',
    blocks: [
      {
        type: 'text',
        paragraphs: [
          'Modern frontier models are tuned for science and coding, and it shows when they write fiction: they slip on continuity — a character with no glasses suddenly pushing them up — and they can\'t convincingly play a character who doesn\'t know something. A novel agent has a harder brief than a chatbot: stay perfectly consistent with everything established so far while still inventing what comes next, and use a toolbox of project actions to do it.',
          'So rather than lean on a general-purpose model, I fine-tuned one for the job. Gemma 4 31B is the sweet spot — large enough to wield the agent\'s tools well, small enough that an individual can actually fine-tune and run it on rented cloud GPUs.',
        ],
      },
    ],
  },
  {
    type: 'section',
    eyebrow: 'Core — The data factory',
    title: 'Turning novels into agent training data',
    blocks: [
      {
        type: 'cardGrid',
        cards: [
          {
            title: 'Public-domain corpus',
            body: 'The raw material is a curated Project Gutenberg fiction collection (organised by Despina), so every novel is copyright-free and safe to train on.',
          },
          {
            title: 'Structured extraction',
            body: 'OpenAI and Gemini, held behind typed Pydantic schemas, read each novel and pull out its characters, locations, lore, outline and chapter structure as a single structured project.',
          },
          {
            title: 'Orchestrator + sub-agents',
            body: 'Processing novels one by one would take forever, so an orchestrator dispatches parallel sub-agents — one novel each — coordinated through a shared [ ] / [-] / [x] progress checklist so hundreds run concurrently.',
          },
          {
            title: 'Render, then fill',
            body: 'Over forty Jinja2 scenario templates render the projects into conversation skeletons, but templates can\'t cover every case — so a second orchestrator pass sends sub-agents to fill the placeholders with long thinking blocks, in-voice prose and realistic tool results.',
          },
        ],
      },
      {
        type: 'custom',
        render: () => <NovelBudsAgentDiagram />,
      },
    ],
  },
  {
    type: 'section',
    eyebrow: 'Training',
    title: 'Fine-tuning a 31B model on one rented GPU',
    blocks: [
      {
        type: 'minis',
        items: [
          {
            title: 'QLoRA on Gemma 4 31B',
            body: 'An Unsloth 4-bit base with LoRA adapters (rank 16, alpha 16) on every attention and MLP projection, across both language and vision layers — trained for a single epoch with train-on-completions and 8-bit AdamW.',
          },
          {
            title: 'A custom chat template',
            body: 'Google\'s official template is built for serving and throws away the thinking content, and Unsloth\'s mis-renders function calls — so I wrote a renderer from scratch that emits Gemma 4\'s turn / channel / tool token stream with thinking, tool calls and tool responses intact, expanding multi-turn samples one per user turn.',
          },
          {
            title: 'Long context, heavy VRAM',
            body: 'Novels are long, so training used a large context window — the main driver of its appetite for VRAM. It ran on an RTX PRO 6000 (about $2/hour on RunPod), roughly 45 hours for a single epoch.',
          },
          {
            title: 'Unsloth Core over the GUI',
            body: 'Unsloth Studio\'s beta GUI lacked fine-grained control over the tuning parameters, so I dropped to Unsloth\'s Python Core library. One epoch was enough — the loss curve stayed stable the whole way.',
          },
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
          { layer: 'Base model', items: ['Gemma 4 31B (instruction-tuned)', 'Unsloth 4-bit (bnb)'] },
          { layer: 'Fine-tuning', items: ['QLoRA / PEFT', 'LoRA r16 · α16', 'Unsloth Core', 'adamw_8bit', '1 epoch'] },
          { layer: 'Data synthesis', items: ['Project Gutenberg (Despina)', 'OpenAI + Gemini', 'Pydantic schemas', 'Jinja2 templates', 'orchestrator + sub-agents'] },
          { layer: 'Tooling', items: ['custom Gemma 4 chat template', 'HuggingFace Hub', 'GGUF (llama.cpp)'] },
          { layer: 'Infra', items: ['RunPod', 'RTX PRO 6000 Blackwell'] },
        ],
      },
    ],
  },
  {
    type: 'links',
    links: [
      {
        label: 'Fine-tuned model (merged)',
        href: 'https://huggingface.co/saneaven/gemma4-31b-novelbuds-agent-merged',
        icon: hfIcon,
        hint: 'Full merged weights',
      },
      {
        label: 'QLoRA adapter',
        href: 'https://huggingface.co/saneaven/gemma4-31b-novelbuds-agent-qlora',
        icon: hfIcon,
        hint: 'LoRA adapter only',
      },
      {
        label: 'GGUF build',
        href: 'https://huggingface.co/saneaven/gemma4-31b-novelbuds-agent-gguf',
        icon: hfIcon,
        hint: 'Quantised for local inference',
      },
      {
        label: 'Training dataset',
        href: 'https://huggingface.co/datasets/saneaven/novelbuds-agent-data-v0',
        icon: hfIcon,
        hint: 'Synthetic agent conversations',
      },
      {
        label: 'Source on GitHub',
        href: 'https://github.com/saneaven/Portfolio/tree/main/projects/ai/NovelAnalyzer',
        icon: githubIcon,
        hint: 'Data pipeline & training code',
      },
    ],
  },
  {
    type: 'closing',
    text: 'Built solo end to end — the public-domain corpus, the multi-agent data factory, a custom Gemma 4 training format, the QLoRA fine-tune on rented GPUs, and the published model in three forms.',
  },
]

export default novelBudsAgentModel
