#include "framework/Application.h"

namespace chess {
  Application::Application(unsigned int windowWidth, unsigned int windowHeight,const std::string &windowTitle,std::uint32_t windowStyle)
      : mWindow{sf::VideoMode({windowWidth, windowHeight}), windowTitle,windowStyle},
      mTargetFrameRate{120.f},
      mTickClock{},
      mCurrentStage{}
      {

      }

  void Application::Run() 
  {
    mTickClock.restart();
    float accumalatedTime = 0.f;
    float targetDeltaTime = 1.f / mTargetFrameRate;

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

      float frameDeltaTime = mTickClock.restart().asSeconds();
      accumalatedTime += frameDeltaTime;
      
      while(accumalatedTime > targetDeltaTime)//Rendering only to achieve det frame rate
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

  void Application::Render()
  { 
    mWindow.clear();
    RenderInternal();
    mWindow.display();
  }

  sf::RenderWindow& Application::GetWindow()
  {
    return mWindow;
  }

  sf::Vector2u Application::GetWindowSize()const
  {
    return mWindow.getSize();
  }

  void Application::Tick(float deltaTime)
  {

  }

  void Application::QuitApplication()
  {
    mWindow.close();
  }

  void Application::ReachedEndState(int state)
  {
    QuitApplication();
  }
  bool Application::DispathEvent(const std::optional<sf::Event> &event)
  {
      if(mCurrentStage)
        return mCurrentStage->HandleEvent(event);
      return false;
  }

  void Application::TickInternal(float deltaTime)
  {
    if(mCurrentStage)
    {
      mCurrentStage->TickInternal(deltaTime);
    }
  }
  void Application::RenderInternal()
  {
    if(mCurrentStage)
      mCurrentStage->Render();
  }
} // namespace chess
