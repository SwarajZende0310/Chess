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
        int ranksForward = abs(endCoordinate.rank - startCoordinate.rank);
        int filesRightward = abs(endCoordinate.file - startCoordinate.file);

        if(((ranksForward == 1 && filesRightward == 1) || (ranksForward == 0 && filesRightward == 1 ) || (ranksForward == 1 && filesRightward == 0 )) && 
            (ChessState::Get().GetPieceOnChessCoordinate(endCoordinate) == invalid || isEnemy(endCoordinate)) && 
            ((mWhitePieces && (ChessState::Get().GetBlackAttackedSquares().find(endCoordinate) == ChessState::Get().GetBlackAttackedSquares().end())) || (!mWhitePieces && (ChessState::Get().GetWhiteAttackedSquares().find(endCoordinate) == ChessState::Get().GetWhiteAttackedSquares().end())) ) )
        {
            return true;
        }
        return false;
    }

    void King::MakeMove(ChessCoordinate &startCoordinate, ChessCoordinate &endCoordinate)
    {
        if(mWhitePieces)
        {
            ChessState::Get().SetPiecePosition(whiteKing,startCoordinate,endCoordinate);
        }
        else
        {
            ChessState::Get().SetPiecePosition(blackKing,startCoordinate,endCoordinate);
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

    List<ChessCoordinate> King::GetAllPossibleMoves(const ChessCoordinate pieceCoordinate)
    {
        List<ChessCoordinate> moves;
        moves.reserve(8);

        int offsetRank[8] = { -1,  -1,  1,  1,  1, -1,  0,  0};
        int offsetFile[8] = {  1,  -1, -1,  1,  0,  0,  1, -1};
        
        ChessCoordinate start{pieceCoordinate.rank,pieceCoordinate.file};
        for(int i = 0; i < 8; i++)
        {
            ChessCoordinate end{pieceCoordinate.rank, pieceCoordinate.file};
            end.rank += offsetRank[i];
            end.file += offsetFile[i];

            if(end.isValid() && MovePossible(start,end))
            {
                moves.emplace_back(end);
            }
        }
        return moves;
    }
    bool King::IsInCheck()
    {
        Set<ChessCoordinate,ChessCoordinateHashFunction> attackedSquares = mWhitePieces ? ChessState::Get().GetBlackAttackedSquares() : ChessState::Get().GetWhiteAttackedSquares();
        if(mWhitePieces && attackedSquares.find(ChessState::Get().GetPiecePosiiton(whiteKing)[0]) != attackedSquares.end())
        {
            return true;
        }
        else if(!mWhitePieces && attackedSquares.find(ChessState::Get().GetPiecePosiiton(blackKing)[0]) != attackedSquares.end())
        {
            return true;
        }
        return false;
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

    void King::CenterPivot()
    {
        sf::FloatRect bound ;
        if(mWhitePieces)
        {
            bound = mWhiteKingSprite.getGlobalBounds();
            mWhiteKingSprite.setOrigin({float(bound.position.x) ,float(bound.position.y)});
        }
        else
        {
            bound = mBlackKingSprite.getGlobalBounds();
            mBlackKingSprite.setOrigin({float(bound.position.x) ,float(bound.position.y)});
        }
    }
    bool chess::King::isEnemy(ChessCoordinate &endCoordinate)
    {
        return ((mWhitePieces && !Piece::GetPieceColor(ChessState::Get().GetPieceOnChessCoordinate(endCoordinate))) || (!mWhitePieces && Piece::GetPieceColor(ChessState::Get().GetPieceOnChessCoordinate(endCoordinate))));
    }
}