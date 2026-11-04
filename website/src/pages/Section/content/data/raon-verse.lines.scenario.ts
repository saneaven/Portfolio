// A Verse split branches every lifeline present; a jump moves a line without
// splitting it. Branches inherit their parent's past records.
// Tide Reckoning uses 12 months of 30 days; all times are day counts.
export const YEAR = 360
export const MONTH = 30

export const at = (y: number, m = 1, d = 1, h = 0) => (y * 12 + (m - 1)) * MONTH + (d - 1) + h / 24

export type VerseId = 'main' | 'a' | 'b' | 'c' | 'd' | 'e'
export type EntityId = 'mira' | 'oren' | 'vael'

export type Verse = {
  id: VerseId
  name: string
  color: string
  parent?: VerseId
  branchAt?: number
  origin: string
}

// List children latest-first so earlier split curves pass the later ones.
export const VERSES: Verse[] = [
  { id: 'main', name: 'Main', color: 'oklch(70% 0.15 248)', origin: 'The root Verse, made with the Multiverse.' },
  {
    id: 'a',
    name: 'First Return',
    color: 'oklch(76% 0.1 205)',
    parent: 'main',
    branchAt: at(1000, 9, 12, 6),
    origin: 'Split from Main by Mira’s first time travel.',
  },
  {
    id: 'd',
    name: 'Quiet Harbor',
    color: 'oklch(83% 0.15 92)',
    parent: 'a',
    branchAt: at(1000, 9, 15, 5),
    origin: 'Split from First Return when “Let it be” was picked instead.',
  },
  {
    id: 'b',
    name: 'Second Return',
    color: 'oklch(74% 0.15 158)',
    parent: 'a',
    branchAt: at(1000, 9, 13),
    origin: 'Split from First Return by Mira’s second time travel.',
  },
  {
    id: 'c',
    name: 'Grey Wake',
    color: 'oklch(80% 0.035 270)',
    parent: 'b',
    branchAt: at(1006, 6, 1),
    origin: 'Split from Second Return when Vael fled forward with a shard.',
  },
  {
    id: 'e',
    name: 'Ashen Lantern',
    color: 'oklch(67% 0.19 19)',
    parent: 'c',
    branchAt: at(1006, 6, 3, 10),
    origin: 'Split from Grey Wake when “Let him keep it” was picked instead.',
  },
]

export const ENTITIES: { id: EntityId; name: string; born: number }[] = [
  { id: 'mira', name: 'Mira', born: at(980, 5, 2) },
  { id: 'oren', name: 'Oren', born: at(986, 11, 20) },
  { id: 'vael', name: 'Vael', born: at(968, 2, 10) },
]

export type LifeMark = {
  kind: 'snapshot' | 'event' | 'knowledge'
  verse: VerseId
  t: number
  label: string
  major?: boolean
  // Knowledge state from this mark onward.
  known?: boolean
}

export type Lifeline = {
  id: string
  entity: EntityId
  tag: string
  note: string
  parent?: string
  // Birth for a root lifeline; the split time for a branch.
  start: { verse: VerseId; t: number }
  jumps?: { depart: number; verse: VerseId; arrive: number }[]
  marks?: LifeMark[]
}

const SECRET = 'The ember secret'

