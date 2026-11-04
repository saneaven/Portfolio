import {
  ENTITIES,
  LIFELINES,
  MONTH,
  STORYLINES,
  VERSES,
  WORLD_EVENTS,
  YEAR,
  at,
  type EntityId,
  type Lifeline,
  type StoryNode,
  type Verse,
  type VerseId,
} from './raon-verse.lines.scenario'

// Each segment has two layouts: world time within Verse bands, and own time on a flat row.
// The morph interpolates their control points. Coordinates are SVG viewBox units.
export const W = 1000
export const PLOT_L = 156
export const PLOT_R = 984
const PLOT_W = PLOT_R - PLOT_L

// World axis

// Expand day-scale windows; compress the centuries between them.
const AXIS = [
  { from: at(894), to: at(1000, 9, 12), w: 0.18, detail: false },
  { from: at(1000, 9, 12), to: at(1000, 9, 16), w: 0.37, detail: true },
  { from: at(1000, 9, 16), to: at(1006, 6, 1), w: 0.07, detail: false },
  { from: at(1006, 6, 1), to: at(1006, 6, 4), w: 0.17, detail: true },
  { from: at(1006, 6, 4), to: at(1012, 3, 1, 6), w: 0.07, detail: false },
  { from: at(1012, 3, 1, 6), to: at(1012, 3, 1, 12), w: 0.14, detail: true },
]
const START = AXIS[0].from
const END = AXIS[AXIS.length - 1].to

export function xOf(t: number): number {
  let x = PLOT_L
  for (const s of AXIS) {
    const w = s.w * PLOT_W
    if (t < s.to) return x + Math.max(0, (t - s.from) / (s.to - s.from)) * w
    x += w
  }
  return PLOT_R
}

export const AXIS_SPANS = AXIS.map((s) => ({ x0: xOf(s.from), x1: xOf(s.to - 1e-9), detail: s.detail, from: s.from, to: s.to }))

// Calendar formatting

const pad = (n: number) => String(n).padStart(2, '0')

// Round once before splitting the calendar fields to avoid minute overflow.
function parts(t: number) {
  const mins = Math.round(t * 1440)
  const days = Math.floor(mins / 1440)
  const y = Math.floor(days / YEAR)
  const m = Math.floor((days - y * YEAR) / MONTH)
  const rest = mins - days * 1440
  return { y, m: m + 1, d: days - y * YEAR - m * MONTH + 1, h: Math.floor(rest / 60), min: rest % 60 }
}

export function fmtWorld(t: number, precise: boolean): string {
  const { y, m, d, h, min } = parts(t)
  return precise ? `TR ${y}-${pad(m)}-${pad(d)} ${pad(h)}:${pad(min)}` : `TR ${y}`
}

const inDetail = (t: number) => AXIS.some((s) => s.detail && t >= s.from && t < s.to)

function fmtRange(from: number, to: number, open: boolean): string {
  const precise = inDetail(from) || (!open && inDetail(to))
  const a = fmtWorld(from, precise)
  if (open) return `${a} → …`
  const b = fmtWorld(to, precise)
  return precise && a.slice(0, 7) === b.slice(0, 7) ? `${a} → ${b.slice(8)}` : `${a} → ${b.slice(3)}`
}

export function fmtSpan(dt: number): string {
  const mins = Math.round(dt * 1440)
  const d = Math.floor(mins / 1440)
  const y = Math.floor(d / YEAR)
  const m = Math.floor((d - y * YEAR) / MONTH)
  if (y > 0) return m > 0 ? `${y}y ${m}m` : `${y}y`
  if (m > 0) return `${m}m`
  const h = Math.floor((mins - d * 1440) / 60)
  return d > 0 ? (h > 0 ? `${d}d ${h}h` : `${d}d`) : `${h}h`
}

// Record lookup

export const VERSE = Object.fromEntries(VERSES.map((v) => [v.id, v])) as Record<VerseId, Verse>
const LINE = Object.fromEntries(LIFELINES.map((l) => [l.id, l])) as Record<string, Lifeline>
const ENTITY = Object.fromEntries(ENTITIES.map((e) => [e.id, e]))

