import { useCallback, useEffect, useRef, useState, type CSSProperties } from 'react'
import { animate, useInView } from 'motion/react'
import { EASE_IN_OUT, useReducedMotion } from '../../../../styles/motion'
import { LIFELINES, MONTH, STORYLINES, VERSES, YEAR, type LifeMark, type VerseId } from './raon-verse.lines.scenario'
import {
  BANDS,
  H,
  PLOT_L,
  PLOT_R,
  VERSE,
  W,
  flatXOf,
  focusOf,
  fmtWorld,
  labelRows,
  laneY,
  lerpPt,
  lifePieces,
  lifeTitle,
  lifelineBase,
  segPath,
  storyNodes,
  storylineBase,
  verseStart,
  verseTag,
  verseTitle,
  worldlineBase,
  xOf,
  type Seg,
} from './raon-verse.lines.layout'
import { Axis, Glyph, LineGroup } from './raon-verse.lines'
import './raon-verse.sort-lines.css'

const V: VerseId = 'b'
const S0 = storyNodes('s0').map((n) => n.node)
const NOW = S0[S0.length - 1].t
const NOW_X = xOf(NOW)

const clamp01 = (v: number) => Math.max(0, Math.min(1, v))
const tint = (color: string) => ({ '--c': color }) as CSSProperties
const LINE = Object.fromEntries(LIFELINES.map((l) => [l.id, l]))

// Reading inherited records

function ownOf(id: string, v: VerseId, t: number): number | null {
  const p = lifePieces(id).find((q) => q.verse === v && q.from <= t && t <= q.to)
  return p ? p.own + (t - p.from) : null
}

// Keep ancestor marks only up to the line’s branch point, in own-time coordinates.
function chainMarks(id: string): { m: LifeMark; own: number }[] {
  const l = LINE[id]
  const own = (l.marks ?? []).flatMap((m) => {
    const u = ownOf(id, m.verse, m.t)
    return u === null ? [] : [{ m, own: u }]
  })
  if (!l.parent) return own
  const cut = lifePieces(id)[0].own
  return [...chainMarks(l.parent).filter((x) => x.own <= cut), ...own]
}

function fmtAge(dt: number): string {
  const d = Math.floor(Math.round(dt * 1440) / 1440)
  const y = Math.floor(d / YEAR)
  const m = Math.floor((d - y * YEAR) / MONTH)
  return `${y}y ${m}m ${d - y * YEAR - m * MONTH}d`
}

function gather(id: string, u: number): string {
  const marks = chainMarks(id)
  const latest = (kind: LifeMark['kind']) =>
    marks.filter((x) => x.m.kind === kind && x.own <= u).sort((a, b) => b.own - a.own)[0]?.m
  const snaps = marks.filter((x) => x.m.kind === 'snapshot').sort((a, b) => a.own - b.own)
  const row = latest('knowledge')
  // Mira owns the secret unless a knowledge mark overrides it.
  const known = row ? !!row.known : LINE[id].entity === 'mira'
  const snapshot = (latest('snapshot') ?? snaps[0]?.m)?.label ?? 'entity content only'
  const event = latest('event')?.label ?? 'no events yet'
  return `age ${fmtAge(u)} · ${snapshot} · ember secret ${known ? 'known' : 'unknown'} · last: ${event}`
}

// Tangled lines

const WORLD = VERSES.map((v) => ({ id: v.id, base: worldlineBase(v) }))
const LIVES = LIFELINES.map((l) => ({
  id: l.id,
  base: lifelineBase(l),
  here: ownOf(l.id, V, NOW) !== null,
  marks: (l.marks ?? []).map((m) => ({ key: m.label, x: xOf(m.t), y: laneY(l.id, m.verse), color: VERSE[m.verse].color })),
}))
const STORIES = STORYLINES.map((s) => ({ id: s.id, base: storylineBase(s.id) }))

const GONE = LIFELINES.flatMap((l) => {
  if (ownOf(l.id, V, NOW) !== null) return []
  const p = lifePieces(l.id).find((q) => q.verse === V && q.to < NOW && q.to >= verseStart(V))
  return p ? [{ id: l.id, title: lifeTitle(l), x: xOf(p.to), y: laneY(l.id, V), at: fmtWorld(p.to, true).slice(-5) }] : []
})

// Flat lanes

const LANE_TOP = 66
const LANE_GAP = 92