export const LIFELINES: Lifeline[] = [
  {
    id: 'mira',
    entity: 'mira',
    tag: 'root',
    note: 'The traveller. One lifeline that jumps four times and never splits.',
    start: { verse: 'main', t: at(980, 5, 2) },
    jumps: [
      { depart: at(1012, 3, 1, 10), verse: 'a', arrive: at(1000, 9, 12, 6) },
      { depart: at(1000, 9, 15, 18), verse: 'b', arrive: at(1000, 9, 13) },
      { depart: at(1000, 9, 14, 23), verse: 'c', arrive: at(1006, 6, 1) },
      { depart: at(1006, 6, 3, 22), verse: 'b', arrive: at(1000, 9, 15, 6) },
    ],
    marks: [
      { kind: 'knowledge', verse: 'main', t: at(980, 5, 2), label: `${SECRET}: unknown`, known: false },
      { kind: 'snapshot', verse: 'main', t: at(990, 5), label: 'Keeper’s daughter' },
      { kind: 'snapshot', verse: 'main', t: at(1000, 5), label: 'Harbor runner' },
      { kind: 'event', verse: 'main', t: at(1000, 9, 14, 21), label: 'Watches Oren drown', major: true },
      { kind: 'snapshot', verse: 'main', t: at(1011, 5), label: 'Keeper of a dead light' },
      { kind: 'event', verse: 'main', t: at(1012, 3, 1, 8), label: 'Relights the ember' },
      { kind: 'knowledge', verse: 'main', t: at(1012, 3, 1, 8.5), label: `${SECRET}: known`, known: true },
      { kind: 'event', verse: 'a', t: at(1000, 9, 14, 21), label: 'Pulls Oren from the tide', major: true },
      { kind: 'event', verse: 'a', t: at(1000, 9, 15, 12), label: 'Says goodbye in the ruin' },
      { kind: 'event', verse: 'b', t: at(1000, 9, 13), label: 'Meets herself, twice' },
      { kind: 'event', verse: 'b', t: at(1000, 9, 14, 21), label: 'Holds the light with her other selves', major: true },
      { kind: 'event', verse: 'c', t: at(1006, 6, 2, 12), label: 'Fights Vael for the shard' },
      { kind: 'event', verse: 'c', t: at(1006, 6, 3, 17), label: 'Takes the shard back' },
      { kind: 'event', verse: 'b', t: at(1000, 9, 15, 6), label: 'Home to the morning after' },
    ],
  },
  {
    id: 'mira-a',
    entity: 'mira',
    tag: 'A',
    note: 'Her 20-year-old self in First Return, branched when Main split.',
    parent: 'mira',
    start: { verse: 'a', t: at(1000, 9, 12, 6) },
    marks: [
      { kind: 'event', verse: 'a', t: at(1000, 9, 13, 10), label: 'Meets a stranger with her face' },
      { kind: 'snapshot', verse: 'a', t: at(1004, 5), label: 'Keeper of the rebuilt Lantern' },
    ],
  },
  {
    id: 'mira-b',
    entity: 'mira',
    tag: 'B',
    note: 'Her younger self in Second Return. Grows up with her brother alive.',
    parent: 'mira-a',
    start: { verse: 'b', t: at(1000, 9, 13) },
    marks: [
      { kind: 'event', verse: 'b', t: at(1000, 9, 14, 21), label: 'Holds the light with two strangers' },
      { kind: 'snapshot', verse: 'b', t: at(1005, 5), label: 'Apprentice keeper, brother alive' },
      { kind: 'snapshot', verse: 'b', t: at(1010, 1), label: 'Keeper of the held light' },
    ],
  },
  {
    id: 'mira-b2',
    entity: 'mira',
    tag: 'B′',
    note: 'The traveller as she was eighteen hours after her first arrival. She was in First Return when it split, so she branched too, and stays.',
    parent: 'mira',
    start: { verse: 'b', t: at(1000, 9, 13) },
    marks: [
      { kind: 'event', verse: 'b', t: at(1000, 9, 13), label: 'Meets herself, almost three days older' },
      { kind: 'event', verse: 'b', t: at(1000, 9, 14, 21), label: 'Holds the light with the others' },
      { kind: 'snapshot', verse: 'b', t: at(1002, 1), label: 'Second keeper of the harbor' },
      { kind: 'event', verse: 'b', t: at(1010, 1), label: 'Hands the Lantern to her younger self' },
    ],
  },
  {
    id: 'mira-c',
    entity: 'mira',
    tag: 'C',
    note: 'Branched from Mira · B when Grey Wake split. Age 26 here.',
    parent: 'mira-b',
    start: { verse: 'c', t: at(1006, 6, 1) },
  },
  {
    id: 'mira-c2',
    entity: 'mira',
    tag: 'C′',
    note: 'Branched from Mira′ · B when Grey Wake split. Age 37 here, older than the traveller.',
    parent: 'mira-b2',
    start: { verse: 'c', t: at(1006, 6, 1) },
  },
  {
    id: 'mira-d',
    entity: 'mira',
    tag: 'D',
    note: 'Her younger self, branched again when Quiet Harbor split.',
    parent: 'mira-a',
    start: { verse: 'd', t: at(1000, 9, 15, 5) },
  },
  {
    id: 'mira-d2',
    entity: 'mira',
    tag: 'D′',
    note: 'The traveller, branched where she stood when Quiet Harbor split. She lets it be.',
    parent: 'mira',
    start: { verse: 'd', t: at(1000, 9, 15, 5) },
    marks: [{ kind: 'event', verse: 'd', t: at(1000, 10, 1), label: 'Raises a small light from the ruin' }],
  },
  {
    id: 'mira-e',
    entity: 'mira',
    tag: 'E',
    note: 'Branched from Mira · C when Ashen Lantern split.',
    parent: 'mira-c',
    start: { verse: 'e', t: at(1006, 6, 3, 10) },
  },
  {
    id: 'mira-e2',
    entity: 'mira',
    tag: 'E′',
    note: 'Branched from Mira′ · C when Ashen Lantern split.',
    parent: 'mira-c2',
    start: { verse: 'e', t: at(1006, 6, 3, 10) },
  },
  {
    id: 'mira-e3',
    entity: 'mira',
    tag: 'E″',
    note: 'The traveller, branched in Grey Wake before she could take the shard back.',
    parent: 'mira',
    start: { verse: 'e', t: at(1006, 6, 3, 10) },
    marks: [{ kind: 'event', verse: 'e', t: at(1006, 7, 1), label: 'Signs on as the Grey Wake’s navigator' }],
  },
  {
    id: 'oren',
    entity: 'oren',
    tag: 'root',
    note: 'Drowns in the Black Tide in Main.',
    start: { verse: 'main', t: at(986, 11, 20) },
    marks: [
      { kind: 'snapshot', verse: 'main', t: at(994, 1), label: 'Boy of the docks' },
      { kind: 'snapshot', verse: 'main', t: at(999, 11), label: 'Apprentice diver' },
      { kind: 'event', verse: 'main', t: at(1000, 9, 14, 21), label: 'Drowns in the Black Tide', major: true },
    ],
  },
  {
    id: 'oren-a',
    entity: 'oren',
    tag: 'A',
    note: 'Branched when Main split. The drowning was after the split, so it is not inherited.',
    parent: 'oren',
    start: { verse: 'a', t: at(1000, 9, 12, 6) },
    marks: [
      { kind: 'event', verse: 'a', t: at(1000, 9, 14, 21), label: 'Pulled from the tide by a stranger', major: true },
      { kind: 'event', verse: 'a', t: at(1004, 4), label: 'Rebuilds the Lantern' },
    ],
  },
  {
    id: 'oren-b',
    entity: 'oren',
    tag: 'B',
    note: 'Learns his sister’s secret, and every branch after him keeps knowing it.',
    parent: 'oren-a',
    start: { verse: 'b', t: at(1000, 9, 13) },
    marks: [
      { kind: 'knowledge', verse: 'b', t: at(1000, 9, 13, 10), label: `${SECRET}: known`, known: true },
      { kind: 'event', verse: 'b', t: at(1000, 9, 14, 21), label: 'Survives the tide' },
      { kind: 'snapshot', verse: 'b', t: at(1005, 1), label: 'Harbor pilot' },
    ],
  },
  {
    id: 'oren-c',
    entity: 'oren',
    tag: 'C',
    note: 'Branched from Oren · B, so he already knows the secret.',
    parent: 'oren-b',
    start: { verse: 'c', t: at(1006, 6, 1) },
  },
  {
    id: 'oren-d',
    entity: 'oren',
    tag: 'D',
    note: 'Branched from Oren · A, so he never learns the secret.',
    parent: 'oren-a',
    start: { verse: 'd', t: at(1000, 9, 15, 5) },
    marks: [{ kind: 'event', verse: 'd', t: at(1000, 10, 1), label: 'Tends the small light' }],
  },
  {
    id: 'oren-e',
    entity: 'oren',
    tag: 'E',
    note: 'Branched from Oren · C.',
    parent: 'oren-c',
    start: { verse: 'e', t: at(1006, 6, 3, 10) },
  },
  {
    id: 'vael',
    entity: 'vael',
    tag: 'root',
    note: 'A harbor smuggler in Main.',
    start: { verse: 'main', t: at(968, 2, 10) },
    marks: [
      { kind: 'snapshot', verse: 'main', t: at(998, 1), label: 'Harbor smuggler' },
      { kind: 'event', verse: 'main', t: at(1000, 9, 15, 6), label: 'Loots the drowned harbor' },
    ],
  },
  {
    id: 'vael-a',
    entity: 'vael',
    tag: 'A',
    note: 'Branched when Main split.',
    parent: 'vael',
    start: { verse: 'a', t: at(1000, 9, 12, 6) },
    marks: [{ kind: 'event', verse: 'a', t: at(1000, 9, 15, 4), label: 'Loots the fallen Lantern' }],
  },
  {
    id: 'vael-b',
    entity: 'vael',
    tag: 'B',
    note: 'Sees the ember work, steals a shard and jumps forward six years, splitting Second Return.',
    parent: 'vael-a',
    start: { verse: 'b', t: at(1000, 9, 13) },
    jumps: [{ depart: at(1000, 9, 14, 23), verse: 'c', arrive: at(1006, 6, 1) }],
    marks: [
      { kind: 'knowledge', verse: 'b', t: at(1000, 9, 14, 21), label: `${SECRET}: known`, known: true },
      { kind: 'event', verse: 'b', t: at(1000, 9, 14, 22), label: 'Steals a shard of the ember', major: true },
      { kind: 'event', verse: 'c', t: at(1006, 6, 2, 12), label: 'Seizes the Lantern', major: true },
      { kind: 'event', verse: 'c', t: at(1006, 6, 3, 17), label: 'Loses the shard to Mira' },
    ],
  },
  {
    id: 'vael-d',
    entity: 'vael',
    tag: 'D',
    note: 'Branched from Vael · A. Never sees the ember.',
    parent: 'vael-a',
    start: { verse: 'd', t: at(1000, 9, 15, 5) },
  },
  {
    id: 'vael-e',
    entity: 'vael',
    tag: 'E',
    note: 'Branched from Vael · B after his jump, so he keeps the secret and the shard.',
    parent: 'vael-b',
    start: { verse: 'e', t: at(1006, 6, 3, 10) },
    marks: [{ kind: 'event', verse: 'e', t: at(1006, 7, 1), label: 'Rules the harbor' }],
  },
]

