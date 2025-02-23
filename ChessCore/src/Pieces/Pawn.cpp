#include "Pieces/Pawn.h"
#include "framework/AssetManager.h"
#include "framework/Stage.h"
#include "framework/ChessState.h"

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
        mWhitePawnSprite.setScale(mOwningStage->GetSpriteScale() - sf::Vector2f{0.01,0.01});
        mBlackPawnSprite.setScale(mOwningStage->GetSpriteScale() - sf::Vector2f{0.01,0.01});

        // Initialising first move of every pawn to true
        int rank = mWhitePieces ? 2 : 7;
        for(int i = 0 ; i < 8; i++)
        {
            mFirstMove[ChessCoordinate{rank,(char)('a'+i)}] = true;
        }
    }

    bool Pawn::MovePossible(ChessCoordinate &startCoordinate, ChessCoordinate &endCoordinate)
    {
        int pawnForwardMoves = mWhitePieces ? endCoordinate.rank - startCoordinate.rank : startCoordinate.rank - endCoordinate.rank ;
        if((startCoordinate.file == endCoordinate.file) && ChessState::Get().GetPieceOnChessCoordinate(endCoordinate) == invalid)
        {
            if(mFirstMove[startCoordinate])
            {
                return (pawnForwardMoves == 1 || pawnForwardMoves == 2);
            }
            else
            {
                return pawnForwardMoves == 1;
            }
        }
        else if((startCoordinate.file - 1 == endCoordinate.file || startCoordinate.file + 1 == endCoordinate.file) && 
                (pawnForwardMoves == 1) && 
                (ChessState::Get().GetPieceOnChessCoordinate(endCoordinate) != invalid))//Capturing pieces
        {
            return true;
        }
        return false;
    }

    void Pawn::MakeMove(ChessCoordinate &startCoordinate, ChessCoordinate &endCoordinate)
    {
        if(mWhitePieces)
        {
            ChessState::Get().SetPiecePosition(whitePawn,startCoordinate,endCoordinate);
        }
        else
        {
            ChessState::Get().SetPiecePosition(blackPawn,startCoordinate,endCoordinate);
        }
        mFirstMove[endCoordinate] = false;
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