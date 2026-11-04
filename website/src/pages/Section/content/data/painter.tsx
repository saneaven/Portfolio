import type { ContentBlocks } from '../contentblock/types'
import githubIcon from '../../../../assets/github-white-icon.svg'
import concept from '../../../../assets/section/game/painter/concept.avif'
import mechanic from '../../../../assets/section/game/painter/mechanic.avif'

const painter: ContentBlocks = [
  {
    type: 'lead',
    eyebrow: 'Game · Gameplay programming',
    title: 'Painter',
    subtitle:
      'Painter is a 2D platformer about a boy who suddenly can\'t see color, going on an adventure to get back the color he lost. You pick up color from the map and throw color balls to paint blocks and enemies — and a single rule ties it together: anything the same color as you is passable, so you walk straight through it. I wrote the gameplay programming: a custom force-based physics layer, the player state machine, the color-gated collision and a follow camera.',
    meta: [
      { label: 'Role', value: 'Primary author — gameplay programmer & designer (physics, player, collision, camera)' },
      { label: 'Focus', value: 'Color-gated movement · custom 2D physics · state machine' },
      { label: 'Stack', value: 'C++ · doodle (DigiPen\'s in-house engine) · Visual Studio 2019 (Release)' },
    ],
  },
  {
    type: 'section',
    eyebrow: 'Overview',
    title: 'A boy who lost color, painting his way back',
    blocks: [
      {
        type: 'split',
        paragraphs: [
          'The premise is small and literal: a boy can no longer see color, so he goes to get it back. That fiction becomes the whole verb set. You pick up color items scattered across the map, which recolor you; you throw color balls that paint the blocks and enemies they hit; and reaching the end of the map clears the stage.',
          'The twist that makes it a puzzle rather than a runner is one rule applied everywhere: if your color matches a block\'s color, you pass straight through it, and if it matches an enemy\'s, you can defeat that enemy by stepping on it. So every wall is also a door, and every door is the wrong color until you make it the right one.',
        ],
        media: { caption: 'Concept — the boy with his color wheel, facing the shadow', image: concept },
      },
    ],
  },
  {
    type: 'section',
    eyebrow: 'Core — the color rule',
    title: 'Same color is passable',
    blocks: [
      {
        type: 'split',
        paragraphs: [
          'Collision is gated on color. Every object carries a 32-bit color, and the player\'s solid-collision pass only runs against a block when the two colors differ — `if (getColor() != block.getColor())`. Match the block and the test is skipped entirely, so you fall or walk through it; mismatch and it\'s a wall. The same gate applies between enemies and blocks, so a painted enemy can drop through floors a differently-coloured one can\'t.',
          'Throwing is how you change the world\'s color. Pressing Z cycles the held ball through red, green and blue; space fires a ball from the muzzle in your facing direction, inheriting your horizontal velocity, and drains one from a paint reserve of twenty. Where it lands, it repaints the block or enemy — and now that surface either blocks you or lets you through, depending on what you\'re wearing.',
        ],
        media: { caption: 'Mechanic — same-colored blocks pass through; jump and wall-jump', image: mechanic },
      },
    ],
  },
  {
    type: 'section',
    eyebrow: 'Player & physics',
    title: 'A state machine driving a custom force-based body',
    blocks: [
      {
        type: 'cardGrid',
        cards: [
          {
            title: 'Force-based movement',
            body: 'Every entity is an Object with position, velocity, acceleration and an accumulated force. Each frame the player gathers a step force from the held arrow keys, a jump impulse and gravity, then resolves force → acceleration → velocity → position. Input sets intent; the physics decides the motion.',
          },
          {
            title: 'Custom friction',
            body: 'Friction is hand-rolled rather than a flat damping constant: the opposing force scales with the square root of how close you are to max speed, so the body accelerates briskly off the line and eases as it approaches the cap — giving acceleration and stop a hand-tuned feel instead of an on/off slide.',
          },
          {
            title: 'Swept edge collision',
            body: 'Collision casts the body\'s leading corner along its relative velocity and intersects that segment against the four edges of the other box, classifying the hit as up, down, left, right or a corner case. On a real hit it snaps the body to the contact point and zeroes only the blocked velocity axis — no tunnelling, no sticking.',
          },
          {
            title: 'Stomp & death',
            body: 'Resolved hit direction decides the outcome against an enemy: land on top of a same-colour enemy and you stomp it (and rebound with a jump); touch a wrong-colour enemy, or get hit from any other side, and the player dies. Fall below the world and it\'s a restart — R reloads the stage from scratch.',
          },
        ],
      },
    ],
  },
  {
    type: 'section',
    eyebrow: 'World & camera',
    title: 'Items, four-color enemies and a camera that follows',
    blocks: [
      {
        type: 'minis',
        items: [
          {
            title: 'Color items',
            body: 'Items recolor you on pickup — red, green, blue, white or black — using an axis-aligned overlap test. One special item type is the goal: touch it and the stage is marked clear instead of changing your color.',
          },
          {
            title: 'Four-color enemies',
            body: 'Enemies come in four colors — red, green, blue and black — sharing one body that patrols a platform, flips direction at edges via a ledge-probe ahead of each foot, and turns on wall contact. Color decides who is dangerous and who is stompable for the color you\'re currently wearing.',
          },
          {
            title: 'Follow camera',
            body: 'A Camera object holds a pointer to the player and snaps to its position every frame at 1.3× zoom, with every draw call offset and scaled by the camera so the boy stays centred as the level scrolls beneath him.',
          },
          {
            title: 'Vec2 math layer',
            body: 'All of it rides on a hand-written Vec2 — operators, dot product, length and angle helpers — used for forces, velocities, muzzle offsets and the collision segment maths, so the gameplay code reads in vectors rather than loose floats.',
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
          { layer: 'Language', items: ['C++'] },
          { layer: 'Engine', items: ['doodle (DigiPen\'s in-house engine)'] },
          { layer: 'Gameplay', items: ['Force-based physics', 'Custom friction', 'Swept collision', 'Player state machine'] },
          { layer: 'Systems', items: ['Color-gated movement', 'Follow camera (1.3×)', 'Vec2 math'] },
          { layer: 'Build', items: ['Visual Studio 2019', 'Release'] },
        ],
      },
    ],
  },
  {
    type: 'links',
    links: [
      {
        label: 'Source on GitHub',
        href: 'https://github.com/saneaven/Portfolio/tree/main/projects/game/Project416_prototype',
        icon: githubIcon,
        hint: 'Code & technical docs',
      },
    ],
  },
  {
    type: 'closing',
    text: 'A puzzle platformer where one idea — same color is passable — reaches from the collision test up to the level design, built as the primary author on the gameplay programming with Haeun Park on maps.',
  },
]

export default painter
