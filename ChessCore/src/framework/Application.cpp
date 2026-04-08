/**
 * @file Application.cpp
 * @brief Implementation of the Application class
 * 
 * This file contains the implementation of the Application class which serves as the
 * main entry point and core of the game application, managing the game loop,
 * rendering, and stage management.
 */

#include "framework/Application.h"

#include <iostream>
#include <sstream>
#include <thread>

namespace chess {
  /**
   * @brief Construct a new Application::Application object
   * 
   * @param windowWidth The width of the application window
   * @param windowHeight The height of the application window
   * @param windowTitle The title of the application window
   * @param windowStyle The style of the window (default: Titlebar | Close)
   */
  Application::Application(unsigned int windowWidth, unsigned int windowHeight,
                         const std::string &windowTitle, std::uint32_t windowStyle)
      : mWindow{sf::VideoMode({windowWidth, windowHeight}), windowTitle, windowStyle},
        mTargetFrameRate{120.f},
        mTickClock{},
        mCurrentStage{},
        mCommandInputState{std::make_shared<CommandInputState>()}
  {
    const std::shared_ptr<CommandInputState> commandInputState = mCommandInputState;
    std::thread([commandInputState]()
    {
      std::string command;
      while(!commandInputState->stopRequested.load() && std::getline(std::cin, command))
      {
        if(command.empty())
        {
          continue;
        }

        std::lock_guard<std::mutex> lock(commandInputState->mutex);
        commandInputState->pendingCommands.push(command);
      }
    }).detach();
  }

  Application::~Application()
  {
    if(mCommandInputState)
    {
      mCommandInputState->stopRequested.store(true);
    }
  }

  /**
   * @brief Main game loop
   * 
   * This method contains the main game loop which handles:
   * - Event processing
   * - Fixed time step updates
   * - Rendering
   * - Stage initialization
   */
  void Application::Run() 
  {
    mTickClock.restart();
    float accumalatedTime = 0.f;
    float targetDeltaTime = 1.f / mTargetFrameRate;

    while (mWindow.isOpen()) 
    {
      // Process all pending events
      while (const std::optional event = mWindow.pollEvent()) 
      {
        if (event->is<sf::Event::Closed>()) 
        {
          QuitApplication();
        }
        else
        {
          DispathEvent(event);
        }
      }

      float frameDeltaTime = mTickClock.restart().asSeconds();
      accumalatedTime += frameDeltaTime;
      
      // Fixed time step update loop
      while(accumalatedTime > targetDeltaTime)
      {
          accumalatedTime -= targetDeltaTime;
          TickInternal(targetDeltaTime);
          Render();
      }

      if(mCurrentStage)
      {
        mCurrentStage->BeginPlayInternal();
      }

      // Render only if piece is moved
      // if(mCurrentStage && mCurrentStage->IsPieceMoved())
      // {
      //   mWindow.clear();

      //   Render();
      //   mCurrentStage->SetPieceMoved(false);

      //   mWindow.display();
      // }
    }
  }

  /**
   * @brief Renders the current frame
   * 
   * This method clears the window, renders the current frame using RenderInternal(),
   * and displays the rendered frame.
   */
  void Application::Render()
  { 
    mWindow.clear();
    RenderInternal();
    mWindow.display();
  }

  /**
   * @brief Gets a reference to the render window
   * 
   * @return sf::RenderWindow& Reference to the SFML render window
   */
  sf::RenderWindow& Application::GetWindow()
  {
    return mWindow;
  }

  /**
   * @brief Gets the current window size
   * 
   * @return sf::Vector2u The size of the window as a 2D vector
   */
  sf::Vector2u Application::GetWindowSize()const
  {
    return mWindow.getSize();
  }

  /**
   * @brief Updates the game state
   * 
   * @param deltaTime Time elapsed since the last frame in seconds
   * 
   * @note This is a virtual method that can be overridden by derived classes
   * to implement custom update logic.
   */
  void Application::Tick(float deltaTime)
  {
    // Base implementation does nothing
    // Can be overridden by derived classes
  }

  /**
   * @brief Closes the application window
   * 
   * This method closes the main application window, which will cause
   * the main game loop to exit.
   */
  void Application::QuitApplication()
  {
    mWindow.close();
  }

  /**
   * @brief Handles the end state of the game
   * 
   * @param state The end state code (WhiteWon, BlackWon, Draw, etc.)
   * 
   * @note This method is called when the game reaches an end state.
   * Currently, it's a placeholder for end-game logic.
   */
  void Application::ReachedEndState(GameState state)
  {
    // TODO:: Implement what will happen after the end state is reached
    // QuitApplication();
  }
  /**
   * @brief Dispatches an event to the current stage
   * 
   * @param event The SFML event to dispatch
   * @return true if the event was handled, false otherwise
   */
  bool Application::DispathEvent(const std::optional<sf::Event> &event)
  {
      if(mCurrentStage)
        return mCurrentStage->HandleEvent(event);
      return false;
  }

  /**
   * @brief Internal method to update the current stage
   * 
   * @param deltaTime Time elapsed since the last frame in seconds
   */
  void Application::TickInternal(float deltaTime)
  {
    ProcessPendingTextCommands();
    if(mCurrentStage)
    {
      mCurrentStage->TickInternal(deltaTime);
    }
  }
  /**
   * @brief Internal method to render the current stage
   * 
   * This method is called by Render() to delegate rendering to the current stage.
   */
  void Application::RenderInternal()
  {
    if(mCurrentStage)
      mCurrentStage->Render();
  }

  void Application::ProcessPendingTextCommands()
  {
    if(!mCommandInputState)
    {
      return;
    }

    std::queue<std::string> pendingCommands;
    {
      std::lock_guard<std::mutex> lock(mCommandInputState->mutex);
      pendingCommands.swap(mCommandInputState->pendingCommands);
    }

    while(!pendingCommands.empty())
    {
      ExecuteTextCommand(pendingCommands.front());
      pendingCommands.pop();
    }
  }

  void Application::ExecuteTextCommand(const std::string& command)
  {
    std::istringstream commandStream(command);
    std::string verb;
    commandStream >> verb;

    if(verb.empty())
    {
      return;
    }

    if(verb == "help")
    {
      PrintTextCommandHelp();
      return;
    }

    if(verb == "quit")
    {
      std::cout << "ok quit" << std::endl;
      QuitApplication();
      return;
    }

    std::string response;
    if(HandleApplicationTextCommand(command, response))
    {
      std::cout << response << std::endl;
      return;
    }

    if(!mCurrentStage)
    {
      std::cout << "error no_active_stage" << std::endl;
      return;
    }

    std::cout << mCurrentStage->HandleTextCommand(command) << std::endl;
  }

  void Application::PrintTextCommandHelp() const
  {
    std::cout << "ok help commands: help, quit, world main_menu, world analysis, move <uci>, undo, fen, eval, bestmove, turn" << std::endl;
  }

  bool Application::HandleApplicationTextCommand(const std::string& command, std::string& response)
  {
    (void)command;
    (void)response;
    return false;
  }
} // namespace chess
