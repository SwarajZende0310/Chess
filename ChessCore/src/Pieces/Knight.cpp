#include "Pieces/Knight.h"
#include "framework/AssetManager.h"
#include "framework/Stage.h"
#include "framework/ChessState.h"

namespace chess
{
    Knight::Knight(Stage *owningStage, bool whitePiece)
        :Piece{owningStage},
        mOwningStage{owningStage},
        mWhiteKnightTexture{AssetManager::Get().LoadTexture("JohnPablok Cburnett Chess set/PNGs/With Shadow/1024px/w_knight_png_shadow_1024px.png")},
        mBlackKnightTexture{AssetManager::Get().LoadTexture("JohnPablok Cburnett Chess set/PNGs/With Shadow/1024px/b_knight_png_shadow_1024px.png")},
        mWhiteKnightSprite{*(mWhiteKnightTexture)},
        mBlackKnightSprite{*(mBlackKnightTexture)},
        mWhitePieces{whitePiece}
    {
        mWhiteKnightSprite.setScale(mOwningStage->GetSpriteScale() - sf::Vector2f{0.01,0.01});
        mBlackKnightSprite.setScale(mOwningStage->GetSpriteScale() - sf::Vector2f{0.01,0.01});
    }

    bool Knight::MovePossible(ChessCoordinate &startCoordinate, ChessCoordinate &endCoordinate)
    {
        int ranksForward = endCoordinate.rank - startCoordinate.rank;
        int filesRightward = endCoordinate.file - startCoordinate.file;
        
        //Knight moves forward and backward
        if((((ranksForward == 2 || ranksForward == -2) && (filesRightward == 1 || filesRightward == -1))  ||
            ((filesRightward == 2 || filesRightward == -2) && (ranksForward == 1 || ranksForward == -1))) && 
            (ChessState::Get().GetPieceOnChessCoordinate(endCoordinate) == invalid || isEnemy(endCoordinate))) //Knight moves sidewards
        {
            return true;
        }
        return false;
    }

    void Knight::MakeMove(ChessCoordinate &startCoordinate, ChessCoordinate &endCoordinate)
    {
        if(mWhitePieces)
        {
            ChessState::Get().SetPiecePosition(whiteKnight,startCoordinate,endCoordinate);
        }
        else
        {
            ChessState::Get().SetPiecePosition(blackKnight,startCoordinate,endCoordinate);
        }
    }

    void Knight::RenderPiece()
    {
        if(mWhitePieces)
        {
            mOwningStage->GetWindow().draw(mWhiteKnightSprite);
        }
        else
        {
            mOwningStage->GetWindow().draw(mBlackKnightSprite);
        }
    }

    void Knight::SetPieceLocation(const sf::Vector2f &newLocation, bool whitePieces)
    {
        if(whitePieces)
            mWhiteKnightSprite.setPosition(newLocation);
        else
            mBlackKnightSprite.setPosition(newLocation);
    }
    
    void Knight::SetPieceRotation(float newRotation, bool whitePieces)
    {
        // sf::Angle newRot{newRotation};
        // mWhiteKnightSprite.setRotation(newRot);
    }
    
    sf::Vector2f Knight::GetPieceLocation() const
    {
        if(mWhitePieces)
            return mWhiteKnightSprite.getPosition();
        else
            return mBlackKnightSprite.getPosition();
    }
    
    float Knight::GetPieceRotation() const
    {
        return 0.0f;
    }

    bool Knight::isEnemy(ChessCoordinate &endCoordinate)
    {
        return ((mWhitePieces && !Piece::GetPieceColor(ChessState::Get().GetPieceOnChessCoordinate(endCoordinate))) || (!mWhitePieces && Piece::GetPieceColor(ChessState::Get().GetPieceOnChessCoordinate(endCoordinate))));
    }
}