export const verseTag = (v: VerseId) => (v === 'main' ? 'Main' : v.toUpperCase())
export const verseTitle = (v: VerseId) => (v === 'main' ? 'Main' : `${verseTag(v)} · ${VERSE[v].name}`)
export const lifeTitle = (l: Lifeline) => `${ENTITY[l.entity].name} · ${l.tag}`

const DASH: Record<EntityId, string | undefined> = { mira: undefined, oren: '1.5 2.6', vael: '6 3' }

// Lifeline pieces

// `own` is elapsed lifeline time at `from`.
type Piece = { line: string; verse: VerseId; from: number; to: number; own: number }

const piecesMemo = new Map<string, Piece[]>()

export function lifePieces(id: string): Piece[] {
  const hit = piecesMemo.get(id)
  if (hit) return hit
  const l = LINE[id]
  let own = 0
  if (l.parent) own = ownAt(l.parent, VERSE[l.start.verse].parent!, l.start.t)
  const out: Piece[] = []
  let cur = { verse: l.start.verse, from: l.start.t }
  for (const j of l.jumps ?? []) {
    out.push({ line: id, verse: cur.verse, from: cur.from, to: j.depart, own })
    own += j.depart - cur.from
    cur = { verse: j.verse, from: j.arrive }
  }
  out.push({ line: id, verse: cur.verse, from: cur.from, to: Infinity, own })
  piecesMemo.set(id, out)
  return out
}

function ownAt(id: string, verse: VerseId, t: number): number {
  const p = lifePieces(id).find((q) => q.verse === verse && q.from <= t && t <= q.to)
  if (!p) throw new Error(`${id} is not in ${verse} at ${t}`)
  return p.own + (t - p.from)
}

// Verse bands

export const AXIS_H = 40
const STORY_H = 16
const EVENT_H = 12
const LANE = 8
const BAND_PAD = 10
const BAND_GAP = 6

type Band = { verse: Verse; top: number; base: number; bottom: number; lanes: Map<string, number> }

export const BANDS: Band[] = (() => {
  let y = AXIS_H
  return VERSES.map((verse) => {
    const ids = LIFELINES.filter((l) => lifePieces(l.id).some((p) => p.verse === verse.id)).map((l) => l.id)
    const top = y
    const base = top + STORY_H
    const lanes = new Map(ids.map((id, i) => [id, base + EVENT_H + i * LANE]))
    const bottom = base + EVENT_H + (ids.length - 1) * LANE + BAND_PAD
    y = bottom + BAND_GAP
    return { verse, top, base, bottom, lanes }
  })
})()

export const H = BANDS[BANDS.length - 1].bottom + 12
const BAND = Object.fromEntries(BANDS.map((b) => [b.verse.id, b])) as Record<VerseId, Band>

export const laneY = (line: string, verse: VerseId) => BAND[verse].lanes.get(line)!
export const storyY = (verse: VerseId) => BAND[verse].base - 7
export const baseY = (verse: VerseId) => BAND[verse].base
export const verseStart = (v: VerseId) => VERSE[v].branchAt ?? START

// Segment geometry

export type Pt = { x: number; y: number }

// Cubic control points in tangled (`t`) and flat (`f`) coordinates.
export type Seg = {
  t: [Pt, Pt, Pt, Pt]
  f: [Pt, Pt, Pt, Pt]
  color: string
  kind: 'run' | 'bend' | 'jump'
  inherited: boolean
}

const lerp = (a: number, b: number, p: number) => a + (b - a) * p
export const lerpPt = (a: Pt, b: Pt, p: number): Pt => ({ x: lerp(a.x, b.x, p), y: lerp(a.y, b.y, p) })

export function segPath(s: Seg, p: number): string {
  const [a, c1, c2, b] = s.t.map((q, i) => lerpPt(q, s.f[i], p))
  return `M${a.x.toFixed(1)},${a.y.toFixed(1)} C${c1.x.toFixed(1)},${c1.y.toFixed(1)} ${c2.x.toFixed(1)},${c2.y.toFixed(1)} ${b.x.toFixed(1)},${b.y.toFixed(1)}`
}