const LANES = LIFELINES.filter((l) => ownOf(l.id, V, NOW) !== null).map((l, i) => {
  const y = LANE_TOP + i * LANE_GAP
  const f = focusOf({ kind: 'lifeline', id: l.id })
  const flat = (q: { x: number; y: number }) => ({ x: q.x, y })
  const breakRows = labelRows(f.breaks.map((b) => ({ x: (b.x0 + b.x1) / 2, text: b.label })), 1)
  return {
    id: l.id,
    title: lifeTitle(l),
    y,
    dash: f.dash,
    segs: f.segs.map((s) => ({ ...s, f: s.f.map(flat) as Seg['f'] })),
    marks: f.marks.map((m) => ({ ...m, f: flat(m.f) })),
    stretches: f.stretches,
    breaks: f.breaks.map((b, j) => ({ ...b, row: breakRows[j] })),
    tagY: laneY(l.id, V),
    nowX: flatXOf(l.id, V, NOW)!,
    found: gather(l.id, ownOf(l.id, V, NOW)!),
  }
})

const STEPS: { title: string; text: string; dur: number }[] = [
  {
    title: 'Tangled',
    text: `This is the multiverse as it is stored: ${WORLD.length} Verses, ${LIVES.length} lifelines and ${STORIES.length} storylines, crossing wherever someone travels in time or a world splits. Read whole, it is where continuity breaks.`,
    dur: 0.6,
  },
  {
    title: 'Sort by Verse',
    text: `The play is in ${VERSE[V].name} at ${fmtWorld(NOW, true)}. Sorting by Verse keeps only the lines that are there at that moment; every other world drops out.`,
    dur: 1.1,
  },
  {
    title: 'Sort by entity',
    text: `Sorting by entity says whose each line is: ${LANES.length - 1} lifelines of Mira and one of Oren. ${GONE.map((g) => g.title).join(', ')} left at ${GONE[0]?.at} the night before, so it has no place in this scene.`,
    dur: 1,
  },
  {
    title: 'Unfold',
    text: 'Each of those lines comes out of the tangle and is laid flat in its own time, with everything it inherited: where it branched, the Verses it lived in, every snapshot, event and knowledge row on it.',
    dur: 1.6,
  },
  {
    title: 'One agent, one line',
    text: 'Each agent is given exactly one flat line, its own entity in its own Verse, and reads it up to now through that character’s eyes. Nobody has to untangle anyone else’s history, so the agents gather what they need side by side.',
    dur: 2.2,
  },
  {
    title: 'The next node',
    text: 'What the agents gathered meets in the play’s next node, and the records that node creates go back onto their own lines for the next turn.',
    dur: 1.2,
  },
]

const DWELL = 2600
const SCAN = { delay: [0, 0.12, 0.05, 0.18], span: [0.72, 0.8, 0.62, 0.7] }

export default function RaonVerseSortLines() {
  const reduce = useReducedMotion()
  const ref = useRef<HTMLDivElement>(null)
  const inView = useInView(ref, { amount: 0.35 })
  const [pos, setPos] = useState(1)
  const [step, setStep] = useState(1)
  // Defer the default until the reduced-motion preference is hydrated.
  const [playChoice, setPlaying] = useState<boolean | null>(null)
  const playing = playChoice ?? !reduce
  const posRef = useRef(1)
  const ctl = useRef<{ stop: () => void } | null>(null)
  const [tick, setTick] = useState(0)
  const tickRef = useRef(0)
  const autoCtl = useRef<{ stop: () => void } | null>(null)
  const setTickBoth = (v: number) => {
    tickRef.current = v
    setTick(v)
  }

  useEffect(() => () => ctl.current?.stop(), [])

  const go = useCallback(
    (k: number) => {
      ctl.current?.stop()
      // Stop autoplay before resetting, or its last frame can overwrite the reset.
      autoCtl.current?.stop()
      setStep(k)
      tickRef.current = 0
      setTick(0)
      const set = (v: number) => {
        posRef.current = v
        setPos(v)
      }
      if (reduce) return set(k)
      const forward = k === Math.round(posRef.current) + 1
      ctl.current = animate(posRef.current, k, {
        duration: forward ? STEPS[k - 1].dur : 0.5,
        ease: EASE_IN_OUT,
        onUpdate: set,
      })
    },
    [reduce],
  )

  const done = step >= STEPS.length

  useEffect(() => {
    if (!playing || done || !inView || reduce) return
    const total = STEPS[step - 1].dur + DWELL / 1000
    const run = animate(tickRef.current, 1, {
      duration: total * (1 - tickRef.current),
      ease: 'linear',
      onUpdate: setTickBoth,
      onComplete: () => go(step + 1),
    })
    autoCtl.current = run
    return () => run.stop()
  }, [playing, done, inView, step, reduce, go])

  const toggle = () => {
    if (done) {
      ctl.current?.stop()
      posRef.current = 1
      setPos(1)
      setStep(1)
      setTickBoth(0)
      setPlaying(true)
      return
    }
    setPlaying(!playing)
  }

  const a = (k: number) => clamp01(pos - (k - 1))

  return (
    <div className="rvs" ref={ref}>
      <div className="rvs__head">
        <span className="rvs__eyebrow">Walkthrough · Untangling the multiverse</span>
        <p className="rvs__synopsis">
          A multiverse is many worlds whose lines keep crossing. Before the play writes its next node, Raon:Verse does not
          hand that tangle to one agent: it sorts the lines by Verse and by entity, lays each line that matters flat in its
          own time, and gives every agent a single line to focus on, its own entity in its own Verse.
        </p>
      </div>

      <div className="rvs__stage">
        <svg className="rvl__svg" viewBox={`0 0 ${W} ${H}`} role="img" aria-label="Sorting the Lanternfall lines by Verse and entity, one flat line per agent">
          <Tangle a={a} />
          {a(4) > 0 && <Lanes a={a} />}
        </svg>
      </div>

      <div className="rvs__controls">
        <button type="button" className="rvs__play" onClick={toggle}>
          {done ? 'Replay' : playing ? 'Pause' : 'Play'}
        </button>
        <div className="rvs__steps">
          {STEPS.map((s, i) => (
            <button
              type="button"
              key={s.title}
              className={`rvs__step${step === i + 1 ? ' is-on' : ''}${step > i + 1 ? ' is-done' : ''}`}
              style={step === i + 1 && !done && (playing || tick > 0) ? ({ '--p': tick } as CSSProperties) : undefined}
              onClick={() => {
                setPlaying(false)
                go(i + 1)
              }}
            >
              <span className="rvs__step-n">{i + 1}</span>
              {s.title}
            </button>
          ))}
        </div>
      </div>
      <p className="rvs__caption" aria-live="polite">
        {STEPS[step - 1].text}
      </p>
    </div>
  )
}

