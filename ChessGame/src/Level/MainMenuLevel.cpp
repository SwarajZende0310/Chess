#include"Level/MainMenuLevel.h"
#include"framework/Application.h"
#include"widgets/MainMenuHUD.h"

namespace chess
{
    MainMenuLevel::MainMenuLevel(Application *owningApp)
        :Stage{owningApp}
    {
        mMainMenuHUD = SpawnHUD<MainMenuHUD>();
    }
}