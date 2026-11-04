import { useEffect, useMemo, useRef, useState, type CSSProperties } from 'react'
import { animate } from 'motion/react'
import { EASE_IN, EASE_IN_OUT, useReducedMotion } from '../../../../styles/motion'
import { ENTITIES, LIFELINES, STORYLINES, VERSES } from './raon-verse.lines.scenario'
import {
  AXIS_H,
  AXIS_SPANS,
  BANDS,
  baseY,
  FLAT_Y,
  H,
  PLOT_L,
  PLOT_R,
  VERSE,
  W,
  eventsIn,
  fmtSpan,
  fmtWorld,
  focusOf,
  labelRows,
  laneY,
  lerpPt,
  lifelineBase,
  segPath,
  storyNodes,
  storyY,
  storylineBase,
  verseStart,
  verseTag,
  verseTitle,
  worldlineBase,
  xOf,
  type BaseLine,
  type Focus,
  type FocusMark,
  type Pt,
  type Sel,
} from './raon-verse.lines.layout'
import './raon-verse.lines.css'

const keyOf = (s: Sel) => `${s.kind}:${s.id}`
const clamp01 = (v: number) => Math.max(0, Math.min(1, v))
const tint = (color: string) => ({ '--c': color }) as CSSProperties

const WORLD = VERSES.map((v) => ({ sel: { kind: 'worldline', id: v.id } as Sel, base: worldlineBase(v), verse: v }))
const LIVES = LIFELINES.map((l) => ({ sel: { kind: 'lifeline', id: l.id } as Sel, base: lifelineBase(l), line: l }))
// Keep drawing order in the Verse tree, but show picker chips in name order.
const WORLD_CHIPS = [...WORLD].sort((x, y) => (x.verse.id === 'main' ? -1 : y.verse.id === 'main' ? 1 : x.verse.id.localeCompare(y.verse.id)))
const STORIES = STORYLINES.map((s) => ({ sel: { kind: 'storyline', id: s.id } as Sel, base: storylineBase(s.id) }))

const BAND_EVENTS = BANDS.map((b) => ({
  band: b,
  events: eventsIn(b.verse.id)
    .map((e) => ({ ...e, x0: xOf(Math.max(e.from, verseStart(b.verse.id))), x1: xOf(e.to) }))
    .filter((e) => e.x1 - e.x0 > 0.5),
}))

const LIFE_MARKS = LIFELINES.flatMap((l) =>
  (l.marks ?? []).map((m) => ({ key: `${l.id}:${m.label}`, line: l.id, kind: m.kind, x: xOf(m.t), y: laneY(l.id, m.verse), color: VERSE[m.verse].color })),
)

// Split markers sit on the parent line at the branch point.
const SPLITS = VERSES.filter((v) => v.parent).map((v) => ({ id: v.id, x: xOf(v.branchAt!), y: baseY(v.parent!), color: v.color }))

const STORY_SPLITS = STORYLINES.filter((st) => st.parent).map((st) => {
  const leave = storyNodes(st.parent!)[st.after!].node
  return { id: st.id, x: xOf(leave.t), y: storyY(leave.verse), color: VERSE[st.nodes[0].verse].color }
})

const MINOR_TICKS = AXIS_SPANS.filter((s) => s.detail).flatMap((s) => {
  const step = s.to - s.from > 1 ? 1 : 1 / 12
  const out: { x: number; label: string }[] = []
  for (let t = s.from + step; t < s.to - 1e-6; t += step) {
    const label = step === 1 ? fmtWorld(t, true).slice(11, 13) : `${fmtWorld(t, true).slice(14, 16)}h`
    out.push({ x: xOf(t), label })
  }
  return out
})

const TOTAL = WORLD.length + LIVES.length + STORIES.length

