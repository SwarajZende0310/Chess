#pragma once

#include <SFML/Graphics.hpp>
#include"framework/Stage.h"

namespace chess 
{
  class Application 
  {
  public:
    Application(unsigned int windowWidth, unsigned int windowHeight,const std::string &windowTitle,std::uint32_t windowStyle = sf::Style::Titlebar |sf::Style::Close);

    template<typename StageType>
    weak<StageType> LoadWorld();

    void Run();
    virtual void Render();

    sf::RenderWindow& GetWindow();
    sf::Vector2u GetWindowSize()const;

    void Tick(float deltaTime);

    void QuitApplication();

    void ReachedEndState(int state);

  private:
    bool DispathEvent(const std::optional<sf::Event>& event);
    void TickInternal(float deltaTime);

    sf::RenderWindow mWindow;

    float mTargetFrameRate;
    sf::Clock mTickClock;

    shared<Stage> mCurrentStage;
    void RenderInternal();
  };
  template <typename StageType>
  inline weak<StageType> Application::LoadWorld()
  {
      mCurrentStage = std::make_shared<StageType>(this);
      return mCurrentStage;
  }
} // namespace chess