type WorldEvent = {
  id: string
  track: 'Lantern' | 'Harbor'
  label: string
  // Branches that split before `t` do not inherit this event.
  verse: VerseId
  t: number
  // The same event may end at different times in different branches.
  ends?: { verse: VerseId; t: number }[]
  major?: boolean
}

const TIDE = { from: at(1000, 9, 14, 20), to: at(1000, 9, 15, 4) }

export const WORLD_EVENTS: WorldEvent[] = [
  { id: 'raised', track: 'Lantern', label: 'Raised', verse: 'main', t: at(896), ends: [{ verse: 'main', t: at(900) }] },
  {
    id: 'lit',
    track: 'Lantern',
    label: 'Lit',
    verse: 'main',
    t: at(900),
    ends: [
      { verse: 'main', t: at(1000, 9, 14, 21) },
      { verse: 'a', t: at(1000, 9, 14, 21.5) },
      { verse: 'c', t: at(1006, 6, 2, 12) },
    ],
  },
  { id: 'relit', track: 'Lantern', label: 'Relit', verse: 'main', t: at(1012, 3, 1, 8) },
  {
    id: 'ruin',
    track: 'Lantern',
    label: 'Ruin',
    verse: 'a',
    t: at(1000, 9, 14, 21.5),
    ends: [
      { verse: 'd', t: at(1000, 10, 1) },
      { verse: 'a', t: at(1004, 4) },
    ],
  },
  { id: 'rebuilt', track: 'Lantern', label: 'Rebuilt by Oren', verse: 'a', t: at(1004, 4) },
  { id: 'small-light', track: 'Lantern', label: 'Small light', verse: 'd', t: at(1000, 10, 1) },
  { id: 'seized', track: 'Lantern', label: 'Seized by the Grey Wake', verse: 'c', t: at(1006, 6, 2, 12), ends: [{ verse: 'c', t: at(1009, 1) }], major: true },
  { id: 'retaken', track: 'Lantern', label: 'Retaken by the harbor', verse: 'c', t: at(1009, 1) },
  { id: 'tide-main', track: 'Harbor', label: 'Black Tide', verse: 'main', t: TIDE.from, ends: [{ verse: 'main', t: TIDE.to }], major: true },
  { id: 'tide-a', track: 'Harbor', label: 'Black Tide', verse: 'a', t: TIDE.from, ends: [{ verse: 'a', t: TIDE.to }], major: true },
  { id: 'tide-b', track: 'Harbor', label: 'Black Tide', verse: 'b', t: TIDE.from, ends: [{ verse: 'b', t: TIDE.to }], major: true },
  { id: 'wake-rules', track: 'Harbor', label: 'Grey Wake rules the harbor', verse: 'e', t: at(1006, 7, 1) },
  { id: 'festival', track: 'Harbor', label: 'Festival of the held light', verse: 'b', t: at(1008, 8), ends: [{ verse: 'b', t: at(1008, 8, 4) }] },
]

