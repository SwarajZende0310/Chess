#include "Pieces/Bishop.h"
#include "framework/AssetManager.h"
#include "framework/Stage.h"

namespace chess
{
    Bishop::Bishop(Stage *owningStage, bool whitePiece)
        :Piece{owningStage},
        mOwningStage{owningStage},
        mWhiteBishopTexture{AssetManager::Get().LoadTexture("JohnPablok Cburnett Chess set/PNGs/With Shadow/1024px/w_bishop_png_shadow_1024px.png")},
        mBlackBishopTexture{AssetManager::Get().LoadTexture("JohnPablok Cburnett Chess set/PNGs/With Shadow/1024px/b_bishop_png_shadow_1024px.png")},
        mWhiteBishopSprite{*(mWhiteBishopTexture)},
        mBlackBishopSprite{*(mBlackBishopTexture)},
        mWhitePieces{whitePiece}
    {
        mWhiteBishopSprite.setScale(mOwningStage->GetSpriteScale() - sf::Vector2f{0.01,0.01});
        mBlackBishopSprite.setScale(mOwningStage->GetSpriteScale() - sf::Vector2f{0.01,0.01});
    }

    bool Bishop::MovePossible(ChessCoordinate *endCoordinate)
    {
        return false;
    }
    
    void Bishop::MaveMove(ChessCoordinate *endCoorinate)
    {
    }

    void Bishop::RenderPiece()
    {
        if(mWhitePieces)
        {
            mOwningStage->GetWindow().draw(mWhiteBishopSprite);
        }
        else
        {
            mOwningStage->GetWindow().draw(mBlackBishopSprite);
        }
    }

    void Bishop::SetPieceLocation(const sf::Vector2f &newLocation, bool whitePieces)
    {
        if(whitePieces)
            mWhiteBishopSprite.setPosition(newLocation);
        else
            mBlackBishopSprite.setPosition(newLocation);
    }
    
    void Bishop::SetPieceRotation(float newRotation, bool whitePieces)
    {
        // sf::Angle newRot{newRotation};
        // mWhiteBishopSprite.setRotation(newRot);
    }
    
    sf::Vector2f Bishop::GetPieceLocation() const
    {
        if(mWhitePieces)
            return mWhiteBishopSprite.getPosition();
        else
            return mBlackBishopSprite.getPosition();
    }
    
    float Bishop::GetPieceRotation() const
    {
        return 0.0f;
    }
}