#include "gameFramework/GameApplication.h"
#include "framework/AssetManager.h"
#include "framework/Stage.h"
#include "Level/MainMenuLevel.h"
#include"config.h"

namespace chess 
{
    GameApplication::GameApplication()
        : Application{1000, 1000, "Chess Game"} 
    {
        AssetManager::Get().SetRootDirectory(GetResourceDir());
        weak<Stage> newStage = Application::LoadWorld<MainMenuLevel>();
    }
} // namespace chess
