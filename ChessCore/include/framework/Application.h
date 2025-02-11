#pragma once

#include "SFML/Window/WindowEnums.hpp"
#include <SFML/Graphics.hpp>
#include <string>

namespace chess {
class Application {
public:
  Application(unsigned int windowWidth, unsigned int windowHeight,
              const std::string &windowTitle,
              std::uint32_t windowStyle = sf::Style::Titlebar |
                                          sf::Style::Close);
  void Run();
  void Render();

private:
  sf::RenderWindow mWindow;
  void RenderInternal();
};
} // namespace chess
