#pragma once

#include"widgets/HUD.h"
#include"widgets/Button.h"
#include"widgets/TextWidget.h"


namespace chess
{
    class AnalysisBoardHUD : public HUD
    {
        public:
            AnalysisBoardHUD();

            virtual void Draw(sf::RenderWindow & windowRef)override;
            
            virtual bool HandleEvent(const std::optional<sf::Event> &event)override;

            virtual void Tick(float deltaTime)override;

            void UpdateCurrentEvaluation(float eval);

            Delegate<> onHomeButtonClicked;
            Delegate<> onQuitButtonClicked;

        private:
            virtual void Init(const sf::RenderWindow& windowRef)override;

            Button mHome;
            Button mQuit;

            TextWidget mCurrEvaluation;

            ButtonColor mQuitButtonColor;

            void HomeButtonClicked();
            void QuitButtonClicked();
    };
}