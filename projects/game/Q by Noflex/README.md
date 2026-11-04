# Q by Noflex

*A side-view roguelike platformer on a custom C++ engine, and the ECS core, parallel system scheduler and enemy AI I built for it.*

## Overview

Q is a side-view roguelike platformer where you fight through randomly arranged rooms to collect the cube bricks of a station adrift in orbit. The concept takes the trapped-in-a-shifting-maze premise of the film *Cube* and lifts it into science fiction: the maze is a derelict station on orbit, broken into rooms that are arranged differently every run. The player drops in, clears each room, and pushes on to gather every cube brick.

Because layouts and encounters are rebuilt per run, the underlying systems have to spin entities up and tear them down cheaply every time. That requirement is what shaped the data-oriented core: an Entity Component System backed by contiguous component storage, with independent systems scheduled across worker threads.

Q was built by team **Noflex** at DigiPen in 2021 on an engine the team wrote from scratch.

## My role

This was a six-person team project. On the code side I owned exactly three things end to end:

- **The ECS architecture**: entities, components, and the managers that hold them.
- **The parallel system scheduler**: the thread pool and the partitioning that lets systems run across cores.
- **The enemy AI**: a per-enemy finite-state-machine framework and the nine enemy/boss behaviours built on it.

The rest of the engine was built by teammates: random map generation, collision, graphics, and other subsystems each had their own owners. Beyond the code, I also led team Noflex as **product manager**: owning concept, scope and schedule, and carrying the build through the course milestones (concept proof, pre-production, pre-alpha, alpha, beta, final).

The sections below describe only the parts I authored.

## Highlights

- **Entity Component System.** Entities are ids, behaviour lives in plain components, and systems iterate over the data. Splitting gameplay this way keeps it composable and makes building or clearing a room a cheap, uniform operation on every run.
- **Cache-friendly component storage.** A custom `ContiguousHashMap` keeps each component type in a packed, contiguous array (with a side map from entity id to element), so the hot per-frame loops stream through memory instead of chasing pointers.
- **Parallel system scheduler.** A worker-thread pool runs independent work across cores. In Release builds the pool sizes itself to `std::thread::hardware_concurrency()`, so per-frame work scales past a single core rather than ticking everything serially on the main thread.
- **Contention-free partitioning.** Components are split into disjoint ranges per worker thread, so threads operate on separate data and synchronisation is kept to the explicit hand-off points between systems.
- **Per-enemy state machines.** Each enemy state (Searching, Chasing, Attacking, Missing, Hurting, Dying) is its own class, and switching state swaps the live state's function pointers under the thread-pool mutex so it stays safe under the parallel scheduler.
- **Nine distinct behaviours.** Seven enemy types and two bosses share one state-machine backbone but specialise it: a path-following self-destructing Bomber, a stationary Turret, a shield-raising Tanker, a sacrificial Engineer that spawns turrets, and two bosses whose random-pattern hubs pick one of four attack states each cycle.

## How it works

### ECS core

The ECS is split across small managers:

- `EntityManager` issues entity ids and tracks types; `BaseEntity` exposes `AddComponent`, `HasComponent`, `GetComponent`, `DeleteComponent` against the owning scene.
- `ComponentManager` stores one `ContiguousHashMap<Comp>` per component type, plus queues for deferred deletion so entities can be removed safely between frames.
- `SystemManager` holds the systems and drives their `Load` / `Update` / `Unload` lifecycle, and can route events to a specific system.
- `ContiguousHashMap<T>` is the storage primitive: a contiguous element array for fast iteration paired with an `unordered_map` from id to element pointer for lookups.

### Parallel scheduling

The thread pool (`ThreadPool`) spins up a fixed set of worker threads and hands them `std::function` work items; `WakeUpThreads` / `WaitAll` bracket a parallel pass. To parallelise a system over its components, `ContiguousHashMap::GetPartContainer(maxPart, partNum)` returns a disjoint slice of the packed array for a given worker, so each thread iterates its own range with no overlap. A shared static mutex (`ThreadPool::GetMutex()`) guards the few places where threads must touch shared state.

### Enemy AI

The enemy AI is a finite-state-machine framework. `IEnemyState<T>` defines the interface for a state (`Load`, `Update`, `Unload`, plus `OnTriggerEvent`, `OnRayEvent`, `OnAttackEvent` handlers) and every concrete state (e.g. `WarriorChasingState`, `BomberExplodingState`) is its own class implementing that interface.