const run = (a: Pt, b: Pt): [Pt, Pt, Pt, Pt] => [a, a, b, b]

// A split connects the parent and child at the same world time.
function bend(from: Pt, to: Pt): [Pt, Pt, Pt, Pt] {
  const k = Math.max(14, Math.min(40, Math.abs(to.y - from.y) * 0.3))
  return [from, { x: from.x + k, y: from.y }, { x: to.x - k, y: to.y }, to]
}

// Backward jumps loop above the line; forward jumps use a soft curve.
function arc(from: Pt, to: Pt): [Pt, Pt, Pt, Pt] {
  if (to.x < from.x - 1) {
    const top = Math.min(from.y, to.y) - 16 - Math.min(26, Math.abs(to.y - from.y) * 0.18)
    return [from, { x: from.x + 28, y: top }, { x: to.x - 28, y: top }, to]
  }
  const dx = (to.x - from.x) * 0.45
  return [from, { x: from.x + dx, y: from.y }, { x: to.x - dx, y: to.y }, to]
}

// Tangled base drawing

export type BaseLine = { segs: Seg[]; dash?: string; width: number; start?: Pt }

function still(t: [Pt, Pt, Pt, Pt], color: string, kind: Seg['kind']): Seg {
  return { t, f: t, color, kind, inherited: false }
}

export function worldlineBase(v: Verse): BaseLine {
  const y = baseY(v.id)
  const x0 = xOf(verseStart(v.id))
  const segs: Seg[] = []
  if (v.parent) segs.push(still(bend({ x: x0, y: baseY(v.parent) }, { x: x0, y }), v.color, 'bend'))
  segs.push(still(run({ x: x0, y }, { x: PLOT_R, y }), v.color, 'run'))
  return { segs, width: 2 }
}

function lifeSegs(id: string): Seg[] {
  const l = LINE[id]
  const ps = lifePieces(id)
  const segs: Seg[] = []
  if (l.parent) {
    const pv = VERSE[l.start.verse].parent!
    const from = { x: xOf(l.start.t), y: laneY(l.parent, pv) }
    segs.push(still(bend(from, { x: from.x, y: laneY(id, l.start.verse) }), VERSE[l.start.verse].color, 'bend'))
  }
  ps.forEach((p, i) => {
    const y = laneY(id, p.verse)
    const a = { x: xOf(p.from), y }
    segs.push(still(run(a, { x: xOf(p.to), y }), VERSE[p.verse].color, 'run'))
    const next = ps[i + 1]
    if (next) segs.push(still(arc({ x: xOf(p.to), y }, { x: xOf(next.from), y: laneY(id, next.verse) }), VERSE[p.verse].color, 'jump'))
  })
  return segs
}

export function lifelineBase(l: Lifeline): BaseLine {
  const first = lifePieces(l.id)[0]
  return {
    segs: lifeSegs(l.id),
    dash: DASH[l.entity],
    width: 1.3,
    start: l.parent ? undefined : { x: xOf(first.from), y: laneY(l.id, first.verse) },
  }
}

// A branch inherits its parent’s prefix through the split node.
export function storyNodes(id: string): { node: StoryNode; inherited: boolean }[] {
  const s = STORYLINES.find((q) => q.id === id)!
  const own = s.nodes.map((node) => ({ node, inherited: false }))
  if (!s.parent) return own
  const prefix = storyNodes(s.parent).slice(0, s.after! + 1)
  return [...prefix.map((n) => ({ ...n, inherited: true })), ...own]
}

const nodePt = (n: StoryNode): Pt => ({ x: xOf(n.t), y: storyY(n.verse) })

