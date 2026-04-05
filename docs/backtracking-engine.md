# Backtracking Engine

This document describes the current custom chess engine and how it is integrated into the Analysis Board.

## Overview

The project currently uses a custom `BacktrackingEngine` for analysis mode. It is a bounded, iterative-deepening alpha-beta searcher that works from a FEN representation of the current position.

This engine is intentionally simple and easy to inspect. It is not a high-performance engine yet. The current design prioritizes correctness, explicit control flow, and safe integration with the UI over maximum search strength.

## Current Search Model

The engine exposes two layers:

- `GetEvaluation(fen, depth)`: compatibility wrapper for callers that only want a score.
- `AnalyzePosition(request)`: full bounded analysis API used by the Analysis Board.

The search model is:

1. Parse the root FEN.
2. Search depth 1.
3. Search depth 2.
4. Continue iterative deepening until the target depth is reached or the search is interrupted.

Only the last fully completed depth is published as a result. If the search times out, is cancelled, or hits the node budget, the engine returns the deepest stable completed result instead of a partial subtree result.

## Constructor-Owned Budgets

`BacktrackingEngine` now owns its default search policy through its constructor:

```cpp
BacktrackingEngine(int maxDepth, size_t nodeBudget = 200000, int timeBudgetMs = 0)
```

These defaults are used unless a search request explicitly overrides them.

Current Analysis Board configuration:

- `maxDepth = 10`
- `nodeBudget = 20000000`
- `timeBudgetMs = 1500`

### What these values do

- `maxDepth`: upper bound for iterative deepening.
- `nodeBudget`: hard safety cap on explored nodes for one analysis request.
- `timeBudgetMs`: wall-clock limit for one analysis request.

Increasing any of these values may improve analysis quality, but it also increases latency and CPU cost.

## Search Result Model

`EngineSearchResult` contains:

- `evaluation`: score from the deepest fully completed depth
- `bestMove`: UCI move string for that result
- `visitedNodes`: total visited nodes during the search
- `completedDepth`: deepest fully completed depth
- `elapsedMs`: total search time
- `aborted`: whether the target depth was not fully completed

This is what the Analysis Board uses to decide what to display and what to log.

## Analysis Board Integration

The Analysis Board does not run search on the main thread.

Instead:

1. The board state changes.
2. `AnalysisBoardLevel` converts the current position to FEN.
3. A new analysis generation is queued.
4. Any in-flight search is cancelled.
5. A background worker thread runs `AnalyzePosition(...)`.
6. The completed result is stored.
7. The main thread polls for completed results and publishes the newest one to the HUD.

This prevents input/render stalls while analysis is running.

## Threading Model

Main thread responsibilities:

- handle input
- render board and HUD
- queue new analysis requests
- publish completed analysis results to the evaluation bar

Worker thread responsibilities:

- wait for queued FEN requests
- run bounded engine analysis
- discard stale generations
- store only the latest completed result

Important rule:

- SFML/UI updates stay on the main thread only.

## Cancellation and Generations

The Analysis Board uses a generation counter to prevent stale analysis from being shown.

If the user makes another move while analysis is in progress:

- the queued generation is incremented
- the current search sees the cancel flag
- the old result is ignored if a newer generation already exists

This is what keeps the evaluation display aligned with the latest board state.

## Debug Logging

When a completed result is published, the current implementation logs:

- best move
- evaluation
- elapsed time
- visited nodes
- completed depth
- whether the search was aborted

This is useful for tuning constructor budgets and understanding responsiveness.

## Known Bottlenecks

The engine is still structurally expensive. The biggest current costs are:

- FEN parsing in the search path
- FEN rebuilding and string-based undo records
- string move representation in the hot path
- repeated allocation of move lists and attacked-square containers
- no transposition table
- limited move ordering

These issues are why the engine still needs time and node budgets even after moving to background analysis.

## Tuning Guidance

For better responsiveness:

- lower `maxDepth`
- lower `nodeBudget`
- lower `timeBudgetMs`

For stronger but slower analysis:

- raise `timeBudgetMs` first
- then raise `nodeBudget`
- only raise `maxDepth` if the engine is already completing earlier depths consistently

Recommended tuning order:

1. `timeBudgetMs`
2. `nodeBudget`
3. `maxDepth`

## Next Optimization Targets

The next practical improvements are:

1. Replace FEN snapshots in the hot path with compact incremental undo state.
2. Replace string moves with a compact move struct internally.
3. Improve move ordering so alpha-beta cuts more aggressively.
4. Remove allocation-heavy attacked-square structures from legality checks.
5. Add a transposition table once the internal position representation is stable.
