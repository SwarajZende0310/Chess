#include "Pieces/King.h"
#include "framework/AssetManager.h"
#include "framework/Stage.h"

namespace chess
{
    King::King(Stage *owningStage, bool whitePiece)
        :Piece{owningStage},
        mOwningStage{owningStage},
        mWhiteKingTexture{AssetManager::Get().LoadTexture("JohnPablok Cburnett Chess set/PNGs/With Shadow/1024px/w_king_png_shadow_1024px.png")},
        mBlackKingTexture{AssetManager::Get().LoadTexture("JohnPablok Cburnett Chess set/PNGs/With Shadow/1024px/b_king_png_shadow_1024px.png")},
        mWhiteKingSprite{*(mWhiteKingTexture)},
        mBlackKingSprite{*(mBlackKingTexture)},
        mWhitePieces{whitePiece}
    {
        mWhiteKingSprite.setScale(mOwningStage->GetSpriteScale());
        mBlackKingSprite.setScale(mOwningStage->GetSpriteScale());
        mWhiteKingSprite.setPosition({0.f,0.f});
    }

    bool King::MovePossible(ChessCoordinate *endCoordinate)
    {
        return false;
    }
    
    void King::MaveMove(ChessCoordinate *endCoorinate)
    {
    }

    void King::RenderPiece()
    {
        if(mWhitePieces)
        {
            mOwningStage->GetWindow().draw(mWhiteKingSprite);
        }
        else
        {
            mOwningStage->GetWindow().draw(mBlackKingSprite);
        }
    }

    void King::SetPieceLocation(const sf::Vector2f &newLocation)
    {
    }
    
    void King::SetPieceRotation(float newRotation)
    {
    }
    
    sf::Vector2f King::GetPieceLocation() const
    {
        return sf::Vector2f();
    }
    
    float King::GetPieceRotation() const
    {
        return 0.0f;
    }
}