#include "Pieces/Rook.h"
#include "framework/AssetManager.h"
#include "framework/Stage.h"

namespace chess
{
    Rook::Rook(Stage *owningStage, bool whitePiece)
        :Piece{owningStage},
        mOwningStage{owningStage},
        mWhiteRookTexture{AssetManager::Get().LoadTexture("JohnPablok Cburnett Chess set/PNGs/With Shadow/1024px/w_rook_png_shadow_1024px.png")},
        mBlackRookTexture{AssetManager::Get().LoadTexture("JohnPablok Cburnett Chess set/PNGs/With Shadow/1024px/b_rook_png_shadow_1024px.png")},
        mWhiteRookSprite{*(mWhiteRookTexture)},
        mBlackRookSprite{*(mBlackRookTexture)},
        mWhitePieces{whitePiece}
    {
        mWhiteRookSprite.setScale(mOwningStage->GetSpriteScale() - sf::Vector2f{0.01,0.01});
        mBlackRookSprite.setScale(mOwningStage->GetSpriteScale() - sf::Vector2f{0.01,0.01});
    }

    bool Rook::MovePossible(ChessCoordinate *endCoordinate)
    {
        return false;
    }
    
    void Rook::MaveMove(ChessCoordinate *endCoorinate)
    {
    }

    void Rook::RenderPiece()
    {
        if(mWhitePieces)
        {
            mOwningStage->GetWindow().draw(mWhiteRookSprite);
        }
        else
        {
            mOwningStage->GetWindow().draw(mBlackRookSprite);
        }
    }

    void Rook::SetPieceLocation(const sf::Vector2f &newLocation, bool whitePieces)
    {
        if(whitePieces)
            mWhiteRookSprite.setPosition(newLocation);
        else
            mBlackRookSprite.setPosition(newLocation);
    }
    
    void Rook::SetPieceRotation(float newRotation, bool whitePieces)
    {
        // sf::Angle newRot{newRotation};
        // mWhiteRookSprite.setRotation(newRot);
    }
    
    sf::Vector2f Rook::GetPieceLocation() const
    {
        if(mWhitePieces)
            return mWhiteRookSprite.getPosition();
        else
            return mBlackRookSprite.getPosition();
    }
    
    float Rook::GetPieceRotation() const
    {
        return 0.0f;
    }
}