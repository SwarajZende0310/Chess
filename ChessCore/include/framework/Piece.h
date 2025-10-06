#ifndef CHESS_PIECE_H_DOXY
/**
 * @file Piece.h
 * @brief Abstract base for all chess piece types.
 */
#endif
#pragma once

#include<SFML/Graphics.hpp>
#include"framework/Core.h"

namespace chess
{
    class Stage;
    /**
     * @brief Abstract interface for chess piece behavior and rendering.
     */
    class Piece
    {
        public:
            /**
             * @brief Construct a new Piece.
             * @param owningStage Stage that owns/renders this piece.
             */
            Piece(Stage* owningStage);

            /**
             * @brief Check if a move from start to end is legal for this piece.
             * @param startCoordinate Start square
             * @param endCoordinate Destination square
             * @return true if move is possible
             */
            virtual bool MovePossible(ChessCoordinate& startCoordinate,ChessCoordinate& endCoordinate) = 0;
            /**
             * @brief Execute a move (updates board state, logs history).
             */
            virtual void MakeMove(ChessCoordinate& startCoordinate,ChessCoordinate& endCoordinate) = 0;
            /**
             * @brief Render this piece at its current location.
             */
            virtual void RenderPiece() = 0;

            /**
             * @brief Get true if the piece is white.
             */
            inline virtual bool GetPieceColor() = 0;
            /**
             * @brief Utility to determine color from a piece code.
             */
            bool GetPieceColor(char piece);

            /**
             * @brief Set piece position in window coordinates.
             */
            virtual void SetPieceLocation(const sf::Vector2f& newLocation, bool whitePieces) = 0;
            /**
             * @brief Set piece rotation (degrees).
             */
            virtual void SetPieceRotation(float newRotation, bool whitePieces) = 0;

            /**
             * @brief Enumerate all pseudo-legal moves for the piece from a square.
             */
            virtual List<ChessCoordinate> GetAllPossibleMoves(const ChessCoordinate pieceCoordinate) = 0;
        private:
            /** @brief Get current piece location in window coordinates. */
            virtual sf::Vector2f GetPieceLocation()const = 0;
            /** @brief Get current rotation (degrees). */
            virtual float GetPieceRotation()const = 0;

            /** @brief Center the sprite's origin to its visual center. */
            virtual void CenterPivot() = 0;

            Stage* mOwningSage; ///< Owning stage (render context and state)
    };
}