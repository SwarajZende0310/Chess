/**
 * @file Application.h
 * @brief Main application class for the Chess game framework
 * 
 * This file contains the declaration of the Application class which is inherited by GameApplication and serves as the
 * main entry point and core of the game application, managing the game loop,
 * rendering, and stage management.
 */

#pragma once

#include <SFML/Graphics.hpp>
#include"framework/Stage.h"

namespace chess 
{
  /**
   * @brief Main application class that manages the game loop and window
   * 
   * The Application class is responsible for initializing the game window,
   * managing the game loop, handling events, and coordinating between
   * different game stages/levels.
   */
  class Application 
  {
  public:
    /**
     * @brief Construct a new Application
     * 
     * @param windowWidth The width of the application window
     * @param windowHeight The height of the application window
     * @param windowTitle The title of the application window
     * @param windowStyle The style of the window (default: Titlebar | Close)
     */
    Application(unsigned int windowWidth, unsigned int windowHeight, 
               const std::string &windowTitle, 
               std::uint32_t windowStyle = sf::Style::Titlebar | sf::Style::Close);

    /**
     * @brief Loads a new stage/world into the application
     * 
     * @tparam StageType The type of stage to load (must inherit from Stage)
     * @return weak<StageType> A weak pointer to the loaded stage
     */
    template<typename StageType>
    weak<StageType> LoadWorld();

    /**
     * @brief Starts the main game loop
     */
    void Run();

    /**
     * @brief Renders the current frame by calling Render for CurrentStage
     */
    virtual void Render();

    /**
     * @brief Gets the SFML render window
     * 
     * @return sf::RenderWindow& Reference to the render window
     */
    sf::RenderWindow& GetWindow();

    /**
     * @brief Gets the current window size
     * 
     * @return sf::Vector2u The size of the window as a 2D vector
     */
    sf::Vector2u GetWindowSize() const;

    /**
     * @brief Updates the game state
     * 
     * @param deltaTime Time elapsed since the last frame in seconds
     */
    void Tick(float deltaTime);

    /**
     * @brief Signals the application to quit
     */
    void QuitApplication();

    /**
     * @brief Called when an end state(WhiteWon,BlackWon,DRAW) is reached 
     * 
     * @param state The end state
     */
    void ReachedEndState(GameState state);

  private:
    /**
     * @brief Dispatches an SFML event to the current stage
     * 
     * @param event The SFML event to dispatch
     * @return true if the event was handled, false otherwise
     */
    bool DispathEvent(const std::optional<sf::Event>& event);
    
    /**
     * @brief Internal tick function that handles frame timing
     * 
     * @param deltaTime Time elapsed since the last frame in seconds
     */
    void TickInternal(float deltaTime);

    /**
     * @brief Internal render function that handles the rendering pipeline
     */
    void RenderInternal();

    sf::RenderWindow mWindow;     ///< The main render window

    float mTargetFrameRate;       ///< The target frames per second
    sf::Clock mTickClock;         ///< Clock used for frame timing
    
    shared<Stage> mCurrentStage;  ///< The currently active stage
  };

  /**
   * @brief Loads a new stage/world into the application
   * 
   * @tparam StageType The type of stage to load (must inherit from Stage)
   * @return weak<StageType> A weak pointer to the loaded stage
   */
  template <typename StageType>
  inline weak<StageType> Application::LoadWorld()
  {
      mCurrentStage = std::make_shared<StageType>(this);
      return std::static_pointer_cast<StageType>(mCurrentStage);
  }
} // namespace chess
