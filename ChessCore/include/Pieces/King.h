#pragma once

/**
 * @file King.h
 * @brief King piece interface and API.
 */

#include"framework/Piece.h"

namespace chess
{
  /**
   * @brief King chess piece: one-square movement, check state, rendering.
   */
  class King : public Piece
  {
    public:
      /**
       * @brief Construct a King.
       * @param owningStage Stage used for rendering and scaling.
       * @param whitePiece True for white king, false for black king.
       */
      King(Stage* owningStage, bool whitePiece);

      /**
       * @brief Validate king's move (one square in any direction, not into check).
       * @param startCoordinate Start square.
       * @param endCoordinate destination square.
       * @return true if legal per king rules and not moving into attacked square.
       */
      virtual bool MovePossible(ChessCoordinate& startCoordinate,ChessCoordinate& endCoordinate)override;
      /**
       * @brief Execute king move and update `ChessState`.
       */
      virtual void MakeMove(ChessCoordinate& startCoordinate,ChessCoordinate& endCoordinate)override;
      /**
       * @brief Render the king sprite for the owning side.
       */
      virtual void RenderPiece()override;

      /** @brief True if this is a white king, false if black. */
      inline virtual bool GetPieceColor()override{ return mWhitePieces; }
      
      /** @brief Set sprite location. */
      virtual void SetPieceLocation(const sf::Vector2f& newLocation, bool whitePieces) override;
      /** @brief Set sprite rotation (typically unused). */
      virtual void SetPieceRotation(float newRotation, bool whitePieces) override;

      /**
       * @brief Gives List of all possible moves for the king.
       */
      virtual List<ChessCoordinate> GetAllPossibleMoves(const ChessCoordinate pieceCoordinate)override;

      /** @brief Whether the king is currently in check. */
      bool IsInCheck();
    private:
      /** @brief Current on-screen location. */
      virtual sf::Vector2f GetPieceLocation()const override;
      /** @brief Current sprite rotation in degrees. */
      virtual float GetPieceRotation()const override;

      /** @brief Center sprite origin for simpler transforms. */
      virtual void CenterPivot() override;

      /** @brief Whether target square contains an enemy piece. */
      bool isEnemy(ChessCoordinate& endCoordinate);

      Stage* mOwningStage; ///< Owning stage for rendering context
      
      shared<sf::Texture> mWhiteKingTexture; ///< Texture for white king
      shared<sf::Texture> mBlackKingTexture; ///< Texture for black king
      
      sf::Sprite mWhiteKingSprite; ///< Sprite for white king
      sf::Sprite mBlackKingSprite; ///< Sprite for black king

      bool mWhitePieces; ///< True if white, else black
  };
}