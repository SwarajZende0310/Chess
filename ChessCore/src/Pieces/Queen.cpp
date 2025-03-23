#include "Pieces/Queen.h"
#include "framework/AssetManager.h"
#include "framework/Stage.h"
#include "framework/ChessState.h"

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

    bool Queen::MovePossible(ChessCoordinate &startCoordinate, ChessCoordinate &endCoordinate)
    {
        int ranksForward = abs(endCoordinate.rank - startCoordinate.rank);
        int filesRightward = abs(endCoordinate.file - startCoordinate.file);

        if(((ranksForward == filesRightward) || (ranksForward == 0 && filesRightward > 0 ) || (ranksForward > 0 && filesRightward == 0 )) && 
            !PiecesInBetweenPath(startCoordinate,endCoordinate) &&
            (ChessState::Get().GetPieceOnChessCoordinate(endCoordinate) == invalid || isEnemy(endCoordinate)))
        {
            return true;
        }
        return false;
    }

    void Queen::MakeMove(ChessCoordinate &startCoordinate, ChessCoordinate &endCoordinate)
    {
        if(mWhitePieces)
        {
            ChessState::Get().SetPiecePosition(whiteQueen,startCoordinate,endCoordinate);
        }
        else
        {
            ChessState::Get().SetPiecePosition(blackQueen,startCoordinate,endCoordinate);
        }
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
            mWhiteQueenSprite.setPosition(newLocation + sf::Vector2f{-6.f,0.f});
        else
            mBlackQueenSprite.setPosition(newLocation + sf::Vector2f{-6.f,0.f});
    }
    
    void Queen::SetPieceRotation(float newRotation, bool whitePieces)
    {
        // sf::Angle newRot{newRotation};
        // mWhiteQueenSprite.setRotation(newRot);
    }

    List<ChessCoordinate> Queen::GetAllPossibleMoves(const ChessCoordinate pieceCoordinate)
    {
        List<ChessCoordinate> moves;
        moves.reserve(32);

        int offsetRank[8] = { -1,  -1,  1,  1,  0,   0,  1, -1};
        int offsetFile[8] = {  1,  -1, -1,  1,  1,  -1,  0,  0};

        for(int i = 0; i < 8; i++)
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

    void Queen::CenterPivot()
    {
        sf::FloatRect bound ;
        if(mWhitePieces)
        {
            bound = mWhiteQueenSprite.getGlobalBounds();
            mWhiteQueenSprite.setOrigin({float(bound.position.x) ,float(bound.position.y)});
        }
        else
        {
            bound = mBlackQueenSprite.getGlobalBounds();
            mBlackQueenSprite.setOrigin({float(bound.position.x) ,float(bound.position.y)});
        }
    }
    bool Queen::isEnemy(ChessCoordinate &endCoordinate)
    {
        return ((mWhitePieces && !Piece::GetPieceColor(ChessState::Get().GetPieceOnChessCoordinate(endCoordinate))) || (!mWhitePieces && Piece::GetPieceColor(ChessState::Get().GetPieceOnChessCoordinate(endCoordinate))));
    }

    bool Queen::PiecesInBetweenPath(ChessCoordinate &startCoordinate, ChessCoordinate &endCoordinate)
    {
        int offsetX = (endCoordinate.file - startCoordinate.file);
        int offsetY = (endCoordinate.rank - startCoordinate.rank);

        if(offsetX > 0)
            offsetX = 1;
        else if(offsetX < 0)
            offsetX = -1;

        if(offsetY > 0)
            offsetY = 1;
        else if(offsetY < 0)
            offsetY = -1;

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