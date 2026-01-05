/**
 * @file AnalysisBoardHUD.cpp
 * @brief HUD for analysis board: draws Home/Quit buttons and handles clicks.
 */
#include"widgets/AnalysisBoardHUD.h"

namespace chess
{
    /**
     * @brief Construct HUD and initialize button labels.
     */
    AnalysisBoardHUD::AnalysisBoardHUD()
        :mHome{"Home"},
        mQuit{"Quit"},
        mCurrEvaluation{"0.0"},
        mEvaluationBar{{50.f,800.f}}
    {
        
    }

    /**
     * @brief Draw Home and Quit buttons.
     */
    void AnalysisBoardHUD::Draw(sf::RenderWindow & windowRef)
    {
        mHome.NativeDraw(windowRef);
        mQuit.NativeDraw(windowRef);
        mCurrEvaluation.NativeDraw(windowRef);
        mEvaluationBar.NativeDraw(windowRef);
    }

    /**
     * @brief Dispatch events to buttons and return whether handled.
     */
    bool AnalysisBoardHUD::HandleEvent(const std::optional<sf::Event> &event)
    {
        return mHome.HandleEvent(event) || mQuit.HandleEvent(event) ;
    }

    /*
     *  @brief Update the HUD.
    */
    void AnalysisBoardHUD::Tick(float deltaTime)
    {
        return;
    }

    /**
     * @brief Initialize widget properties and bind button click delegates.
     */
    void AnalysisBoardHUD::Init(const sf::RenderWindow& windowRef)
    {
        mHome.SetTextSize(17);
        mHome.mOnButtonClicked.BindAction(GetWeakRef(),&AnalysisBoardHUD::HomeButtonClicked);

        mQuit.SetWidgetLocation({700.f,900.f});
        mQuit.SetTextSize(17);
        mQuitButtonColor.buttonDefaultColor = sf::Color{180,50,50,100};
        mQuitButtonColor.buttonHoverColor = sf::Color{200,50,50,255};
        mQuit.SetColor(mQuitButtonColor);
        mQuit.mOnButtonClicked.BindAction(GetWeakRef(),&AnalysisBoardHUD::QuitButtonClicked);

        mCurrEvaluation.SetTextSize(30);
        mCurrEvaluation.SetWidgetLocation({400.f,0.f});

        mEvaluationBar.SetWidgetLocation({940.f,100.f});
    }

    /**
     * @brief Broadcast Home action.
     */
    void AnalysisBoardHUD::HomeButtonClicked()
    {
        onHomeButtonClicked.Broadcast();
    }

    /**
     * @brief Broadcast Quit action.
     */
    void AnalysisBoardHUD::QuitButtonClicked()
    {
        onQuitButtonClicked.Broadcast();
    }


    /**
     * @brief Updates current evaluation when evaluation changed.
     */
    void AnalysisBoardHUD::UpdateCurrentEvaluation(float eval)
    {
        mCurrEvaluation.SetTextString(fmt::format("{:.1f}", eval));
        mEvaluationBar.UpdateCurrentEvaluation(eval);
    }
}