function storySegs(nodes: { node: StoryNode; inherited: boolean }[], flat: (i: number) => Pt): Seg[] {
  const segs: Seg[] = []
  for (let i = 0; i < nodes.length - 1; i++) {
    const a = nodes[i].node
    const b = nodes[i + 1].node
    const pa = nodePt(a)
    const pb = nodePt(b)
    const kind: Seg['kind'] = a.kind === 'jump' ? 'jump' : a.verse !== b.verse ? 'bend' : 'run'
    const t = kind === 'jump' ? arc(pa, pb) : kind === 'bend' ? bend(pa, pb) : run(pa, pb)
    segs.push({ t, f: run(flat(i), flat(i + 1)), color: VERSE[a.verse].color, kind, inherited: nodes[i + 1].inherited })
  }
  return segs
}

export function storylineBase(id: string): BaseLine {
  const s = STORYLINES.find((q) => q.id === id)!
  const nodes = s.nodes.map((node) => ({ node, inherited: false }))
  let segs = storySegs(nodes, (i) => nodePt(nodes[i].node))
  // Draw the branch origin from the parent’s split node.
  if (s.parent) {
    const leave = storyNodes(s.parent)[s.after!].node
    segs = [still(bend(nodePt(leave), nodePt(s.nodes[0])), VERSE[s.nodes[0].verse].color, 'bend'), ...segs]
  }
  return { segs, width: 2.4 }
}

// World events

// Each ancestor contributes history only up to the next branch point.
function verseChain(v: VerseId): { verse: VerseId; from: number; to: number }[] {
  const path: VerseId[] = []
  for (let q: VerseId | undefined = v; q; q = VERSE[q].parent) path.unshift(q)
  return path.map((id, i) => ({
    verse: id,
    from: verseStart(id),
    to: i < path.length - 1 ? VERSE[path[i + 1]].branchAt! : Infinity,
  }))
}

type SeenEvent = { id: string; track: 'Lantern' | 'Harbor'; label: string; from: number; to: number; major: boolean; placed: VerseId; ongoing: boolean }

export function eventsIn(v: VerseId): SeenEvent[] {
  const chain = verseChain(v)
  const sees = (verse: VerseId, t: number) => chain.some((c) => c.verse === verse && t >= c.from && t < c.to)
  return WORLD_EVENTS.filter((e) => sees(e.verse, e.t)).map((e) => {
    // Resolve the end against the same inherited path as the start.
    const end = e.ends?.find((r) => sees(r.verse, r.t))
    return {
      id: e.id,
      track: e.track,
      label: e.label,
      from: e.t,
      to: end ? end.t : END,
      major: !!e.major,
      placed: e.verse,
      ongoing: !end,
    }
  })
}

// Focus layouts

export type Sel = { kind: 'worldline' | 'lifeline' | 'storyline'; id: string }

export type FocusMark = {
  t: Pt
  f: Pt
  kind: 'snapshot' | 'event' | 'knowledge' | 'fork' | 'origin' | 'start' | 'scene' | 'select' | 'choice' | 'jump'
  label: string
  color: string
  major?: boolean
  known?: boolean
  inherited: boolean
}

type Stretch = { x0: number; x1: number; verse: VerseId; own: string; world: string; inherited: boolean }

export type Focus = {
  sel: Sel
  title: string
  kindLabel: string
  sub: string
  note: string
  segs: Seg[]
  marks: FocusMark[]
  stretches: Stretch[]
  // `w` is world time normalized to the compressed axis, in 0..1.
  trace: { x: number; w: number; jump: boolean }[]
  events: SeenEvent[]
  presence: { label: string; dash?: string; color: string; x0: number; x1: number; row: number }[]
  breaks: { x0: number; x1: number; label: string }[]
  axisNote?: string
  dash?: string
  width: number
}

export const FLAT_Y = 200
const GAP = 22

function squeezed(from: number, to: number, x: (t: number) => number) {
  return AXIS.filter((s) => !s.detail).flatMap((s) => {
    const a = Math.max(from, s.from)
    const b = Math.min(to, END, s.to)
    return b - a > 0 ? [{ x0: x(a), x1: x(b), label: `≈ ${fmtSpan(b - a)}` }] : []
  })
}

type ChainPiece = Piece & { inherited: boolean }

