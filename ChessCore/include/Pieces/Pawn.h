#pragma once

#include"framework/Piece.h"

namespace chess
{
    class Pawn : public Piece
    {
        public:
            Pawn(Stage* owningStage, bool whitePiece);

            virtual bool MovePossible(ChessCoordinate& startCoordinate,ChessCoordinate& endCoordinate)override;
            virtual void MakeMove(ChessCoordinate& startCoordinate,ChessCoordinate& endCoordinate)override;
            virtual void RenderPiece()override;

            inline virtual bool GetPieceColor()override{ return mWhitePieces; }
            
            virtual void SetPieceLocation(const sf::Vector2f& newLocation, bool whitePieces) override;
            virtual void SetPieceRotation(float newRotation, bool whitePieces) override;
        private:
            virtual sf::Vector2f GetPieceLocation()const override;
            virtual float GetPieceRotation()const override;

            virtual void CenterPivot() override;

            bool isEnemy(ChessCoordinate& endCoordinate);

            Stage* mOwningStage;
            
            shared<sf::Texture> mWhitePawnTexture;
            shared<sf::Texture> mBlackPawnTexture;
            
            sf::Sprite mWhitePawnSprite;
            sf::Sprite mBlackPawnSprite;

            bool mWhitePieces;

            Dictionary<ChessCoordinate,bool,ChessCoordinateHashFunction> mFirstMove;
    };
}