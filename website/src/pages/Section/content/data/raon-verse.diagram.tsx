import { Fragment } from 'react'
import '../contentblock/diagram.css'

// Source: Raon:Verse's private doc/plan/contracts: data/agent.md, data/module.md,
// core/{consent,versioning,timeline,multiverse-verse}.md and llm/{rag-search,prompt-pipeline}.md.
type Stage = { t: string; s?: string }

function StageColumn({ head, stages, note }: { head: string; stages: Stage[]; note?: string }) {
  return (
    <div className="cb__dg-col">
      <div className="cb__dg-col-head">{head}</div>
      {stages.map((stage, i) => (
        <Fragment key={stage.t}>
          {i > 0 && <span className="cb__dg-link cb__dg-link--sm" />}
          <div className="cb__dg-node">
            <div className="cb__dg-node-title">{stage.t}</div>
            {stage.s && <div className="cb__dg-node-sub">{stage.s}</div>}
          </div>
        </Fragment>
      ))}
      {note && <div className="cb__dg-col-note">{note}</div>}
    </div>
  )
}

const JOURNEY_KINDS: Stage[] = [
  { t: 'General', s: 'writing assistant' },
  { t: 'Play', s: 'Direct · Agent' },
  { t: 'Utility', s: 'translation · document · image prompt' },
  { t: 'Subagent', s: 'inherits tools · narrows only' },
]

export function AgentRuntimeDiagram() {
  return (
    <figure className="cb__dg">
      <figcaption className="cb__dg-cap">
        Every LLM actor runs on one hierarchy, and every write it makes goes through the same
        Mutation executor as the author&apos;s own edits, so it can be reviewed, undone or restored.
      </figcaption>

      <div className="cb__dg-node">
        <div className="cb__dg-node-title">AgentThread</div>
        <div className="cb__dg-node-sub">ask / auto · plan / work · standard / pro</div>
      </div>
      <span className="cb__dg-link" />

      <div className="cb__dg-group">
        <span className="cb__dg-badge">Journey kinds</span>
        <div className="cb__dg-row">
          {JOURNEY_KINDS.map((k) => (
            <div className="cb__dg-node" key={k.t}>
              <div className="cb__dg-node-title">{k.t}</div>
              <div className="cb__dg-node-sub">{k.s}</div>
            </div>
          ))}
        </div>
        <div className="cb__dg-tag">Pro · parallel subagents · adversarial cross-check</div>
      </div>

      <span className="cb__dg-link" />
      <div className="cb__dg-node">
        <div className="cb__dg-node-title">Run → Step → ToolCall</div>
        <div className="cb__dg-node-sub">persisted · parks on approval or background work · resumes</div>
      </div>

      <span className="cb__dg-link" />
      <div className="cb__dg-node cb__dg-node--alt">
        <div className="cb__dg-node-title">Consent</div>
        <div className="cb__dg-node-sub">apply · approval · plan changed · restricted · plan fingerprint</div>
      </div>

      <span className="cb__dg-link" />
      <div className="cb__dg-node">
        <div className="cb__dg-node-title">Mutation executor</div>
        <div className="cb__dg-node-sub">shared with REST · per-field CAS · content / lifecycle / move effects</div>
      </div>

      <span className="cb__dg-link" />
      <div className="cb__dg-out">
        <div className="cb__dg-node-title">History</div>
        <div className="cb__dg-node-sub">undo · point restore · trash</div>
      </div>
    </figure>
  )
}

const LINES: { head: string; stages: Stage[]; note: string }[] = [
  {
    head: 'Worldline',
    stages: [
      { t: 'Verse', s: 'each branch is a Verse' },
      { t: 'Tracks · events', s: 'worldline_time · importance' },
    ],
    note: 'dated in the world’s own calendar',
  },
  {
    head: 'Lifeline',
    stages: [
      { t: 'StoryEntity', s: 'one lifeline per Verse it lives in' },
      { t: 'Snapshots · knowledge · jumps', s: 'lifeline_time from birthdate' },
    ],
    note: 'who they were, and what they knew',
  },
  {
    head: 'Storyline',
    stages: [
      { t: 'Play', s: 'root + branch storylines' },
      { t: 'Scene & parasitic nodes', s: 'messages · shots · chapters · choices' },
    ],
    note: 'a story branch forks the Verse too',
  },
]

