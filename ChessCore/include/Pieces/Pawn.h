#pragma once

#include"framework/Piece.h"

namespace chess
{
    class Pawn : public Piece
    {
        public:
            Pawn(Stage* owningStage, bool whitePiece);

            virtual bool MovePossible(ChessCoordinate* endCoordinate)override;
            virtual void MaveMove(ChessCoordinate* endCoorinate)override;
            virtual void RenderPiece()override;
            
            virtual void SetPieceLocation(const sf::Vector2f& newLocation, bool whitePieces) override;
            virtual void SetPieceRotation(float newRotation, bool whitePieces) override;
        private:
            virtual sf::Vector2f GetPieceLocation()const override;
            virtual float GetPieceRotation()const override;

            Stage* mOwningStage;
            
            shared<sf::Texture> mWhitePawnTexture;
            shared<sf::Texture> mBlackPawnTexture;
            
            sf::Sprite mWhitePawnSprite;
            sf::Sprite mBlackPawnSprite;

            bool mWhitePieces;
    };
}