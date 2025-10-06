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
        mQuit{"Quit"}
    {
        
    }

    /**
     * @brief Draw Home and Quit buttons.
     */
    void AnalysisBoardHUD::Draw(sf::RenderWindow & windowRef)
    {
        mHome.NativeDraw(windowRef);
        mQuit.NativeDraw(windowRef);
    }

    /**
     * @brief Dispatch events to buttons and return whether handled.
     */
    bool AnalysisBoardHUD::HandleEvent(const std::optional<sf::Event> &event)
    {
        return mHome.HandleEvent(event) || mQuit.HandleEvent(event) ;
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
        mQuit.mOnButtonClicked.BindAction(GetWeakRef(),&AnalysisBoardHUD::QuitButtonClicked);
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
}