function lifeChain(id: string): ChainPiece[] {
  const l = LINE[id]
  const own = lifePieces(id).map((p) => ({ ...p, inherited: false }))
  if (!l.parent) return own
  const pv = VERSE[l.start.verse].parent!
  const up = lifeChain(l.parent)
  const i = up.findIndex((p) => p.line === l.parent && p.verse === pv && p.from <= l.start.t && l.start.t <= p.to)
  // Truncate inherited pieces at this branch point.
  const cut = [...up.slice(0, i), { ...up[i], to: l.start.t }]
  return [...cut.map((p) => ({ ...p, inherited: true })), ...own]
}

// Preserve compressed-axis proportions; jumps consume a gap but no elapsed time.
function flatLayout(pieces: { from: number; to: number }[], jumps: boolean[]) {
  const lens = pieces.map((p) => xOf(p.to) - xOf(p.from))
  const gaps = jumps.filter(Boolean).length * GAP
  const s = (PLOT_W - gaps) / lens.reduce((a, b) => a + b, 0)
  const x0: number[] = []
  let x = PLOT_L
  pieces.forEach((_, i) => {
    x0.push(x)
    x += lens[i] * s + (jumps[i] ? GAP : 0)
  })
  return (i: number, t: number) => {
    const p = pieces[i]
    const span = xOf(p.to) - xOf(p.from)
    const u = span > 0 ? (xOf(t) - xOf(p.from)) / span : 0
    return x0[i] + u * lens[i] * s
  }
}

function lifeFlat(id: string) {
  const chain = lifeChain(id)
  const links = chain.slice(0, -1).map((p, i) => (p.to === chain[i + 1].from ? 'bend' : 'jump'))
  const fx = flatLayout(chain, links.map((k) => k === 'jump'))
  return { chain, links, fx }
}

/** Position on the flattened lifeline; null if it is absent from this Verse/time. */
export function flatXOf(id: string, verse: VerseId, t: number): number | null {
  const { chain, fx } = lifeFlat(id)
  const i = chain.findIndex((p) => p.line === id && p.verse === verse && p.from <= t && t <= p.to)
  return i < 0 ? null : fx(i, t)
}

