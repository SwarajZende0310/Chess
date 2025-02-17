#pragma once

#include <SFML/Graphics.hpp>
#include <string>
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

  private:
    sf::RenderWindow mWindow;
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
