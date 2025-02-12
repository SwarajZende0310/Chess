#include "gameFramework/GameApplication.h"
#include "framework/AssetManager.h"
#include"config.h"

namespace chess 
{
    GameApplication::GameApplication()
        : Application{800, 800, "Chess Game"} 
    {
        AssetManager::Get().SetRootDirectory(GetResourceDir());
    }
} // namespace chess
