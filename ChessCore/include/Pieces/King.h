#pragma once

#include"framework/Piece.h"

namespace chess
{
    class King : public Piece
    {
        public:
            King(Stage* owningStage, bool whitePiece);

            virtual bool MovePossible(ChessCoordinate* endCoordinate)override;
            virtual void MaveMove(ChessCoordinate* endCoorinate)override;
            virtual void RenderPiece()override;
            
            virtual void SetPieceLocation(const sf::Vector2f& newLocation, bool whitePieces) override;
            virtual void SetPieceRotation(float newRotation, bool whitePieces) override;
        private:
            virtual sf::Vector2f GetPieceLocation()const override;
            virtual float GetPieceRotation()const override;

            Stage* mOwningStage;
            
            shared<sf::Texture> mWhiteKingTexture;
            shared<sf::Texture> mBlackKingTexture;
            
            sf::Sprite mWhiteKingSprite;
            sf::Sprite mBlackKingSprite;

            bool mWhitePieces;
    };
}