`EnemyStateComponent` holds the currently active state as a set of `std::function` handlers. Transitioning is just `SetCurrentState<NextState>()`, which unloads the old state, swaps in the new state's function pointers, and loads it, all under the thread-pool mutex so it is safe while the AI runs in parallel.

`EnemyStateSystem::Update` drives the AI across threads: it wakes the pool, gives each worker a `ThreadedUpdate` over its `GetPartContainer` slice of `EnemyStateComponent`s, waits for all of them, then resolves deaths and item drops on the main thread. Transitions are driven by three signals: a sight raycast (a view cone plus a sight distance), weapon-range hits, and collisions, all delivered into the active state through the event handlers.

The nine behaviours built on this backbone:

| Enemy | Behaviour |
| --- | --- |
| Warrior | Chases the last-seen player and jumps over cliffs and walls; attacks at weapon range. |
| FlameThrower | Chases but stops at platform edges instead of jumping, so it never walks off. |
| Scout | While chasing, a timer auto-fires its attack state to launch a projectile, then resumes the chase. |
| Turret | Never moves; loops its attack, firing a bullet aimed at the player, and idles when it loses line of sight. |
| Tanker | Arms then defends, invincible while the shield is up, rotating to keep facing the player. |
| Engineer | Runs from the player, summons a Turret, and dies in the act: a sacrificial spawner. |
| Bomber | Follows an A* path that re-plans periodically to navigate platforms, then self-destructs on contact. |
| Assassin (boss) | A change-pattern hub picks one of four melee attack states at random each cycle. |
| Magician (boss) | The same random four-pattern hub as the Assassin, but ranged: each pattern fires its own projectile volley. |

## Tech stack

- **Language:** C++17.
- **Engine:** a custom data-oriented engine: ECS, contiguous component storage, a worker-thread pool, A* pathfinding, and Lua scripting for effects and gameplay scripts.
- **Toolchain:** Visual Studio 2019 (MSVC `v142`), x64, Debug/Release.

## Build & run

The game is a Visual Studio solution built for Windows x64.

1. Open `QGame/game-project-noflex/Q/Q.sln` in Visual Studio 2019 (MSVC `v142`, Windows SDK 10).
2. Select the `Q` project, set the platform to **x64**, and build in **Release** (Debug also builds; Release is where the thread pool sizes itself to the machine's core count).
3. Run the `Q` target. Assets are loaded from the project's `resource/` directory.

The solution also includes a `Q-Editor` configuration (ImGui-based) and a separate `LogAnalyzer` tool.

To just play it, download the installer from the [repository's Releases](https://github.com/saneaven/Portfolio/releases).

## Project structure

```
Q by Noflex/
├─ QGame/game-project-noflex/Q/
│  ├─ Q.sln                         # Visual Studio 2019 solution
│  └─ Q/
│     ├─ Engine/
│     │  ├─ ECS/                    # Entity/Component/System managers (my work)
│     │  │  ├─ Entity*.h            # entities as ids + component access
│     │  │  ├─ ComponentManager.*   # one contiguous map per component type
│     │  │  └─ SystemManager.*      # system lifecycle + event routing
│     │  ├─ Core/
│     │  │  ├─ ThreadPool.*         # worker-thread pool (my work)
│     │  │  ├─ ContiguousHashMap.*  # packed component storage (my work)
│     │  │  └─ AStarPathFinding.*   # pathfinding used by the Bomber
│     │  ├─ System/                 # engine-side systems (collision, physics, draw…)
│     │  ├─ RandomMap/              # per-run map generation (teammate)
│     │  └─ Scripting/              # Lua / Python scripting glue
│     ├─ Game/
│     │  ├─ System/EnemyStateSystem.*           # threaded AI driver (my work)
│     │  └─ Component/EnemyStateComponent.h     # active-state function pointers (my work)
│     │     └─ EnemyStateSyscomps/              # per-enemy state classes (my work)
│     │        ├─ WarriorStates/  FlameThrowerStates/  ScoutStates/
│     │        ├─ TurretStates/   TankerStates/        EngineerStates/
│     │        ├─ BomberStates/   BossAssassinStates/  BossMagicianStates/
│     │        └─ IEnemyState.h                 # state interface
│     └─ resource/                  # lua, animations, textures, shaders, sound…
└─ (milestone reports & design art)
```

## Links

- [Source on GitHub](https://github.com/saneaven/Portfolio/tree/main/projects/game/Q%20by%20Noflex)

## Status

A 2021 DigiPen team game project by the six-person team Noflex. My contribution: the ECS architecture, the parallel system scheduler, and the enemy AI, plus leading the team as product manager.
