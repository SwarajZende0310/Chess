/**
 * @file MainMenuLevel.h
 * @brief Level for main menu UI and navigation.
 *
 * Displays the title and menu buttons to start various game modes,
 * navigate home, or quit the application.
 */
#pragma once

#include"framework/Stage.h"

namespace chess
{
    class Application;
    class MainMenuHUD;

    /**
     * @brief Level for main menu UI and navigation.
     *
     * Displays the title and menu buttons to start various game modes,
     * navigate home, or quit the application.
     */
    class MainMenuLevel : public Stage
    {
        public:
            /**
             * @brief Construct the level with its owning application.
             */
            MainMenuLevel(Application* owningApp);

            /**
             * @brief Spawn the HUD and bind all button callbacks.
             */
            virtual void BeginPlay()override;

        protected:

        private:
            /** Weak reference to main menu HUD. */
            weak<MainMenuHUD> mMainMenuHUD;

            /** Log and navigate to online play (placeholder). */
            void StartPlayOnlineChessGame();
            /** Log and navigate to two-player game (placeholder). */
            void StartTwoplayerChessGame();
            /** Log and navigate to bot game (placeholder). */
            void PlayBot();
            /** Load the analysis board level. */
            void StartAnalysisBoard();
            /** Placeholder home action. */
            void GoHome();
            /** Request application quit. */
            void EndGame();
    };
}