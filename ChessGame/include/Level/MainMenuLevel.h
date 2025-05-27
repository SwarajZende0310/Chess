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

        protected:

        private:
            weak<MainMenuHUD> mMainMenuHUD;
    };
}