#include"widgets/MainMenuHUD.h"

namespace chess
{
    MainMenuHUD::MainMenuHUD()
        :mTitleText{"Chess Game"},
        mHome{"Home"},
        mPlayOnline{"Play Online"},
        mTwoPlayer{"Two Player"},
        mAnalysisBoard{"Analysis Board"},
        mPlayBot{"Play Bot"},
        mQuit{"Quit"}
    {
        mTitleText.SetWidgetLocation({320.f,100.f});
        mTitleText.SetTextSize(40);

        mHome.SetTextSize(17);
        mHome.mOnButtonClicked.BindAction(GetWeakRef(),&MainMenuHUD::HomeButtonClicked);

        mPlayOnline.SetWidgetLocation({360.f,300.f});
        mPlayOnline.SetTextSize(17);
        mPlayOnline.mOnButtonClicked.BindAction(GetWeakRef(),&MainMenuHUD::PlayOnlineButtonClicked);

        mTwoPlayer.SetWidgetLocation({360.f,400.f});
        mTwoPlayer.SetTextSize(17);
        mTwoPlayer.mOnButtonClicked.BindAction(GetWeakRef(),&MainMenuHUD::TwoPlayerButtonClicked);

        mAnalysisBoard.SetWidgetLocation({360.f,500.f});
        mAnalysisBoard.SetTextSize(17);
        mAnalysisBoard.mOnButtonClicked.BindAction(GetWeakRef(),&MainMenuHUD::AnalysisBoardButtonClicked);

        mPlayBot.SetWidgetLocation({360.f,600.f});
        mPlayBot.SetTextSize(17);
        mPlayBot.mOnButtonClicked.BindAction(GetWeakRef(),&MainMenuHUD::PlayBotButtonClicked);

        mQuit.SetWidgetLocation({700.f,900.f});
        mQuit.SetTextSize(17);
        mQuit.mOnButtonClicked.BindAction(GetWeakRef(),&MainMenuHUD::QuitButtonClicked);
    }

    void MainMenuHUD::Draw(sf::RenderWindow & windowRef)
    {
        mTitleText.NativeDraw(windowRef);
        mHome.NativeDraw(windowRef);
        mPlayOnline.NativeDraw(windowRef);
        mTwoPlayer.NativeDraw(windowRef);
        mAnalysisBoard.NativeDraw(windowRef);
        mPlayBot.NativeDraw(windowRef);
        mQuit.NativeDraw(windowRef);
    }

    bool MainMenuHUD::HandleEvent(const std::optional<sf::Event> &event)
    {
        return mHome.HandleEvent(event) 
            || mPlayOnline.HandleEvent(event) 
            || mTwoPlayer.HandleEvent(event) 
            || mAnalysisBoard.HandleEvent(event) 
            || mPlayBot.HandleEvent(event) 
            || mQuit.HandleEvent(event) ;
    }

    void MainMenuHUD::HomeButtonClicked()
    {
        LOG("Home Button Clicked");
    }

    void MainMenuHUD::PlayOnlineButtonClicked()
    {
        LOG("PlayOnline Button Clicked");
    }

    void MainMenuHUD::TwoPlayerButtonClicked()
    {
        LOG("TwoPlayer Button Clicked");
    }

    void MainMenuHUD::AnalysisBoardButtonClicked()
    {
        LOG("AnalysisBoard Button Clicked");
    }

    void MainMenuHUD::PlayBotButtonClicked()
    {
        LOG("PlayBot Button Clicked");
    }

    void MainMenuHUD::QuitButtonClicked()
    {
        LOG("Quit Button Clicked");
    }
}