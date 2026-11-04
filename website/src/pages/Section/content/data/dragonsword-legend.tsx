import type { ContentBlocks } from '../contentblock/types'
import githubIcon from '../../../../assets/github-white-icon.svg'

const dragonswordLegend: ContentBlocks = [
  {
    type: 'lead',
    eyebrow: 'Game · Solo systems programming',
    title: 'DragonSword Legend',
    subtitle:
      'DragonSword Legend is a Korean-language Windows-console RPG: stat-based combat, five wearable equipment slots fed by around 185 hand-authored items, around eight regional monster zones each capped by a scaling boss, a quest chain, an inventory, and leveling with skill points to spend. It\'s the earliest thing here — an ambitious, self-taught solo build whose depth lives in its data tables and its text UI rather than its polish.',
    meta: [
      { label: 'Role', value: 'Main Programmer — implemented most of the game solo' },
      { label: 'Context', value: 'Self-taught personal project · 2015' },
      { label: 'Stack', value: 'C · Windows console (windows.h) · Visual Studio' },
    ],
  },
  {
    type: 'section',
    eyebrow: 'Overview',
    title: 'Where the journey started',
    blocks: [
      {
        type: 'split',
        paragraphs: [
          'Before any engine or framework, this was the project that taught me how a game is actually wired together. There are no graphics and no audio library — it is pure C drawing into the Windows console, with colour attributes and cursor positioning standing in for sprites.',
          'What makes it hold up is scope: rather than one clever mechanic, it commits to the full RPG loop — fight monsters, earn experience and gold, level up, spend the gold on gear across five wearable equipment slots, take quests, and push deeper into harder zones — and every piece of that loop is backed by data tables I wrote by hand.',
        ],
        media: { caption: 'The full RPG loop, rendered entirely in the text console' },
      },
    ],
  },
  {
    type: 'section',
    eyebrow: 'Core — Data tables',
    title: 'The whole game is hand-authored data',
    blocks: [
      {
        type: 'cardGrid',
        cards: [
          {
            title: 'Coded item catalogue',
            body: 'Every item carries a numeric code, and an item() lookup decodes the leading digit into a category — weapons (1xxx), head armour (2xxx), body armour (3xxx), shoes (4xxx), jewelry (5xxx), consumables (6xxx) and quest items (7xxx) — then indexes the right table. Around 185 entries, each with its own price, attack, speed, defence, HP bonus and required level. The first five categories are wearable equipment slots; consumables and quest items are handled separately.',
          },
          {
            title: 'Eight regional monster zones',
            body: 'A mon() lookup maps a monster code into one of around eight zones — open field, desert, mountain, undead pirates, sea, outback, the dead lands and the final kingdom — each a tier of progressively tougher enemies. The last entry in every zone is a boss with a huge HP and attack spike over the rank-and-file.',
          },
          {
            title: 'Quest chain',
            body: 'A quest() table holds named missions keyed by progress, each pointing at the monster codes to defeat (or items to gather) and the gold, experience and item reward on completion — a story spine threaded through the zones rather than ad-hoc fetch tasks.',
          },
          {
            title: 'One decode per concern',
            body: 'Items, monsters and quests each resolve through a single typed struct returned by code, so gameplay code asks for an entity by number and gets a fully populated record back. Adding content meant adding a row, not touching logic — the discipline that kept a project this size editable by one person.',
          },
        ],
      },
    ],
  },
  {
    type: 'section',
    eyebrow: 'Combat & progression',
    title: 'A speed gauge decides turns; experience reshapes the hero',
    blocks: [
      {
        type: 'pipeline',
        title: 'Turn battle and leveling',
        body: 'Combat is driven by a speed gauge: each tick adds the combatant\'s SPD, and once a gauge crosses its threshold that side acts — so a faster build simply gets more turns. The player picks Attack, Use item, Dodge (evade) or Flee — healing runs through the Item action, not a separate menu entry. The player\'s attack lands at full value, while incoming enemy damage is mitigated by the player\'s defence. On a kill, expadd() animates the experience bar filling, and each level-up raises attack, HP, defence and speed while granting skill points — the SP gain scaling up by level band — which the player allocates to push a build further.',
        steps: [
          'SPD fills each combatant\'s gauge',
          'Gauge crosses threshold → that side acts',
          'Attack · Item · Dodge · Flee',
          'Defence mitigates incoming damage',
          'Kill → EXP, gold, level-up + skill points',
        ],
      },
    ],
  },
  {
    type: 'section',
    eyebrow: 'Console UI',
    title: 'A text interface made to feel like a real screen',
    blocks: [
      {
        type: 'minis',
        items: [
          {
            title: 'Cursor-addressed drawing',
            body: 'A gotoxy() wrapper over SetConsoleCursorPosition moves the caret anywhere on screen, with companion wherex/wherey helpers reading it back — so menus, the battle layout and the inventory redraw in place instead of scrolling endlessly.',
          },
          {
            title: 'Colour as feedback',
            body: 'A textColor() wrapper over SetConsoleTextAttribute switches foreground colours from windows.h, so action prompts, damage numbers and headings read at a glance in an all-text screen.',
          },
          {
            title: 'Equipment view',
            body: 'The inventory tracks a set[] of equipped items across the five wearable slots plus stacks of consumables and quest items; equipping a piece folds its attack, speed, defence and HP into the live stats, and the screen resizes via mode con so battle, inventory and stat allocation each get a fitting canvas.',
          },
          {
            title: 'Keypress navigation',
            body: 'Most navigation reads keys directly with _getch(), so the player moves through the maps and the inventory with single presses; some menus, like the battle menu, take numeric line input via scanf — the closest a console build gets to feeling responsive.',
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
          { layer: 'Language', items: ['C', 'Plain structs', 'Code-keyed lookups'] },
          { layer: 'Rendering', items: ['Windows console (windows.h)', 'SetConsoleCursorPosition', 'SetConsoleTextAttribute'] },
          { layer: 'Input', items: ['conio.h _getch', 'mode con sizing'] },
          { layer: 'Tooling', items: ['Visual Studio', 'Built in 2015'] },
        ],
      },
    ],
  },
  {
    type: 'links',
    links: [
      {
        label: 'Source on GitHub',
        href: 'https://github.com/saneaven/Portfolio/tree/main/projects/game/DragonSword%20legend%204.0(Alpha)',
        icon: githubIcon,
        hint: 'Code & technical docs',
      },
    ],
  },
  {
    type: 'closing',
    text: 'An early, ambitious solo RPG in pure console C — the hand-built item, monster and quest tables and the text UI behind it are where I first learned to think in systems.',
  },
]

export default dragonswordLegend
