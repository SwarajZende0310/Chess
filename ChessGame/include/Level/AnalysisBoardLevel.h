/**
 * @file AnalysisBoardLevel.h
 * @brief Level for analysis board mode.
 *
 * Renders a free-play board with no timers and a minimal HUD containing
 * Home and Quit actions.
 */
#pragma once

#include"framework/Stage.h"

namespace chess
{
    class Application;
    class AnalysisBoardHUD;

    /**
     * @brief Level for analysis board mode.
     *
     * Hosts a board-only experience with an analysis HUD (Home/Quit).
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
             * @brief Initialize gameplay state for analysis mode.
             *
             * Spawns `AnalysisBoardHUD`, binds button delegates, and prepares
             * the scene for free-play analysis.
             */
            virtual void BeginPlay()override;

            virtual void Render()override;
            virtual bool HandleEventInternal(const std::optional<sf::Event> & event)override;

        protected:

        private:
            weak<AnalysisBoardHUD> mAnalysisBoardHUD;

            void GoHome();
            void EndGame();
    };
}