function lifelineFocus(id: string): Focus {
  const l = LINE[id]
  const { chain, links, fx } = lifeFlat(id)
  const tp = (i: number, t: number): Pt => ({ x: xOf(t), y: laneY(chain[i].line, chain[i].verse) })
  const fp = (i: number, t: number): Pt => ({ x: fx(i, t), y: FLAT_Y })
  const segs: Seg[] = []
  chain.forEach((p, i) => {
    const color = VERSE[p.verse].color
    segs.push({ t: run(tp(i, p.from), tp(i, p.to)), f: run(fp(i, p.from), fp(i, p.to)), color, kind: 'run', inherited: p.inherited })
    const next = chain[i + 1]
    if (!next) return
    const a = tp(i, p.to)
    const b = tp(i + 1, next.from)
    const t = links[i] === 'jump' ? arc(a, b) : bend(a, b)
    segs.push({ t, f: run(fp(i, p.to), fp(i + 1, next.from)), color, kind: links[i] as Seg['kind'], inherited: next.inherited })
  })

  const marks: FocusMark[] = []
  chain.forEach((p, i) => {
    for (const m of LINE[p.line].marks ?? []) {
      if (m.verse !== p.verse || m.t < p.from || m.t > p.to) continue
      marks.push({ t: tp(i, m.t), f: fp(i, m.t), kind: m.kind, label: m.label, color: VERSE[p.verse].color, major: m.major, known: m.known, inherited: p.inherited })
    }
    for (const c of LIFELINES) {
      if (c.parent !== p.line || c.id === id) continue
      const pv = VERSE[c.start.verse].parent
      if (pv !== p.verse || c.start.t < p.from || c.start.t > p.to) continue
      if (chain.some((q) => q.line === c.id)) continue
      marks.push({ t: tp(i, c.start.t), f: fp(i, c.start.t), kind: 'fork', label: `↳ ${lifeTitle(c)}`, color: VERSE[c.start.verse].color, inherited: p.inherited })
    }
  })
  const firstOwn = chain.findIndex((p) => !p.inherited)
  if (l.parent && firstOwn > 0) {
    const p = chain[firstOwn]
    marks.push({ t: tp(firstOwn, p.from), f: fp(firstOwn, p.from), kind: 'origin', label: `branches from ${lifeTitle(LINE[l.parent])}`, color: VERSE[p.verse].color, inherited: false })
  }

  const stretches: Stretch[] = chain.map((p, i) => {
    const end = Math.min(p.to, END)
    const open = p.to === Infinity
    const dur = end - p.from
    const age = p.own === 0 ? 'age 0' : `age ${fmtSpan(p.own)}`
    return {
      x0: fx(i, p.from),
      x1: fx(i, end),
      verse: p.verse,
      own: open ? `from ${age}` : dur < MONTH ? `${fmtSpan(dur)} here` : `${age} → ${fmtSpan(p.own + dur)}`,
      world: fmtRange(p.from, end, open),
      inherited: p.inherited,
    }
  })
  const trace = chain.flatMap((p, i) => {
    const end = Math.min(p.to, END)
    return [
      { x: fx(i, p.from), w: (xOf(p.from) - PLOT_L) / PLOT_W, jump: i > 0 && links[i - 1] === 'jump' },
      { x: fx(i, end), w: (xOf(end) - PLOT_L) / PLOT_W, jump: false },
    ]
  })

  const parentNote = l.parent ? `Branch of ${lifeTitle(LINE[l.parent])} · starts in ${verseTitle(l.start.verse)}` : `Root lifeline · born in Main, ${fmtWorld(l.start.t, false)}`
  return {
    sel: { kind: 'lifeline', id },
    title: lifeTitle(l),
    kindLabel: 'Lifeline',
    sub: parentNote,
    note: l.note,
    segs,
    marks,
    stretches,
    trace,
    events: [],
    presence: [],
    breaks: chain.flatMap((p, i) => squeezed(p.from, p.to, (t) => fx(i, t))),
    dash: DASH[l.entity],
    width: 2,
  }
}

function worldlineFocus(v: VerseId): Focus {
  const chain = verseChain(v).map((c) => ({ ...c, to: Math.min(c.to, END), inherited: c.verse !== v }))
  const segs: Seg[] = []
  chain.forEach((c, i) => {
    const y = baseY(c.verse)
    const color = VERSE[c.verse].color
    const a = { x: xOf(c.from), y }
    const b = { x: xOf(c.to), y }
    segs.push({ t: run(a, b), f: run({ x: a.x, y: FLAT_Y }, { x: b.x, y: FLAT_Y }), color, kind: 'run', inherited: c.inherited })
    const next = chain[i + 1]
    if (next) {
      const to = { x: xOf(next.from), y: baseY(next.verse) }
      // Collapse the bend at the split so it cannot cover a short parent stretch.
      const flat = { x: to.x, y: FLAT_Y }
      segs.push({ t: bend({ x: to.x, y }, to), f: run(flat, flat), color: VERSE[next.verse].color, kind: 'bend', inherited: next.inherited })
    }
  })
  const own = chain[chain.length - 1]
  const marks: FocusMark[] = []
  chain.forEach((c, i) => {
    if (i > 0) {
      const x = xOf(c.from)
      marks.push({ t: { x, y: baseY(c.verse) }, f: { x, y: FLAT_Y }, kind: 'origin', label: `${verseTag(c.verse)} splits from ${verseTag(chain[i - 1].verse)}`, color: VERSE[c.verse].color, inherited: c.inherited })
    }
  })
  for (const child of VERSES.filter((q) => q.parent === v)) {
    const x = xOf(child.branchAt!)
    marks.push({ t: { x, y: baseY(v) }, f: { x, y: FLAT_Y }, kind: 'fork', label: `↳ ${verseTitle(child.id)}`, color: child.color, inherited: false })
  }
  let row = 0
  const presence = LIFELINES.flatMap((l) => {
    const ps = lifePieces(l.id).filter((p) => p.verse === v)
    if (!ps.length) return []
    const r = row++
    return ps.map((p) => ({ label: lifeTitle(l), dash: DASH[l.entity], color: VERSE[v].color, x0: xOf(Math.max(p.from, own.from)), x1: xOf(Math.min(p.to, END)), row: r }))
  })
  const ver = VERSE[v]
  return {
    sel: { kind: 'worldline', id: v },
    title: verseTitle(v),
    kindLabel: 'Worldline',
    sub: ver.parent ? `Branch of ${verseTitle(ver.parent)} at ${fmtWorld(ver.branchAt!, true)}` : 'Root Verse · owns the world timeline',
    note: ver.origin,
    segs,
    marks,
    stretches: chain.map((c) => ({ x0: xOf(c.from), x1: xOf(c.to), verse: c.verse, own: '', world: '', inherited: c.inherited })),
    trace: [],
    events: eventsIn(v),
    presence,
    breaks: chain.flatMap((c) => squeezed(c.from, c.to, xOf)),
    width: 2.4,
  }
}

