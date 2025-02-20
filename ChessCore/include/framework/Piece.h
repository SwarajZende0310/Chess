#pragma once

#include<SFML/Graphics.hpp>
#include"framework/Core.h"

namespace chess
{
    class Stage;
    class Piece
    {
        public:
            Piece(Stage* owningStage);

            virtual bool MovePossible(ChessCoordinate* endCoordinate) = 0;
            virtual void MaveMove(ChessCoordinate* endCoorinate) = 0;
            virtual void RenderPiece() = 0;

            virtual void SetPieceLocation(const sf::Vector2f& newLocation, bool whitePieces) = 0;
            virtual void SetPieceRotation(float newRotation, bool whitePieces) = 0;
        private:
            virtual sf::Vector2f GetPieceLocation()const = 0;
            virtual float GetPieceRotation()const = 0;

            Stage* mOwningSage;
    };
}