#include "framework/Application.h"

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
          QuitApplication();
        }
        else
        {
          DispathEvent(event);
        }
      }

      // Render only if piece is moved
      if(mCurrentStage && mCurrentStage->IsPieceMoved())
      {
        mWindow.clear();

        Render();
        mCurrentStage->SetPieceMoved(false);

        mWindow.display();
      }
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

  void Application::QuitApplication()
  {
    mWindow.close();
  }

  bool Application::DispathEvent(const std::optional<sf::Event> &event)
  {
      if(mCurrentStage)
        return mCurrentStage->HandleEvent(event);
      return false;
  }

  void Application::RenderInternal()
  {
    if(mCurrentStage)
      mCurrentStage->Render();
  }
} // namespace chess