export default function RaonVerseLines() {
  const reduce = useReducedMotion()
  const [sel, setSel] = useState<Sel | null>(null)
  const [p, setP] = useState(0)
  const [hover, setHover] = useState<Sel | null>(null)
  const pRef = useRef(0)
  const ctl = useRef<{ stop: () => void } | null>(null)
  const focus = useMemo(() => (sel ? focusOf(sel) : null), [sel])
  const hoverInfo = useMemo(() => (hover ? focusOf(hover) : null), [hover])

  useEffect(() => () => ctl.current?.stop(), [])

  const tween = (from: number, to: number, done?: () => void) => {
    ctl.current?.stop()
    const set = (v: number) => {
      pRef.current = v
      setP(v)
    }
    if (reduce) {
      set(to)
      done?.()
      return
    }
    ctl.current = animate(from, to, {
      duration: to > from ? 0.95 : 0.4,
      ease: to > from ? EASE_IN_OUT : EASE_IN,
      onUpdate: set,
      onComplete: done,
    })
  }

  const choose = (next: Sel | null) => {
    const target = next && sel && keyOf(next) === keyOf(sel) ? null : next
    const go = () => {
      setSel(target)
      if (target) tween(0, 1)
    }
    setHover(null)
    if (sel) tween(pRef.current, 0, go)
    else go()
  }

  const hoverKey = !sel && hover ? keyOf(hover) : null
  const state = (s: Sel) => (hoverKey ? (hoverKey === keyOf(s) ? ' is-hot' : ' is-cold') : '')
  const info = focus ?? hoverInfo
  const pick = (s: Sel) => ({
    onClick: () => choose(s),
    onMouseEnter: () => setHover(s),
    onMouseLeave: () => setHover(null),
  })

  return (
    <div className={`rvl${sel ? ' is-focused' : ''}`}>
      <div className="rvl__head">
        <span className="rvl__eyebrow">Interactive Demo · “Lanternfall”</span>
        <p className="rvl__synopsis">
          A lighthouse keeper goes back to save her drowned brother, twice, and a smuggler follows her six years
          forward. Every world it splits, every life and every story is drawn below in world time. Pick a line to lay it
          flat in its own time.
        </p>
      </div>

      <div className="rvl__pick">
        <div className="rvl__group">
          <span className="rvl__group-label">Worldlines</span>
          <div className="rvl__chips">
            {WORLD_CHIPS.map(({ sel: s, verse }) => (
              <Chip key={s.id} label={verseTitle(verse.id)} color={verse.color} on={!!sel && keyOf(sel) === keyOf(s)} {...pick(s)} />
            ))}
          </div>
        </div>
        <div className="rvl__group">
          <span className="rvl__group-label">Lifelines</span>
          <div className="rvl__entities">
            {ENTITIES.map((e) => (
              <div className="rvl__chips" key={e.id}>
                <span className="rvl__entity">{e.name}</span>
                {LIVES.filter((l) => l.line.entity === e.id).map(({ sel: s, line }) => (
                  <Chip key={s.id} label={line.tag} color={VERSE[line.start.verse].color} on={!!sel && keyOf(sel) === keyOf(s)} {...pick(s)} />
                ))}
              </div>
            ))}
          </div>
        </div>
        <div className="rvl__group">
          <span className="rvl__group-label">Storylines</span>
          <div className="rvl__chips">
            {STORYLINES.map((st, i) => (
              <Chip key={st.id} label={st.name} color="var(--main-light)" on={!!sel && keyOf(sel) === keyOf(STORIES[i].sel)} {...pick(STORIES[i].sel)} />
            ))}
          </div>
        </div>
      </div>

      <div className="rvl__stage">
        <svg className="rvl__svg" viewBox={`0 0 ${W} ${H}`} role="img" aria-label={`${TOTAL} lines of the Lanternfall scenario in world time`}>
          <g className="rvl__base" style={{ opacity: 1 - 0.95 * p }}>
            <Axis />
            {BAND_EVENTS.map(({ band, events }) => {
              const x0 = xOf(verseStart(band.verse.id))
              return (
                <g key={band.verse.id} style={tint(band.verse.color)}>
                  <rect className="rvl__band" x={x0} y={band.top} width={PLOT_R - x0} height={band.bottom - band.top} rx={6} />
                  <text className="rvl__band-label" x={12} y={band.base + 3}>
                    {verseTitle(band.verse.id)}
                  </text>
                  <text className="rvl__band-count" x={12} y={band.base + 15}>
                    {band.lanes.size} lifelines
                  </text>
                  {events.map((e) => (
                    <rect
                      key={e.id}
                      className={`rvl__event-bar${e.major ? ' is-major' : ''}`}
                      x={e.x0}
                      y={band.base + 2.5 + (e.track === 'Harbor' ? 4 : 0)}
                      width={e.x1 - e.x0}
                      height={2.5}
                    />
                  ))}
                </g>
              )
            })}

            {WORLD.map(({ sel: s, base }) => (
              <LineGroup key={keyOf(s)} base={base} cls={`rvl__world${state(s)}`} />
            ))}
            {SPLITS.map((sp) => (
              <circle key={sp.id} className={`rvl__split${state({ kind: 'worldline', id: sp.id })}`} cx={sp.x} cy={sp.y} r={2.8} style={tint(sp.color)} />
            ))}
            {LIVES.map(({ sel: s, base }) => (
              <LineGroup key={keyOf(s)} base={base} cls={`rvl__life${state(s)}`} />
            ))}
            {LIFE_MARKS.map((m) => (
              <circle key={m.key} className={`rvl__life-mark${state({ kind: 'lifeline', id: m.line })}`} cx={m.x} cy={m.y} r={m.kind === 'event' ? 1.8 : 1.4} style={tint(m.color)} />
            ))}
            {STORIES.map(({ sel: s, base }) => (
              <LineGroup key={keyOf(s)} base={base} cls={`rvl__story${state(s)}`} />
            ))}
            {STORY_SPLITS.map((sp) => (
              <circle key={sp.id} className={`rvl__split${state({ kind: 'storyline', id: sp.id })}`} cx={sp.x} cy={sp.y} r={3.4} style={tint(sp.color)} />
            ))}

            {!sel && (
              <g className="rvl__hits">
                {[...WORLD, ...LIVES, ...STORIES].map(({ sel: s, base }) => (
                  <path key={keyOf(s)} d={base.segs.map((q) => segPath(q, 0)).join(' ')} {...pick(s)} />
                ))}
              </g>
            )}
          </g>

          {focus && <FocusLayer focus={focus} p={p} />}
        </svg>
      </div>

      <div className="rvl__info" aria-live="polite">
        {info ? (
          <>
            <div className="rvl__info-head">
              <span className="rvl__info-kind">{info.kindLabel}</span>
              <span className="rvl__info-title">{info.title}</span>
              <span className="rvl__info-sub">{info.sub}</span>
            </div>
            <p className="rvl__info-note">{info.note}</p>
          </>
        ) : (
          <p className="rvl__info-note">
            {TOTAL} lines: {WORLD.length} worldlines, {LIVES.length} lifelines and {STORIES.length} storylines. Hover to trace one,
            click to lay it flat.
          </p>
        )}
        {sel && (
          <button type="button" className="rvl__reset" onClick={() => choose(null)}>
            All lines
          </button>
        )}
      </div>
    </div>
  )
}

