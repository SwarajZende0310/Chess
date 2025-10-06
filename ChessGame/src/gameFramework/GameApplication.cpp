/**
 * @file GameApplication.cpp
 * @brief Game application bootstrap: window config, assets root, and initial level.
 */
#include "gameFramework/GameApplication.h"
#include "framework/AssetManager.h"
#include "framework/Stage.h"
#include "Level/MainMenuLevel.h"
#include"config.h"

namespace chess 
{
    /**
     * @brief Construct the game application.
     * Sets asset root, then loads the `MainMenuLevel` as the initial world.
     */
    GameApplication::GameApplication()
        : Application{1000, 1000, "Chess Game"} 
    {
        AssetManager::Get().SetRootDirectory(GetResourceDir());
        weak<Stage> newStage = Application::LoadWorld<MainMenuLevel>();
    }
} // namespace chess
