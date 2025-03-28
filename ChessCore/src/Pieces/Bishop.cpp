#include "Pieces/Bishop.h"
#include "framework/AssetManager.h"
#include "framework/Stage.h"
#include "framework/ChessState.h"

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

    bool Bishop::MovePossible(ChessCoordinate &startCoordinate, ChessCoordinate &endCoordinate)
    {
        int ranksForward = abs(endCoordinate.rank - startCoordinate.rank);
        int filesRightward = abs(endCoordinate.file - startCoordinate.file);

        if((ranksForward == filesRightward) && 
            !PiecesInBetweenPath(startCoordinate,endCoordinate) &&
            (ChessState::Get().GetPieceOnChessCoordinate(endCoordinate) == invalid || isEnemy(endCoordinate)))
        {
            return true;
        }
        return false;
    }

    void Bishop::MakeMove(ChessCoordinate &startCoordinate, ChessCoordinate &endCoordinate)
    {
        if(mWhitePieces)
        {
            ChessState::Get().SetPiecePosition(whiteBishop,startCoordinate,endCoordinate);
        }
        else
        {
            ChessState::Get().SetPiecePosition(blackBishop,startCoordinate,endCoordinate);
        }
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

    List<ChessCoordinate> Bishop::GetAllPossibleMoves(const ChessCoordinate pieceCoordinate)
    {
        List<ChessCoordinate> moves;
        moves.reserve(32);

        int offsetRank[4] = { -1,  -1,  1,  1};
        int offsetFile[4] = {  1,  -1, -1,  1};

        for(int i = 0; i < 4; i++)
        {
            ChessCoordinate iter{pieceCoordinate.rank, pieceCoordinate.file};
            iter.rank += offsetRank[i];
            iter.file += offsetFile[i];

            while(iter.isValid())
            {
                if(ChessState::Get().GetPieceOnChessCoordinate(iter) != invalid)
                {
                    if(isEnemy(iter))
                        moves.emplace_back(iter);
                    break;
                }
                moves.emplace_back(iter);
                iter.rank += offsetRank[i];
                iter.file += offsetFile[i];
            }
        }
        return moves;
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

    void Bishop::CenterPivot()
    {
        sf::FloatRect bound ;
        if(mWhitePieces)
        {
            bound = mWhiteBishopSprite.getGlobalBounds();
            mWhiteBishopSprite.setOrigin({float(bound.position.x) ,float(bound.position.y)});
        }
        else
        {
            bound = mBlackBishopSprite.getGlobalBounds();
            mBlackBishopSprite.setOrigin({float(bound.position.x) ,float(bound.position.y)});
        }
    }

    bool Bishop::isEnemy(ChessCoordinate &endCoordinate)
    {
        return ((mWhitePieces && !Piece::GetPieceColor(ChessState::Get().GetPieceOnChessCoordinate(endCoordinate))) || (!mWhitePieces && Piece::GetPieceColor(ChessState::Get().GetPieceOnChessCoordinate(endCoordinate))));
    }

    bool Bishop::PiecesInBetweenPath(ChessCoordinate& startCoordinate, ChessCoordinate& endCoordinate)
    {
        int offsetX = (endCoordinate.file - startCoordinate.file) > 0 ? 1 : -1 ;
        int offsetY = (endCoordinate.rank - startCoordinate.rank) > 0 ? 1 : -1 ;

        ChessCoordinate iterator{startCoordinate.rank,startCoordinate.file};
        iterator.file += offsetX;
        iterator.rank += offsetY;

        while(iterator.isValid() && !(iterator == endCoordinate))
        {
            if(ChessState::Get().GetPieceOnChessCoordinate(iterator) != invalid)
            {
                return true;
            }
            iterator.file += offsetX;
            iterator.rank += offsetY;
        }
        return false;
    }
}