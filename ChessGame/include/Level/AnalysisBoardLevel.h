#pragma once

#include"framework/Stage.h"

namespace chess
{
    class Application;
    class AnalysisBoardHUD;

    class AnalysisBoardLevel : public Stage
    {
        public:
            AnalysisBoardLevel(Application* owningApp);

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