function storylineFocus(id: string): Focus {
  const s = STORYLINES.find((q) => q.id === id)!
  const nodes = storyNodes(id)
  // Reserve space for the last tilted label.
  const slot = (PLOT_W - 72) / nodes.length
  const fx = (i: number) => PLOT_L + (i + 0.5) * slot
  const flat = (i: number): Pt => ({ x: fx(i), y: FLAT_Y })
  const segs = storySegs(nodes, flat)
  const marks: FocusMark[] = nodes.map(({ node, inherited }, i) => ({
    t: nodePt(node),
    f: flat(i),
    kind: node.kind,
    label: node.label,
    color: VERSE[node.verse].color,
    inherited,
  }))
  const stretches: Stretch[] = []
  let k = 0
  for (let i = 1; i <= nodes.length; i++) {
    const prev = nodes[i - 1].node
    const cur = nodes[i]?.node
    if (cur && prev.kind !== 'jump' && cur.verse === prev.verse) continue
    const a = nodes[k].node
    const precise = inDetail(a.t) || inDetail(prev.t)
    stretches.push({
      x0: fx(k) - slot / 2 + 2,
      x1: fx(i - 1) + slot / 2 - 2,
      verse: a.verse,
      own: '',
      world: precise ? fmtWorld(a.t, true) : fmtWorld(a.t, false),
      inherited: nodes[i - 1].inherited,
    })
    k = i
  }
  const trace = nodes.map(({ node }, i) => ({ x: fx(i), w: (xOf(node.t) - PLOT_L) / PLOT_W, jump: i > 0 && nodes[i - 1].node.kind === 'jump' }))
  return {
    sel: { kind: 'storyline', id },
    title: s.name,
    kindLabel: 'Storyline',
    sub: s.parent ? `Branch of the root storyline after node ${s.after} · ${nodes.length} nodes` : `Play “Lanternfall” · ${nodes.length} nodes`,
    note: s.note,
    segs,
    marks,
    stretches,
    trace,
    events: [],
    presence: [],
    breaks: [],
    axisNote: 'in flow order',
    width: 2.6,
  }
}

export function focusOf(sel: Sel): Focus {
  if (sel.kind === 'worldline') return worldlineFocus(sel.id as VerseId)
  if (sel.kind === 'lifeline') return lifelineFocus(sel.id)
  return storylineFocus(sel.id)
}

// Label placement

/** Assign labels greedily to non-overlapping rows above the flat line. */
export function labelRows(items: { x: number; text: string }[], rows: number): number[] {
  const used: [number, number][][] = Array.from({ length: rows }, () => [])
  return items.map(({ x, text }) => {
    const w = text.length * 5.4 + 10
    const span: [number, number] = [x - w / 2, x + w / 2]
    for (let r = 0; r < rows; r++) {
      if (used[r].every(([a, b]) => span[1] < a || span[0] > b)) {
        used[r].push(span)
        return r
      }
    }
    return -1
  })
}
