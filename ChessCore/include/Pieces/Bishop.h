#pragma once

#include"framework/Piece.h"

namespace chess
{
    class Bishop : public Piece
    {
        public:
            Bishop(Stage* owningStage, bool whitePiece);

            virtual bool MovePossible(ChessCoordinate* endCoordinate)override;
            virtual void MaveMove(ChessCoordinate* endCoorinate)override;
            virtual void RenderPiece()override;
            
            virtual void SetPieceLocation(const sf::Vector2f& newLocation, bool whitePieces) override;
            virtual void SetPieceRotation(float newRotation, bool whitePieces) override;
        private:
            virtual sf::Vector2f GetPieceLocation()const override;
            virtual float GetPieceRotation()const override;

            Stage* mOwningStage;
            
            shared<sf::Texture> mWhiteBishopTexture;
            shared<sf::Texture> mBlackBishopTexture;
            
            sf::Sprite mWhiteBishopSprite;
            sf::Sprite mBlackBishopSprite;

            bool mWhitePieces;
    };
}