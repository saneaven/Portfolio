# Game Programming

*Engines and gameplay built from the ground up: physics, ECS cores, AI, and the systems that make a game playable.*

This category collects my game work, most of it built at DigiPen and one piece self-taught before that. The throughline is engine and gameplay programming rather than content: hand-written physics and collision, data-oriented architecture, finite-state-machine and behavior-tree AI, and the data tables and level systems that drive a game. The projects span pure C in a Windows console, C++ on in-house engines, a custom-built C++ engine, JavaScript in the browser, and UI/UX work in Unreal, but in nearly every one the focus is the same: owning the systems underneath the game.

## Projects

| Project | What it is | Key stack |
| --- | --- | --- |
| [Q by Noflex](Q%20by%20Noflex/README.md) | A side-view roguelike platformer on a custom C++ engine: I built the ECS core, the parallel system scheduler, and the per-enemy state-machine AI. | C++17, custom data-oriented engine (ECS, contiguous storage, worker-thread pool, Lua) |
| [An Omen In The Mirror](An%20Omen%20In%20The%20Mirror/README.md) | UI/UX design and in-engine implementation for the interface of a first-person narrative puzzle game. | Unreal Engine, UMG |
| [Game AI Projects](game_ai/README.md) | Three focused studies in classic game-AI algorithms: a behavior-tree "mingle" scene, a tuned A\* pathfinder, and a terrain-analysis toolkit driving a seek-and-hide enemy. | C++20, in-house AI framework, Direct2D/DirectWrite |
| [The Forest of Food](The%20Forest%20of%20Food/README.md) | A top-view seasonal cooking adventure: pick a season, hunt and forage the forest, then cook the dish the forest fairy asks for to escape. | C++17, in-house `doodle` engine, SFML Audio |
| [Bounce NDS](Bounce_NDS/README.md) | A 2D physics ball-platformer (in-game title BOUND) with no jump button: steer a ball and wall-jump off surfaces across ten hand-built stages. | JavaScript (ES6), p5.js |
| [Painter](Project416_prototype/README.md) | A 2D color-swapping puzzle platformer where one rule (same color is passable) reaches from the collision test up to the level design. | C++17, in-house `doodle` engine |
| [Sonar Soul](Project3Dumb_prototype/README.md) | A side-view action game where each enemy holds a soul tied to one musical note; sonar waves only disable the enemy whose note matches, and melody-keyed gates open on the right sequence. | C++, in-house `doodle` framework, SFML Audio |
| [DragonSword Legend](DragonSword%20legend%204.0(Alpha)/README.md) | A Korean-language Windows-console RPG in pure C, committing to the full RPG loop: speed-gauge battle, five-slot equipment from ~185 hand-authored items, regional monster zones with bosses, a quest chain, and leveling with skill points. | C, Windows console (`windows.h`, `conio.h`) |

## Common themes

- **Hand-written physics and collision.** Bounce NDS, Painter, Sonar Soul and The Forest of Food each carry a custom 2D vector math kit and a from-scratch collision layer (AABB, swept-edge, directional, and circle-vs-rectangle responses).
- **State machines and game AI.** Finite-state-machine AI recurs across Q by Noflex (per-enemy states under a parallel scheduler), The Forest of Food (the three-state animal brain) and the standalone game-AI studies (behavior trees, A\*, influence maps).
- **Engine and architecture work.** From a data-oriented ECS with a worker-thread pool (Q by Noflex), to gameplay layers built on DigiPen's in-house engines, to data-table-driven content (DragonSword Legend), the work consistently sits at the systems level.
- **Owning a clear slice of a team build.** Most of these were team projects where I held a defined scope (gameplay programmer, technical director, or product manager) alongside two solo builds.
