#pragma once

#include"widgets/HUD.h"
#include"widgets/Button.h"
#include"widgets/TextWidget.h"
#include"widgets/ImageWidget.h"


namespace chess
{
    class MainMenuHUD : public HUD
    {
        public:
            MainMenuHUD();

            virtual void Draw(sf::RenderWindow & windowRef)override;
            
            virtual bool HandleEvent(const std::optional<sf::Event> &event)override;

            Delegate<> onHomeButtonClicked;
            Delegate<> onPlayOnlineButtonClicked;
            Delegate<> onTwoPlayerButtonClicked;
            Delegate<> onAnalysisButtonClicked;
            Delegate<> onPlayBotButtonClicked;
            Delegate<> onQuitButtonClicked;

        private:
            virtual void Init(const sf::RenderWindow& windowRef)override;

            TextWidget mTitleText;

            Button mHome;
            Button mPlayOnline;
            Button mTwoPlayer;
            Button mAnalysisBoard;
            Button mPlayBot;
            Button mQuit;

            ButtonColor mQuitButtonColor;

            ImageWidget mBackGroundImage;

            void HomeButtonClicked();
            void PlayOnlineButtonClicked();
            void TwoPlayerButtonClicked();
            void AnalysisBoardButtonClicked();
            void PlayBotButtonClicked();
            void QuitButtonClicked();
    };
}