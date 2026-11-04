import type { ContentBlocks } from '../contentblock/types'
import mainscreen from '../../../../assets/section/ai/novelbuds/mainscreen.avif'
import storyEntityCard from '../../../../assets/section/ai/novelbuds/story_entity_card.avif'
import timeline from '../../../../assets/section/ai/novelbuds/timeline.avif'
import imageEdit from '../../../../assets/section/ai/novelbuds/image_edit.avif'
import novelBudsIcon from '../../../../assets/section/ai/novelbuds/novelbuds_icon.svg'
import githubIcon from '../../../../assets/github-white-icon.svg'

const novelBuds: ContentBlocks = [
  {
    type: 'lead',
    eyebrow: 'AI · Full-stack',
    title: 'Plan, write and illustrate a novel in one workspace with AI agents.',
    subtitle:
      'Novel Buds folds structural story management and agent-driven generation into a single full-stack platform, so a story moves from planning to draft to localisation without ever leaving the project.',
    meta: [
      { label: 'Role', value: 'Solo full-stack — design, frontend, backend, infra' },
      { label: 'Focus', value: 'Agentic LLM systems & writing UX' },
      { label: 'Stack', value: 'FastAPI · pgvector · React 19 · Docker' },
    ],
  },
  {
    type: 'section',
    eyebrow: 'Overview',
    title: 'One workspace instead of three',
    blocks: [
      {
        type: 'split',
        paragraphs: [
          'Most authors bounce between a writing tool, a pile of setting notes, and a separate set of AI tools. Novel Buds unifies all three into one project workspace: organise characters, places and lore as cards, build the outline by dragging beats around, draft in a rich-text editor, and hand the same agent tasks like “draft chapter three from these character notes”.',
          'Because every entity is multilingual-aware from the ground up, a single project carries a story from first idea through writing and on to localisation.',
        ],
        media: { caption: 'The unified project workspace', image: mainscreen },
      },
    ],
  },
  {
    type: 'section',
    eyebrow: 'Core — AI / LLM Engineering',
    title: 'An agent that reads the whole project and edits it directly',
    blocks: [
      {
        type: 'cardGrid',
        cards: [
          {
            title: 'Multi-provider abstraction',
            body: 'Claude, OpenAI, Gemini, xAI, OpenRouter and Ollama sit behind one interface, with per-project model, thinking mode and effort level. Models without native tool calling get an on-the-fly <tool_call> tag parser; models without native reasoning get prompt-driven chain-of-thought.',
          },
          {
            title: 'Permission-gated tool engine',
            body: 'Tools are gated by read / write / delete / translate / generate scopes. Within them the model patches manuscripts, creates and deletes story entities, and runs semantic search directly — and new tools drop into the modular registry.',
          },
          {
            title: 'Editable prompt pipeline',
            body: 'A Jinja2 template engine composes prompt fragments recursively and versions them, so system and user prompts can be tuned to any voice or style without touching code.',
          },
          {
            title: 'Journeys, agents & MCP',
            body: 'Goal-driven structured Journeys and free-form multi-turn Agents are separate primitives. Sub-agents with their own prompt, model and tools are invoked via call_<name>, and external MCP servers attach per preset.',
          },
          {
            title: 'RAG & conversation memory',
            body: 'Story content is chunked and embedded into pgvector, and a hybrid regex + vector search grounds generation. As a chat grows, older turns are summarised, re-embedded and selectively re-injected so only the relevant detail returns to context.',
          },
        ],
      },
      {
        type: 'pipeline',
        title: 'Human-in-the-loop by default',
        body: 'Every tool call streams to the UI over SSE and moves through an explicit lifecycle. Nothing touches the project until the author accepts it — each step can be reviewed, reshaped or rejected.',
        steps: ['Pending', 'Validating', 'Applying', 'Applied'],
      },
    ],
  },
  {
    type: 'section',
    eyebrow: 'Going further',
    title: 'A model fine-tuned for the agent itself',
    blocks: [
      {
        type: 'text',
        paragraphs: [
          'The platform runs on any major provider, but frontier models are tuned for science and coding and slip on fiction — continuity and consistency are exactly where a novel agent needs to be strongest. So beyond connecting to existing models, I fine-tuned a dedicated one: a QLoRA fine-tune of Gemma 4 31B into a novel-writing agent that drives these same project tools, trained on a synthetic dataset built by a multi-agent data factory.',
        ],
      },
      {
        type: 'links',
        links: [
          {
            label: 'The Novel Buds Agent model',
            href: '/ai/other-projects/novelbuds-agent-model',
            icon: novelBudsIcon,
            hint: 'Fine-tuning deep-dive',
          },
        ],
      },
    ],
  },
  {
    type: 'section',
    eyebrow: 'Product & UX',
    title: 'Built for the way authors actually work',
    blocks: [
      {
        type: 'rows',
        rows: [
          {
            eyebrow: 'Story entities',
            title: 'Characters, places and lore as cards',
            body: 'Characters, organisations, places and lorebooks live as rich cards with images and a folder hierarchy. Framer Motion shared-layout animation expands a card in place, and dnd-kit handles reordering.',
            media: { caption: 'Story entity cards expanding in place', image: storyEntityCard },
          },
          {
            eyebrow: 'Timeline',
            title: 'A timeline in your world’s own calendar',
            body: 'Define custom calendar units for the world, place dated events across multiple tracks, and link each event to the characters, places and chapters it touches.',
            media: { caption: 'Multi-track world timeline', image: timeline },
          },
          {
            eyebrow: 'Illustration',
            title: 'Art generated and approved in place',
            body: 'Generate illustrations across OpenAI, Gemini, xAI, NovelAI and OpenRouter with per-object positive / negative prompts, then move each through a generate → review → approve flow with AVIF output.',
            media: { caption: 'Image generation review queue', image: imageEdit },
          },
        ],
      },
      {
        type: 'minis',
        items: [
          {
            title: 'One project workspace',
            body: 'Brief, outline, manuscript, story entities, timeline and agents all live in a single project view.',
          },
          {
            title: 'Writing tools',
            body: 'A TipTap rich-text editor with autosave and version history, plus a drag-and-drop outline → act → chapter manager.',
          },
          {
            title: 'Multilingual by design',
            body: 'Every content entity carries per-language versions with coverage tracking, AI batch translation, and base-language fallback flagged by a warning badge. The UI itself is localised with i18next.',
          },
          {
            title: 'Realtime sync',
            body: 'SSE pushes object, asset and run-state changes so every open view stays in sync.',
          },
        ],
      },
    ],
  },
  {
    type: 'section',
    eyebrow: 'Stack',
    title: 'What it’s built on',
    blocks: [
      {
        type: 'stack',
        rows: [
          {
            layer: 'Backend',
            items: ['Python 3.12+', 'FastAPI', 'SQLAlchemy + Alembic', 'PostgreSQL 18 + pgvector', 'JWT auth', 'Jinja2'],
          },
          {
            layer: 'AI / LLM',
            items: ['Anthropic', 'OpenAI', 'Google GenAI', 'MCP', 'tiktoken', 'prompt caching'],
          },
          {
            layer: 'Frontend',
            items: ['React 19', 'TypeScript 5.8', 'Vite 7', 'React Router 7', 'Zustand', 'TipTap', 'dnd-kit', 'i18next'],
          },
          {
            layer: 'Infra',
            items: ['Docker Compose', 'Caddy 2 (TLS)', 'AWS Lightsail', 'S3', 'Cloudflare'],
          },
        ],
      },
    ],
  },
  {
    type: 'links',
    links: [
      {
        label: 'Source on GitHub',
        href: 'https://github.com/saneaven/NovelGenerator',
        icon: githubIcon,
        hint: 'Code & technical docs',
      },
      { label: 'Visit Novel Buds', href: 'https://novelbuds.com', icon: novelBudsIcon, hint: 'Live platform' },
    ],
  },
  {
    type: 'closing',
    text: 'Designed and built end to end — product and UX, frontend, backend and infrastructure.',
  },
]

export default novelBuds
