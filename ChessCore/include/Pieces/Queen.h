#pragma once

/**
 * @file Queen.h
 * @brief Queen piece interface and API.
 */

#include"framework/Piece.h"

namespace chess
{
  /**
   * @brief Queen chess piece: movement and rendering.
   */
  class Queen : public Piece
  {
    public:
      /**
       * @brief Construct a Queen.
       * @param owningStage Stage used for rendering and scaling.
       * @param whitePiece True for white queen, false for black queen.
       */
      Queen(Stage* owningStage, bool whitePiece);

      /**
       * @brief Validate queen move (rook or bishop patterns, clear path, capture rules).
       * @param startCoordinate Start square.
       * @param endCoordinate destination square.
       * @return true if legal per queen rules.
       */
      virtual bool MovePossible(ChessCoordinate& startCoordinate,ChessCoordinate& endCoordinate)override;
      /**
       * @brief Execute queen move and update `ChessState`.
       */
      virtual void MakeMove(ChessCoordinate& startCoordinate,ChessCoordinate& endCoordinate)override;
      /**
       * @brief Render the queen sprite for the owning side.
       */
      virtual void RenderPiece()override;

      /** @brief True if this is a white queen, false if black. */
      inline virtual bool GetPieceColor()override{ return mWhitePieces; }
      
      /** @brief Set sprite location. */
      virtual void SetPieceLocation(const sf::Vector2f& newLocation, bool whitePieces) override;
      /** @brief Set sprite rotation (typically unused). */
      virtual void SetPieceRotation(float newRotation, bool whitePieces) override;

      /**
       * @brief legal queen moves from a square.
       */
      virtual List<ChessCoordinate> GetAllPossibleMoves(const ChessCoordinate pieceCoordinate)override;
    private:
      /** @brief Current on-screen location. */
      virtual sf::Vector2f GetPieceLocation()const override;
      /** @brief Current sprite rotation in degrees. */
      virtual float GetPieceRotation()const override;

      /** @brief Center sprite origin for simpler transforms. */
      virtual void CenterPivot() override;

      /** @brief Whether target square contains an enemy piece. */
      bool isEnemy(ChessCoordinate& endCoordinate);
      /** @brief Whether any pieces block the path to destination. */
      bool PiecesInBetweenPath(ChessCoordinate& startCoordinate, ChessCoordinate& endCoordinate);

      Stage* mOwningStage; ///< Owning stage for rendering context
      
      shared<sf::Texture> mWhiteQueenTexture; ///< Texture for white queen
      shared<sf::Texture> mBlackQueenTexture; ///< Texture for black queen
      
      sf::Sprite mWhiteQueenSprite; ///< Sprite for white queen
      sf::Sprite mBlackQueenSprite; ///< Sprite for black queen

      bool mWhitePieces; ///< True if white, else black
  };
}