function Chip({
  label,
  color,
  on,
  onClick,
  onMouseEnter,
  onMouseLeave,
}: {
  label: string
  color: string
  on: boolean
  onClick: () => void
  onMouseEnter: () => void
  onMouseLeave: () => void
}) {
  return (
    <button
      type="button"
      className={`rvl__chip${on ? ' is-on' : ''}`}
      style={tint(color)}
      aria-pressed={on}
      onClick={onClick}
      onMouseEnter={onMouseEnter}
      onMouseLeave={onMouseLeave}
      onFocus={onMouseEnter}
      onBlur={onMouseLeave}
    >
      <span className="rvl__chip-dot" />
      {label}
    </button>
  )
}

export function Axis() {
  return (
    <g className="rvl__axis">
      {AXIS_SPANS.map((s) => (
        <g key={s.from}>
          {!s.detail && <rect className="rvl__squeeze" x={s.x0} y={AXIS_H - 10} width={s.x1 - s.x0} height={H - AXIS_H + 10} />}
          <line className="rvl__tick" x1={s.x0} x2={s.x0} y1={AXIS_H - 16} y2={H} />
          {s.detail ? (
            <text className="rvl__axis-label" x={s.x0 + 4} y={AXIS_H - 22}>
              {fmtWorld(s.from, true)}
            </text>
          ) : (
            <text className="rvl__axis-label is-squeeze" x={(s.x0 + s.x1) / 2} y={AXIS_H - 22}>
              ≈ {fmtSpan(s.to - s.from)}
            </text>
          )}
        </g>
      ))}
      {MINOR_TICKS.map((t) => (
        <g key={t.x}>
          <line className="rvl__tick is-minor" x1={t.x} x2={t.x} y1={AXIS_H - 9} y2={H} />
          <text className="rvl__axis-minor" x={t.x + 3} y={AXIS_H - 9}>
            {t.label}
          </text>
        </g>
      ))}
    </g>
  )
}

