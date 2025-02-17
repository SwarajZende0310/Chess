#include "gameFramework/GameApplication.h"
#include "framework/AssetManager.h"
#include "framework/Stage.h"
#include"config.h"

namespace chess 
{
    GameApplication::GameApplication()
        : Application{800, 800, "Chess Game"} 
    {
        AssetManager::Get().SetRootDirectory(GetResourceDir());
        weak<Stage> newStage = Application::LoadWorld<Stage>();
    }
} // namespace chess
