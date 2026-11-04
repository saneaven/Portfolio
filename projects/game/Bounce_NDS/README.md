# Bounce NDS

*BOUND, a physics ball-platformer with no jump button: steer a ball left and right and wall-jump off surfaces to reach the exit.*

## Overview

Bounce NDS (in-game title **BOUND**) is a 2D physics platformer written from scratch in JavaScript with p5.js. The whole game is one verb: move the ball. The left and right arrow keys accelerate it, the walls bounce it, and the only way up is to wall-jump off a surface, the way you would in a 2D Mario. Reach the right edge of the stage and it clears; fall off the bottom and you lose a life; press **R** to restart the current stage.

Because there is no dedicated jump, every stage is really a physics puzzle: you read the geometry, build horizontal speed, and convert it into height by slamming into the right wall at the right moment. That makes the feel of the simulation (gravity, top speeds, the bounce response) the actual gameplay. The game runs entirely in the browser on an HTML canvas, across ten hand-built stages that introduce bouncy pads, launchers, breakable stone and lethal bombs.

This was a two-person team project at DigiPen in 2019.

## My Role

This was built with one teammate, Junhyuk Cha. My part was the **physics engine and gameplay**:

- The custom `_vec2` vector math class.
- The `_ball` class: position/velocity/acceleration integration, gravity, and per-axis speed clamping.
- The AABB collision detection and response (`Collision`, `Interact`, `IsBallHitsEdge`).
- The wall-jump and the block interactions (bouncy, launcher, stone-break, bomb).
- The input handling and the game's state-machine loop.

My teammate built the **map system**: the level grids and the stage layouts that sit on top of the physics.

## Highlights

- **Vector math from scratch.** Everything sits on a custom `_vec2` class with add / subtract / multiply / divide, length and angle, dot product, and in-place variants, so position, velocity, acceleration and gravity are all plain 2D vectors the rest of the engine composes.
- **Hand-written integration with clamping.** Each frame the ball integrates gravity (a constant downward vector) and input (a horizontal acceleration), then `checkMaxSpeed` clamps both axes to per-axis caps so the ball never outruns the collision test and tunnels through a block.
- **AABB collision with case routing.** Collision first tests the ball against each block as an axis-aligned box, then decides whether it struck a vertical face, a horizontal face, or only a corner, routing each case to a different response so attached blocks do not double-resolve.
- **Corner handling.** When the ball touches only a block corner, `IsBallHitsEdge` measures the distance from the ball centre to each of the four corners against its radius and treats a corner hit as a top/bottom hit, keeping the response stable on tight diagonal gaps.
- **The wall-jump.** The signature move. On a side collision the ball is first pushed just outside the block to avoid resolving the same hit twice; then, if you are holding the arrow into the wall, `wallJump` fires: it reverses horizontal velocity and overshoots it past the cap, kicks the ball upward, and steps the simulation forward until it is clear of geometry before settling into a clean launch velocity. If you are not pressing into the wall, the engine instead arms a brief window in which a press still counts as a wall-jump and simply reflects the horizontal velocity.
- **Six block types reusing the same velocity plumbing.** A normal block bounces the ball back at full vertical speed; a stone block bounces once and then breaks, splicing itself out of the level and triggering a redraw of the ball's trail; a bouncy block raises the vertical speed cap for an extra-high rebound; left- and right-arrow launchers cut gravity and fling the ball horizontally at double speed; the bomb block is instant death: any contact flips the game into an explosion state, costs a life, and restarts the stage.

## How it works

The game is a single p5.js sketch driven by a status code that acts as a state machine:

- **`sketch.js`** holds the global state and p5 entry points. `preload` loads every block, UI and map image plus the sound effects; `setup` creates the 640×480 canvas and initialises the first map; `draw` switches on the `status` variable to dispatch between in-game play, stage-load animations, health loss, the bomb explosion, game over and the ending.
- **`classes.js`** defines the three core classes: `_vec2` (the vector math), `_ball` (the player: integration, input, wall-jump, gravity/bouncy toggles and the trail rendering), and `_block` (a 32px tile with a type and a sprite).
- **`physics.js`** runs the collision. `Collision` does the broad-phase AABB test against every block and classifies the hit; `Interact` applies the per-face, per-block-type response; `IsBallHitsEdge` resolves the corner case.
- **`game.js`** is the gameplay and presentation loop: `InGame` ties input, movement, gravity, acceleration, collision and the win/lose checks together each frame, and the rest of the file handles the stage screens, number rendering, map-load animation, explosion effect and ending.
- **`map.js`** (teammate's work) holds the ten stages. Each stage is an integer grid where the value selects the block type and a sentinel value marks the ball's spawn point; `InitMap` walks the grid and instantiates `_block` objects at 32px tile positions.

## Tech stack

- **Language:** JavaScript (ES6)
- **Runtime / rendering:** p5.js v0.9.0 on an HTML5 canvas, with `p5.dom.js` and `p5.sound.js`
- **No build step**: plain `.js` files loaded directly from `index.html`

## Build & run

There is no build step. The game is static files served to a browser:

1. Serve the project directory over a local web server (any static server works; loading `index.html` directly via `file://` may be blocked from fetching the image and sound assets).
2. Open `index.html` in a browser.
3. Controls: **Left / Right arrows** to move and wall-jump, **R** to restart the current stage.

The original release was deployed live as a static site.

## Project structure

```
Bounce_NDS/
├── index.html        # Loads p5 + the game scripts in order
├── sketch.js         # Global state, preload/setup/draw, status state machine
├── classes.js        # _vec2 (vectors), _ball (player physics), _block (tiles)
├── physics.js        # AABB collision detection + response
├── game.js           # In-game loop, stage screens, animations, ending
├── map.js            # Ten stage grids + InitMap (teammate's map system)
├── images/           # Block sprites, UI, per-stage map art
├── sounds/           # Sound effects (bounce, wall-jump, break, bomb, ...)
└── p5.js, p5.dom.js, p5.sound.js   # Vendored p5.js v0.9.0 libraries
```

## Links

- [Source on GitHub](https://github.com/saneaven/Portfolio/tree/main/projects/game/Bounce_NDS)

## Status

Two-person team project (team NDS) built at DigiPen in 2019. My first game: a hand-written 2D physics engine shipped in the browser.
