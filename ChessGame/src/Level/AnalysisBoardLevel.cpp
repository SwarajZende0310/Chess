#include"Level/AnalysisBoardLevel.h"
#include"Level/MainMenuLevel.h"
#include"framework/Application.h"
#include"widgets/AnalysisBoardHUD.h"

namespace chess
{
    AnalysisBoardLevel::AnalysisBoardLevel(Application *owningApp)
        :Stage{owningApp}
    {
        
    }

    void AnalysisBoardLevel::BeginPlay()
    {
        mAnalysisBoardHUD = SpawnHUD<AnalysisBoardHUD>();

        mAnalysisBoardHUD.lock()->onHomeButtonClicked.BindAction(GetWeakRef(), &AnalysisBoardLevel::GoHome);
        mAnalysisBoardHUD.lock()->onQuitButtonClicked.BindAction(GetWeakRef(), &AnalysisBoardLevel::EndGame);
    }

    void AnalysisBoardLevel::Render()
    {
        RenderBoard();
        RenderPieces();
        RenderHUD(GetApplication()->GetWindow());
    }

    bool AnalysisBoardLevel::HandleEventInternal(const std::optional<sf::Event> & event)
    {
        return HandleBoardEvent(event);
    }

    void AnalysisBoardLevel::GoHome()
    {
        GetApplication()->LoadWorld<MainMenuLevel>();
    }

    void AnalysisBoardLevel::EndGame()
    {
        GetApplication()->QuitApplication();
    }
}