// Steps 1–3: filter by Verse and entity.
function Tangle({ a }: { a: (k: number) => number }) {
  const verse = a(2)
  const unfold = a(4)
  const keep = (here: boolean) => (here ? 1 : 1 - 0.88 * verse)

  return (
    <g className="rvl__base" style={{ opacity: 1 - 0.94 * unfold }}>
      <Axis />
      {BANDS.map((b) => {
        const x0 = xOf(verseStart(b.verse.id))
        const mine = b.verse.id === V
        return (
          <g key={b.verse.id} style={{ ...tint(b.verse.color), opacity: mine ? 1 : 1 - 0.75 * verse }}>
            <rect className="rvl__band" x={x0} y={b.top} width={PLOT_R - x0} height={b.bottom - b.top} rx={6} style={mine ? { fillOpacity: 0.045 + 0.08 * verse } : undefined} />
            <text className="rvl__band-label" x={12} y={b.base + 3}>
              {verseTitle(b.verse.id)}
            </text>
          </g>
        )
      })}

      {WORLD.map((w) => (
        <g key={w.id} style={{ opacity: keep(w.id === V) }}>
          <LineGroup base={w.base} cls="rvl__world" />
        </g>
      ))}
      {LIVES.map((l) => (
        <g key={l.id} style={{ opacity: keep(l.here) }}>
          <LineGroup base={l.base} cls={`rvl__life${l.here && verse > 0 ? ' is-hot' : ''}`} />
          {l.marks.map((m) => (
            <circle key={m.key} className="rvl__life-mark" cx={m.x} cy={m.y} r={1.6} style={tint(m.color)} />
          ))}
        </g>
      ))}
      {STORIES.map((s) => (
        <g key={s.id} style={{ opacity: 1 - 0.75 * verse }}>
          <LineGroup base={s.base} cls="rvl__story" />
        </g>
      ))}

      <g style={{ opacity: verse }}>
        <line className="rvs__now" x1={NOW_X} x2={NOW_X} y1={30} y2={H - 8} />
        <text className="rvs__now-label" x={NOW_X - 6} y={H - 12}>
          now · {fmtWorld(NOW, true)}
        </text>
      </g>

      <g style={{ opacity: a(3) }}>
        {LANES.map((l, i) => {
          // Separate labels from the densely packed 8-unit lanes.
          const ty = LANES[0].tagY - 18 + i * 13
          return (
            <g key={l.id}>
              <path className="rvs__leader" d={`M${NOW_X},${l.tagY} C${NOW_X + 14},${l.tagY} ${NOW_X + 14},${ty} ${NOW_X + 28},${ty}`} />
              <rect className="rvs__tag-box" x={NOW_X + 28} y={ty - 6.5} width={l.title.length * 5.4 + 12} height={13} rx={6.5} />
              <text className="rvs__tag" x={NOW_X + 34} y={ty + 3}>
                {l.title}
              </text>
            </g>
          )
        })}
        {GONE.map((g) => (
          <text key={g.id} className="rvs__tag is-gone" x={g.x - 4} y={g.y + 12}>
            {g.title} · left {g.at}
          </text>
        ))}
      </g>
    </g>
  )
}

