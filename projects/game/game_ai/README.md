# Game AI Projects

*Three focused studies in classic game-AI algorithms, built on DigiPen's in-house AI framework.*

## Overview

Rather than building a game, each of these three assignments isolates a single game-AI problem and pushes it as far as the framework allows. The engine, the editor and the test harness are provided; the work is the algorithm that plugs into them: how an agent decides, how it routes, and how it reads the terrain it moves through. Working against a shared framework meant respecting fixed function signatures and reusing the provided debug tooling, then doing everything interesting inside that boundary.

The three studies are:

1. **Behavior trees**: a scene that recreates Squid Game 2's "mingle" round.
2. **A\* pathfinding**: a pathfinder tuned to a fraction of the framework's baseline.
3. **Terrain analysis**: a toolkit (openness, visibility, influence propagation) that drives a seek-and-hide enemy.

All three assignments were implemented solo (DigiPen, 2024-2025).

## Assignment 1: Behavior trees (the "mingle" round)

On a mouse click the participants line up and march forward, then circle around; on the next click they pair off in twos, and the participant left without a pair is eliminated by the staff. Driving the whole sequence are four behavior trees, `AgentTriangle`, `Bullet`, `ParticipantFollower` and `ParticipantLeader`, composed from custom control, decorator and leaf nodes.

Key custom nodes:

- **`C_ModifiedParallelSequencer`**: a parallel sequencer that, the moment one child fails, forces the remaining children to fail too so their on-exit paths actually run. This closes a gap in the stock parallel node, which left siblings' `on_exit` uncalled.
- **`D_ShortDelay` / `D_RepeatThreeTimes`**: decorators that gate a child behind a brief wait and loop a child three times respectively, chained together to fire a short burst of shots from the staff.
- **`L_CircularFormation`**: reads each agent's circular position off the blackboard and orbits it around a shared centre, ramping turn speed and briefly boosting movement when the target is far so the ring snaps into place. `L_FormationFollow` and `L_MoveToLeader` keep followers locked to the leader.
- **Scene sequencing leaves**: `L_CheckMouseClick`, `L_CheckOrder`, `L_OrderFollow` and `L_OrderClear` advance the round on input; `L_SpawnAgent`, `L_Shoot`, `L_Explode`, `L_WaitForDeath` and `L_DeleteSelf` handle the staff, the gunfire and the elimination, with BGM and SFX leaf nodes scoring it.

The full node and tree registrations live in [`Source/Student/Project_1/Nodes.def`](Source/Student/Project_1/Nodes.def) and [`Source/Student/Project_1/Trees.def`](Source/Student/Project_1/Trees.def).

## Assignment 2: A\* pathfinding

A grid A\* pathfinder built behind the framework's fixed `compute_path` signature. On the framework's speed test it ran in about **975 ms**, against roughly **2400 ms** for the provided reference code, the second-fastest result in the class.

Most of the win came from the open list and the neighbour representation:

- **Bucketed open list**: instead of a heap, nodes are bucketed by final cost into fixed `0.5`-wide intervals. A cached fastest-bucket index walks forward as buckets empty, so a pop finds the next node by scanning a single bucket for its minimum rather than re-sorting the whole frontier. (See `OpenList` and `Bucket` in [`P2_Pathfinding.h`](Source/Student/Project_2/P2_Pathfinding.h).)
- **Per-cell neighbour bitmask**: each cell pre-bakes its walkable neighbours into a single byte. Diagonals are pruned when an adjacent cardinal is a wall, so the search never squeezes a diagonal through a corner, and the neighbour loop is a handful of bit tests instead of repeated wall queries. The masks are rebuilt once on the map-change message and reused across every request.
- **Five heuristics**: Octile, Manhattan, Chebyshev, Euclidean and an intentionally inconsistent mode share a single `calcHeuristic` switch, each scaled by the request weight, so one pather covers every grading case.
- **Path post-processing**: rubber-banding collapses runs of collinear waypoints whose bounding box is wall-free; smoothing then re-inserts midpoints on long gaps and runs a Catmull-Rom spline through the result for a curved, natural path.

