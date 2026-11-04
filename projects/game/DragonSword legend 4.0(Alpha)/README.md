# DragonSword Legend

_A Korean-language Windows-console RPG written in pure C: the full RPG loop rendered entirely in text._

## Overview

DragonSword Legend is a single-player role-playing game that runs in the Windows console. There are no graphics and no audio library: it is plain C drawing into the text screen, using cursor positioning and console colour attributes from `windows.h` in place of sprites.

What gives it scope is that it commits to the whole RPG loop rather than one isolated mechanic: fight monsters, earn experience and gold, level up, spend gold on gear across five wearable equipment slots, take quests, and push into progressively harder regions. Every piece of that loop is backed by data tables authored by hand: a catalogue of around 185 items, around eight regional monster zones each capped by a scaling boss, and a quest chain threaded through those zones.

This is the earliest project in the portfolio, an ambitious, self-taught solo build (2015) whose depth lives in its data tables and its text UI.

## My role

Solo project. I designed and implemented the game myself: the data tables, the turn battle, leveling and skill-point allocation, the inventory and equipment system, the town and field maps, the shop, the quest system, and the console rendering layer.

## Highlights

- **Code-keyed data tables.** Items, monsters and quests each resolve through a single decode function that returns a fully-populated `struct` for a numeric code, so gameplay code asks for an entity by number and gets a record back. Adding content meant adding a row, not touching logic.
- **Around 185 hand-authored items.** Each item carries a price, attack, speed, defence, HP bonus and required level. The taxonomy spans seven categories (weapons, head armour, body armour, shoes, jewelry, consumables and quest items), of which the first five are wearable equipment slots that fold their stats into the hero; consumables and quest items are handled separately.
- **Speed-gauge turn battle.** Each tick adds a combatant's speed to their gauge; whoever crosses the threshold first acts, so a faster build simply gets more turns. The player chooses Attack, Use item, Dodge (evade) or Flee. Healing is done through the Item action, not a separate menu entry. The player's attack lands on the enemy at full value, while incoming enemy damage is reduced by the player's defence through a mitigation formula (roughly 100 · enemy_attack / (100 + defence)).
- **Leveling with skill points.** On a kill, an animated experience bar fills; each level-up raises attack, HP, defence and speed and grants skill points (the amount scaling up by level band) that the player allocates to shape a build.
- **Regional monster zones with bosses.** Around eight zones (field, desert, mountain, undead pirates, sea, outback, the dead lands and the final kingdom) form a difficulty ramp, with the last entry in each zone a boss carrying a large HP and attack spike over the rank-and-file.
- **A quest chain.** Named missions point at the monster codes to defeat (or items to gather) and the gold, experience and item reward on completion, forming a story spine rather than ad-hoc fetch tasks.
- **Tile-based town and field maps.** The player walks an `@`-style avatar with the arrow keys; collision is resolved against a block grid, portal tiles move between towns and fields, and steps trigger random encounters.
- **Save and load.** Player stats, the full inventory, equipped set and quest progress are serialised to a text save file and read back.

## How it works

The codebase is two C files:

- **`struct(prototype).c`** holds the static game content. Three `typedef struct` types (`itm`, `monster`, `qst`) and three decode functions (`item()`, `mon()` and `quest()`) turn a numeric code into a populated record. The leading digit of an item code selects its category table (1xxx weapons, 2xxx head armour, 3xxx body armour, 4xxx shoes, 5xxx jewelry, 6xxx consumables, 7xxx quest items); a monster code's tens digit selects its zone; a quest code indexes the mission list.

- **`mainfunc(prototype).c`** holds all the gameplay and rendering. A thin console layer wraps the Win32 API: `gotoxy()` over `SetConsoleCursorPosition` (with `wherex()`/`wherey()` reading the caret back) for in-place redraws, and `textColor()` over `SetConsoleTextAttribute` for foreground colour. Most navigation (maps, inventory) reads keys directly with `_getch()` from `conio.h`, though some menus, the battle menu among them, take numeric line input via `scanf`. Screens resize via `mode con` so battle, inventory and stat allocation each get a fitting canvas.

The player's live stats (attack, HP, defence, speed, gold, level, experience and skill points) are global integers. Equipping a piece folds its bonuses into those stats; the inventory tracks the five wearable equipment slots plus stacks of consumables and quest items. The game runs as a loop of town and field screens (`potion_village`, `field_field`, and their siblings) that hand off to `battle()`, `shop()`, `questfunc()`, `expadd()` and `stat()` as the player explores, fights and grows.

## Tech stack

- **Language:** C (plain structs and code-keyed lookups)
- **Rendering:** Windows console via `windows.h`: `SetConsoleCursorPosition`, `SetConsoleTextAttribute`, `mode con` sizing
- **Input:** `conio.h` `_getch`
- **Tooling:** Visual Studio (the solution targets Visual Studio 14, 2015)

## Build & run

This is a Windows console application built with Visual Studio. Open `DragonSword legend 4.0(Alpha).sln` in Visual Studio (the solution was created with the 2015 toolset) and build the `Release` configuration, then run the resulting executable in a Windows console.

> Note: the tracked files are the two `.c` sources plus the `.sln`; the > `.vcxproj` that the solution references is not committed, so the project may > need to be re-created or re-added in Visual Studio before it will build.

The Korean text is stored in a legacy code page (CP949/EUC-KR), so it renders correctly only in a console using that code page.

> Note: the save feature writes to and reads from a fixed path (`c:\save\SAVE.txt`), > so that directory must exist for Save/Load to work.

## Project structure

```
DragonSword legend 4.0(Alpha)/
├─ DragonSword legend 4.0(Alpha).sln        # Visual Studio solution (references a .vcxproj not committed here)
└─ DragonSword legend 4.0(Alpha)/
   ├─ struct(prototype).c                    # item / monster / quest data tables + decode functions
   └─ mainfunc(prototype).c                  # gameplay: battle, leveling, inventory, shop, quests, maps, console UI, main()
```

## Status

Early self-taught solo project, built in 2015. Marked "4.0 (Alpha)".

## Links

- [Source on GitHub](https://github.com/saneaven/Portfolio/tree/main/projects/game/DragonSword%20legend%204.0(Alpha))
