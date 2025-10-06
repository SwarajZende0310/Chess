#pragma once

/**
 * @file Knight.h
 * @brief Knight piece interface and API.
 */

#include"framework/Piece.h"

namespace chess
{
    /**
     * @brief Knight chess piece: L-shaped movement and rendering.
     */
    class Knight : public Piece
    {
        public:
            /**
             * @brief Construct a Knight.
             * @param owningStage Stage used for rendering and scaling.
             * @param whitePiece True for white knight, false for black knight.
             */
            Knight(Stage* owningStage, bool whitePiece);

            /**
             * @brief Validate knight move (2 by 1 or 1 by 2 L-shape).
             * @param startCoordinate Start square.
             * @param endCoordinate destination square.
             * @return true if legal and destination is empty or enemy.
             */
            virtual bool MovePossible(ChessCoordinate& startCoordinate,ChessCoordinate& endCoordinate)override;
            /** @brief Execute knight move and update `ChessState`. */
            virtual void MakeMove(ChessCoordinate& startCoordinate,ChessCoordinate& endCoordinate)override;
            /** @brief Render the knight sprite for the owning side. */
            virtual void RenderPiece()override;

            /** @brief True if this is a white knight, false if black. */
            inline virtual bool GetPieceColor()override{ return mWhitePieces; }
            
            /** @brief Set sprite location. */
            virtual void SetPieceLocation(const sf::Vector2f& newLocation, bool whitePieces) override;
            /** @brief Set sprite rotation (typically unused). */
            virtual void SetPieceRotation(float newRotation, bool whitePieces) override;

            /** @brief legal knight moves from a square. */
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

            Stage* mOwningStage; ///< Owning stage for rendering context
            
            shared<sf::Texture> mWhiteKnightTexture; ///< Texture for white knight
            shared<sf::Texture> mBlackKnightTexture; ///< Texture for black knight
            
            sf::Sprite mWhiteKnightSprite; ///< Sprite for white knight
            sf::Sprite mBlackKnightSprite; ///< Sprite for black knight

            bool mWhitePieces; ///< True if white, else black
    };
}