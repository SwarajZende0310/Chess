#pragma once

#include"framework/Piece.h"

namespace chess
{
    class Knight : public Piece
    {
        public:
            Knight(Stage* owningStage, bool whitePiece);

            virtual bool MovePossible(ChessCoordinate* endCoordinate)override;
            virtual void MaveMove(ChessCoordinate* endCoorinate)override;
            virtual void RenderPiece()override;
            
            virtual void SetPieceLocation(const sf::Vector2f& newLocation, bool whitePieces) override;
            virtual void SetPieceRotation(float newRotation, bool whitePieces) override;
        private:
            virtual sf::Vector2f GetPieceLocation()const override;
            virtual float GetPieceRotation()const override;

            Stage* mOwningStage;
            
            shared<sf::Texture> mWhiteKnightTexture;
            shared<sf::Texture> mBlackKnightTexture;
            
            sf::Sprite mWhiteKnightSprite;
            sf::Sprite mBlackKnightSprite;

            bool mWhitePieces;
    };
}