#pragma once

/**
 * @file Pawn.h
 * @brief Pawn piece interface and API.
*/
#include"framework/Piece.h"

namespace chess
{
  /**
   * @brief Pawn chess piece: forward moves, captures, en passant, promotion.
   * @details This class implements the rules of a pawn in chess, including
   *          its movement, capture, en passant, and promotion.
   */
  class Pawn : public Piece
  {
    public:
      /**
       * @brief Construct a Pawn.
       * @param owningStage Stage used for rendering and scaling.
       * @param whitePiece True for white pawn, false for black pawn.
       */
      Pawn(Stage* owningStage, bool whitePiece);

      /**
       * @brief Validate pawn move (push, double-push on first move, diagonal capture, en passant).
       * @param startCoordinate Start square.
       * @param endCoordinate Destination square.
       * @return true if legal per pawn rules (including en passant).
       */
      virtual bool MovePossible(ChessCoordinate& startCoordinate,ChessCoordinate& endCoordinate)override;
      /** @brief Execute pawn move and update `ChessState` and first-move flags. */
      virtual void MakeMove(ChessCoordinate& startCoordinate,ChessCoordinate& endCoordinate)override;
      /** @brief Render the pawn sprite for the owning side. */
      virtual void RenderPiece()override;

      /** @brief True if this is a white pawn, false if black. */
      inline virtual bool GetPieceColor()override{ return mWhitePieces; }
      
      /** @brief Set sprite location. */
      virtual void SetPieceLocation(const sf::Vector2f& newLocation, bool whitePieces) override;
      /** @brief Set sprite rotation (typically unused). */
      virtual void SetPieceRotation(float newRotation, bool whitePieces) override;

      /** @brief legal pawn moves from a square. */
      virtual List<ChessCoordinate> GetAllPossibleMoves(const ChessCoordinate pieceCoordinate)override;

      /**
       * @brief Detect if any pawn reached last rank and must be promoted.
       * @return The coordinate of the pawn to promote, or invalid if none.
       */
      ChessCoordinate PawnToPromote();
    private:
      /** @brief Current on-screen location. */
      virtual sf::Vector2f GetPieceLocation()const override;
      /** @brief Current sprite rotation in degrees. */
      virtual float GetPieceRotation()const override;

      /** @brief Center sprite origin for simpler transforms. */
      virtual void CenterPivot() override;

      /** @brief Whether target square contains an enemy piece. */
      bool isEnemy(ChessCoordinate& endCoordinate);

      /**
       * @brief Determine if en passant capture is legal for this move.
       * @param startCoordinate Start square.
       * @param endCoordinate Candidate capture destination.
       * @return true if en passant is legal for current position/history.
       */
      bool EnPassantPossible(ChessCoordinate startCoordinate, ChessCoordinate endCoordinate);

      Stage* mOwningStage; ///< Owning stage for rendering context
      
      shared<sf::Texture> mWhitePawnTexture; ///< Texture for white pawn
      shared<sf::Texture> mBlackPawnTexture; ///< Texture for black pawn
      
      sf::Sprite mWhitePawnSprite; ///< Sprite for white pawn
      sf::Sprite mBlackPawnSprite; ///< Sprite for black pawn

      bool mWhitePieces; ///< True if white, else black

      /**
       * @brief Tracks whether each pawn on its starting file has moved yet,
       * enabling the double-step on its first move.
       */
      Dictionary<ChessCoordinate,bool,ChessCoordinateHashFunction> mFirstMove;
  };
}