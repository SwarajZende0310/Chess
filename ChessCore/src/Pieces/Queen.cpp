#include "Pieces/Queen.h"
#include "framework/AssetManager.h"
#include "framework/Stage.h"

namespace chess
{
    Queen::Queen(Stage *owningStage, bool whitePiece)
        :Piece{owningStage},
        mOwningStage{owningStage},
        mWhiteQueenTexture{AssetManager::Get().LoadTexture("JohnPablok Cburnett Chess set/PNGs/With Shadow/1024px/w_queen_png_shadow_1024px.png")},
        mBlackQueenTexture{AssetManager::Get().LoadTexture("JohnPablok Cburnett Chess set/PNGs/With Shadow/1024px/b_queen_png_shadow_1024px.png")},
        mWhiteQueenSprite{*(mWhiteQueenTexture)},
        mBlackQueenSprite{*(mBlackQueenTexture)},
        mWhitePieces{whitePiece}
    {
        mWhiteQueenSprite.setScale(mOwningStage->GetSpriteScale() - sf::Vector2f{0.01,0.01});
        mBlackQueenSprite.setScale(mOwningStage->GetSpriteScale() - sf::Vector2f{0.01,0.01});
    }

    bool Queen::MovePossible(ChessCoordinate *endCoordinate)
    {
        return false;
    }
    
    void Queen::MaveMove(ChessCoordinate *endCoorinate)
    {
    }

    void Queen::RenderPiece()
    {
        if(mWhitePieces)
        {
            mOwningStage->GetWindow().draw(mWhiteQueenSprite);
        }
        else
        {
            mOwningStage->GetWindow().draw(mBlackQueenSprite);
        }
    }

    void Queen::SetPieceLocation(const sf::Vector2f &newLocation, bool whitePieces)
    {
        if(whitePieces)
            mWhiteQueenSprite.setPosition(newLocation);
        else
            mBlackQueenSprite.setPosition(newLocation);
    }
    
    void Queen::SetPieceRotation(float newRotation, bool whitePieces)
    {
        // sf::Angle newRot{newRotation};
        // mWhiteQueenSprite.setRotation(newRot);
    }
    
    sf::Vector2f Queen::GetPieceLocation() const
    {
        if(mWhitePieces)
            return mWhiteQueenSprite.getPosition();
        else
            return mBlackQueenSprite.getPosition();
    }
    
    float Queen::GetPieceRotation() const
    {
        return 0.0f;
    }
}