/**
 * @file Engine.h
 * @brief Abstract base for all chess engines 
 * which follow UCI(Universal Chess Interface) protcol.
 */
 #pragma once

 #include "framework/Core.h"
 #include <atomic>

 namespace chess
 {
     /**
      * @brief Search input for engine analysis requests.
      *
      * The request describes the position to analyze and optional search
      * limits. Zero-valued budgets mean "use the engine's constructor
      * defaults". `cancelFlag` allows an external owner, such as the
      * Analysis Board worker, to interrupt search when a newer position
      * supersedes the current request.
      */
     struct EngineSearchRequest
     {
         std::string fen;                         ///< Position to analyze in FEN format
         int maxDepth = 0;                       ///< Max iterative-deepening depth, or engine default if <= 0
         size_t nodeBudget = 0;                  ///< Hard node budget, or engine default if 0
         int timeBudgetMs = 0;                   ///< Hard wall-clock budget in milliseconds, or engine default if <= 0
         const std::atomic<bool>* cancelFlag = nullptr; ///< Optional external cancellation signal
     };

     /**
      * @brief Result returned from a bounded engine search.
      *
      * The engine publishes the best fully completed depth rather than an
      * arbitrary partial subtree. `aborted` indicates that the target depth
      * was not fully completed due to time, node, or cancellation limits.
      */
     struct EngineSearchResult
     {
         float evaluation = 0.0f;    ///< Best evaluation from the deepest fully completed search
         std::string bestMove;       ///< Best move in UCI format for the completed result
         size_t visitedNodes = 0;    ///< Total nodes visited across the iterative-deepening pass
         int completedDepth = 0;     ///< Deepest fully completed depth
         long long elapsedMs = 0;    ///< End-to-end search time in milliseconds
         bool aborted = false;       ///< True when the target depth was not completed
     };

     /**
      * @brief Abstract interface for chess engine.
      */
     class Engine
     {
        public:
            /**
             * @brief Construct a new Engine.
             */
            Engine() = default;

            /**
             * @brief Default destructor.
             */
            virtual ~Engine() = default;

            /**
             * @brief Get the Evaluation of the current position.
             * @param fen FEN string representing the board state.
             * @param depth Max depth to search, or engine default if negative.
             * @return Evaluation as a float in unit of pawns.
             */
            virtual float GetEvaluation(std::string& fen, int depth) = 0;

            /**
             * @brief Analyze a position under explicit search limits.
             * @param request Position and search limits.
             * @return Search result with best move and metadata.
             */
            virtual EngineSearchResult AnalyzePosition(const EngineSearchRequest& request) = 0;

            /**
             * @brief Get the best move of the engine.
             * @return Best move in standard notation (UCI) (e.g. "e2e4").
             */
            virtual std::string GetBestMove() const = 0;

            /**
             * @brief Get the version of the engine.
             * @return Engine version as a string.
             */
            virtual std::string GetEngineVersion() const = 0;

            /**
             * @brief Get the name of the engine.
             * @return Engine name as a string.
             */
            virtual std::string GetEngineName() const = 0;
     };
 }
