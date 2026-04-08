/**
 * @file GameApplication.h
 * @brief Game application bootstrap: window config, assets root, and initial level.
 */
#pragma once

#include"framework/Application.h"


namespace chess
{
    /**
     * @brief Top-level game application that creates the window and loads the initial level.
     */
    class GameApplication : public Application
    {
        public:
            /**
             * @brief Construct the application and set up resources/initial world.
             */
            GameApplication();

            /**
             * @brief Handle world-switching commands for the concrete game app.
             */
            virtual bool HandleApplicationTextCommand(const std::string& command, std::string& response) override;
    };
}
