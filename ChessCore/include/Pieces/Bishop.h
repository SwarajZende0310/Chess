/**
 * @file Bishop.h
 * @brief Bishop piece interface and API.
 */
#pragma once

#include"framework/Piece.h"

namespace chess
{
  /**
   * @brief Bishop chess piece: diagonal movement logic and rendering.
   */
  class Bishop : public Piece
  {
    public:
      /**
       * @brief Construct a Bishop.
       *
       * Loads textures, initializes sprites, and binds to the owning `Stage`.
       * @param owningStage The stage used for rendering and scaling information.
       * @param whitePiece True if this bishop belongs to white, false for black.
       */
      Bishop(Stage* owningStage, bool whitePiece);

      /**
       * @brief Check bishop move legality (diagonals, path clear, target capture rules).
       *
       * Checks that the move is along a diagonal, there are no pieces in-between,
       * and the destination is either empty or an enemy piece.
       * @param startCoordinate Starting square.
       * @param endCoordinate destination square.
       * @return true if move is legal for a bishop.
       */
      virtual bool MovePossible(ChessCoordinate& startCoordinate,ChessCoordinate& endCoordinate)override;
      /**
       * @brief Execute the bishop move and update `ChessState`.
       * @param startCoordinate Starting square.
       * @param endCoordinate destination square.
       */
      virtual void MakeMove(ChessCoordinate& startCoordinate,ChessCoordinate& endCoordinate)override;
      /**
       * @brief Render the bishop sprite for the owning side.
       */
      virtual void RenderPiece()override;

      /**
       * @brief Get true if this bishop is white, false for black.
       */
      inline virtual bool GetPieceColor()override{ return mWhitePieces; }
      
      /**
       * @brief Set current on-screen location of the bishop sprite.
       * @param newLocation Window-space position.
       * @param whitePieces Ignored here; side is fixed in the instance.
       */
      virtual void SetPieceLocation(const sf::Vector2f& newLocation, bool whitePieces) override;
      /**
       * @brief Set current rotation of the sprite (not typically used).
       * @param newRotation Degrees angle.
       * @param whitePieces Ignored here; side is fixed in the instance.
       */
      virtual void SetPieceRotation(float newRotation, bool whitePieces) override;

      /**
       * @brief Enumerate pseudo-legal bishop moves from a coordinate.
       *
       * Traverses diagonals in four directions until blocked, collecting empty
       * squares and one capture square per ray.
       * @param pieceCoordinate Current bishop coordinate.
       * @return List of reachable squares without considering king safety.
       */
      virtual List<ChessCoordinate> GetAllPossibleMoves(const ChessCoordinate pieceCoordinate)override;
    private:
      /** @brief Current on-screen location of the bishop sprite. */
      virtual sf::Vector2f GetPieceLocation()const override;
      /** @brief Current rotation in degrees. */
      virtual float GetPieceRotation()const override;

      /** @brief Center the sprite origin to simplify positioning/rotation. */
      virtual void CenterPivot() override;

      /**
       * @brief Whether a piece at `endCoordinate` belongs to the opponent.
       */
      bool isEnemy(ChessCoordinate& endCoordinate);
      /**
       * @brief Check if there are any pieces between start and end on a diagonal.
       */
      bool PiecesInBetweenPath(ChessCoordinate& startCoordinate, ChessCoordinate& endCoordinate);

      Stage* mOwningStage; ///< Owning stage used for rendering and scaling
      
      shared<sf::Texture> mWhiteBishopTexture; ///< Texture for white bishop
      shared<sf::Texture> mBlackBishopTexture; ///< Texture for black bishop
      
      sf::Sprite mWhiteBishopSprite; ///< Sprite instance for white bishop
      sf::Sprite mBlackBishopSprite; ///< Sprite instance for black bishop

      bool mWhitePieces; ///< True if white, false if black
  };
}