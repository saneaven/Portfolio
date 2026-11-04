import type { ContentBlocks } from '../contentblock/types'
import githubIcon from '../../../../assets/github-white-icon.svg'

const sonarSoul: ContentBlocks = [
  {
    type: 'lead',
    eyebrow: 'Game · Gameplay & audio programming',
    title: 'Sonar Soul',
    subtitle:
      'Sonar Soul is a side-view action game where every enemy holds a soul tied to one musical note. You fire expanding sonar waves that each carry a note from Do to Do_high — a wave only disables the enemy whose note matches it — and locked gates open only when you play the right sequence. I owned the gameplay programming: the class set, the circle-vs-rectangle physics, the gate mechanic and the audio.',
    meta: [
      { label: 'Role', value: 'Programming — classes, physics & game mechanics (team 3D)' },
      { label: 'Team', value: 'With Haneul Lee and Hoseob Jeong' },
      { label: 'Context', value: 'DigiPen · 2019' },
      { label: 'Stack', value: 'C++ · doodle (DigiPen\'s in-house framework) · SFML/Audio' },
    ],
  },
  {
    type: 'section',
    eyebrow: 'Overview',
    title: 'Enemies took the notes\' souls — and you have to get them back',
    blocks: [
      {
        type: 'split',
        paragraphs: [
          'The premise is simple to state and the whole game grows out of it: an enemy has taken a note\'s soul, and the player must reach those souls and release them. Each enemy is bound to exactly one of the seven solfège notes — Do, Re, Mi, Fa, Sol, La, Ti — plus a high Do, so the world reads as a scale you have to play back at it.',
          'You don\'t shoot to kill so much as sound the right pitch at the right thing. A sonar wave goes out, expands across the level, and only the enemy whose note matches the wave\'s note answers it — everything else is left untouched.',
        ],
        media: { caption: 'Expanding sonar carries one note; only the matching enemy answers' },
      },
    ],
  },
  {
    type: 'section',
    eyebrow: 'Core — Sonar & matching',
    title: 'A wave that carries a note, and only acts on its match',
    blocks: [
      {
        type: 'cardGrid',
        cards: [
          {
            title: 'Note-tagged sonar',
            body: 'Each Sonar is an expanding circle tagged with a Syllable (Do … Do_high). Every frame its radius grows by a sonar speed scaled by delta time while its alpha fades out, so the wave visibly ripples outward and dies once it has gone fully transparent.',
          },
          {
            title: 'Match disables the enemy',
            body: 'On overlap the wave compares its own syllable against the enemy\'s note. Only an exact match takes effect — the enemy is recoloured to show its soul has been released — so spamming sonar does nothing unless the pitch is right.',
          },
          {
            title: 'Circle-vs-rectangle collision',
            body: 'I wrote the circle-against-rectangle test the sonar needs: when the circle\'s centre is inside the box\'s extended bands it is treated as a direct hit, otherwise each of the four corners is distance-checked against the radius — catching the diagonal cases a naive box test would miss.',
          },
          {
            title: 'Enemy notes from a switch',
            body: 'An enemy\'s type maps straight to its note (type 0 → Do, type 7 → Do_high), and getNote() exposes it to the sonar — so authoring a level is just placing enemies of the right type to lay out the puzzle.',
          },
        ],
      },
    ],
  },
  {
    type: 'section',
    eyebrow: 'Gates',
    title: 'Locks that only a melody opens',
    blocks: [
      {
        type: 'split',
        paragraphs: [
          'Beyond single enemies, gates turn the same note system into a sequence puzzle. A gate is given a key — an ordered list of syllables, for example Do, Mi, La — and stays shut until the player plays exactly that run of notes against it.',
          'The player builds the sequence with a dedicated GateSonar emitter: holding the modifier opens the note bank on the HUD, the player steps a note index up or down and fires the chosen syllable at the gate. Hit the full key in order and the gate yields, opening the path and, at the level\'s end, letting the gate play its song as the game resolves.',
        ],
        media: { caption: 'A gate keyed to Do · Mi · La, opened by playing the sequence' },
      },
    ],
  },
  {
    type: 'section',
    eyebrow: 'My scope',
    title: 'The classes, the physics, and the feel',
    blocks: [
      {
        type: 'minis',
        items: [
          {
            title: 'The class set',
            body: 'I authored the gameplay class hierarchy off a shared Object base — Block, Enemy, Player, Bullet, Sonar, Gate and GateSonar — plus the HUD, Timer and Animation helpers that drive the on-screen note bank and screen transitions.',
          },
          {
            title: 'Physics',
            body: 'The movement and collision layer is mine: gravity, friction and force-accumulation on Object, the block collision that walks enemies along ledges, and the bespoke circle-vs-rectangle test that makes the sonar feel like it actually sweeps the room.',
          },
          {
            title: 'Game mechanics',
            body: 'I wired the note interface to play — Space opens the note bank, then keys map to syllables for movement, shooting and sonar — along with the gate key-matching and the screen flow from main menu through play, reload and ending.',
          },
          {
            title: 'Audio',
            body: 'Each enemy plays its note through SFML\'s audio, indexing a sound buffer by syllable, while looping background music streams from an .ogg via sf::Music. Sound effects were drawn from the licensed Sonniss Game Audio GDC bundle.',
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
          { layer: 'Framework', items: ['doodle (DigiPen\'s in-house framework)'] },
          { layer: 'Gameplay', items: ['Class hierarchy', 'Custom physics', 'Circle-vs-rectangle collision', 'Note matching'] },
          { layer: 'Audio', items: ['SFML/Audio', 'sf::Music', '.ogg streaming', 'Sonniss GDC bundle'] },
        ],
      },
    ],
  },
  {
    type: 'links',
    links: [
      {
        label: 'Source on GitHub',
        href: 'https://github.com/saneaven/Portfolio/tree/main/projects/game/Project3Dumb_prototype',
        icon: githubIcon,
        hint: 'Code & technical docs',
      },
    ],
  },
  {
    type: 'closing',
    text: 'The class set, the sonar physics, the gate puzzles and the audio behind a note-matching action game made with team 3D.',
  },
]

export default sonarSoul