// Steps 4–6: unfold each lifeline, read it, and converge on the next node.
function Lanes({ a }: { a: (k: number) => number }) {
  const p = a(4)
  const settled = clamp01((p - 0.6) / 0.4)
  const reading = a(5)
  const node = a(6)
  const nodeX = PLOT_R - 96
  const nodeY = LANE_TOP + ((LANES.length - 1) * LANE_GAP) / 2

  return (
    <g>
      {LANES.map((l, i) => {
        const scan = clamp01((reading - SCAN.delay[i]) / SCAN.span[i])
        const headX = PLOT_L + (l.nowX - PLOT_L) * scan
        return (
          <g key={l.id}>
            <g style={{ opacity: settled }}>
              {l.breaks.map((b, j) => (
                <g key={j}>
                  <rect className="rvl__squeeze" x={b.x0} y={l.y - 10} width={b.x1 - b.x0} height={38} />
                  {b.row === 0 && (
                    <text className="rvl__axis-label is-squeeze" x={(b.x0 + b.x1) / 2} y={l.y - 13}>
                      {b.label}
                    </text>
                  )}
                </g>
              ))}
              <text className="rvl__flat-title" x={PLOT_L - 14} y={l.y + 4}>
                {l.title}
              </text>
              {l.stretches.map((s, j) => {
                const w = s.x1 - s.x0
                const tag = verseTag(s.verse)
                return (
                  <g key={j} style={{ ...tint(VERSE[s.verse].color), opacity: s.inherited ? 0.5 : 1 }}>
                    <rect className="rvl__stretch" x={s.x0} y={l.y + 9} width={Math.max(2, w)} height={3} rx={1.5} />
                    {w > 14 && (
                      <text className="rvl__stretch-verse" x={s.x0 + 1} y={l.y + 22}>
                        {w > s.own.length * 5.2 + 30 ? `${tag} · ${s.own}` : tag}
                      </text>
                    )}
                  </g>
                )
              })}
            </g>

            {l.segs.map((s, j) => (
              <path
                key={j}
                d={segPath(s, p)}
                className={`rvl__focus-seg is-${s.kind}`}
                style={{ ...tint(s.color), opacity: clamp01(p * 6) * (s.inherited ? 1 - 0.5 * p : 1) }}
                strokeWidth={2}
                strokeDasharray={s.kind === 'jump' ? '3 4' : s.kind === 'run' ? l.dash : undefined}
              />
            ))}
            {l.marks.map((m, j) => (
              <g key={j} style={{ opacity: clamp01(p * 6) * (reading > 0 && m.f.x > headX + 0.5 ? 0.35 : 1) }}>
                <Glyph m={m} at={lerpPt(m.t, m.f, p)} dim={m.inherited && p > 0.5} />
              </g>
            ))}

            <g style={{ opacity: reading }}>
              <rect className="rvs__after" x={l.nowX + 4} y={l.y - 11} width={PLOT_R - l.nowX} height={40} />
              <line className="rvs__lane-now" x1={l.nowX} x2={l.nowX} y1={l.y - 9} y2={l.y + 9} />
              {i === 0 && (
                <text className="rvs__lane-now-label" x={l.nowX} y={l.y - 14}>
                  now
                </text>
              )}
              <text className="rvs__agent" x={PLOT_L - 14} y={l.y + 17}>
                agent
              </text>
              <circle className="rvs__scan" cx={headX} cy={l.y} r={4} style={{ opacity: scan < 1 ? 1 : 0 }} />
              <text className="rvs__found" x={PLOT_L} y={l.y + 38} style={{ opacity: clamp01((scan - 0.85) / 0.15) }}>
                {l.found}
              </text>
            </g>
          </g>
        )
      })}

      <g style={{ opacity: node }}>
        {LANES.map((l) => (
          <path
            key={l.id}
            className="rvs__converge"
            d={`M${l.nowX},${l.y} C${l.nowX + 70},${l.y} ${nodeX - 150},${nodeY} ${nodeX - 86},${nodeY}`}
            pathLength={1}
            style={{ strokeDashoffset: 1 - node }}
          />
        ))}
        <rect className="rvs__node" x={nodeX - 86} y={nodeY - 18} width={172} height={36} rx={12} />
        <text className="rvs__node-label" x={nodeX} y={nodeY - 3}>
          next node
        </text>
        <text className="rvs__node-sub" x={nodeX} y={nodeY + 10}>
          written back onto the lines
        </text>
      </g>
    </g>
  )
}
