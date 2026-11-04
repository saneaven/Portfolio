# Algorithms

*A collection of classic algorithm implementations in C++, each built around a different design technique: brute force, divide and conquer, dynamic programming, greedy search, and branch-and-bound.*

## Overview

This is a set of self-contained algorithm assignments completed at DigiPen. Each one takes a well-known computational problem and implements it from scratch in C++, emphasizing a specific algorithmic paradigm and the trade-offs that come with it (correctness, asymptotic complexity, and real runtime against graded performance targets).

Every assignment ships with a driver that runs the implementation against a battery of test cases and diffs the output against a reference, so the focus throughout is on writing solutions that are both correct and fast enough to clear the timing bounds.

This is solo work: all of the implementation code (the core algorithm files and headers) was written by me. The drivers, Makefiles, and handouts were provided as the starting scaffold.

## The assignments

**Convex hull (brute force).** Given a set of 2D points, finds the points that form the convex hull. Two methods are implemented: an `O(n^3)` edge test (for every pair of points, check whether all other points lie on one side of the line through them, and if so both points are hull vertices) and a gift-wrapping variant that starts from the leftmost point and walks counter-clockwise, picking the next vertex such that every other point is to its left. Hull vertices are accumulated in a `std::set` to deduplicate.

**Empirical analysis of the maximum subarray sum.** An experimental study rather than a single algorithm. Kadane's algorithm computes the maximum contiguous subarray sum over arrays of length *N* filled with values in `[-N/3, 2N/3]` and randomly shuffled. The experiment sweeps *N* from 100 to 10,000 (1,000,000 trials per size), and the results are plotted and fitted in a LaTeX report; the average maximum subarray sum tracks `O(N^2)`, approximated closely by `(1/6)N^2`.

**Travelling salesman (best-first search).** Solves TSP as a branch-and-bound / best-first search rooted at city 0. The lower bound comes from a relaxed version of the problem (the sum of each city's cheapest incident edge); branches whose current cost plus relaxation bound already exceed the best known tour are pruned. Neighbours are explored in increasing-distance order via a per-city priority queue, so promising tours are found early and prune more aggressively.

**Closest pair (divide and conquer).** Recursively splits the point set by a vertical line, solves each half, then merges by only comparing points within the strip whose width is the current best distance. Handles the degenerate case where an x-split cannot separate the set (collinear points) by falling back to a horizontal y-split, and the fully-degenerate case where all points coincide (distance 0). Distances are compared in squared form to avoid unnecessary `sqrt` calls.

**Electric-vehicle reachability (modified Dijkstra).** Determines whether an electric vehicle with a given range can travel between any two locations in a town using at most *K* recharges. The graph is connected and undirected, and recharging is only possible at vertices. The search state is a `(recharges left, charge remaining)` pair carried by a `Car`, expanded with a priority queue; a state is "better" when its combined remaining capacity is higher, which is the ordering that makes the modified Dijkstra correct.

**0-1 knapsack (dynamic programming + memoization).** The discrete knapsack problem solved two ways: a bottom-up dynamic-programming table and a top-down recursion with memoization. Both fill the same weight-by-item table and then walk it backwards to reconstruct exactly which items are in the optimal bag.

**Rectangle overlap distribution (scan-line / sweep).** Given up to 10^5 rectangles on a grid that can be up to 10^9 cells per side, computes how many cells are covered by exactly *C* rectangles, for every *C* (counts taken modulo 7001 to avoid overflow). A brute-force grid version is included for validation; the real solution sweeps a horizontal line top-to-bottom over rectangle start/end events, accumulating cell-count frequencies weighted by the height of each band so the enormous grid is never materialized.

**Minimum range for an electric vehicle (binary search over Dijkstra).** A continuation of the EV reachability problem: instead of asking whether a fixed range works, it finds the *minimum* range that still allows every location to reach every other within the recharge budget. It binary-searches the range, invoking the modified-Dijkstra feasibility check at each step, which turns a linear scan over candidate ranges into a logarithmic one.

## Tech stack

- **Language:** C++ (C++11)
- **Compiler / build:** g++ via GNU Make
- **Reporting:** LaTeX (`pgfplots`) for the empirical-analysis write-up
- **Tooling:** Valgrind for leak checking (wired into the Makefile test targets)
- **Standard library:** `std::vector`, `std::set`, `std::map`, `std::priority_queue`, and the C++11 `<random>` engine

## How it works

Each assignment lives in its own folder under `AssignmentN/` and follows the same shape:

- a core source file (e.g. `hull-bruteforce.cpp`, `tsp.cpp`, `closestpair.cpp`) plus its header, the part I implemented;
- a `driver.cpp` that loads test inputs, calls the implementation, and prints results;
- a `Makefile` whose `gcc0` target compiles driver + implementation into a single executable, numbered targets (`make 0`, `make 1`, …) run individual tests and diff against the bundled reference outputs, and `memN` targets run the same tests under Valgrind;
- reference outputs (`out0`, `out1`, …) and, where relevant, input maps or point sets.

The graph problems represent the map as an adjacency matrix (`typedef std::vector<std::vector<int>> MAP`), the geometry problems pass an index vector over a shared point array to avoid copying, and the search problems (TSP, EV) push partial-solution state objects through a `std::priority_queue` so the most promising state is always expanded next.

## Build & run

Each assignment is built and tested from its own directory. For example, the convex-hull assignment:

```sh
cd Assignment1/convhull_BF-files
make gcc0      # compiles driver.cpp + hull-bruteforce.cpp into gnu.exe
make 0         # run test 0 and diff against the reference output
make mem0      # run test 0 under Valgrind
make clean     # remove build artifacts
```

The other assignments use the same convention (`make gcc0` to build, numbered targets to run tests). The compiler flags are C++11 with full warnings (`-Wall -Wextra -std=c++11`).

The empirical-analysis assignment is different: `my.cpp` is compiled standalone (`g++ -std=c++11 -O3 my.cpp`) and run as `./a.out <size> <num_tests>`, and the findings are written up in `report.tex`, which is compiled with LaTeX.

## Project structure

```
Algorithm/
├── Assignment1/convhull_BF-files/      # convex hull, brute force + gift wrapping
│   ├── hull-bruteforce.cpp / .h
│   ├── driver.cpp, Makefile
│   └── handout.txt, out0..out13
├── Assignment2/empirical_analysis-files/   # maximum subarray sum study (Kadane)
│   ├── my.cpp, report.tex
│   └── result_summary                  # per-size experiment summary
├── Assignment3/TSP_bestfirst-files/    # TSP via best-first / branch-and-bound
│   ├── tsp.cpp / .h
│   └── map0..map11 (test inputs)
├── Assignment4/closest_pair_dc-files/  # closest pair, divide and conquer
│   └── closestpair.cpp / .h
├── Assignment5/e_dijkstra-files/       # EV reachability, modified Dijkstra
│   ├── e-dijkstra.cpp / .h
│   └── random.graph.cpp (graph generator)
├── Assignment6/knapsack_dynprog-files/ # 0-1 knapsack, DP + memoization
│   └── knapsack-dp.cpp / .h
├── Assignment7/overlap-files/          # rectangle overlap distribution, sweep line
│   └── overlap.cpp / .h
└── Assignment8/e_dijkstra_2-files/     # minimum EV range, binary search over Dijkstra
    └── e-dijkstra-solver.cpp / .h
```

## Status

Coursework, completed solo. Eight assignments, each independently buildable and graded against bundled reference outputs and timing targets.
