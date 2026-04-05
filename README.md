# Chess

This repository contains a C++ chess game and a custom backtracking analysis engine used by the Analysis Board.

## Documentation

- Engine and analysis architecture: [docs/backtracking-engine.md](/home/swarajzende/Chess/docs/backtracking-engine.md)
- API documentation: build the `doc_doxygen` target to generate browsable Doxygen output from the inline comments

## Analysis Engine

The current Analysis Board uses a custom `BacktrackingEngine` with:

- iterative deepening
- bounded search via constructor-owned depth, node, and time budgets
- asynchronous background analysis so the board remains responsive

For implementation details, tuning notes, and current limitations, see [docs/backtracking-engine.md](/home/swarajzende/Chess/docs/backtracking-engine.md).
