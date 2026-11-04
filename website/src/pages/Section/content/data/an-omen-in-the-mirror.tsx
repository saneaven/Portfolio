import type { ContentBlocks } from '../contentblock/types'
import githubIcon from '../../../../assets/github-white-icon.svg'
import menu from '../../../../assets/section/game/an-omen-in-the-mirror/hero.avif'
import dialogue from '../../../../assets/section/game/an-omen-in-the-mirror/dialogue.avif'
import selection from '../../../../assets/section/game/an-omen-in-the-mirror/selection.avif'
import inspection from '../../../../assets/section/game/an-omen-in-the-mirror/inspection.avif'
import hud from '../../../../assets/section/game/an-omen-in-the-mirror/hud.avif'
import ending from '../../../../assets/section/game/an-omen-in-the-mirror/ending.avif'

const anOmenInTheMirror: ContentBlocks = [
  {
    type: 'lead',
    eyebrow: 'Game · UI/UX design & implementation',
    title: 'Designing and building the interface of a narrative puzzle game.',
    subtitle:
      'An Omen In The Mirror is a first-person narrative puzzle game made in Unreal Engine at DigiPen. I owned its interface end to end — designing the dialogue, inspection, menu and HUD systems, then implementing them in-engine with UMG.',
    video: { id: 'IM009m3V1Qk', title: 'An Omen In The Mirror — Trailer' },
    meta: [
      { label: 'Role', value: 'UI/UX design & in-game implementation' },
      { label: 'Engine', value: 'Unreal Engine · UMG' },
      { label: 'Context', value: 'DigiPen team project' },
    ],
  },
  {
    type: 'section',
    eyebrow: 'Overview',
    title: 'A story told through what you read and inspect',
    blocks: [
      {
        type: 'split',
        paragraphs: [
          'The game is a first-person, narrative-driven puzzle built in Unreal Engine, where progress comes from reading dialogue, inspecting objects and piecing the story together. That makes the interface load-bearing: the UI is how the player understands the world, not just an overlay on top of it.',
          'I designed each surface — menus, dialogue, interaction and HUD — to stay quiet and in-fiction, then built them in the engine so the look survived contact with the real game.',
        ],
        media: { caption: 'Main menu', image: menu },
      },
    ],
  },
  {
    type: 'section',
    eyebrow: 'UI systems',
    title: 'Every surface the player touches',
    blocks: [
      {
        type: 'rows',
        rows: [
          {
            eyebrow: 'Dialogue',
            title: 'Dialogue & choice',
            body: 'The core narrative surface: a dialogue panel with selectable responses and clear hover / clicked states, paced to keep reading comfortable and the choice obvious.',
            media: { caption: 'In-game dialogue', image: dialogue },
          },
          {
            eyebrow: 'Selection',
            title: 'Choosing a response',
            body: 'Selection and hover states give immediate feedback so the player always knows what they are about to pick, without breaking the scene.',
            media: { caption: 'Response selection', image: selection },
          },
          {
            eyebrow: 'Interaction',
            title: 'Inspection & crosshair',
            body: 'A context-aware crosshair signals when something is interactable, and a dedicated inspection view lets the player examine objects up close to solve puzzles.',
            media: { caption: 'Inspection / interaction', image: inspection },
          },
          {
            eyebrow: 'HUD',
            title: 'A quiet heads-up display',
            body: 'The in-game HUD surfaces only what the player needs in the moment, staying out of the way of a first-person, atmosphere-led game.',
            media: { caption: 'In-game HUD', image: hud },
          },
          {
            eyebrow: 'Ending',
            title: 'Closing the story',
            body: 'The ending screen lands the narrative beat and returns the player out, bookending the menu they came in through.',
            media: { caption: 'Ending screen', image: ending },
          },
        ],
      },
    ],
  },
  {
    type: 'section',
    eyebrow: 'Process',
    title: 'From concept mockups to in-engine screens',
    blocks: [
      {
        type: 'minis',
        items: [
          {
            title: 'Concept first',
            body: 'Each surface started as a high-fidelity concept — main menu, dialogue, interaction — establishing type, colour and layout before any engine work.',
          },
          {
            title: 'Built in UMG',
            body: 'I rebuilt the approved concepts as UMG widgets in Unreal, wiring them to game state so they behaved correctly in a running build, not just in a mockup.',
          },
          {
            title: 'Iterated in place',
            body: 'Seeing the UI inside the actual game surfaced spacing, legibility and pacing issues that only show up in motion, and those fed back into the design.',
          },
        ],
      },
    ],
  },
  {
    type: 'section',
    eyebrow: 'Stack',
    title: 'What it’s built with',
    blocks: [
      {
        type: 'stack',
        rows: [
          { layer: 'Engine', items: ['Unreal Engine', 'UMG'] },
          { layer: 'Design', items: ['UI concepts', 'Custom type & layout'] },
        ],
      },
    ],
  },
  {
    type: 'links',
    links: [
      {
        label: 'Source on GitHub',
        href: 'https://github.com/saneaven/Portfolio/tree/main/projects/game/An%20Omen%20In%20The%20Mirror',
        icon: githubIcon,
        hint: 'Code & technical docs',
      },
    ],
  },
  {
    type: 'closing',
    text: 'A complete game interface — designed, then built in-engine and proven in the running game.',
  },
]

export default anOmenInTheMirror
