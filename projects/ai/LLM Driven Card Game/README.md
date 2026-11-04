# LLM Card Game

*A turn-based, player-versus-AI card battler where Google Gemini is the rules engine and the opponent, kept honest by schema-validated JSON.*

## Overview

Most card games run on a hand-coded rules engine. This project asks a different question: how far can a large language model run the game itself? Instead of imperative game logic, one Gemini agent acts as a **game manager** that validates every player action against the full ruleset and returns the new game state as structured data, while a second Gemini agent reads that state and **plays the opponent**.

Both players start at 20 health with a growing mana pool, draw from a shuffled deck, and play creatures that attack, trade and trigger keyword abilities. Each turn the player types a free-form instruction; the manager agent decides whether it is legal, applies it, and hands back the authoritative board. On the enemy's turn a separate agent receives the live state and emits one action command at a time, forming a plan to defeat the player. A small state machine on the Python side sequences the user and enemy turns and detects when a turn, or the game, is over.

The whole point is to let the model drive the logic without the game falling apart, so the model's output is bound to Pydantic schemas: it is forced to emit valid, parseable structures rather than free-form text the program would have to guess at.

## My role

Solo project. It was built as an individual research effort: designing the card set and rules, writing the two agents' prompts, defining the typed output schemas, and implementing the Python game loop and supporting managers.

## Highlights

- **LLM as the rules engine.** A `game-manager` agent embeds the entire ruleset, the full card database and a validation checklist in its system prompt. It returns a `ManagerOutputSchema` (`is_valid`, `error_message`, `is_turn_changed`, and the complete `current_state`) so every player action is refereed and the authoritative state comes back as data.
- **LLM as the opponent.** A separate `enemy` agent is given the rules, the card database and the current game state, and returns an `EnemyOutputSchema` with a single action command per call, building a strategy one move at a time.
- **Strictly-typed output.** Both agents run at temperature `0.1` with a JSON response MIME type bound to a Pydantic `response_schema`, forcing well-formed, parseable structures.
- **Turn-to-turn memory.** A chat-log manager keeps the conversation history and re-frames it differently for the manager/user perspective and the enemy perspective, so each agent reasons with the context of how the match has actually unfolded (and merges consecutive same-role turns for the enemy view).
- **A 20-card economy.** A card database spanning 1-9 mana, with max mana growing each turn, and familiar keyword mechanics (Taunt, Charge, Battlecry, Deathrattle, Lifesteal and Stealth) that the manager agent has to recognise and resolve.
- **Trading combat.** Attacks deal damage both ways: the attacker takes the defender's attack power in return, anything reduced to zero health leaves the field, and a player at zero loses.
- **Themed framing.** A light fantasy frame, a "Planar Wanderer" against an "Echo Sentinel" boss, gives each side a short lore profile the agents lean on for flavour and intent, including in-character dialogue.

## How it works

The code follows a **Manager pattern**: separate Prompt, Card, Schema, ChatLog and Display managers sit around a main loop whose `GameState` enum walks between the user turn, the enemy turn and game over.

- **`LLMCardGame.py`**: the entry point and game loop. It loads the API key, creates the Gemini client, seeds the chat log with the opening message, and runs the `GameState` machine (`USER_TURN` -> `ENEMY_TURN` -> `GAME_OVER`). After each manager response it inspects `is_turn_changed` and both players' health to decide the next state.
- **`PromptManager.py`**: builds the two system prompts. It assembles the game rules, the rendered card database, the player/enemy lore profiles, and the enemy's schema documentation, then shuffles the user and enemy decks and forms the opening "begin game" system message.
- **`CardManager.py`**: the card database: 20 `Card` models, each with name, ability text, attack, health, mana cost and lore.
- **`SchemaManager.py`**: the Pydantic output contracts. `ManagerOutputSchema` wraps a `CurrentStateSchema` (per-player `PlayerStatusSchema` with hand, deck and field cards), and `EnemyOutputSchema` carries the single action string.
- **`ChatLogManager.py`**: stores the typed message history and converts it into Gemini `Content` objects, mapping roles differently for the user-facing manager call versus the enemy call.
- **`DisplayManager.py`**: renders the board to the terminal, laying out hand and field cards horizontally and clearing the screen between states.

Keeping prompts, card data and schemas in their own modules makes it straightforward to tune the rules text or adjust a schema without touching the game loop. The project is also honest about its edges: it runs on the API's free tier, which means a small daily request budget (roughly two manager calls per enemy turn), so the project documents the quota and API-key handling rather than pretending the model is free to call.

## Tech stack

- **Language model:** Google Gemini 2.5 Pro (the free-tier demo uses the experimental endpoint of the same underlying model)
- **SDK / output:** `google-genai`, JSON `response_schema` with strict MIME type
- **Validation:** Pydantic (`ManagerOutputSchema`, `EnemyOutputSchema` and the nested state schemas)
- **Game core:** Python, a `GameState` enum-driven loop, the Manager pattern
- **Infra:** `python-dotenv` for the API key, an in-memory chat-log history

## Build & run

The game is a terminal Python program that calls the Gemini API, so it needs a valid API key.

1. Install the dependencies:

   ```bash
   pip install google-genai pydantic python-dotenv
   ```

2. Put a Gemini API key in `Source/.env`:

   ```
   API_KEY=your_google_ai_studio_key
   ```

A key can be obtained from <https://aistudio.google.com/>.

3. Run the game:

   ```bash
   python Source/LLMCardGame.py
   ```

On Windows, `Source/PlayGame.bat` launches the same script.

When it is your turn, type any instruction describing what you want to do (type `exit` to end the game). The manager agent validates it, updates the board, and play passes to the Gemini-driven opponent.

> Note: the project targets the free API tier, which has a small daily request > quota (and the enemy turn spends more than one call), so a full match can hit > the daily limit.

## Project structure

```
LLM Driven Card Game/
├─ Source/
│  ├─ LLMCardGame.py          # entry point + GameState loop
│  ├─ PlayGame.bat            # Windows launcher
│  ├─ .env                    # API_KEY (not committed)
│  └─ Manager/
│     ├─ PromptManager.py     # system prompts, rules, lore, deck shuffling
│     ├─ CardManager.py       # 20-card database
│     ├─ SchemaManager.py     # Pydantic output schemas
│     ├─ ChatLogManager.py    # history + Gemini Content conversion
│     └─ DisplayManager.py    # terminal board rendering
├─ Paper/                     # write-up of the experiment
└─ MinjaeKyung_LLMDrivenGame/ # demo video + slides
```

## Links

- [Source on GitHub](https://github.com/saneaven/Portfolio/tree/main/projects/ai/LLM%20Driven%20Card%20Game)
