#pragma once

#include<SFML/Graphics.hpp>
#include"framework/Core.h"

namespace chess
{
  class Stage;
  class Board
  {
    public:
    Board(Stage* owningStage, const sf::Vector2f& boardStart = sf::Vector2f{0.f,0.f}, const sf::Vector2f& boardDimensions = sf::Vector2f{800.f,800.f});
    
    void RefreshBoard();

    inline sf::Vector2f GetBoardStart()const{ return mBoardStart; }
    inline sf::Vector2f GetBoardDimensions()const { return mBoardDimensions; }
    inline float GetSquareOffsetX(){ return mOffsetX; }
    inline float GetSquareOffsetY(){ return mOffsetY; }
    inline sf::Vector2f GetSpriteScale() { return {mScaleX, mScaleY};}

    const sf::Vector2f& GetSquareBound(const ChessCoordinate& chessCoordinate);
    
    private:
    void RenderBlackSquare(const sf::Vector2f& position);
    void RenderWhiteSquare(const sf::Vector2f& position);

    void Calculate_Square_Offset();

    Stage* mOwingStage;
    sf::Vector2f mBoardStart;
    sf::Vector2f mBoardDimensions;

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