The implementation lives in [`Source/Student/Project_2/`](Source/Student/Project_2/).

## Assignment 3: Terrain analysis

The last assignment pre-analyses a static map into a stack of float layers that an agent can sample at runtime: the idea being that to put genuinely smart behaviour into a game AI, you analyse the terrain up front rather than reasoning about it every frame. The layers feed a seek-and-hide game of tag: the enemy stamps its field-of-view cone into an influence layer, propagates that influence outward through the map, and walks toward the strongest cell to chase the player.

The analyses (all in [`Source/Student/Project_3/P3_TerrainAnalysis.cpp`](Source/Student/Project_3/P3_TerrainAnalysis.cpp)):

- **Openness** (`analyze_openness`): each open cell is scored `1 / d²` against the Euclidean distance to its nearest wall or map edge, so corridors and corners read as more enclosed than open floor.
- **Visibility** (`analyze_visibility`, `is_clear_path`): line-of-sight between two cells is a wall-corner crossing test: walk the candidate walls between them and use a 2D cross-product sign check on each wall box's corners to decide whether the sight line clips it.
- **Agent vision cone** (`analyze_agent_vision`): a cell is visible to an agent when the dot product of the agent's forward vector and the direction to the cell clears the field-of-view cosine (comparing cosines directly to avoid arccosine error) and the sight line is clear.
- **Influence and propagation** (`propagate_solo_occupancy`, `normalize_solo_occupancy`): influence spreads cell to cell with exponential distance decay, lerping each cell toward its strongest decayed neighbour by a growth factor into a temp layer, then normalising the whole map into `[0, 1]`.

## Tech stack

| Layer       | Details                                                          |
| ----------- | ---------------------------------------------------------------- |
| Language    | C++ (C++20), Visual Studio                                       |
| Framework   | DigiPen's in-house AI framework (behavior-tree editor, map/grid primitives) |
| Rendering   | Direct2D, DirectWrite                                            |
| Techniques  | Behavior trees, A\*, bucketed open list, influence maps, line-of-sight |

The student work is confined to `Source/Student/`; everything under `Source/Framework/` is the provided engine. `rapidjson` (under `Include/`) is a bundled third-party dependency.

## Build & run

This is a Visual Studio C++ project targeting the v143 toolset (Visual Studio 2022) and C++20.

1. Open `The-Rabin-Engine.sln` in Visual Studio.
2. Select the `Release | x64` configuration.
3. Build and run.

The active assignment is chosen in the framework's project setup; map files live in `Maps/` and the A\* grading scenarios in `Tests/`.

## Project structure

```
game_ai/
├── The-Rabin-Engine.sln              # Visual Studio solution
├── The-Rabin-Engine.vcxproj          # Project (v143 toolset, C++20)
├── Source/
│   ├── Student/                      # My work
│   │   ├── Project_1/                # Behavior trees (Nodes.def, Trees.def, NodeHeaders.h)
│   │   ├── Project_2/                # A* pathfinding (P2_Pathfinding.h/.cpp)
│   │   └── Project_3/                # Terrain analysis (P3_TerrainAnalysis.cpp)
│   └── Framework/                    # Provided engine (BehaviorTrees, Terrain, Agent, Rendering, …)
├── Include/rapidjson/                # Bundled JSON dependency
├── Maps/                             # Grid maps
├── Tests/                            # A* grading scenarios (heuristics, smoothing, speed, …)
└── Assets/                           # Meshes, sprites, audio
```

## Links

- [Source on GitHub](https://github.com/saneaven/Portfolio/tree/main/projects/game/game_ai)

## Status

Three solo assignments completed on a shared in-house AI framework (DigiPen, 2024-2025).
