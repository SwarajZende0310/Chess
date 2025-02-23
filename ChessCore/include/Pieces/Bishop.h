#pragma once

#include"framework/Piece.h"

namespace chess
{
    class Bishop : public Piece
    {
        public:
            Bishop(Stage* owningStage, bool whitePiece);

            virtual bool MovePossible(ChessCoordinate& startCoordinate,ChessCoordinate& endCoordinate)override;
            virtual void MakeMove(ChessCoordinate& startCoordinate,ChessCoordinate& endCoordinate)override;
            virtual void RenderPiece()override;

            inline virtual bool GetPieceColor()override{ return mWhitePieces; }
            
            virtual void SetPieceLocation(const sf::Vector2f& newLocation, bool whitePieces) override;
            virtual void SetPieceRotation(float newRotation, bool whitePieces) override;
        private:
            virtual sf::Vector2f GetPieceLocation()const override;
            virtual float GetPieceRotation()const override;

            bool isEnemy(ChessCoordinate& endCoordinate);
            bool PiecesInBetween(ChessCoordinate& startCoordinate, ChessCoordinate& endCoordinate);

            Stage* mOwningStage;
            
            shared<sf::Texture> mWhiteBishopTexture;
            shared<sf::Texture> mBlackBishopTexture;
            
            sf::Sprite mWhiteBishopSprite;
            sf::Sprite mBlackBishopSprite;

            bool mWhitePieces;
    };
}