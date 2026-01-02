/**
 * @file AnalysisBoardLevel.cpp
 * @brief Implementation of the analysis board level: HUD wiring, rendering, and input forwarding.
 */
#include"Level/AnalysisBoardLevel.h"
#include"Level/MainMenuLevel.h"
#include"framework/Application.h"
#include"widgets/AnalysisBoardHUD.h"

namespace chess
{
    /**
     * @brief Construct the analysis level with the owning application context.
     */
    AnalysisBoardLevel::AnalysisBoardLevel(Application *owningApp)
        :Stage{owningApp}
    {
        
    }

    /**
     * @brief Spawn the analysis HUD and bind its button delegates.
     */
    void AnalysisBoardLevel::BeginPlay()
    {
        mAnalysisBoardHUD = SpawnHUD<AnalysisBoardHUD>();

        mAnalysisBoardHUD.lock()->onHomeButtonClicked.BindAction(GetWeakRef(), &AnalysisBoardLevel::GoHome);
        mAnalysisBoardHUD.lock()->onQuitButtonClicked.BindAction(GetWeakRef(), &AnalysisBoardLevel::EndGame);
        mOnEvaluationUpdate.BindAction(mAnalysisBoardHUD, &AnalysisBoardHUD::UpdateCurrentEvaluation);
    }

    /**
     * @brief Render the board, pieces, and HUD each frame.
     */
    void AnalysisBoardLevel::Render()
    {
        RenderBoard();
        RenderPieces();
        RenderHUD(GetApplication()->GetWindow());
    }

    /**
     * @brief Forward events to the board handler for piece interactions.
     * @return true if the event was consumed by the board logic.
     */
    bool AnalysisBoardLevel::HandleEventInternal(const std::optional<sf::Event> & event)
    {
        return HandleBoardEvent(event);
    }

    /**
     * @brief Navigate back to the main menu level.
     */
    void AnalysisBoardLevel::GoHome()
    {
        GetApplication()->LoadWorld<MainMenuLevel>();
    }

    /**
     * @brief Quit the application from the analysis HUD.
     */
    void AnalysisBoardLevel::EndGame()
    {
        GetApplication()->QuitApplication();
    }
}