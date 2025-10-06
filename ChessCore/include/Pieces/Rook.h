#pragma once

/**
 * @file Rook.h
 * @brief Rook piece interface and API.
**/
#include"framework/Piece.h"

namespace chess
{
  /**
   * @brief Rook chess piece: horizontal/vertical movement and rendering.
   */
  class Rook : public Piece
  {
    public:
      /**
       * @brief Construct a Rook.
       * @param owningStage Stage used for rendering and scaling.
       * @param whitePiece True for white rook, false for black rook.
       */
      Rook(Stage* owningStage, bool whitePiece);

      /**
       * @brief Validate rook move (rank/file movement, clear path, capture rules).
       * @param startCoordinate Start square.
       * @param endCoordinate destination square.
       * @return true if legal per rook rules.
       */
      virtual bool MovePossible(ChessCoordinate& startCoordinate,ChessCoordinate& endCoordinate)override;
      /** @brief Execute rook move and update `ChessState`. */
      virtual void MakeMove(ChessCoordinate& startCoordinate,ChessCoordinate& endCoordinate)override;
      /** @brief Render the rook sprite for the owning side. */
      virtual void RenderPiece()override;

      /** @brief True if this is a white rook, false if black. */
      inline virtual bool GetPieceColor()override{ return mWhitePieces; }
      
      /** @brief Set sprite location. */
      virtual void SetPieceLocation(const sf::Vector2f& newLocation, bool whitePieces) override;
      /** @brief Set sprite rotation (typically unused). */
      virtual void SetPieceRotation(float newRotation, bool whitePieces) override;

      /** @brief legal rook moves from a square. */
      virtual List<ChessCoordinate> GetAllPossibleMoves(const ChessCoordinate pieceCoordinate)override;

    private:
      /** @brief Current on-screen location. */
      virtual sf::Vector2f GetPieceLocation()const override;
      /** @brief Current sprite rotation in degrees. */
      virtual float GetPieceRotation()const override;

      /** @brief Center sprite origin for simpler transforms. */
      virtual void CenterPivot() override;

      /**
       * @brief Whether target square contains an enemy piece.
       * @param endCoordinate Target square.
       * @return true if enemy piece present.
       */
      bool isEnemy(ChessCoordinate& endCoordinate);
      /**
       * @brief Whether any pieces block the path to destination.
       * @param startCoordinate Start square.
       * @param endCoordinate destination square.
       * @return true if any pieces block the path.
       */
      bool PiecesInBetweenPath(ChessCoordinate& startCoordinate, ChessCoordinate& endCoordinate);

      Stage* mOwningStage; ///< Owning stage for rendering context
      
      shared<sf::Texture> mWhiteRookTexture; ///< Texture for white rook
      shared<sf::Texture> mBlackRookTexture; ///< Texture for black rook
      
      sf::Sprite mWhiteRookSprite; ///< Sprite for white rook
      sf::Sprite mBlackRookSprite; ///< Sprite for black rook

      bool mWhitePieces; ///< True if white, else black
  };
}