export function TimelineLinesDiagram() {
  return (
    <figure className="cb__dg">
      <figcaption className="cb__dg-cap">
        The world, every entity and every story live on lines of the same shape: a tree of branches,
        where a branch stores only what follows its branch point and inherits the past as the very
        same records.
      </figcaption>

      <div className="cb__dg-node">
        <div className="cb__dg-node-title">Multiverse</div>
        <div className="cb__dg-node-sub">shared StoryEntities · calendar · Plays</div>
      </div>
      <span className="cb__dg-link" />

      <div className="cb__dg-cols">
        {LINES.map((line) => (
          <StageColumn key={line.head} {...line} />
        ))}
      </div>

      <span className="cb__dg-link" />
      <div className="cb__dg-tag">every record = line time + world position (Verse, world time)</div>
      <span className="cb__dg-link" />

      <div className="cb__dg-node cb__dg-node--alt">
        <div className="cb__dg-node-title">Links</div>
        <div className="cb__dg-node-sub">
          snapshot / event / node → entity / event / node · resolve in time · optional secret gate
        </div>
      </div>
    </figure>
  )
}

export function RetrievalDiagram() {
  return (
    <figure className="cb__dg">
      <figcaption className="cb__dg-cap">
        Retrieval starts from one coordinate, the story&apos;s current position, and returns the world
        as it stands there: computed state for whoever is on stage, ranked search for everything else.
      </figcaption>

      <div className="cb__dg-node">
        <div className="cb__dg-node-title">Storyline current position</div>
        <div className="cb__dg-node-sub">Verse · world time · the cast&apos;s lifeline positions</div>
      </div>

      <span className="cb__dg-fork" />
      <div className="cb__dg-cols cb__dg-cols--pair">
        <StageColumn
          head="Deterministic context"
          stages={[
            { t: 'Cast & lore', s: 'persona · lore list · recent speakers' },
            { t: 'State at this moment', s: 'selected snapshot · overlapping events · known secrets only' },
          ]}
        />
        <StageColumn
          head="search tool"
          stages={[
            { t: 'Source', s: 'scope · story stream · links · timeline' },
            { t: 'Filter → rank', s: 'time range · pgvector · PGroonga · hybrid RRF · 1-hop Links' },
          ]}
        />
      </div>

      <span className="cb__dg-link" />
      <div className="cb__dg-tag">visibility · knowledge · importance joined at query time, never copied into chunks</div>
      <span className="cb__dg-link" />

      <div className="cb__dg-out">
        <div className="cb__dg-node-title">Agent context</div>
        <div className="cb__dg-node-sub">front-loaded per session · topped up per run · each chunk once per Journey</div>
      </div>
    </figure>
  )
}

const ATTACHMENT_STACK = ['Play', 'Multiverse', 'User']

export function ModuleStackDiagram() {
  return (
    <figure className="cb__dg">
      <figcaption className="cb__dg-cap">
        A Module bundles prompts, play extensions and code. For each prompt target the most specific
        attached module that defines it wins, and the shipped base module covers the rest.
      </figcaption>

      <div className="cb__dg-group">
        <span className="cb__dg-badge">Module</span>
        <div className="cb__dg-cols">
          <StageColumn
            head="Agent side"
            stages={[
              { t: 'Prompt templates', s: 'fragments · template variables' },
              { t: 'Skills', s: 'subagent presets · theme' },
            ]}
          />
          <StageColumn
            head="Play side"
            stages={[
              { t: 'State variables', s: 'Chat & VN node types' },
              { t: 'Panels', s: 'scripts · Python tools' },
            ]}
          />
        </div>
      </div>

      <span className="cb__dg-link" />
      <div className="cb__dg-group">
        <span className="cb__dg-badge">attachment stack · first match wins</span>
        {ATTACHMENT_STACK.map((scope) => (
          <Fragment key={scope}>
            <div className="cb__dg-node">
              <div className="cb__dg-node-title">{scope}</div>
            </div>
            <span className="cb__dg-link cb__dg-link--sm" />
          </Fragment>
        ))}
        <div className="cb__dg-node cb__dg-node--alt">
          <div className="cb__dg-node-title">base</div>
          <div className="cb__dg-node-sub">shipped with the server · defines every target</div>
        </div>
      </div>

      <span className="cb__dg-link" />
      <div className="cb__dg-out">
        <div className="cb__dg-node-title">Sandboxed Jinja2 render</div>
        {/* Keep target case: cb__dg-tag would uppercase it. */}
        <div className="cb__dg-node-sub">per render target, e.g. system:play:chat · engine selects · template serialises</div>
      </div>
    </figure>
  )
}
