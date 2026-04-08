/**
 * @file AnalysisBoardLevel.h
 * @brief Level for analysis board mode.
 *
 * Renders a free-play board with no timers and a minimal HUD containing
 * Home and Quit actions.
 */
#pragma once

#include"framework/Stage.h"
#include"engines/BacktrackingEngine.h"
#include <atomic>
#include <condition_variable>
#include <mutex>
#include <thread>

namespace chess
{
    class Application;
    class AnalysisBoardHUD;

    /**
     * @brief Level for analysis board mode.
     *
     * Hosts a board-only experience with an analysis HUD (Home/Quit).
     *
     * Analysis work is intentionally decoupled from the render/input thread.
     * The level submits FEN requests to a background worker, the worker runs
     * bounded engine analysis, and the main thread publishes only the latest
     * completed generation back to the HUD.
     */

    class AnalysisBoardLevel : public Stage
    {
        public:
            /**
             * @brief Construct the analysis level.
             * @param owningApp Pointer to the owning `Application` used to
             *                  load worlds and access the render window.
             */
            AnalysisBoardLevel(Application* owningApp);
            /**
             * @brief Stop the background analysis worker before the level dies.
             */
            virtual ~AnalysisBoardLevel() override;

            /**
             * @brief Initialize gameplay state for analysis mode.
             *
             * Spawns `AnalysisBoardHUD`, binds button delegates, and prepares
             * the scene for free-play analysis.
             */
            virtual void BeginPlay()override;

            /** @brief Render the board, pieces, and HUD. */
            virtual void Render()override;
            /** @brief Submit a fresh analysis request after board state changes. */
            virtual bool HandleEventInternal(const std::optional<sf::Event> & event)override;
            /** @brief Handle analysis-specific text commands such as `bestmove`. */
            virtual std::string HandleTextCommand(const std::string& command) override;

        protected:
            /** @brief Analysis mode supports shared board text commands. */
            virtual bool SupportsBoardTextInterface() const override;

        private:
            /**
             * @brief Last completed worker result paired with its generation id.
             */
            struct CompletedAnalysis
            {
                uint64_t generation = 0;
                EngineSearchResult result;
            };

            weak<AnalysisBoardHUD> mAnalysisBoardHUD; ///< HUD that displays the latest completed evaluation
            unique<Engine> mEngine;                   ///< Engine instance configured with constructor-owned budgets
            std::thread mAnalysisWorker;             ///< Background worker that runs bounded analysis requests
            std::mutex mAnalysisMutex;               ///< Protects queued/completed worker state
            std::condition_variable mAnalysisCv;     ///< Wakes the worker when a newer position is available
            std::atomic<bool> mStopWorker;           ///< Requests worker shutdown during level teardown
            std::atomic<bool> mCancelSearch;         ///< Cancels an in-flight search when a newer generation arrives
            uint64_t mQueuedGeneration;              ///< Latest requested position generation
            uint64_t mActiveGeneration;              ///< Generation currently being analyzed by the worker
            uint64_t mLastPublishedGeneration;       ///< Latest generation already published to the HUD
            bool mHasQueuedRequest;                  ///< True when a newer FEN is waiting for the worker
            bool mHasCompletedAnalysis;              ///< True when the worker has produced a result to poll
            std::string mQueuedFen;                  ///< Most recent requested position in FEN format
            CompletedAnalysis mCompletedAnalysis;    ///< Last completed result available to the main thread

            /** @brief Return to the main menu. */
            void GoHome();
            /** @brief Quit the application from analysis mode. */
            void EndGame();
            /** @brief Queue analysis for the current board state. */
            void RefreshEvaluationFromCurrentPosition();
            /** @brief Worker loop that resolves queued generations in the background. */
            void AnalysisWorkerLoop();

        protected:
            /** @brief Poll completed worker results and publish them on the main thread. */
            virtual void CalculateCurrentEvaluation() override;
    };
}
