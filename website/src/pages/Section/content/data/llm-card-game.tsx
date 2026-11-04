import type { ContentBlocks } from '../contentblock/types'
import githubIcon from '../../../../assets/github-white-icon.svg'

const llmCardGame: ContentBlocks = [
  {
    type: 'lead',
    eyebrow: 'AI · LLM Systems',
    title: 'LLM Card Game',
    subtitle:
      'A turn-based, player-versus-AI card battler built on Google Gemini. Instead of a hand-coded rules engine, one Gemini agent validates every move against the rules and returns the new game state as structured JSON, while a second Gemini agent reads that state and plays the opponent. Pydantic schemas keep the model\'s output strictly typed, so the LLM can drive the logic without the game falling apart.',
    meta: [
      { label: 'Role', value: 'Solo — rules, card set, prompt design & implementation' },
      { label: 'Focus', value: 'LLM-as-engine · structured output · prompt design' },
      { label: 'Stack', value: 'Python · google-genai (Gemini 2.5 Pro) · Pydantic · dotenv' },
    ],
  },
  {
    type: 'section',
    eyebrow: 'Overview',
    title: 'Hand the rules to the model — carefully',
    blocks: [
      {
        type: 'text',
        paragraphs: [
          'The experiment is to see how far an LLM can run a game whose rules are usually hard-coded. Both players start at 20 health and a growing mana pool, draw from a shuffled deck, and play creatures that attack, trade and trigger abilities. Every turn the player\'s action is sent to a Gemini "game manager" that checks it against the full ruleset and replies with whether it was legal and what the board now looks like.',
          'On the enemy\'s turn a separate Gemini agent receives the current state and emits one action at a time, forming and executing a strategy to beat the player. A state machine on the Python side sequences user and enemy turns, refills mana, and detects when a turn — or the game — is over.',
        ],
      },
    ],
  },
  {
    type: 'section',
    eyebrow: 'Core — Two LLM agents',
    title: 'A referee and an opponent, each schema-bound',
    blocks: [
      {
        type: 'cardGrid',
        cards: [
          {
            title: 'Game-manager agent',
            body: 'Its system prompt embeds the full rules, the entire card database and a validation checklist; it returns a ManagerOutputSchema (is_valid, error_message, is_turn_changed, current_state) so every player action is checked and the authoritative state comes back as data.',
          },
          {
            title: 'Enemy agent',
            body: 'Given the rules, card database and the live game state, it returns an EnemyOutputSchema with a single action command per call — forming a plan to defeat the player one move at a time.',
          },
          {
            title: 'Strictly-typed output',
            body: 'Both agents run with temperature 0.1 and a JSON response MIME type bound to a Pydantic response_schema, so the model is forced to emit valid, parseable structures instead of free-form text the game would have to guess at.',
          },
          {
            title: 'Turn-to-turn memory',
            body: 'A ChatLogManager keeps the conversation history and re-frames it for the player and enemy perspectives, so each agent reasons with the context of how the match has actually unfolded.',
          },
        ],
      },
    ],
  },
  {
    type: 'section',
    eyebrow: 'Card economy',
    title: 'Twenty cards and a handful of mechanics',
    blocks: [
      {
        type: 'minis',
        items: [
          {
            title: 'Mana curve',
            body: 'A 20-card database spans 1–9 mana, with max mana growing each turn — the usual pressure between playing cheap early and holding for a swing.',
          },
          {
            title: 'Keyword abilities',
            body: 'Cards carry familiar mechanics — Taunt, Charge, Battlecry, Deathrattle, Lifesteal and Stealth — each of which the manager agent has to recognise and resolve correctly.',
          },
          {
            title: 'Trading combat',
            body: 'Attacks deal damage both ways: the attacker takes the defender\'s attack in return, anything reduced to zero health is removed, and a player at zero loses.',
          },
          {
            title: 'Themed framing',
            body: 'A light fantasy frame — a Planar Wanderer against an Echo Sentinel boss — gives each side a short lore profile the agents can lean on for flavour and intent.',
          },
        ],
      },
    ],
  },
  {
    type: 'section',
    eyebrow: 'Architecture',
    title: 'Modular managers around the model',
    blocks: [
      {
        type: 'text',
        paragraphs: [
          'The code is organised as a Manager pattern — separate Prompt, Card, Schema, ChatLog and Display managers around a main loop whose GameState enum walks between the user turn, the enemy turn and game over. Keeping prompts, card data and output schemas in their own modules made it straightforward to tune the rules text or adjust a schema without touching the game loop.',
          'It is honest about its edges: running on the API\'s free tier means a small daily request budget — a couple of calls per enemy turn — so the project documents the quota and key handling rather than pretending the model is free to call.',
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
          { layer: 'Language model', items: ['Google Gemini 2.5 Pro', 'google-genai SDK', 'response_schema JSON'] },
          { layer: 'Validation', items: ['Pydantic', 'ManagerOutputSchema', 'EnemyOutputSchema'] },
          { layer: 'Game core', items: ['Python', 'GameState machine', 'Manager pattern'] },
          { layer: 'Infra', items: ['python-dotenv', 'ChatLog history', 'Free-tier quota handling'] },
        ],
      },
    ],
  },
  {
    type: 'links',
    links: [
      {
        label: 'Source on GitHub',
        href: 'https://github.com/saneaven/Portfolio/tree/main/projects/ai/LLM%20Driven%20Card%20Game',
        icon: githubIcon,
        hint: 'Code & technical docs',
      },
    ],
  },
  {
    type: 'closing',
    text: 'Designed and built solo — a turn-based card game whose referee and opponent are both Gemini agents, kept honest by Pydantic-typed JSON, a modular manager architecture, and a 20-card economy of trading combat and keyword abilities.',
  },
]

export default llmCardGame
