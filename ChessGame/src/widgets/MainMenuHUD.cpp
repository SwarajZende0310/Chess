/**
 * @file MainMenuHUD.cpp
 * @brief HUD for main menu: draws title and buttons, dispatches clicks.
 */
#include"widgets/MainMenuHUD.h"

namespace chess
{
    /**
     * @brief Construct HUD and initialize title and button labels.
     */
    MainMenuHUD::MainMenuHUD()
        :mTitleText{"Chess Game"},
        mHome{"Home"},
        mPlayOnline{"Play Online"},
        mTwoPlayer{"Two Player"},
        mAnalysisBoard{"Analysis Board"},
        mPlayBot{"Play Bot"},
        mQuit{"Quit"},
        mBackGroundImage{"UI/chess_background.png"}
    {
        
    }

    /**
     * @brief Draw the title and all menu buttons.
     */
    void MainMenuHUD::Draw(sf::RenderWindow & windowRef)
    {
        mBackGroundImage.NativeDraw(windowRef);
        mTitleText.NativeDraw(windowRef);
        mHome.NativeDraw(windowRef);
        mPlayOnline.NativeDraw(windowRef);
        mTwoPlayer.NativeDraw(windowRef);
        mAnalysisBoard.NativeDraw(windowRef);
        mPlayBot.NativeDraw(windowRef);
        mQuit.NativeDraw(windowRef);
    }

    /**
     * @brief Dispatch events to all buttons, reporting whether any handled them.
     */
    bool MainMenuHUD::HandleEvent(const std::optional<sf::Event> &event)
    {
        return mHome.HandleEvent(event) 
            || mPlayOnline.HandleEvent(event) 
            || mTwoPlayer.HandleEvent(event) 
            || mAnalysisBoard.HandleEvent(event) 
            || mPlayBot.HandleEvent(event) 
            || mQuit.HandleEvent(event) ;
    }

    /*
     *  @brief Update the HUD.
    */
    void MainMenuHUD::Tick(float deltaTime)
    {
        return;
    }

    /**
     * @brief Initialize positions, sizes, and bind button click delegates.
     */
    void MainMenuHUD::Init(const sf::RenderWindow& windowRef)
    {
        mBackGroundImage.SetWidgetLocation({-480.f,0.f});

        mTitleText.SetWidgetLocation({320.f,100.f});
        mTitleText.SetTextSize(40);

        mHome.SetWidgetLocation({80.f,300.f});
        mHome.SetTextSize(17);
        mHome.mOnButtonClicked.BindAction(GetWeakRef(),&MainMenuHUD::HomeButtonClicked);

        mPlayOnline.SetWidgetLocation({80.f,450.f});
        mPlayOnline.SetTextSize(17);
        mPlayOnline.mOnButtonClicked.BindAction(GetWeakRef(),&MainMenuHUD::PlayOnlineButtonClicked);

        mTwoPlayer.SetWidgetLocation({80.f,600.f});
        mTwoPlayer.SetTextSize(17);
        mTwoPlayer.mOnButtonClicked.BindAction(GetWeakRef(),&MainMenuHUD::TwoPlayerButtonClicked);

        mAnalysisBoard.SetWidgetLocation({700.f,300.f});
        mAnalysisBoard.SetTextSize(17);
        mAnalysisBoard.mOnButtonClicked.BindAction(GetWeakRef(),&MainMenuHUD::AnalysisBoardButtonClicked);

        mPlayBot.SetWidgetLocation({700.f,450.f});
        mPlayBot.SetTextSize(17);
        mPlayBot.mOnButtonClicked.BindAction(GetWeakRef(),&MainMenuHUD::PlayBotButtonClicked);

        mQuit.SetWidgetLocation({700.f,600.f});
        mQuit.SetTextSize(17);
        mQuitButtonColor.buttonDefaultColor = sf::Color{180,50,50,100};
        mQuitButtonColor.buttonHoverColor = sf::Color{200,50,50,255};
        mQuit.SetColor(mQuitButtonColor);
        mQuit.mOnButtonClicked.BindAction(GetWeakRef(),&MainMenuHUD::QuitButtonClicked);
    }

    /**
     * @brief Broadcast Home action.
     */
    void MainMenuHUD::HomeButtonClicked()
    {
        onHomeButtonClicked.Broadcast();
    }

    /**
     * @brief Broadcast Play Online action.
     */
    void MainMenuHUD::PlayOnlineButtonClicked()
    {
        onPlayOnlineButtonClicked.Broadcast();
    }

    /**
     * @brief Broadcast Two Player action.
     */
    void MainMenuHUD::TwoPlayerButtonClicked()
    {
        onTwoPlayerButtonClicked.Broadcast();
    }

    /**
     * @brief Broadcast Analysis Board action.
     */
    void MainMenuHUD::AnalysisBoardButtonClicked()
    {
        onAnalysisButtonClicked.Broadcast();
    }

    /**
     * @brief Broadcast Play Bot action.
     */
    void MainMenuHUD::PlayBotButtonClicked()
    {
        onPlayBotButtonClicked.Broadcast();
    }

    /**
     * @brief Broadcast Quit action.
     */
    void MainMenuHUD::QuitButtonClicked()
    {
        onQuitButtonClicked.Broadcast();
    }
}