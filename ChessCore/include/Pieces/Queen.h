#pragma once

#include"framework/Piece.h"

namespace chess
{
    class Queen : public Piece
    {
        public:
            Queen(Stage* owningStage, bool whitePiece);

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
            bool PiecesInBetweenPath(ChessCoordinate& startCoordinate, ChessCoordinate& endCoordinate);

            Stage* mOwningStage;
            
            shared<sf::Texture> mWhiteQueenTexture;
            shared<sf::Texture> mBlackQueenTexture;
            
            sf::Sprite mWhiteQueenSprite;
            sf::Sprite mBlackQueenSprite;

            bool mWhitePieces;
    };
}