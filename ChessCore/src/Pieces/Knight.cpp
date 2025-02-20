#include "Pieces/Knight.h"
#include "framework/AssetManager.h"
#include "framework/Stage.h"

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
        mWhiteKnightSprite.setScale(mOwningStage->GetSpriteScale());
        mBlackKnightSprite.setScale(mOwningStage->GetSpriteScale());
    }

    bool Knight::MovePossible(ChessCoordinate *endCoordinate)
    {
        return false;
    }
    
    void Knight::MaveMove(ChessCoordinate *endCoorinate)
    {
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
}