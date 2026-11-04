# The Forest of Food

*A top-view seasonal cooking adventure: pick a season, hunt and forage the forest, then cook the dish that buys your way out.*

## Overview

The Forest of Food is a top-view adventure game. You choose a season, drop into the forest, and the forest answers in kind: spring brings cows, summer chickens, autumn turkeys, winter pigs, each with its own crops and mushrooms to forage. You attack animals for meat, pick what grows, and carry it all back to the pot. The fairy of the forest sets the bar: cook the dish she asks for and she lets you leave; fall short and you stay.

The team built the game on DigiPen's in-house graphics-and-input engine (`doodle`), writing its own gameplay engine layer on top of it.

## My Role

This was a three-person team project (Team Gathering). I was the **Technical Director**, and I owned both the engine layer the rest of the game runs on and the gameplay systems built on top of it:

- The **object manager** that owns and drives the live world.
- The **basic math types**: `Vector2D`, `Rectangle`, `Line`.
- The **Bezier interpolation** system.
- The **sound manager** wrapping the audio library.
- The **collision physics** and the **animal AI state machine**.

Production and design were handled by my teammates; this README describes my part.

## Highlights

- **A uniform object world.** Every gameplay actor (player, items, animals, trees) derives from a shared `Object` base carrying position, velocity, a sprite, a hitbox and a current state, so one manager can tick and draw them all against the camera matrix.
- **A reusable math kit.** A templated `Vector2D` with `Normalize` (used by the AI), a `Rectangle` that doubles as the hitbox (`isPointInside`, `isOverlapped`, directional vertex queries), and a `Line`.
- **Smooth value animation.** A Bezier `InterpolationManager` animates positions and scalars over time without per-frame easing code at the call site.
- **A three-state animal brain.** Each animal patrols, flees when hit, and dies dropping the right meat for its species.
- **Direction-resolved collisions.** Hits are detected on rectangle hitboxes and resolved by the edge they met on, so animals bounce off trees instead of clipping through them.

## How It Works

### Object manager and the object base

`GameObjectManager` owns the live world (the `Player`, `Items`, `Animals` and `Trees`) and drives each frame with `UpdateAll` and `DrawAll`. Everything in the world is an `Object`: the base class holds position, velocity, acceleration, a `Sprite`, a `Rectangle` hitbox, a per-object map of sound effects and a pointer to its current `State`. Because the actors share that base, the manager ticks and draws them uniformly against the camera transform.

### Math types

The geometry kit the rest of the code leans on:

- `Vector2D<>`: templated 2D vector with the usual arithmetic plus the `Normalize` the AI uses to compute flee directions.
- `Rectangle`: built from two corner vertices and a hotspot; it answers `isPointInside`, `isOverlapped` and directional vertex queries, and serves as every object's hitbox.
- `Line`: the small primitive the higher-level geometry builds on.

### Bezier interpolation

`InterpolationManager` owns the running interpolations and reaps the ones that finish. `AddSpaceLerp` moves a `Vector2D` along a cubic Bezier path defined by four control points; `AddTimeLerp` eases a scalar from a start to an end value with tunable start/end Bezier strength. The two share a `BezierInterpolation` base, with `TimeInterpolation` specialising the scalar case.

### Animal AI state machine

Each animal is a `Creature` running a finite-state machine over the engine's `Object::State` base, where every state implements `Enter`, `Update` and `TestForExit`:

- **`State_Moving`** patrols: it walks, flips direction on a randomised turn timer with a short pause between turns, and periodically plays its species cry.
- **`State_Runaway`** triggers on a hit: the creature flees along the normalised vector away from the player at roughly twice its walk speed.
- **`State_Dying`** runs when HP reaches zero: the creature drops the right meat item (beef, chicken, turkey or pork) for the player to collect.

`Animal_Lock` clamps each creature inside the map rectangle, and collisions with trees are resolved directionally so animals push back out of the scenery instead of clipping through it.

### Physics and collision

Collision runs on the rectangle hitboxes. `CollisionCheck` reports which edge two objects met on; the resolver reads that direction, snaps the moving body just outside the other's hitbox with a fixed correction offset, and flips the matching velocity component so it rebounds cleanly. The knife swing only lands while the animal is past its invincibility window: a hit decrements HP, retriggers the brief invincibility timer, and routes the creature into Runaway, or into Dying when HP hits zero.

### Sound

The `SoundManager` wraps the audio library behind a small interface, caching loaded sound buffers and music tracks by file path. Objects register effects by name through `AddSFX` and fire them by name through `PlaySFX`, so an animal can cue its own cry on a timer and a scene can swap its background track on a state change.

## Tech Stack

- **Language:** C++ (C++17).
- **Engine:** DigiPen's in-house `doodle` graphics-and-input engine, with a team-built 2D gameplay layer (object manager, math types, sprites, transforms, state and game-state managers) on top.
- **Audio:** SFML Audio.
- **Toolchain:** Visual Studio 2019 (MSVC v142), Windows / OpenGL.

## Build & Run

The game is a Windows desktop application built with Visual Studio.

- **To build from source:** open `The_Forest_of_Food/Source/doodle.sln` in Visual Studio 2019 and build. The project links against the `doodle` engine and SFML, expected under `external/doodle` and `external/SFML` in the solution directory; the build copies the SFML and OpenAL DLLs next to the executable.
- **To just play it:** download the installer from the [repository's Releases](https://github.com/saneaven/Portfolio/releases).

### Controls

| Key | Action |
| --- | --- |
| Arrow keys | Move |
| Space | Attack |
| Shift | Run |
| Z | Inventory |
| Ctrl | Interact / eat |

## Project Structure

```
The_Forest_of_Food/
├─ Source/
│  └─ doodle/
│     ├─ Engine/                 # team-built engine layer
│     │  ├─ GameObjectManager.*  # owns and drives the live world (mine)
│     │  ├─ Object.*             # shared actor base + State (mine)
│     │  ├─ Vector2D.* / Rectangle.* / Line.*  # math + hitbox kit (mine)
│     │  ├─ BasicDataTypes.*     # shared data types (mine)
│     │  ├─ Interpolation.*      # Bezier space/time lerp (mine)
│     │  ├─ Sound.* / SoundManager.*           # SFML audio layer (mine)
│     │  ├─ Sprite / Texture / Transform / Animation / Map
│     │  ├─ GameState* / Input / window         # engine plumbing
│     ├─ Game/
│     │  ├─ Creature.*           # animal AI state machine (mine + Producer)
│     │  ├─ Player.* / Item.* / Environment.*   # gameplay actors
│     │  ├─ Cooking / Inventory / Fairy / Ending
│     │  ├─ Spring / Summer / Fall / Winter      # per-season recipe checks
│     │  └─ Splash / Story / Credit / HowToplay  # screens
│     ├─ WinMain.cpp
│     └─ doodle.vcxproj
│  └─ doodle.sln
├─ Documents/                    # design and technical docs
├─ Marketing/                    # poster, sell sheet
└─ ReadMe.txt                    # original shipping readme
```

## Links

- [Source on GitHub](https://github.com/saneaven/Portfolio/tree/main/projects/game/The%20Forest%20of%20Food)

## Status

Built at DigiPen in 2020 by Team Gathering (three people): I was the Technical Director, alongside Sieun Choi (Producer) and Hyeonseo Yoon (Designer).