export function LineGroup({ base, cls }: { base: BaseLine; cls: string }) {
  return (
    <g className={cls}>
      {base.segs.map((s, i) => (
        <path
          key={i}
          d={segPath(s, 0)}
          style={tint(s.color)}
          strokeWidth={base.width}
          strokeDasharray={s.kind === 'jump' ? '2 3' : base.dash}
          className={s.kind === 'jump' ? 'is-jump' : undefined}
        />
      ))}
      {base.start && <circle className="rvl__birth" cx={base.start.x} cy={base.start.y} r={2.2} style={tint(base.segs[0].color)} />}
    </g>
  )
}

export function Glyph({ m, at, dim }: { m: FocusMark; at: Pt; dim: boolean }) {
  const cls = `rvl__glyph is-${m.kind}${m.major ? ' is-major' : ''}${m.known === false ? ' is-unknown' : ''}`
  const style = { ...tint(m.color), opacity: dim ? 0.5 : 1 }
  const { x, y } = at
  switch (m.kind) {
    case 'snapshot':
      return <rect className={cls} style={style} x={x - 3.5} y={y - 3.5} width={7} height={7} transform={`rotate(45 ${x} ${y})`} />
    case 'select':
    case 'choice':
      return <rect className={cls} style={style} x={x - 3.5} y={y - 3.5} width={7} height={7} rx={1} />
    case 'jump':
      return <path className={cls} style={style} d={`M${x - 4},${y - 4} L${x + 1},${y} L${x - 4},${y + 4} M${x},${y - 4} L${x + 5},${y} L${x},${y + 4}`} />
    case 'fork':
    case 'origin':
    case 'start':
      return <circle className={cls} style={style} cx={x} cy={y} r={m.kind === 'start' ? 4 : 5} />
    default:
      return <circle className={cls} style={style} cx={x} cy={y} r={m.major ? 4.2 : 3.2} />
  }
}

const ABOVE_ROWS = 6
const TRACE_TOP = FLAT_Y + 58
const TRACE_H = 52

