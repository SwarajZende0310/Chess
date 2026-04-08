/**
 * @file GameApplication.cpp
 * @brief Game application bootstrap: window config, assets root, and initial level.
 */
#include "gameFramework/GameApplication.h"
#include "framework/AssetManager.h"
#include "framework/Stage.h"
#include "Level/AnalysisBoardLevel.h"
#include "Level/MainMenuLevel.h"
#include"config.h"

#include <sstream>

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

    bool GameApplication::HandleApplicationTextCommand(const std::string& command, std::string& response)
    {
        std::istringstream commandStream(command);
        std::string verb;
        commandStream >> verb;

        if(verb != "world")
        {
            return false;
        }

        std::string worldName;
        commandStream >> worldName;

        if(worldName == "analysis")
        {
            LoadWorld<AnalysisBoardLevel>();
            response = "ok world analysis";
            return true;
        }

        if(worldName == "main_menu")
        {
            LoadWorld<MainMenuLevel>();
            response = "ok world main_menu";
            return true;
        }

        response = "error unknown_world";
        return true;
    }
} // namespace chess
