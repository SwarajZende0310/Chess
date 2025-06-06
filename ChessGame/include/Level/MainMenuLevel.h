#pragma once

#include"framework/Stage.h"

namespace chess
{
    class Application;
    class MainMenuHUD;

    class MainMenuLevel : public Stage
    {
        public:
            MainMenuLevel(Application* owningApp);

            virtual void BeginPlay()override;

        protected:

        private:
            weak<MainMenuHUD> mMainMenuHUD;

            void StartPlayOnlineChessGame();
            void StartTwoplayerChessGame();
            void PlayBot();
            void StartAnalysisBoard();
            void GoHome();
            void EndGame();
    };
}