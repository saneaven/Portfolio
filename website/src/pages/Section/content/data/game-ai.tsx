import type { ContentBlocks } from '../contentblock/types'
import githubIcon from '../../../../assets/github-white-icon.svg'

const gameAi: ContentBlocks = [
  {
    type: 'lead',
    eyebrow: 'Game · AI programming',
    title: 'Game AI Projects',
    subtitle:
      'Three assignments on DigiPen\'s in-house AI framework, each a focused dive into a classic game-AI technique. I built a behavior-tree scene that recreates Squid Game 2\'s "mingle" round, optimised an A* pathfinder to a fraction of the framework\'s baseline, and wrote a terrain-analysis toolkit — openness, line-of-sight visibility and influence propagation — that feeds a seek-and-hide enemy.',
    meta: [
      { label: 'Role', value: 'Solo — implemented all three assignments' },
      { label: 'Context', value: 'DigiPen · 2024–2025' },
      { label: 'Focus', value: 'Behavior trees · A* pathfinding · terrain analysis' },
      { label: 'Stack', value: 'C++ · DigiPen\'s in-house AI framework · Direct2D / DirectWrite · Visual Studio' },
    ],
  },
  {
    type: 'section',
    eyebrow: 'Overview',
    title: 'One framework, three classic problems',
    blocks: [
      {
        type: 'split',
        paragraphs: [
          'Rather than build a game, each assignment isolates a single game-AI problem and pushes it as far as the framework allows. The brief is provided — the engine, the editor, the test harness — and the work is the algorithm that plugs into it: how an agent decides, how it routes, and how it reads the terrain it moves through.',
          'Working against a shared framework meant respecting fixed function signatures and reusing the provided debug tooling, then doing everything interesting inside that boundary — custom behavior-tree nodes, a bucketed A* open list, and a stack of terrain analyses built from the same grid primitives.',
        ],
        media: { caption: 'Three assignments · one in-house AI framework' },
      },
    ],
  },
  {
    type: 'section',
    eyebrow: 'Assignment 1 — Behavior trees',
    title: 'Recreating Squid Game 2\'s "mingle" round',
    blocks: [
      {
        type: 'split',
        paragraphs: [
          'The first assignment recreates the "mingle" scene: on a mouse click the participants line up and march forward, then circle around; on the next click they pair off in twos, and the one participant left without a pair is eliminated by the staff. Driving that whole sequence are four behavior trees — AgentTriangle, Bullet, ParticipantFollower and ParticipantLeader — composed from custom control, decorator and leaf nodes.',
          'The headline custom node is C_ModifiedParallelSequencer: a parallel sequencer that, the moment one child fails, forces every other child to fail too so their on-exit paths actually run — closing a gap I hit where the stock parallel sequencer left sibling nodes\' on_exit uncalled.',
        ],
        media: { caption: 'Line up · circle · pair off · eliminate' },
      },
      {
        type: 'cardGrid',
        cards: [
          {
            title: 'Custom control flow',
            body: 'C_ModifiedParallelSequencer ticks every child each frame, but on any failure it calls on_failure on the remaining children so their exit logic fires deterministically — the fix for the stock parallel node skipping siblings\' on_exit.',
          },
          {
            title: 'Decorators for timing',
            body: 'D_ShortDelay gates a child behind a brief wait, and D_RepeatThreeTimes loops its child three times — chained together to fire a short burst of shots from the staff.',
          },
          {
            title: 'Formation leaf nodes',
            body: 'L_CircularFormation reads each agent\'s "Circular Position" off the blackboard and orbits it around a shared centre, ramping turn speed and briefly boosting movement when the target is far so the ring snaps into place. L_FormationFollow and L_MoveToLeader keep the followers locked to the leader.',
          },
          {
            title: 'Sequencing the scene',
            body: 'L_CheckMouseClick, L_CheckOrder, L_OrderFollow and L_OrderClear advance the round on input; L_SpawnAgent, L_Shoot, L_Explode, L_WaitForDeath and L_DeleteSelf handle the staff, the gunfire and the elimination — with BGM and SFX leaf nodes scoring it.',
          },
        ],
      },
    ],
  },
  {
    type: 'section',
    eyebrow: 'Assignment 2 — A* pathfinding',
    title: 'A* tuned to a fraction of the baseline',
    blocks: [
      {
        type: 'minis',
        items: [
          {
            title: 'My A*',
            body: 'Ran the framework\'s speed test in about 975 ms — the second-fastest result in the whole class.',
          },
          {
            title: 'Framework baseline',
            body: 'The provided reference code ran the same test in roughly 2400 ms.',
          },
        ],
      },
      {
        type: 'pipeline',
        title: 'Where the time went',
        body: 'The win came from the open list. Instead of a heap, nodes are bucketed by final cost into fixed 0.5-wide intervals, and a cached fastest-bucket index walks forward as buckets empty — so pop avoids re-sorting the whole frontier. Each cell pre-bakes its walkable neighbours into a single byte bitmask (with diagonal moves pruned when a cardinal side is blocked) so the neighbour loop is a handful of bit tests instead of repeated wall queries. On the map-change message the neighbour masks are rebuilt once and reused across every request.',
        steps: [
          'Push start node into its cost bucket',
          'Pop from the fastest non-empty bucket',
          'Expand neighbours via the precomputed bitmask',
          'Re-bucket on a cheaper path found',
          'Reconstruct, rubber-band, then smooth',
        ],
      },
      {
        type: 'cardGrid',
        cards: [
          {
            title: 'Bucketed open list',
            body: 'finalCost maps to a bucket index; the OpenList tracks the lowest occupied bucket and only scans within a bucket for its minimum, so adds and removals stay cheap and the next node is found without sorting the full frontier.',
          },
          {
            title: 'Neighbour bitmask',
            body: 'A per-cell byte encodes which of the eight neighbours are reachable. A post-pass clears each diagonal whose adjacent cardinal is a wall, so the search never tries to squeeze a diagonal through a corner.',
          },
          {
            title: 'Five heuristics',
            body: 'Octile, Manhattan, Chebyshev, Euclidean and an intentionally inconsistent mode share one calcHeuristic switch, each scaled by the request weight so the same pather covers every grading case.',
          },
          {
            title: 'Path post-processing',
            body: 'Rubber-banding collapses runs of collinear waypoints whose bounding box is wall-free, then smoothing re-inserts midpoints on long gaps and runs a Catmull-Rom spline through the result for a curved, natural path.',
          },
        ],
      },
    ],
  },
  {
    type: 'section',
    eyebrow: 'Assignment 3 — Terrain analysis',
    title: 'Reading the map so the AI can use it',
    blocks: [
      {
        type: 'text',
        paragraphs: [
          'The last assignment pre-analyses a static map into a stack of float layers an agent can sample at runtime. The takeaway that stuck with me: to put genuinely smart behaviour into a game AI, you analyse the terrain up front rather than reasoning about it every frame.',
          'The layers feed a seek-and-hide game of tag — the enemy stamps its field-of-view cone into an influence layer, propagates that influence outward through the map, and walks toward the strongest cell to chase the player. It works well until the player hides cleverly; weighting the player\'s last-known heading is the obvious next improvement.',
        ],
      },
      {
        type: 'minis',
        items: [
          {
            title: 'Openness',
            body: 'Each open cell is scored 1 / d² against the Euclidean distance to its nearest wall or map edge, so corridors and corners read as more enclosed than open floor.',
          },
          {
            title: 'Visibility',
            body: 'Line-of-sight between two cells is a wall-corner crossing test: walk the candidate walls between them and use a 2D cross-product sign check on each wall box\'s corners to decide whether the sight line clips it.',
          },
          {
            title: 'Agent vision cone',
            body: 'A cell is visible to an agent when the dot product of the agent\'s forward vector and the direction to the cell clears the field-of-view cosine — comparing cosines directly to dodge arccosine error — and the sight line is clear.',
          },
          {
            title: 'Influence & propagation',
            body: 'Influence spreads cell to cell with exponential distance decay, lerping each cell toward its strongest decayed neighbour by a growth factor into a temp layer, then normalising the whole map into [0, 1].',
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
          { layer: 'Language', items: ['C++', 'Visual Studio'] },
          { layer: 'Framework', items: ['DigiPen\'s in-house AI framework', 'Behavior-tree editor', 'Map / grid primitives'] },
          { layer: 'Rendering', items: ['Direct2D', 'DirectWrite'] },
          { layer: 'Techniques', items: ['Behavior trees', 'A*', 'Bucketed open list', 'Influence maps', 'Line-of-sight'] },
        ],
      },
    ],
  },
  {
    type: 'links',
    links: [
      {
        label: 'Source on GitHub',
        href: 'https://github.com/saneaven/Portfolio/tree/main/projects/game/game_ai',
        icon: githubIcon,
        hint: 'Code & technical docs',
      },
    ],
  },
  {
    type: 'closing',
    text: 'Three solo assignments on a shared framework — a behavior-tree mingle scene, an A* pather tuned to roughly 975 ms against a 2400 ms baseline (second-fastest in the class), and a terrain-analysis toolkit driving a seek-and-hide enemy.',
  },
]

export default gameAi
