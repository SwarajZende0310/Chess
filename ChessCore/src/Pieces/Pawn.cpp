#include "Pieces/Pawn.h"
#include "framework/AssetManager.h"
#include "framework/Stage.h"

namespace chess
{
    Pawn::Pawn(Stage *owningStage, bool whitePiece)
        :Piece{owningStage},
        mOwningStage{owningStage},
        mWhitePawnTexture{AssetManager::Get().LoadTexture("JohnPablok Cburnett Chess set/PNGs/With Shadow/1024px/w_pawn_png_shadow_1024px.png")},
        mBlackPawnTexture{AssetManager::Get().LoadTexture("JohnPablok Cburnett Chess set/PNGs/With Shadow/1024px/b_pawn_png_shadow_1024px.png")},
        mWhitePawnSprite{*(mWhitePawnTexture)},
        mBlackPawnSprite{*(mBlackPawnTexture)},
        mWhitePieces{whitePiece}
    {
        mWhitePawnSprite.setScale(mOwningStage->GetSpriteScale());
        mBlackPawnSprite.setScale(mOwningStage->GetSpriteScale());
    }

    bool Pawn::MovePossible(ChessCoordinate *endCoordinate)
    {
        return false;
    }
    
    void Pawn::MaveMove(ChessCoordinate *endCoorinate)
    {
    }

    void Pawn::RenderPiece()
    {
        if(mWhitePieces)
        {
            mOwningStage->GetWindow().draw(mWhitePawnSprite);
        }
        else
        {
            mOwningStage->GetWindow().draw(mBlackPawnSprite);
        }
    }

    void Pawn::SetPieceLocation(const sf::Vector2f &newLocation, bool whitePieces)
    {
        if(whitePieces)
            mWhitePawnSprite.setPosition(newLocation);
        else
            mBlackPawnSprite.setPosition(newLocation);
    }
    
    void Pawn::SetPieceRotation(float newRotation, bool whitePieces)
    {
        // sf::Angle newRot{newRotation};
        // mWhitePawnSprite.setRotation(newRot);
    }
    
    sf::Vector2f Pawn::GetPieceLocation() const
    {
        if(mWhitePieces)
            return mWhitePawnSprite.getPosition();
        else
            return mBlackPawnSprite.getPosition();
    }
    
    float Pawn::GetPieceRotation() const
    {
        return 0.0f;
    }
}