export type StoryNode = {
  kind: 'start' | 'scene' | 'select' | 'choice' | 'jump'
  verse: VerseId
  t: number
  label: string
  to?: { verse: VerseId; t: number }
}

type Storyline = {
  id: string
  name: string
  note: string
  parent?: string
  // Index in the parent's inherited node sequence.
  after?: number
  nodes: StoryNode[]
}

export const STORYLINES: Storyline[] = [
  {
    id: 's0',
    name: 'Root storyline',
    note: 'The play as it was first played: a flashback, two time travels and a chase six years forward.',
    nodes: [
      { kind: 'start', verse: 'main', t: at(1012, 3, 1, 8), label: 'Play starts' },
      { kind: 'scene', verse: 'main', t: at(1012, 3, 1, 8), label: 'Mira relights the ember' },
      { kind: 'jump', verse: 'main', t: at(1012, 3, 1, 9), label: 'Flashback', to: { verse: 'main', t: at(1000, 9, 14, 20.5) } },
      { kind: 'scene', verse: 'main', t: at(1000, 9, 14, 21), label: 'The night Oren drowned' },
      { kind: 'jump', verse: 'main', t: at(1000, 9, 14, 21.5), label: 'Back to the present', to: { verse: 'main', t: at(1012, 3, 1, 9.5) } },
      { kind: 'jump', verse: 'main', t: at(1012, 3, 1, 10), label: 'Time travel, splits Main', to: { verse: 'a', t: at(1000, 9, 12, 6) } },
      { kind: 'scene', verse: 'a', t: at(1000, 9, 13, 10), label: 'She warns Oren; he laughs' },
      { kind: 'scene', verse: 'a', t: at(1000, 9, 14, 21), label: 'Oren saved, the Lantern falls' },
      { kind: 'select', verse: 'a', t: at(1000, 9, 15, 5), label: 'Go back again / Let it be' },
      { kind: 'choice', verse: 'a', t: at(1000, 9, 15, 5), label: 'Go back again' },
      { kind: 'scene', verse: 'a', t: at(1000, 9, 15, 12), label: 'Goodbye to Oren in the ruin' },
      { kind: 'jump', verse: 'a', t: at(1000, 9, 15, 18), label: 'Time travel, splits First Return', to: { verse: 'b', t: at(1000, 9, 13) } },
      { kind: 'scene', verse: 'b', t: at(1000, 9, 13), label: 'Three Miras in one room' },
      { kind: 'scene', verse: 'b', t: at(1000, 9, 13, 10), label: 'She tells Oren the truth' },
      { kind: 'scene', verse: 'b', t: at(1000, 9, 14, 21), label: 'Three keepers hold the light' },
      { kind: 'jump', verse: 'b', t: at(1000, 9, 14, 23), label: 'Vael flees forward, splits Second Return', to: { verse: 'c', t: at(1006, 6, 1) } },
      { kind: 'scene', verse: 'c', t: at(1006, 6, 2, 12), label: 'The Grey Wake seizes the Lantern' },
      { kind: 'select', verse: 'c', t: at(1006, 6, 3, 10), label: 'Take back the shard / Let him keep it' },
      { kind: 'choice', verse: 'c', t: at(1006, 6, 3, 10), label: 'Take back the shard' },
      { kind: 'scene', verse: 'c', t: at(1006, 6, 3, 17), label: 'She wrests the shard from Vael' },
      { kind: 'jump', verse: 'c', t: at(1006, 6, 3, 22), label: 'Home to Second Return', to: { verse: 'b', t: at(1000, 9, 15, 6) } },
      { kind: 'scene', verse: 'b', t: at(1000, 9, 15, 6), label: 'Dawn: Oren alive, three Miras' },
    ],
  },
  {
    id: 's1',
    name: 'Let it be',
    note: 'Re-picked at the first choice. Shares everything up to it as the same records, then splits Quiet Harbor.',
    parent: 's0',
    after: 8,
    nodes: [
      { kind: 'start', verse: 'd', t: at(1000, 9, 15, 5), label: 'Branch starts, splits First Return' },
      { kind: 'choice', verse: 'd', t: at(1000, 9, 15, 5), label: 'Let it be' },
      { kind: 'scene', verse: 'd', t: at(1000, 10, 1), label: 'A small light from the ruin' },
    ],
  },
  {
    id: 's2',
    name: 'Let him keep it',
    note: 'Re-picked at the second choice. Shares the flashback, both time travels and the chase, then splits Ashen Lantern.',
    parent: 's0',
    after: 17,
    nodes: [
      { kind: 'start', verse: 'e', t: at(1006, 6, 3, 10), label: 'Branch starts, splits Grey Wake' },
      { kind: 'choice', verse: 'e', t: at(1006, 6, 3, 10), label: 'Let him keep it' },
      { kind: 'scene', verse: 'e', t: at(1006, 7, 1), label: 'Mira joins the Grey Wake' },
    ],
  },
]