function FocusLayer({ focus, p }: { focus: Focus; p: number }) {
  const o = clamp01((p - 0.55) / 0.45)
  const story = focus.sel.kind === 'storyline'
  const world = focus.sel.kind === 'worldline'
  const labelled = useMemo(() => {
    const items = focus.marks.map((m) => ({ x: m.f.x, text: m.label })).sort((a, b) => a.x - b.x)
    const rows = labelRows(items, ABOVE_ROWS)
    return items.map((it, i) => ({ ...it, row: rows[i] }))
  }, [focus])
  const trackRows = useMemo(() => {
    const by = (track: 'Lantern' | 'Harbor') => {
      const evs = focus.events.filter((e) => e.track === track).sort((a, b) => a.from - b.from)
      const rows = labelRows(evs.map((e) => ({ x: xOf(e.from) + e.label.length * 2.7 + 4, text: e.label })), 2)
      return evs.map((e, i) => ({ ...e, row: rows[i] }))
    }
    return { Lantern: by('Lantern'), Harbor: by('Harbor') }
  }, [focus])
  // Squeeze labels can overhang their bands; only other labels block them.
  const breaks = useMemo(() => {
    const rows = labelRows(focus.breaks.map((b) => ({ x: (b.x0 + b.x1) / 2, text: b.label })), 1)
    return focus.breaks.map((b, i) => ({ ...b, row: rows[i] }))
  }, [focus])
  const traceY = (w: number) => TRACE_TOP + (1 - w) * TRACE_H

  return (
    <g className="rvl__focus">
      {focus.segs.map((s, i) => (
        <path
          key={i}
          d={segPath(s, p)}
          className={`rvl__focus-seg is-${s.kind}`}
          style={{ ...tint(s.color), opacity: s.inherited ? 1 - 0.55 * p : 1 }}
          strokeWidth={focus.width}
          strokeDasharray={s.kind === 'jump' ? '3 4' : s.kind === 'run' ? focus.dash : undefined}
        />
      ))}
      {focus.marks.map((m, i) => (
        <Glyph key={i} m={m} at={lerpPt(m.t, m.f, p)} dim={m.inherited && p > 0.5} />
      ))}

      <g className="rvl__overlay" style={{ opacity: o }}>
        {breaks.map((b, i) => (
          <g key={i}>
            <rect className="rvl__squeeze" x={b.x0} y={AXIS_H - 10} width={b.x1 - b.x0} height={H - AXIS_H + 10} />
            {b.row === 0 && (
              <text className="rvl__axis-label is-squeeze" x={(b.x0 + b.x1) / 2} y={AXIS_H - 22}>
                {b.label}
              </text>
            )}
          </g>
        ))}
        <text className="rvl__flat-title" x={PLOT_L - 14} y={FLAT_Y + 4}>
          {focus.title}
        </text>
        {focus.axisNote && (
          <text className="rvl__side-label" x={PLOT_L - 14} y={FLAT_Y + 18}>
            {focus.axisNote}
          </text>
        )}

        {story
          ? focus.marks.map((m, i) => (
              <text key={i} className={`rvl__flat-label is-tilted${m.inherited ? ' is-dim' : ''}`} transform={`translate(${m.f.x + 2} ${FLAT_Y - 12}) rotate(-50)`}>
                {m.label}
              </text>
            ))
          : labelled.map(
              (l, i) =>
                l.row >= 0 && (
                  <g key={i}>
                    <line className="rvl__leader" x1={l.x} x2={l.x} y1={FLAT_Y - 7} y2={FLAT_Y - 16 - l.row * 13} />
                    <text className="rvl__flat-label" x={l.x} y={FLAT_Y - 19 - l.row * 13} textAnchor="middle">
                      {l.text}
                    </text>
                  </g>
                ),
            )}

        {!world &&
          focus.stretches.map((s, i) => {
            const w = s.x1 - s.x0
            return (
              <g key={i} style={{ ...tint(VERSE[s.verse].color), opacity: s.inherited ? 0.5 : 1 }}>
                <rect className="rvl__stretch" x={s.x0} y={FLAT_Y + 12} width={Math.max(2, w)} height={3} rx={1.5} />
                {w > 26 && (
                  <text className="rvl__stretch-verse" x={s.x0 + 1} y={FLAT_Y + 27}>
                    {s.own && w > s.own.length * 5.2 + 30 ? `${verseTag(s.verse)} · ${s.own}` : verseTag(s.verse)}
                  </text>
                )}
                {w > s.world.length * 5.2 + 4 && (
                  <text className="rvl__stretch-world" x={s.x0 + 1} y={FLAT_Y + 38}>
                    {s.world}
                  </text>
                )}
              </g>
            )
          })}

        {focus.trace.length > 0 && (
          <g className="rvl__trace">
            <text className="rvl__side-label" x={PLOT_L - 14} y={TRACE_TOP + TRACE_H / 2 + 3}>
              world time
            </text>
            <line className="rvl__trace-axis" x1={PLOT_L} x2={PLOT_R} y1={TRACE_TOP + TRACE_H} y2={TRACE_TOP + TRACE_H} />
            {focus.trace.slice(1).map((pt, i) => {
              const a = focus.trace[i]
              return <line key={i} className={pt.jump ? 'is-jump' : undefined} x1={a.x} y1={traceY(a.w)} x2={pt.x} y2={traceY(pt.w)} />
            })}
          </g>
        )}

        {world && (
          <g className="rvl__tracks">
            {(['Lantern', 'Harbor'] as const).map((track, ti) => {
              const y = FLAT_Y + 16 + ti * 34
              return (
                <g key={track}>
                  <text className="rvl__side-label" x={PLOT_L - 14} y={y + 6}>
                    {track}
                  </text>
                  {trackRows[track].map((e) => (
                    <g key={e.id} style={tint(VERSE[e.placed].color)}>
                      <rect className={`rvl__track-event${e.major ? ' is-major' : ''}${e.ongoing ? ' is-ongoing' : ''}`} x={xOf(e.from)} y={y} width={Math.max(3, xOf(e.to) - xOf(e.from))} height={6} rx={3} />
                      {e.row >= 0 && (
                        <text className="rvl__track-label" x={xOf(e.from) + 1} y={y + 16 + e.row * 10}>
                          {e.label}
                          {e.ongoing ? ' · ongoing' : ''}
                        </text>
                      )}
                    </g>
                  ))}
                </g>
              )
            })}
            {focus.presence.map((r, i) => (
              <g key={i} style={tint(r.color)}>
                {(i === 0 || focus.presence[i - 1].row !== r.row) && (
                  <text className="rvl__side-label is-small" x={PLOT_L - 14} y={FLAT_Y + 96 + r.row * 11 + 3}>
                    {r.label}
                  </text>
                )}
                <line className="rvl__presence" x1={r.x0} x2={r.x1} y1={FLAT_Y + 96 + r.row * 11} y2={FLAT_Y + 96 + r.row * 11} strokeDasharray={r.dash} />
              </g>
            ))}
          </g>
        )}
      </g>
    </g>
  )
}
