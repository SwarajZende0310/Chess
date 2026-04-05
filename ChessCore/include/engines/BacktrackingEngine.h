/**
 * @file BacktrackingEngine.h
 * @brief Backtracking chess engines inheriting from Engine base class
 * which follow UCI(Universal Chess Interface) protocol.
 */
#pragma once

#include "engines/Engine.h"
#include <chrono>

namespace chess
{
    /**
     * @brief A simple backtracking chess engine implementation.
     *
     * The engine searches by iterative deepening over a recursive alpha-beta
     * search. It still uses a FEN-backed internal workflow, which keeps the
     * implementation compact but makes it significantly slower than a mature
     * incremental chess engine. Constructor-owned budgets provide the default
     * policy used by asynchronous Analysis Board requests.
     */
    class BacktrackingEngine : public Engine
    {
        public:
            /**
             * @brief Construct a new Backtracking Engine.
             * @param maxDepth Maximum depth that engine should search.
             * @param nodeBudget Safety node budget for a single analysis.
             * @param timeBudgetMs Time budget in milliseconds for a single analysis.
             *
             * These values become the engine defaults. Individual
             * `EngineSearchRequest` instances may still override them.
             */
            BacktrackingEngine(int maxDepth, size_t nodeBudget = 200000, int timeBudgetMs = 0);

            /**
             * @brief Default destructor.
             */
            ~BacktrackingEngine() = default;

            /**
             * @brief Get the Evaluation of the current position.
             * @param fen FEN string representing the board state.
             * @param depth Max Depth at which engine will search
             * @return Evaluation as a float in unit of pawns.
             */
            float GetEvaluation(std::string& fen, int depth) override;

            /**
             * @brief Analyze a position using iterative deepening under explicit limits.
             * @param request Position and search budgets.
             * @return Best completed result within the given limits.
             *
             * The engine deepens one ply at a time and only publishes the last
             * fully completed depth. If a request is cancelled or budgeted out,
             * `aborted` is set in the returned result and the last stable depth
             * is preserved.
             */
            EngineSearchResult AnalyzePosition(const EngineSearchRequest& request) override;

            /**
             * @brief Get the best move of the engine.
             * @return Best move in standard notation (UCI) (e.g. "e2e4").
             */
            virtual std::string GetBestMove() const override;

            /**
             * @brief Get the version of the engine.
             * @return Engine version as a string.
             */
            virtual std::string GetEngineVersion() const override;

            /**
             * @brief Get the name of the engine.
             * @return Engine name as a string.
             */
            virtual std::string GetEngineName() const override;

        private:
            /**
             * @brief Temporary undo record used by the current FEN-based search.
             *
             * This is intentionally simple rather than fast. It allows the
             * search to restore state after recursive move exploration at the
             * cost of extra string copying.
             */
            struct MoveHistoryEntry
            {
                std::string fenBeforeMove;
                std::string move;
            };

            /**
             * @brief Get the current evaluation of the positon basis the difference in material.
             * @param fen FEN string representing the board state.
             * @return Evaluation of current position.
             */
            int GetEvaluationOnPieceValue(const std::string& fen); 

            /**
             * @brief Get all possible moves.
             * @param fen FEN string representing the board state.
             * @param whiteMove True if it's white's turn to move.
             * @return returns all possible legal moves in current position.
             */
            List<std::string> GetAllPossibleMoves(const std::string& fen, bool whiteMove);

            /**
             * @brief Makes the move in the fen string.
             * @return returns true if move was made successfully.
             */            
            bool MakeMove(std::string& fen, std::string move);

            /**
             * @brief Undos the move in the fen string.
             * @param fen FEN string representing the board state.
             * @param move Move to make
             * @return returns true if move was successfully undone.
             */            
            bool UndoMove(std::string& fen, std::string move);

            /**
             * @brief returns Attacked squares by Pieces basis whose move.
             *        if its whites move will return attacked sqaures by black and vice-versa
             * @param board 2D matrix represtation of current state
             * @param whiteMove Whether its whiteMove or not
             * @return returns Attacked Sqaured.
             */  
            std::set<std::pair<int,int>> GetAttackedSquares(const List<List<char>> &board, bool whiteMove);

            /**
             * @brief returns true if row and col are in 8x8 grid(0 indexed)
             * @param row row in board
             * @param col col in board
             * @return returns if row and col are in bound.
             */  
            bool InBounds(int row, int col);

            /**
             * @brief adds attacked squares in place basis start row and col and offsets accordingly
             * @param board 2D matrix represtation of current state
             * @param startRow start row in board
             * @param startCol start col in board
             * @param dirOffsetRow array of offset to cast the raw in row
             * @param dirOffsetCol array of offset to cast the raw in col
             * @param offsetLen size of offsets array
             * @param attackedSquares set of attacked sqaures to add inplace
             * @return void
             */
            void AddSquaresInRayAttacks(const List<List<char>>& board, int startRow, int startCol,
                                        int* dirOffsetRow, int* dirOffsetCol, int offsetLen, 
                                        std::set<std::pair<int,int>>& attackedSquares);

            /**
             * @brief Caluclates All Possible King Moves in place
             * @param board 2D matrix represtation of current state
             * @param whiteMove Whether its whiteMove or not
             * @param moves adds possible move in place
             * @return returns void.
             */
            void GetKingMoves(const List<List<char>>& board, bool whiteMove, List<std::string>& moves);

            /**
             * @brief Returns whether King is in Check
             * @param board 2D matrix represtation of current state
             * @param whiteMove Whether its whiteMove or not
             * @param attackedSquares attacked Square according to whose move
             * @return returns whether king is in check.
             */
            bool IsKingInCheck(const List<List<char>>& board, bool whiteMove, const std::set<std::pair<int,int>>& attackedSquares);

            /**
             * @brief Alpha-beta search entry used by GetEvaluation.
             * @param fen Current position in FEN.
             * @param depth Remaining search depth.
             * @param alpha Current alpha bound.
             * @param beta Current beta bound.
             * @param rootDepth Original requested depth used to update mBestMove only at the root.
             * @return returns evaluation in pawn units.
             */
            float Search(std::string& fen, int depth, float alpha, float beta, int rootDepth);
            /**
             * @brief Returns true when the current search should stop.
             *
             * This checks constructor/request-owned limits such as external
             * cancellation, node budget, and time budget.
             */
            bool ShouldAbortSearch() const;

            int mMaxDepth;          ///< Max Depth for Backtracking 
            std::string mBestMove;  ///< Latest Best Move after Backtracking 
            List<MoveHistoryEntry> mMoveHistory; ///< FEN snapshots used to undo in-search moves
            size_t mVisitedNodes;   ///< Number of nodes visited in current search
            size_t mNodeBudget;     ///< Active node budget for current analysis
            bool mSearchAborted;    ///< True if current search hit the safety budget
            std::chrono::steady_clock::time_point mSearchStartTime; ///< Start time for current analysis
            int mTimeBudgetMs;      ///< Active time budget for current analysis
            const std::atomic<bool>* mCancelFlag; ///< External cancellation signal
            size_t mDefaultNodeBudget; ///< Default node budget configured at construction
            int mDefaultTimeBudgetMs;  ///< Default time budget configured at construction
    };
}
