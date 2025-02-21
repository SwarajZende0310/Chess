#include "Pieces/King.h"
#include "framework/AssetManager.h"
#include "framework/Stage.h"
#include "framework/ChessState.h"

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
        mWhiteKingSprite.setScale(mOwningStage->GetSpriteScale() - sf::Vector2f{0.01,0.01});
        mBlackKingSprite.setScale(mOwningStage->GetSpriteScale() - sf::Vector2f{0.01,0.01});
    }

    bool King::MovePossible(ChessCoordinate &startCoordinate, ChessCoordinate &endCoordinate)
    {
        return true;
    }

    void King::MakeMove(ChessCoordinate &startCoordinate, ChessCoordinate &endCoordinate)
    {
        if(mWhitePieces)
        {
            ChessState::Get().SetWhiteKingPosition(startCoordinate,endCoordinate);
        }
        else
        {
            ChessState::Get().SetBlackKingPosition(startCoordinate,endCoordinate);
        }
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

    void King::SetPieceLocation(const sf::Vector2f &newLocation, bool whitePieces)
    {
        if(whitePieces)
            mWhiteKingSprite.setPosition(newLocation);
        else
            mBlackKingSprite.setPosition(newLocation);
    }
    
    void King::SetPieceRotation(float newRotation, bool whitePieces)
    {
        // sf::Angle newRot{newRotation};
        // mWhiteKingSprite.setRotation(newRot);
    }
    
    sf::Vector2f King::GetPieceLocation() const
    {
        if(mWhitePieces)
            return mWhiteKingSprite.getPosition();
        else
            return mBlackKingSprite.getPosition();
    }
    
    float King::GetPieceRotation() const
    {
        return 0.0f;
    }
}