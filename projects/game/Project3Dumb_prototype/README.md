# Sonar Soul

*A side-view action game where every enemy holds a soul tied to one musical note: fire expanding sonar waves and a wave only disables the enemy whose note matches it.*

## Overview

In Sonar Soul, an enemy has taken a note's soul and the player must reach those souls and release them. Each enemy is bound to exactly one of the seven solfège notes (Do, Re, Mi, Fa, Sol, La, Ti) plus a high Do, so a level reads like a scale you have to play back at it.

You don't shoot to kill so much as sound the right pitch at the right thing. A sonar wave goes out, expands across the level, and only the enemy whose note matches the wave's note answers it; everything else is left untouched. Locked gates extend the same idea into a sequence puzzle: a gate is keyed to an ordered run of notes and stays shut until the player plays exactly that melody against it.

The game was built at DigiPen in 2019 by a three-person team (with Haneul Lee and Hoseob Jeong) on the studio's in-house `doodle` framework in C++.

## My Role

This was a team project. I owned the gameplay programming:

- **The class set:** the gameplay hierarchy built off a shared `Object` base (`Block`, `Enemy`, `Player`, `Bullet`, `Sonar`, `Gate`, `GateSonar`), plus the `Vec2` math type and the `HUD`, `Timer` and `Animation` helpers that drive the on-screen note bank and screen transitions.
- **Physics**, the movement and collision layer in `Object`: gravity, friction, force accumulation, the block collision that walks enemies along ledges, and the bespoke circle-vs-rectangle test the sonar needs.
- **Game mechanics:** the note interface to play (Space opens the note bank, then keys map to syllables for movement, shooting and sonar), gate key-matching, and the screen flow from main menu through play, reload and ending.
- **Audio:** wiring enemy, player and gate notes to playback, and looping background music streamed from an `.ogg`.

Teammates contributed other parts of the codebase (for example the audio loader and the main-menu / screen `GAME` struct); those are credited in the per-file headers and are not claimed here.

## Highlights

- **Note-tagged sonar.** Each `Sonar` is an expanding circle tagged with a `Syllable`. Every frame its radius grows by a sonar speed scaled by delta time while its alpha fades, so the wave visibly ripples outward and dies once it has gone fully transparent.
- **Match disables the enemy.** On overlap the wave compares its own syllable against the enemy's note; only an exact match takes effect, recolouring the enemy to show its soul has been released. So spamming sonar does nothing unless the pitch is right.
- **Circle-vs-rectangle collision.** When the circle's centre sits inside the box's extended bands it is treated as a direct hit; otherwise each of the four corners is distance-checked against the radius, catching the diagonal cases a naive box test would miss.
- **Enemy notes from a switch.** An enemy's `type` maps straight to its note (type 0 -> Do, type 7 -> Do_high) and `getNote()` exposes it to the sonar, so authoring a level is just placing enemies of the right type.
- **Melody-keyed gates.** A `Gate` is given an ordered key (for example Do · Mi · La). The player builds the sequence with a dedicated `GateSonar` emitter, steps a note index up or down, and fires the chosen syllable; hitting the full key in order opens the gate, and at the level's end the gate plays its song as the game resolves.

## How it works

Everything inherits from `Object`, which carries position/velocity/acceleration/force plus width, height and mass, and owns the physics: `gravity()`, `friction()`, force accumulation and an axis-aligned `collisionCheck`. `Enemy::update()` uses this to walk monsters along ledges, sampling points just past each foot so an enemy turns around at the edge of a platform.

`Sonar` extends `Object` with a growing `circle_radius`, a `Syllable` tag and a fading `alpha`. Each frame `update()` advances the radius, decays the alpha, and runs `Circlecollision()` against every enemy. The core test, `collisionCirRect()`, splits into two cases: if the circle centre lies within the rectangle's extended x- or y-band it is a band overlap; otherwise the four corners are distance-checked against the radius. A hit only "answers" the enemy when `syllable == enemy.getNote()`.

The note interface lives in the key handlers: Space opens the note bank on the HUD via a `moveAnimation`, then `Player::use_note()` maps a key to a `Syllable`, plays that note's sound buffer, and pushes a new `Sonar` from the player's position. Gates are driven by `GateSonar` (an outlined sonar variant): on overlap it calls `Gate::addStack()`, which appends the fired syllable to a running stack and checks the tail of that stack against the gate's key: when the full sequence matches, the gate opens and the game advances to its ending screen.

Audio uses SFML: `sf::SoundBuffer`/`sf::Sound` pools for player, enemy and gate notes, indexed by syllable, while the looping background music is streamed from `main_bgm.ogg` via `sf::Music` with `setLoop(true)`. Sound effects were drawn from the licensed Sonniss Game Audio GDC bundle (see `assets/Licensing.pdf`). Levels are loaded from a text map (`map.txt`) that lays out blocks, enemies and the gate.

## Tech stack

- **Language:** C++
- **Framework:** `doodle` (DigiPen's in-house framework)
- **Audio:** SFML/Audio (`sf::Sound`/`sf::SoundBuffer` for notes, `sf::Music` for streamed `.ogg` background music)
- **Build:** Visual Studio (x64 / Win32 solution)

## Build & run

The game is a Visual Studio C++ project. Open `Project3D_prototype.sln` in Visual Studio 2019, select a configuration (the solution defines `Debug`/`Release` for `x64` and `Win32`), and build. It depends on the `doodle` framework and SFML, and the working directory must contain the `sfx/`, `assets/` and `map.txt` data so the audio and level load at runtime.

## Project structure

```
Project3Dumb_prototype/
├── Project3D_prototype.sln          # Visual Studio solution
├── minjae.kyung_SonarSoul_report.docx # development report
└── SonarSoul/
    ├── classes.h                    # gameplay class hierarchy (Object, Enemy, Player, Sonar, Gate, ...)
    ├── sonar.cpp                    # sonar physics + circle-vs-rectangle collision
    ├── Class_Object.cpp            # base Object
    ├── Class_Object_Physics.cpp    # gravity, friction, force, collision
    ├── Class_Enemy.cpp             # enemy type->note mapping, ledge walking
    ├── Class_Player.cpp            # player, note use, gate-sonar emit
    ├── Class_Item_Gate.cpp         # items and the melody-keyed gate
    ├── Class_Bullet.cpp            # bullets
    ├── Class_Camera.cpp            # follow camera
    ├── Class_Vec2.cpp              # 2D vector math (+ program entry)
    ├── doodle.cpp                  # game loop, screen flow, key handlers, BGM
    ├── HUD.cpp / AnimationTimer.cpp # HUD note bank, timers, animations
    ├── map.cpp / map.txt / Map.h   # text-driven level loading
    ├── sfx.cpp / SFX.h             # sound buffer/music loading
    ├── assets/                     # background music + licensing
    └── sfx/                        # per-note and effect .ogg sounds
```

## Links

- [Source on GitHub](https://github.com/saneaven/Portfolio/tree/main/projects/game/Project3Dumb_prototype)

## Status

A prototype built at DigiPen in 2019 by a three-person team. My scope: the class set, the circle-vs-rectangle sonar physics, the gate mechanic and the audio.
