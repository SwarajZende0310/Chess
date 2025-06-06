#include"Level/MainMenuLevel.h"
#include"framework/Application.h"
#include"widgets/MainMenuHUD.h"

namespace chess
{
    MainMenuLevel::MainMenuLevel(Application *owningApp)
        :Stage{owningApp}
    {
        
    }

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

    void MainMenuLevel::StartPlayOnlineChessGame()
    {
        LOG("Starting Online Game Level");
    }

    void MainMenuLevel::StartTwoplayerChessGame()
    {
        LOG("Starting Two Player Game Level");
    }

    void MainMenuLevel::PlayBot()
    {
        LOG("Starting Bot Game Level");
    }

    void MainMenuLevel::StartAnalysisBoard()
    {
        LOG("Starting Analysis Board Game Level");
    }

    void MainMenuLevel::GoHome()
    {
        LOG("Going Home");
    }

    void MainMenuLevel::EndGame()
    {
        LOG("Quitting Appplication");
    }
}