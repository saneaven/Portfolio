# Painter

*A 2D color-swapping puzzle platformer where one rule (same color is passable) reaches from the collision test all the way up to the level design.*

## Overview

Painter is a 2D platformer about a boy who suddenly can no longer see color and sets out to win it back. That small, literal premise becomes the entire verb set: you pick up color items scattered across the map, which recolor you; you throw color balls that paint the blocks and enemies they hit; and reaching the end of the level clears the stage.

The twist that makes it a puzzle rather than a runner is a single rule applied everywhere: if your color matches a block's color, you pass straight through it, and if it matches an enemy's, you can defeat that enemy by stepping on it. Every wall is also a door, and every door is the wrong color until you make it right. The color model is deliberately not additive-light: throwing the three primaries together mixes down to black rather than white.

This was a two-person game I built with Haeun Park at DigiPen in 2019.

## My Role

I was the primary author and gameplay programmer. I wrote:

- the custom **force-based physics** layer (`Object` / `Class_Object_Physics`),
- the **player** logic and its movement/throw/collision/state handling (`Class_Player`),
- the **color-gated collision** that ties movement to color,
- the **follow camera**,
- the supporting **`Vec2`** math layer, the **bullet**, **enemy** and **item** behaviors, and the game loop in `main.cpp`.

My teammate Haeun Park owned the level layout (`Map.cpp`), with me as secondary author there. The headers and shared object code list me as primary author.

## Highlights

- **Color as a collision gate.** Every object carries a 32-bit color. The player's solid-collision pass against a block only runs when the two colors differ (`if (this->getColor() != i.getColor())`); match the block and the test is skipped entirely, so you fall or walk through it. The same gate applies between enemies and blocks.
- **Painting the world.** Pressing `Z` cycles the held ball through red, green and blue; `Space` fires a ball in the facing direction from a muzzle offset, inheriting the player's horizontal velocity and draining one from a reserve of twenty. Where the ball lands it recolors that block or enemy, and that surface now blocks you or lets you through depending on what you're wearing. Repeated hits mix colors additively (e.g. red + green yields a mixed tone; the three primaries combined yield black).
- **A custom force-based body.** Each entity resolves `force -> acceleration -> velocity -> position` every frame. Input only sets intent (step force, jump impulse); gravity and the physics decide the actual motion.
- **Hand-rolled friction.** The opposing force scales with the square root of how close you are to max speed, so the body accelerates briskly off the line and eases as it approaches the cap, instead of an on/off slide.
- **Swept edge collision.** Collision casts the body's leading corner along its velocity relative to the other object and intersects that segment against the four edges of the target box, classifying the hit as up / down / left / right / edge. On a real hit it snaps the body to the contact point and zeroes only the blocked velocity axis.
- **Stomp & death.** A hit resolved as `up` against a same-color enemy stomps it and rebounds the player with a jump; a wrong-color enemy, or a hit from any other side, kills the player. Falling below the world prompts a restart: `R` reloads the stage from scratch.
- **Follow camera.** A `Camera` holds a pointer to the player and snaps to its position every frame at 1.3× zoom; every draw call is offset and scaled by the camera so the boy stays centered as the level scrolls beneath him.

## How it works

Everything inherits from a single `Object` base (`classes.h`) holding position, velocity, acceleration, an accumulated force, a size, a mass and a color. `Block`, `Enemy`, `Bullet`, `Player`, `Item` and `Camera` all extend it.

A typical entity update follows the same shape: gather forces, integrate, then resolve collisions:

```
keyCheck();           // gather step force, jump impulse, fire bullets
gravity();            // add gravity to the force accumulator
friction();           // sqrt-scaled opposing force
setForceAccelation(); // force / mass -> acceleration
accelerate();         // acceleration -> velocity
WallCollisionCheck(); // clear velocity into walls hit last frame
move();               // velocity * DeltaTime -> position
// then iterate blocks/enemies/items: collisionCheck -> collisionInteract
resetForce();
```

`collisionCheck` (in `Class_Object_Physics.cpp`) first does a broad-phase AABB overlap test, then a swept test: it picks the body's leading corner based on relative position, builds the segment from that corner along the relative velocity, and intersects it against each of the four edges of the other box (`GetIntersectPoint`), returning a `Direction`. On a non-edge hit it nudges the body to the contact point; `collisionInteract` then zeroes only the velocity component pointing into the surface.

The color rule lives entirely in `Player::update` and `Enemy::update`: the block-collision loop is wrapped in a color-inequality check, so matching color simply removes the wall. `Object::hitbyball` implements the additive color-mix table used when a bullet repaints something.

`main.cpp` drives a small state machine on a `GameState` char (logo, title, reload, gameplay, credits) and routes keyboard input through `on_key_pressed` / `on_key_released`. `Map.cpp` builds the level from a `17 × 113` integer grid, decoding each cell into a player spawn, a colored block, a colored enemy or an item.

The whole gameplay layer reads in vectors rather than loose floats thanks to a hand-written `Vec2` (`Class_Vec2.cpp`) providing operators, dot product, length and angle helpers, used for forces, velocities, muzzle offsets and the collision segment math.

## Tech stack

- **Language:** C++ (C++17)
- **Engine:** doodle, DigiPen's in-house 2D engine (`doodle/doodle.hpp`, linked against `doodle.lib`, `Opengl32`, `Gdi32`, `User32`)
- **Build:** Visual Studio 2019 solution, Release configuration

## Build & run

This folder is the Painter prototype. It was developed and built in Visual Studio 2019:

1. Open `Project416_Prototype.sln`.
2. Select the **Release** configuration and build.
3. Run the produced executable from a directory containing the bundled PNG assets (the DigiPen logo and copyright textures the title/logo screens load).

The project links against the doodle engine, which it expects under `external\doodle\` relative to the solution (`include` for headers, `bin\<Config>\<Platform>` for the import library). That engine ships separately and is not contained in this prototype folder, so a clean rebuild requires doodle to be present alongside the solution.

### Controls

| Key | Action |
| --- | --- |
| Left / Right | Move left / right |
| Up | Jump |
| Space | Throw a color ball |
| Z | Cycle ball color (red -> green -> blue) |
| R | Restart the stage |

To clear a stage, reach the end of the map. Matching a block's color lets you pass through it; matching an enemy's color lets you defeat it by stomping it.

## Project structure

```
Project416_prototype/
├── Project416_Prototype.sln          # Visual Studio solution
├── README.txt                        # original high concept, controls, build notes
├── Documentation/
│   └── Team416_GCD.docx              # game concept document
├── art/                              # concept art (boy, player)
└── Project416_Prototype/
    ├── main.cpp                      # game loop + state machine + input
    ├── classes.h                     # Object base + Block/Enemy/Bullet/Player/Item/Camera/Vec2
    ├── Class_object.cpp              # Object: integration, color, draw, geometry helpers
    ├── Class_Object_Physics.cpp     # friction + swept collision + resolution
    ├── Class_Player.cpp             # player input, throw, color-gated collision, stomp/death
    ├── Class_Enemy.cpp              # four-color enemies, patrol, ledge probe
    ├── Class_Bullet.cpp             # color balls, painting on hit
    ├── Class_Item.cpp               # color items + goal item + Camera follow
    ├── Class_Vec2.cpp               # 2D vector math
    ├── Class_Button.cpp             # menu buttons
    ├── Map.cpp                      # grid-based level builder (level by Haeun Park)
    └── basicFunc.cpp / .h           # segment-intersection helper used by collision
```

## Links

- [Source on GitHub](https://github.com/saneaven/Portfolio/tree/main/projects/game/Project416_prototype)
