/**
 * @file Board.h
 * @brief Chess board rendering and geometry utilities
 *
 * Declares the `chess::Board` class which renders the 8x8 chessboard,
 * manages tile metrics (size, offsets, scale), and exposes helpers to
 * query tile bounds used by game logic and input handling.
 */

#pragma once

#include<SFML/Graphics.hpp>
#include"framework/Core.h"

namespace chess
{
  class Stage;
  /**
   * @brief Responsible for drawing the chessboard and providing tile metrics.
   *
   * A `Board` owns the square textures/sprites and draws an 8x8 grid starting
   * at `mBoardStart` with overall size `mBoardDimensions`. It computes square
   * offsets and sprite scales to fit the desired dimensions. Use
   * `GetSquareBound()` to query the approximate on-screen bounds of any tile.
   */
  class Board
  {
    public:
    /**
     * @brief Construct a Board.
     *
     * @param owningStage Owning stage used for window access when drawing.
     * @param boardStart Top-left corner where the board should start (pixels).
     * @param boardDimensions Total width/height the board should occupy (pixels).
     */
    Board(Stage* owningStage, const sf::Vector2f& boardStart = sf::Vector2f{0.f,0.f}, const sf::Vector2f& boardDimensions = sf::Vector2f{800.f,800.f});
    
    /**
     * @brief Redraw the entire 8x8 board using current metrics (scale/offsets).
     */
    void RefreshBoard();

    /**
     * @brief Get top-left corner of the board in window coordinates.
     */
    inline sf::Vector2f GetBoardStart()const{ return mBoardStart; }
    /**
     * @brief Get the total width/height the board occupies.
     */
    inline sf::Vector2f GetBoardDimensions()const { return mBoardDimensions; }
    /**
     * @brief Get per-tile horizontal spacing (pixels).
     */
    inline float GetSquareOffsetX(){ return mOffsetX; }
    /**
     * @brief Get per-tile vertical spacing (pixels).
     */
    inline float GetSquareOffsetY(){ return mOffsetY; }
    /**
     * @brief Get the sprite scale applied to square textures.
     */
    inline sf::Vector2f GetSpriteScale() { return {mScaleX, mScaleY};}

    /**
     * @brief Get the approximate on-screen bounds of the given chess tile.
     *
     * @param chessCoordinate Coordinate of the tile (file, rank).
     * @return sf::FloatRect Bounds in window coordinates.
     */
    sf::FloatRect GetSquareBound(const ChessCoordinate& chessCoordinate)const;
    
    private:
    /**
     * @brief Draw a dark/black square at the given position.
     */
    void RenderBlackSquare(const sf::Vector2f& position);
    /**
     * @brief Draw a light/white square at the given position.
     */
    void RenderWhiteSquare(const sf::Vector2f& position);

    /**
     * @brief Compute per-tile offsets and sprite scale from board dimensions.
     */
    void CalculateSquareOffset();

    Stage* mOwingStage;          ///< Back-reference to owning stage (for drawing)
    sf::Vector2f mBoardStart;    ///< Top-left corner of the board in pixels
    sf::Vector2f mBoardDimensions; ///< Desired total board size (width, height)

    shared<sf::Texture> mWhiteTexture; ///< Texture for light squares
    shared<sf::Texture> mBlackTexture; ///< Texture for dark squares
    sf::Sprite mWhiteSquaresSprite;    ///< Sprite used to draw light squares
    sf::Sprite mBlackSquaresSprite;    ///< Sprite used to draw dark squares

    float mScaleX;               ///< Horizontal scale for square sprites
    float mScaleY;               ///< Vertical scale for square sprites
    float mOffsetY;              ///< Per-tile vertical spacing
    float mOffsetX;              ///< Per-tile horizontal spacing
  };
}
