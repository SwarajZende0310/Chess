#include "framework/Application.h"

namespace chess {
Application::Application(unsigned int windowWidth, unsigned int windowHeight,
                         const std::string &windowTitle,
                         std::uint32_t windowStyle)
    : mWindow{sf::VideoMode({windowWidth, windowHeight}), windowTitle,
              windowStyle} {}

void Application::Run() {
  while (mWindow.isOpen()) {
    while (const std::optional event = mWindow.pollEvent()) {
      if (event->is<sf::Event::Closed>()) {
        mWindow.close();
      }
    }

    mWindow.clear();

    Render();

    mWindow.display();
  }
}

void Application::Render() { RenderInternal(); }

void Application::RenderInternal() {}
} // namespace chess
