#include "framework/Application.h"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/System/Vector2.hpp"

namespace chess {
  Application::Application(unsigned int windowWidth, unsigned int windowHeight,const std::string &windowTitle,std::uint32_t windowStyle)
      : mWindow{sf::VideoMode({windowWidth, windowHeight}), windowTitle,windowStyle},
      mCurrentStage{}
      {

      }

  void Application::Run() 
  {
    while (mWindow.isOpen()) 
    {
      while (const std::optional event = mWindow.pollEvent()) 
      {
        if (event->is<sf::Event::Closed>()) 
        {
          mWindow.close();
        }
      }

      mWindow.clear();

      Render();

      mWindow.display();
    }
  }

  void Application::Render()
  { 
    RenderInternal();
  }

  sf::RenderWindow& Application::GetWindow()
  {
    return mWindow;
  }

  sf::Vector2u Application::GetWindowSize()const
  {
    return mWindow.getSize();
  }

  void Application::RenderInternal() 
  {
    if(mCurrentStage)
      mCurrentStage->Render();
  }
} // namespace chess
