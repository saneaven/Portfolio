import type { ContentBlocks } from '../contentblock/types'
import githubIcon from '../../../../assets/github-white-icon.svg'
import poster from '../../../../assets/section/game/the-forest-of-food/poster.avif'
import menu from '../../../../assets/section/game/the-forest-of-food/menu.avif'
import howToPlay from '../../../../assets/section/game/the-forest-of-food/how-to-play.avif'

const theForestOfFood: ContentBlocks = [
  {
    type: 'lead',
    eyebrow: 'Game · Engine & gameplay programming',
    title: 'The Forest of Food',
    subtitle:
      'The Forest of Food is a top-view adventure where you pick a season, roam the forest gathering ingredients — hunting animals, harvesting crops and mushrooms — then cook the season\'s dish to satisfy the forest fairy and earn your way out. Team Gathering built it on DigiPen\'s in-house engine; as Technical Director I owned the object manager, the math types, the Bezier interpolation, the sound manager, the collision physics and the animal AI.',
    meta: [
      { label: 'Role', value: 'Technical Director — engine layer, physics & animal AI' },
      { label: 'Team', value: 'Gathering — Sieun Choi (Producer), Minjae Kyung (Technical Director), Hyeonseo Yoon (Designer)' },
      { label: 'Context', value: 'DigiPen · 2020' },
      { label: 'Stack', value: 'C++ · DigiPen\'s in-house engine (doodle) · team-built 2D layer · DigiPen audio library' },
    ],
  },
  {
    type: 'section',
    eyebrow: 'Overview',
    title: 'Pick a season, gather the forest, cook your way out',
    blocks: [
      {
        type: 'split',
        paragraphs: [
          'You drop into the forest having chosen a season, and the forest answers in kind — spring brings cows, summer chickens, autumn turkeys, winter pigs, each with its own crops and mushrooms to forage. Attack the animals for meat, pick what grows, and carry it all back to the pot.',
          'The fairy of the forest sets the bar: cook the dish she\'s asking for and she lets you leave; fall short and you stay. That single loop — explore, gather, cook, be judged — sits on top of an engine the team wrote ourselves over DigiPen\'s in-house graphics and input layer.',
        ],
        media: { caption: 'Title art — Team Gathering', image: poster },
      },
    ],
  },
  {
    type: 'section',
    eyebrow: 'Core — Engine layer',
    title: 'The object manager and math the whole game runs on',
    blocks: [
      {
        type: 'cardGrid',
        cards: [
          {
            title: 'Object manager',
            body: 'GameObjectManager owns the live world — the player, items, animals and trees — and drives the frame with UpdateAll and DrawAll. Every gameplay actor derives from a shared Object base that carries position, velocity, a sprite, a hitbox and its current state, so the manager can tick and draw them all uniformly against the camera matrix.',
          },
          {
            title: 'Basic data types',
            body: 'I wrote the math the rest of the code leans on: a templated Vector2D with the usual arithmetic and a Normalize used by the AI, a Rectangle that doubles as the hitbox (isPointInside, isOverlapped, directional vertex queries), and a Line — the small kit every system builds geometry from.',
          },
          {
            title: 'Bezier interpolation',
            body: 'An InterpolationManager animates values smoothly over time without per-frame easing code at the call site. AddSpaceLerp moves a Vector2D along a cubic Bezier path; AddTimeLerp eases a scalar with tunable start/end Bezier strength. It owns the running interpolations and reaps the ones that finish.',
          },
          {
            title: 'Sound manager',
            body: 'A sound layer over DigiPen\'s audio library wraps one-shot SFX and looping BGM behind a small interface. Objects register effects by name (AddSFX) and fire them by name (PlaySFX) — so an animal can cue its own cry on a timer, and a scene can swap its background track on a state change.',
          },
        ],
      },
    ],
  },
  {
    type: 'section',
    eyebrow: 'Animal AI',
    title: 'A three-state machine per animal',
    blocks: [
      {
        type: 'text',
        paragraphs: [
          'Each animal is a Creature running a finite-state machine over the engine\'s State base — Enter, Update and TestForExit per state. State_Moving patrols: it walks, flips direction on a randomised turn timer with a short pause between turns, and periodically plays its species cry. Take a hit and it switches to State_Runaway, fleeing along the normalised vector from the player at roughly twice its walk speed. When its HP reaches zero it enters State_Dying and drops the right meat item — beef, chicken, turkey or pork — for you to collect.',
          'Movement is bounded by an Animal_Lock that clamps each creature inside the map rectangle, and collisions with trees are resolved directionally: the physics figures out which side was hit, pushes the animal back out with a small correction and reflects its velocity, so animals bounce off the scenery instead of clipping through it.',
        ],
      },
    ],
  },
  {
    type: 'section',
    eyebrow: 'Physics & collision',
    title: 'Rectangle hitboxes, resolved by direction',
    blocks: [
      {
        type: 'minis',
        items: [
          {
            title: 'Overlap test',
            body: 'Collision detection runs on the Rectangle hitboxes: a CollisionCheck reports which edge two objects met on, and isOverlapped / isPointInside back the world collisions and every mouse-driven UI hit.',
          },
          {
            title: 'Directional resolution',
            body: 'On contact the resolver reads the direction — up, down, left or right — snaps the moving body just outside the other\'s hitbox with a fixed correction offset, and flips the matching velocity component so it rebounds cleanly.',
          },
          {
            title: 'The knife hit',
            body: 'A swing only lands while the animal is past its invincibility window: a hit decrements HP, retriggers the brief invincibility timer, and routes the creature into Runaway — or into Dying when HP hits zero.',
          },
          {
            title: 'Bounded world',
            body: 'Actors stay inside the playfield by clamping their position to the map\'s rectangle each frame, so nothing wanders off the harvestable area of the forest.',
          },
        ],
      },
    ],
  },
  {
    type: 'section',
    eyebrow: 'Cooking & controls',
    title: 'Gather, cook, and three ways it can end',
    blocks: [
      {
        type: 'rows',
        rows: [
          {
            eyebrow: 'Loop',
            title: 'Season picker → cooking pot',
            body: 'After the hunt, the cooking stage lays your gathered ingredients into a grid you click to select. Match the season\'s recipe exactly and the fairy is satisfied — the good ending; miss it and you get the bad ending; sneak in the secret ingredient and the hidden ending unlocks. Each season checks its own recipe, so spring\'s dish is judged differently from winter\'s.',
            media: { caption: 'Season select — the forest menu', image: menu },
          },
          {
            eyebrow: 'Controls',
            title: 'How to play',
            body: 'Arrow keys move, Space attacks the animals, Shift runs, Z opens the inventory and Ctrl eats. Top-view exploration with a kept-simple control set so the focus stays on hunting and gathering before the cook.',
            media: { caption: 'How to play — controls', image: howToPlay },
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
          { layer: 'Engine', items: ['C++', 'DigiPen\'s in-house engine (doodle)', 'Team-built 2D layer'] },
          { layer: 'My systems', items: ['Object manager', 'Vector / Rectangle / Line', 'Bezier interpolation', 'Sound manager'] },
          { layer: 'Gameplay', items: ['Collision physics', 'Animal AI state machine', 'Object interfaces'] },
          { layer: 'Audio', items: ['DigiPen audio library', 'Programmer-made SFX'] },
          { layer: 'Tools', items: ['Visual Studio', 'Aseprite', 'Photoshop', 'MediBang Paint Pro', 'Music Maker 2020'] },
        ],
      },
    ],
  },
  {
    type: 'links',
    links: [
      {
        label: 'Source on GitHub',
        href: 'https://github.com/saneaven/Portfolio/tree/main/projects/game/The%20Forest%20of%20Food',
        icon: githubIcon,
        hint: 'Code & technical docs',
      },
    ],
  },
  {
    type: 'closing',
    text: 'The engine layer — object manager, math types, Bezier interpolation and sound — plus the collision physics and animal AI behind a seasonal cooking adventure shipped with Team Gathering.',
  },
]

export default theForestOfFood
