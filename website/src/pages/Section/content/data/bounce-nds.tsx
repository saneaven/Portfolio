import type { ContentBlocks } from '../contentblock/types'
import githubIcon from '../../../../assets/github-white-icon.svg'
import level1 from '../../../../assets/section/game/bounce-nds/level-1.avif'
import level5 from '../../../../assets/section/game/bounce-nds/level-5.avif'
import level10 from '../../../../assets/section/game/bounce-nds/level-10.avif'

const bounceNds: ContentBlocks = [
  {
    type: 'lead',
    eyebrow: 'Game · Physics & gameplay programming',
    title: 'Bounce NDS',
    subtitle:
      'BOUND is a physics platformer where you have no jump button: you move a ball left and right and wall-jump off surfaces, Mario-style, to reach the exit on the right of each stage. I wrote its physics engine — the vector math, the Ball, the gravity and acceleration integration, the AABB collision and the wall-jump — over ten hand-built stages that introduce bouncy pads, launchers, breakable stone and lethal bombs.',
    meta: [
      { label: 'Role', value: 'Physics engine, vector math & Ball class, input/state, wall-jump' },
      { label: 'Team', value: 'NDS · with Junhyuk Cha (map system)' },
      { label: 'Context', value: 'DigiPen · 2019' },
      { label: 'Stack', value: 'JavaScript · p5.js v0.9.0 · p5.dom / p5.sound' },
    ],
  },
  {
    type: 'section',
    eyebrow: 'Overview',
    title: 'No jump button — just a ball and the walls',
    blocks: [
      {
        type: 'split',
        paragraphs: [
          'The whole game is one verb: move the ball. Left and right arrows accelerate it, the walls bounce it, and the only way up is to wall-jump off a surface the way you would in a 2D Mario. Reach the right edge of the screen and the stage clears; fall off the bottom and you lose a life; R restarts the current stage.',
          'Because there is no dedicated jump, every stage is really a physics puzzle: you read the geometry, build horizontal speed, and convert it into height by slamming into the right wall at the right moment. That made the feel of the simulation — gravity, top speeds, the bounce response — the actual gameplay, which is the part I owned.',
        ],
        media: { caption: 'Stage 1 — the opening room', image: level1 },
      },
    ],
  },
  {
    type: 'section',
    eyebrow: 'Core — Physics engine',
    title: 'A small physics simulation, hand-written',
    blocks: [
      {
        type: 'cardGrid',
        cards: [
          {
            title: 'Vector math from scratch',
            body: 'Everything sits on a custom _vec2 class — add / subtract / multiply / divide, length and angle, dot product, and in-place variants — so position, velocity, acceleration and gravity are all plain 2D vectors the rest of the engine composes.',
          },
          {
            title: 'Integration & clamping',
            body: 'The Ball integrates each frame: gravity adds a constant downward vector, input adds horizontal acceleration, then checkMaxSpeed clamps both axes to per-axis caps so the ball never outruns the collision test and tunnels through a block.',
          },
          {
            title: 'AABB broad phase',
            body: 'Collision first tests the ball against each block as an axis-aligned box, then decides whether it struck a vertical face, a horizontal face, or only a corner — routing each case to a different response so attached blocks do not double-resolve.',
          },
          {
            title: 'Corner case, literally',
            body: 'When the ball touches only a block corner, IsBallHitsEdge measures the distance from the ball centre to each of the four corners against its radius, and treats a corner hit as a top/bottom hit so the response stays stable on tight diagonal gaps.',
          },
        ],
      },
    ],
  },
  {
    type: 'section',
    eyebrow: 'Mechanic — Wall-jump',
    title: 'Turning a wall hit into a jump',
    blocks: [
      {
        type: 'split',
        paragraphs: [
          'The wall-jump is the signature move and the trickiest bit of the physics. On a side collision the ball is first pushed just outside the block to avoid resolving the same hit twice; then, if you are holding the arrow into the wall, wallJump fires: it reverses horizontal velocity and overshoots it to 1.5× the cap, kicks the ball upward, and steps the simulation forward until it is clear of geometry before settling into a clean launch velocity.',
          'If you are not pressing into the wall, the engine instead arms a 100 ms window in which a press still counts as a wall-jump and simply reflects the horizontal velocity — the difference between a deliberate kick off the wall and an accidental bounce. The launcher and bouncy blocks reuse the same velocity plumbing: arrow blocks disable gravity and fling the ball horizontally at double speed, bouncy pads raise the vertical cap so the next bounce sends it higher.',
        ],
        media: { caption: 'Stage 5 — bouncy pads and a launcher', image: level5 },
      },
    ],
  },
  {
    type: 'section',
    eyebrow: 'Content — Blocks & stages',
    title: 'Six block types, ten stages',
    blocks: [
      {
        type: 'minis',
        items: [
          {
            title: 'Normal & stone',
            body: 'A normal block bounces the ball back at full vertical speed; a stone block bounces once and then breaks, splicing itself out of the level and triggering a redraw of the ball\'s trail so the map stays clean.',
          },
          {
            title: 'Bouncy & launchers',
            body: 'A bouncy block boosts the vertical speed cap for an extra-high rebound. Two launcher blocks — left-arrow and right-arrow — cut gravity and fling the ball horizontally at double speed until it lands again.',
          },
          {
            title: 'Bomb',
            body: 'The bomb block is instant death: any contact flips the game into an explosion state, costs a life, and restarts the stage — the hazard the late levels are built around.',
          },
          {
            title: 'Levels as grids',
            body: 'Each of the ten stages is authored as a 20×15 integer grid, where the value selects the block type and a sentinel marks the ball\'s spawn point. My teammate Junhyuk Cha built that map system and laid out the stages on top of the physics.',
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
          { layer: 'Engine', items: ['JavaScript', 'Custom _vec2 math', 'Ball physics', 'AABB collision'] },
          { layer: 'Gameplay', items: ['Wall-jump', 'Block interactions', 'State machine loop'] },
          { layer: 'Runtime', items: ['p5.js v0.9.0', 'p5.dom.js', 'p5.sound.js', 'Browser canvas'] },
          { layer: 'Delivery', items: ['Runs in the browser', 'Deployed live on Bitbucket Pages'] },
        ],
      },
      {
        type: 'rows',
        rows: [
          {
            eyebrow: 'Early',
            title: 'Stage 1',
            body: 'A single open room that teaches the one rule: build speed and wall-jump your way to the exit on the right.',
            media: { caption: 'Stage 1', image: level1 },
          },
          {
            eyebrow: 'Mid',
            title: 'Stage 5',
            body: 'Bouncy pads and a launcher block start stacking the verbs, asking you to chain a boosted bounce into a horizontal fling.',
            media: { caption: 'Stage 5', image: level5 },
          },
          {
            eyebrow: 'Final',
            title: 'Stage 10',
            body: 'The last stage threads bomb blocks around the route to the exit — one wrong bounce ends the run.',
            media: { caption: 'Stage 10 — bomb blocks & the exit', image: level10 },
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
        href: 'https://github.com/saneaven/Portfolio/tree/main/projects/game/Bounce_NDS',
        icon: githubIcon,
        hint: 'Code & technical docs',
      },
    ],
  },
  {
    type: 'closing',
    text: 'My first game: a hand-written 2D physics engine — vectors, integration, AABB collision and the wall-jump — shipped in the browser with team NDS.',
  },
]

export default bounceNds
