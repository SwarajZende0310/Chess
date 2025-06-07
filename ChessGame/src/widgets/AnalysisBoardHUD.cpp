#include"widgets/AnalysisBoardHUD.h"

namespace chess
{
    AnalysisBoardHUD::AnalysisBoardHUD()
        :mHome{"Home"},
        mQuit{"Quit"}
    {
        
    }

    void AnalysisBoardHUD::Draw(sf::RenderWindow & windowRef)
    {
        mHome.NativeDraw(windowRef);
        mQuit.NativeDraw(windowRef);
    }

    bool AnalysisBoardHUD::HandleEvent(const std::optional<sf::Event> &event)
    {
        return mHome.HandleEvent(event) || mQuit.HandleEvent(event) ;
    }

    void AnalysisBoardHUD::Init(const sf::RenderWindow& windowRef)
    {
        mHome.SetTextSize(17);
        mHome.mOnButtonClicked.BindAction(GetWeakRef(),&AnalysisBoardHUD::HomeButtonClicked);

        mQuit.SetWidgetLocation({700.f,900.f});
        mQuit.SetTextSize(17);
        mQuit.mOnButtonClicked.BindAction(GetWeakRef(),&AnalysisBoardHUD::QuitButtonClicked);
    }

    void AnalysisBoardHUD::HomeButtonClicked()
    {
        onHomeButtonClicked.Broadcast();
    }

    void AnalysisBoardHUD::QuitButtonClicked()
    {
        onQuitButtonClicked.Broadcast();
    }
}