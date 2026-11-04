import type { ContentBlocks } from '../contentblock/types'
import githubIcon from '../../../../assets/github-white-icon.svg'
import conceptRender from '../../../../assets/section/game/q-by-noflex/hero.avif'
import EnemyRoster from './q-by-noflex.enemies'

const qByNoflex: ContentBlocks = [
  {
    type: 'lead',
    eyebrow: 'Game · Engine programming',
    title: 'The ECS core, parallel systems and enemy AI of a roguelike platformer.',
    subtitle:
      'Q is a side-view roguelike platformer where you fight through randomly arranged rooms to collect the cube bricks of a station adrift in orbit. Team Noflex built it on a custom C++ engine — my part was the ECS architecture, its parallel system scheduler, and the enemy AI.',
    video: { id: 'htrNr4Ljq0c', title: 'Q by Noflex — Trailer' },
    meta: [
      { label: 'Role', value: 'Engine — ECS, parallelism & enemy AI (also team PM)' },
      { label: 'Focus', value: 'ECS architecture · multithreaded systems · enemy AI' },
      { label: 'Stack', value: 'C++ · custom ECS · worker-thread scheduler' },
    ],
  },
  {
    type: 'section',
    eyebrow: 'Overview',
    title: 'Cube, in orbit',
    blocks: [
      {
        type: 'split',
        paragraphs: [
          'The concept takes the trapped-in-a-shifting-maze premise of the film Cube and lifts it into science fiction: the maze is a derelict station on orbit, broken into rooms that are arranged differently every run. The player drops in, clears each room and pushes on to collect every cube brick.',
          'Because layouts and encounters are rebuilt per run, the underlying systems had to spin entities up and tear them down cheaply every time — which is where the ECS and its parallel scheduling earned their keep.',
        ],
        media: { caption: 'Concept render — the station in orbit', image: conceptRender },
      },
    ],
  },
  {
    type: 'section',
    eyebrow: 'Core — ECS & parallelism',
    title: 'A data-oriented engine that scales across threads',
    blocks: [
      {
        type: 'cardGrid',
        cards: [
          {
            title: 'Entity Component System',
            body: 'I built the ECS core: entities are ids, behaviour lives in plain components, and systems iterate over the data. Splitting gameplay this way kept it composable and made building or clearing a room — for every run — a cheap, uniform operation.',
          },
          {
            title: 'Cache-friendly data layout',
            body: 'Components are stored in packed, contiguous arrays so the hot per-frame loops stream through memory instead of chasing pointers — the groundwork that makes the parallel pass actually pay off.',
          },
          {
            title: 'Parallel system scheduler',
            body: 'Independent systems run across worker threads so per-frame work scales past a single core, rather than ticking everything serially on the main thread.',
          },
          {
            title: 'Contention-free partitioning',
            body: 'Work is partitioned so threads operate on disjoint data, with synchronisation kept to the explicit hand-off points between systems — avoiding shared-state races while keeping the threads busy.',
          },
        ],
      },
    ],
  },
  {
    type: 'section',
    eyebrow: 'Enemy AI',
    title: 'A state machine per enemy, nine of them',
    blocks: [
      {
        type: 'text',
        paragraphs: [
          'I wrote the enemy AI as a finite-state machine framework: every state — Searching, Chasing, Attacking, Missing, Hurting, Dying — is its own class, and the enemy-state system ticks whichever state is active each frame. Switching state just swaps the live state’s function pointers, guarded by the thread-pool mutex so it stays safe under the parallel scheduler.',
          'Transitions are driven by three signals: a sight raycast (a view cone plus a sight distance), weapon-range hits, and collisions. Seven enemy types and two bosses share that backbone but specialise it into very different behaviour.',
        ],
      },
      {
        type: 'custom',
        render: () => <EnemyRoster />,
      },
    ],
  },
  {
    type: 'section',
    eyebrow: 'Role',
    title: 'Engine work, and steering the team',
    blocks: [
      {
        type: 'minis',
        items: [
          {
            title: 'My scope',
            body: 'On the code side I owned exactly three things end to end — the ECS architecture, the parallel system scheduler and the enemy AI. The rest of the engine was built by teammates.',
          },
          {
            title: 'Product manager',
            body: 'I also led team Noflex as PM — owning concept, scope and schedule, and carrying the build through the course milestones (concept proof, pre-production, pre-alpha, alpha, beta, final).',
          },
        ],
      },
    ],
  },
  {
    type: 'section',
    eyebrow: 'Stack',
    title: 'What I worked in',
    blocks: [
      {
        type: 'stack',
        rows: [
          { layer: 'Engine', items: ['C++', 'Custom ECS', 'Multithreading'] },
          { layer: 'Gameplay', items: ['Enemy AI', 'State machines'] },
        ],
      },
    ],
  },
  {
    type: 'links',
    links: [
      {
        label: 'Source on GitHub',
        href: 'https://github.com/saneaven/Portfolio/tree/main/projects/game/Q%20by%20Noflex',
        icon: githubIcon,
        hint: 'Code & technical docs',
      },
    ],
  },
  {
    type: 'closing',
    text: 'The ECS, its parallel scheduler and the enemy AI behind a roguelike shipped with team Noflex.',
  },
]

export default qByNoflex
