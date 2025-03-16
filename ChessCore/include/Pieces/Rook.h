#pragma once

#include"framework/Piece.h"

namespace chess
{
    class Rook : public Piece
    {
        public:
            Rook(Stage* owningStage, bool whitePiece);

            virtual bool MovePossible(ChessCoordinate& startCoordinate,ChessCoordinate& endCoordinate)override;
            virtual void MakeMove(ChessCoordinate& startCoordinate,ChessCoordinate& endCoordinate)override;
            virtual void RenderPiece()override;

            inline virtual bool GetPieceColor()override{ return mWhitePieces; }
            
            virtual void SetPieceLocation(const sf::Vector2f& newLocation, bool whitePieces) override;
            virtual void SetPieceRotation(float newRotation, bool whitePieces) override;

            virtual List<ChessCoordinate> GetAllPossibleMoves(const ChessCoordinate pieceCoordinate)override;

        private:
            virtual sf::Vector2f GetPieceLocation()const override;
            virtual float GetPieceRotation()const override;

            virtual void CenterPivot() override;

            bool isEnemy(ChessCoordinate& endCoordinate);
            bool PiecesInBetweenPath(ChessCoordinate& startCoordinate, ChessCoordinate& endCoordinate);

            Stage* mOwningStage;
            
            shared<sf::Texture> mWhiteRookTexture;
            shared<sf::Texture> mBlackRookTexture;
            
            sf::Sprite mWhiteRookSprite;
            sf::Sprite mBlackRookSprite;

            bool mWhitePieces;
    };
}