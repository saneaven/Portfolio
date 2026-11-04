import type { ContentBlocks } from '../contentblock/types'
import novelBudsIcon from '../../../../assets/section/ai/novelbuds/novelbuds_icon.svg'
import overall from '../../../../assets/section/ai/raonverse/overall.avif'
import editWithAgent from '../../../../assets/section/ai/raonverse/edit_with_agent.avif'
import timelineBranch from '../../../../assets/section/ai/raonverse/timeline_branch.avif'
import playChat from '../../../../assets/section/ai/raonverse/play_chat.avif'
import playVn from '../../../../assets/section/ai/raonverse/play_vn.avif'
import playNovel from '../../../../assets/section/ai/raonverse/play_novel.avif'
import modulePage from '../../../../assets/section/ai/raonverse/module.avif'
import { Media } from '../contentblock/primitives'
import {
  AgentRuntimeDiagram,
  ModuleStackDiagram,
  RetrievalDiagram,
  TimelineLinesDiagram,
} from './raon-verse.diagram'
import RaonVerseLines from './raon-verse.lines'
import RaonVerseSortLines from './raon-verse.sort-lines'

const raonVerse: ContentBlocks = [
  {
    type: 'lead',
    eyebrow: 'AI · Agentic platform · In development',
    title: 'One multiverse of story entities, written, played and branched with agents.',
    subtitle:
      'Raon:Verse is the successor to Novel Buds: a platform where novels, visual novels and chats are built on the same story entities inside one Multiverse, every LLM actor is an agent, and a timeline-based world model gives those agents the state of the world at any moment, in any branch.',
    meta: [
      { label: 'Role', value: 'Solo: product design, architecture, frontend, backend' },
      { label: 'Focus', value: 'Human-agent collaboration · timeline-based RAG' },
      { label: 'Stack', value: 'FastAPI · PostgreSQL 18 + pgvector · React 19' },
      { label: 'Status', value: 'In development · source private' },
    ],
    image: { caption: 'A Multiverse home with the agent drawer open', image: overall },
  },
  {
    type: 'section',
    eyebrow: 'Overview',
    title: 'Starting over from what Novel Buds taught',
    blocks: [
      {
        type: 'text',
        paragraphs: [
          'Novel Buds proved that an agent can work inside a story project. Building it also showed where its structure stops scaling: a project held exactly one novel and one timeline, agent edits could not be traced or cleanly undone, two separate runtimes split the agents, and retrieval treated the story as text chunks sitting beside the timeline rather than inside it.',
          'Raon:Verse starts over from those lessons. Novels, visual novels and chats share the same story entities inside one Multiverse, every LLM actor is an agent, and the timeline itself becomes the structure the agent reads the world through.',
        ],
      },
      {
        type: 'cardGrid',
        cards: [
          {
            title: 'One novel per project → one shared Multiverse',
            body: 'A Novel Buds project owned one outline, one manuscript and one timeline. In Raon:Verse the project is a Multiverse of shared story entities, and any number of novels, visual novels and chats are built on top of it, each in whichever Verse it needs.',
          },
          {
            title: 'Two runtimes → one agent model',
            body: 'Structured Journeys and free-form Agents were separate systems with a fixed list of Journey kinds. Now every LLM actor, from the writing assistant to a character in a chat, runs on the same loop, the same tools and the same approval rules.',
          },
          {
            title: 'Untraceable edits → every write is a Mutation',
            body: 'Rolling back an agent edit meant restoring a whole object, and deletes were permanent. Now every tool call records a Mutation with inverse effects, so any change can be undone, any object restored to a past revision, and deletes go to a trash.',
          },
          {
            title: 'All-or-nothing approval → per-call consent',
            body: 'Pending tool calls were approved or rejected together behind one user-wide auto switch. Now each call is approved against a fingerprint of its exact plan, with ask or auto per conversation, and per-field version checks stop the agent from overwriting what the author just typed.',
          },
          {
            title: 'In-process runs → durable workers',
            body: 'Runs lived inside the API process with an in-memory event bus. Now they execute in a worker, jobs are queued in the same transaction as the change that caused them, and the event stream replays from wherever a client left off.',
          },
          {
            title: 'Text chunks beside a timeline → a timeline the agent reads through',
            body: 'Retrieval embedded story text while the timeline sat apart as its own feature. Now time, Verse, knowledge and importance are first-class structure, and search joins them in at query time.',
          },
        ],
      },
    ],
  },
  {
    type: 'section',
    eyebrow: 'Core: Agent collaboration',
    title: 'Built to be worked on with an agent',
    blocks: [
      {
        type: 'text',
        paragraphs: [
          'Raon:Verse is designed on one premise: the author works together with an agent, rather than calling on one now and then. The agent is always on hand in a drawer that opens from every screen, it sees what the author is looking at, and almost everything in the app is within its reach: entities, timelines, manuscripts, plays, modules and settings.',
          'That reach comes from the architecture, not from a list of special tools. UI edits and agent tool calls run through the same Mutation executor and the same verbs, so the agent can do what the author can, and every change it makes lands in the same History, reviewable and undoable. The writing assistant, the translator, a character speaking in a chat and the subagents they spawn all run on this one hierarchy (thread, Journey, Run, Step, ToolCall).',
        ],
      },
      {
        // Keep the three-pane screenshot at full width.
        type: 'custom',
        render: () => <Media caption="The agent drawer beside an entity editor: tool calls on the rail, and an approval card for a delete" image={editWithAgent} />,
      },
      {
        type: 'custom',
        render: () => <AgentRuntimeDiagram />,
      },
      {
        type: 'cardGrid',
        cards: [
          {
            title: 'One drawer, on every screen',
            body: 'The agent lives in a global drawer that opens with the same button on the home screen and inside any project. Each message carries the screen, object and selection the author is looking at, and @ hands it any object, so “tighten this scene” or “put her on the timeline” needs no setup.',
          },
          {
            title: 'Anything the author can do',
            body: 'Agents work through one fixed set of verbs (read, create, copy, update, patch, move, delete, restore, undo) across every object type, on the same path the UI uses. Revertibility, soft delete and permissions are enforced by the engine, so a new feature is reachable by the agent the day it exists.',
          },
          {
            title: 'Working side by side',
            body: 'The author and the agent can edit at the same time. Per-field version checks stop the agent from overwriting what the author just typed, and when the author changes something the agent has read, the agent is told what changed before its next step and re-reads it before writing.',
          },
          {
            title: 'Standard and Pro strategies',
            body: 'One per-thread switch shared by every agent type. In Pro the lead agent dispatches independent subagents in parallel, has them check each other adversarially, and synthesises by evidence rather than consensus, without changing the tools, approvals or limits.',
          },
          {
            title: 'Subagents that can only narrow',
            body: 'A child inherits its parent’s tool catalog and can only remove from it through its preset’s access and spawn settings, so a read-only child’s child can never gain write access. Subagents run in the background and report back as messages, not tool results.',
          },
          {
            title: 'Undo as the safety net',
            body: 'Because every tool call is one Mutation with inverse effects, letting the agent act on its own is safe: the safety net is undo, not approval. Ask mode approves each call against its exact plan; Auto mode applies at once, and undo first checks that the change still holds.',
          },
          {
            title: 'Errors written for the agent',
            body: 'Every result is an ok / error envelope whose hint addresses the model, not the user. Schema errors return the type’s reference so the agent can correct itself, and a stale write comes back as a version conflict that names what to re-read.',
          },
          {
            title: 'Scoped memory',
            body: 'Agent memory lives at user, Multiverse, Verse and thread scope as versioned, undoable objects; thread scope is the scratchpad a lead agent shares with its subagents. Facts about the story stay out of memory and in the world model.',
          },
        ],
      },
      {
        type: 'pipeline',
        title: 'Runs that can wait',
        body: 'Every tool call is persisted and moves through an explicit lifecycle. While a call waits for approval or background work the run parks, and an external completion schedules it again, so an approval can arrive minutes later or after a restart.',
        steps: ['Streaming', 'Queued', 'Pending', 'Running', 'Done'],
      },
    ],
  },
  {
    type: 'section',
    eyebrow: 'Core: Timeline-based RAG',
    title: 'The world as a structure, not a pile of chunks',
    blocks: [
      {
        type: 'split',
        paragraphs: [
          'Stories in Raon:Verse branch constantly: alternate worlds, time travel, a different choice in a visual novel. A plain index over story text cannot tell which version of a character a paragraph describes, which world it belongs to, or whether the character speaking should know it.',
          'So the timeline is the backbone. The world, every entity and every story each live on a line, every record carries a coordinate on it, and the agent reads the world at a coordinate instead of searching loose text.',
        ],
        media: { caption: 'Timeline branch map', image: timelineBranch },
      },
      {
        type: 'custom',
        render: () => <TimelineLinesDiagram />,
      },
      {
        type: 'custom',
        render: () => <RaonVerseLines />,
      },
      {
        type: 'custom',
        render: () => <RaonVerseSortLines />,
      },
      {
        type: 'cardGrid',
        cards: [
          {
            title: 'Branch a world without copying it',
            body: 'Each Verse is a branch of the world’s timeline. It shares everything before the moment it split as the very same records, and characters appear in it through their own lifelines rather than as copies. When a character lives differently there, only their life after the split is stored separately. Six worlds hold one shared past, and fixing that past fixes it everywhere.',
          },
          {
            title: 'Snapshots: who they were then',
            body: 'A snapshot is a character’s profile at one moment of their life: looks, outfits, relationships, secrets. For any scene the agent gets the latest snapshot from before it, so it meets the character as they were then, not as they end up.',
          },
          {
            title: 'Who knows what, and since when',
            body: 'A secret belongs to a character, and each snapshot can word it differently. Knowledge entries on a lifeline mark from which moment a character knows something, so an agent playing that character only knows what the character knows in that scene.',
          },
          {
            title: 'Links that resolve in time',
            body: 'Links tie records together: a scene to the people in it, an event to the person behind it, a father to his son, optionally hidden behind a secret. A link to a person always points to who they were at that moment, in that world.',
          },
          {
            title: 'Custom calendars',
            body: 'Every Multiverse can define its own calendar: its own months, eras that count backwards, leap years and weekdays. Change the calendar and every date in the world is recalculated and checked in one step, or nothing changes at all.',
          },
          {
            title: 'Play writes back as history',
            body: 'What happens during play is saved as events on the timeline, each linked to the scene that caused it, and never written into the shared character profile. One chat can change where a character’s story goes without rewriting them for every other story.',
          },
        ],
      },
      {
        type: 'custom',
        render: () => <RetrievalDiagram />,
      },
      {
        type: 'minis',
        items: [
          {
            title: 'One coordinate system',
            body: '“Now” is a single position: the story’s Verse and world time. Tools default to it, ambiguous positions are refused with the candidates listed, and calendar or overlap violations come back as structured errors the agent can fix.',
          },
          {
            title: 'State is computed, not summarised',
            body: 'For each character on stage the agent gets the selected snapshot and the events overlapping that moment, instead of a summary blob or a paragraph describing an outdated version.',
          },
          {
            title: 'Branching costs nothing',
            body: 'Visibility, knowledge and importance are joined at query time and never copied into chunks, so branching, jumping or revealing a secret needs no re-embedding. Chunks follow Markdown blocks, and only changed chunks are re-embedded.',
          },
          {
            title: 'Questions vectors cannot answer',
            body: 'Time-range filters, timeline and story-stream sources and one-hop Link expansion let the agent ask temporal and relational questions alongside semantic, CJK-aware keyword and hybrid ranking.',
          },
        ],
      },
    ],
  },
  {
    type: 'section',
    eyebrow: 'Play',
    title: 'One world, three ways to play',
    blocks: [
      {
        type: 'rows',
        rows: [
          {
            eyebrow: 'Chat',
            title: 'A messenger where the model plays everyone else',
            body: 'The model plays every character except the user’s persona, each reaction a separate call: messages, read receipts that tell left-on-read from ignored, and stickers chosen by meaning. Scheduled messages go stale instead of firing out of context when the user speaks first.',
            media: { caption: 'Chat play', image: playChat },
          },
          {
            eyebrow: 'Visual Novel',
            title: 'Staged scenes with choices that fork',
            body: 'Each shot sets the speaker, location, sprite emotion and outfit, time of day and weather, or replaces the stage with a generated CG. Choices take an option or a free-written path, and choosing differently at an answered choice opens a new branch.',
            media: { caption: 'Visual novel play', image: playVn },
          },
          {
            eyebrow: 'Novel',
            title: 'A co-writing editor',
            body: 'The play view is the outline and manuscript editor itself. The agent writes into the chapter on screen with writing framing instead of simulator framing, applying changes directly with undo as the safety net.',
            media: { caption: 'Novel co-writing', image: playNovel },
          },
        ],
      },
      {
        type: 'minis',
        items: [
          {
            title: 'Direct or Agent',
            body: 'A play session runs either as one structured completion turned into tool calls, or as a full tool loop with Standard / Pro and a character subagent that speaks and judges as a specific entity, on the same tools and context.',
          },
          {
            title: 'Branch from here',
            body: 'Any node can start a branch storyline that shares everything before it as the same records, so an upstream edit reaches every branch, and splits off its own branch Verse at that world time.',
          },
          {
            title: 'Play variables from history',
            body: 'Variables are never stored as current state. They are recomputed from the assignments recorded on nodes, so branching, going back or viewing the past always shows the values from that moment.',
          },
          {
            title: 'Assets by meaning',
            body: 'The model asks for sprites, backgrounds and stickers by meaning. The server resolves an exact match, then the nearest vector, then a default, pins the result to the shot, and counts misses so authors know what to draw next.',
          },
        ],
      },
    ],
  },
  {
    type: 'section',
    eyebrow: 'Modules',
    title: 'Prompts and play experiences as attachable modules',
    blocks: [
      {
        type: 'split',
        paragraphs: [
          'Everything authored on top of the engine lives in a Module: the prompts agents run on, the Skills they can load, the state and custom nodes a play uses, and the panels and code that render them.',
          'Modules attach to the user, a Multiverse or a single Play, so one world can run as an RPG with levels and dice rolls while another stays a quiet romance, without touching the engine.',
        ],
        media: { caption: 'A Module page: its version and requirements, with agent-side and play-side contents', image: modulePage },
      },
      {
        type: 'custom',
        render: () => <ModuleStackDiagram />,
      },
      {
        type: 'cardGrid',
        cards: [
          {
            title: 'A sparse prompt cascade',
            body: 'Every prompt is a render target such as system:play:chat. The most specific attached module that defines a target wins and the base module defines all of them, so a module overrides exactly what it needs. Prompts grow as append-only blocks, so provider prefix caches survive.',
          },
          {
            title: 'Engine selects, template serialises',
            body: 'Templates are sandboxed Jinja2: the engine chooses what data goes in, and the template only decides how it reads. Each template’s syntax tree is analysed to fetch just the data it uses, authors place their own cache breakpoints, and overrides sit behind an unlock that doubles as an escape hatch.',
          },
          {
            title: 'Skills',
            body: 'Skills follow the Agent Skills format, supporting files included: an index in the system prompt, the full text on demand, and the same Skill usable as a slash command, with each exposure set per context.',
          },
          {
            title: 'Play extensions',
            body: 'State variables, Chat and VN node types and panels extend play. The model places a typed custom card, the user interacts with it, and the result feeds the next turn; rolling again can open a new branch.',
          },
          {
            title: 'Sandboxed surfaces and tools',
            body: 'Module UI runs in an opaque-origin iframe and worker, rendered through Remote DOM and talking to the host only over a consent-granted capability bridge. Python tools run in a server-side WASM sandbox with no access to the database or keys.',
          },
          {
            title: 'Versions you can branch',
            body: 'Branching a version copies a Module, so one world can get its own customisation. Archives carry the modules a world uses, and turning a module on always shows its content for consent, asked again whenever that content changes.',
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
            items: ['Python 3.14', 'FastAPI', 'SQLAlchemy 2 (async)', 'Alembic', 'Pydantic v2', 'Procrastinate', 'Jinja2 (sandboxed)', 'Marktip'],
          },
          { layer: 'Data & search', items: ['PostgreSQL 18', 'pgvector', 'PGroonga', 'Valkey'] },
          {
            layer: 'AI / LLM',
            items: ['OpenAI', 'Anthropic', 'Google GenAI', 'xAI', 'OpenRouter', 'Ollama', 'adapter / dialect layer'],
          },
          {
            layer: 'Frontend',
            items: ['React 19', 'TypeScript 6', 'Vite 8', 'TanStack Router + Query', 'Tiptap 3', 'Base UI', 'Motion', 'Remote DOM'],
          },
          { layer: 'Quality', items: ['pyright', 'ruff', 'import-linter', 'pytest', 'Vitest', 'Playwright', 'GitHub Actions'] },
          { layer: 'Infra', items: ['Docker Compose', 'API + worker split'] },
        ],
      },
    ],
  },
  {
    type: 'links',
    links: [{ label: 'Novel Buds', href: '/ai/novel-buds', icon: novelBudsIcon, hint: 'The predecessor' }],
  },
  {
    type: 'closing',
    text: 'Designed and built solo, from the product plan and data contracts to the agent runtime, backend and frontend. Raon:Verse is still in active development.',
  },
]

export default raonVerse
