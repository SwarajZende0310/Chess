#pragma once

#include<SFML/Graphics.hpp>
#include"framework/Core.h"

namespace chess
{
  class Stage;
  class Board
  {
    public:
    Board(Stage* owningStage, const sf::Vector2f& boardStart = sf::Vector2f{0.f,0.f}, const sf::Vector2f& boardEnd = sf::Vector2f{800.f,800.f});
    
    void RefreshBoard();

    private:
    void RenderBlackSquare(const sf::Vector2f& position);
    void RenderWhiteSquare(const sf::Vector2f& position);

    const sf::Vector2f& GetSquareBound(const ChessCoordinate& chessCoordinate);

    void Calculate_Square_Offset();

    Stage* mOwingStage;
    sf::Vector2f mBoardStart;
    sf::Vector2f mBoardEnd;

    shared<sf::Texture> mWhiteTexture;
    shared<sf::Texture> mBlackTexture;
    sf::Sprite mWhiteSquaresSprite;
    sf::Sprite mBlackSquaresSprite;

    float mScaleX;
    float mScaleY;
    float mOffsetY;
    float mOffsetX;
  };
}
