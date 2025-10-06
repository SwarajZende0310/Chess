/**
 * @file MainMenuLevel.cpp
 * @brief Implementation of the main menu level: HUD wiring and navigation actions.
 */
#include"Level/MainMenuLevel.h"
#include"Level/AnalysisBoardLevel.h"
#include"framework/Application.h"
#include"widgets/MainMenuHUD.h"

namespace chess
{
    /**
     * @brief Construct main menu level with its owning application.
     */
    MainMenuLevel::MainMenuLevel(Application *owningApp)
        :Stage{owningApp}
    {
        
    }

    /**
     * @brief Create the menu HUD and bind button callbacks.
     */
    void MainMenuLevel::BeginPlay()
    {
        mMainMenuHUD = SpawnHUD<MainMenuHUD>();

        mMainMenuHUD.lock()->onPlayOnlineButtonClicked.BindAction(GetWeakRef(),&MainMenuLevel::StartPlayOnlineChessGame);
        mMainMenuHUD.lock()->onTwoPlayerButtonClicked.BindAction(GetWeakRef(), &MainMenuLevel::StartTwoplayerChessGame);
        mMainMenuHUD.lock()->onPlayBotButtonClicked.BindAction(GetWeakRef(), &MainMenuLevel::PlayBot);
        mMainMenuHUD.lock()->onAnalysisButtonClicked.BindAction(GetWeakRef(), &MainMenuLevel::StartAnalysisBoard);
        mMainMenuHUD.lock()->onHomeButtonClicked.BindAction(GetWeakRef(), &MainMenuLevel::GoHome);
        mMainMenuHUD.lock()->onQuitButtonClicked.BindAction(GetWeakRef(), &MainMenuLevel::EndGame);
    }

    /**
     * @brief Placeholder: start online game flow.
     */
    void MainMenuLevel::StartPlayOnlineChessGame()
    {
        LOG("Starting Online Game Level");
    }

    /**
     * @brief Placeholder: start two-player local game flow.
     */
    void MainMenuLevel::StartTwoplayerChessGame()
    {
        LOG("Starting Two Player Game Level");
    }

    /**
     * @brief Placeholder: start bot game flow.
     */
    void MainMenuLevel::PlayBot()
    {
        LOG("Starting Bot Game Level");
    }

    /**
     * @brief Navigate to analysis board level.
     */
    void MainMenuLevel::StartAnalysisBoard()
    {
        GetApplication()->LoadWorld<AnalysisBoardLevel>();
    }

    /**
     * @brief Placeholder home action.
     */
    void MainMenuLevel::GoHome()
    {
        LOG("Going Home");
    }

    /**
     * @brief Quit the application.
     */
    void MainMenuLevel::EndGame()
    {
        GetApplication()->QuitApplication();
    }
}