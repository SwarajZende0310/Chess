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
                ((ChessState::Get().GetPieceOnChessCoordinate(endCoordinate) != invalid && isEnemy(endCoordinate)) 
                || (ChessState::Get().GetPieceOnChessCoordinate(endCoordinate) == invalid && EnPassantPossible(startCoordinate,endCoordinate))))//Capturing pieces
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

    List<ChessCoordinate> Pawn::GetAllPossibleMoves(const ChessCoordinate pieceCoordinate)
    {
        List<ChessCoordinate> moves;
        moves.reserve(4);

        ChessCoordinate start{pieceCoordinate.rank, pieceCoordinate.file};
        std::array<int,6> offsetRank;
        std::array<int,6> offsetFile;
        
        if(mWhitePieces)
        {
            offsetRank = {  1,  1,  1,  2,  2,  2};
            offsetFile = {  0, -1,  1,  0, -1,  1};
        }
        else
        {
            offsetRank = { -1, -1, -1, -2, -2, -2};
            offsetFile = {  0, -1,  1,  0, -1,  1};
        }
        
        for(int i = 0; i < 6 ; i++)
        {
            ChessCoordinate end{pieceCoordinate.rank, pieceCoordinate.file};
            end.rank += offsetRank[i];
            end.file += offsetFile[i];
            if(MovePossible(start,end))
            {
                moves.emplace_back(end);
            }
        }

        return moves;
    }

    ChessCoordinate Pawn::PawnToPromote()
    {
        ChessCoordinate pawnToPromote{invalid,invalid};
        if(mWhitePieces)
        {
            for(int i = 0 ; i < 8 ; i++)
            {
                if(ChessState::Get().GetPieceOnChessCoordinate(ChessCoordinate{8,char('a'+i)}) == whitePawn)
                {
                    pawnToPromote.file = 'a' + i ;
                    pawnToPromote.rank = 8 ;
                    return pawnToPromote;
                }
            }
        }
        else
        {
            for(int i = 0 ; i < 8 ; i++)
            {
                if(ChessState::Get().GetPieceOnChessCoordinate(ChessCoordinate{1,char('a'+i)}) == blackPawn)
                {
                    pawnToPromote.file = 'a' + i ;
                    pawnToPromote.rank = 1 ;
                    return pawnToPromote;
                }
            }
        }
        return pawnToPromote;
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

    void Pawn::CenterPivot()
    {
        sf::FloatRect bound ;
        if(mWhitePieces)
        {
            bound = mWhitePawnSprite.getGlobalBounds();
            mWhitePawnSprite.setOrigin({float(bound.position.x) ,float(bound.position.y)});
        }
        else
        {
            bound = mBlackPawnSprite.getGlobalBounds();
            mBlackPawnSprite.setOrigin({float(bound.position.x) ,float(bound.position.y)});
        }
    }
    bool Pawn::isEnemy(ChessCoordinate &endCoordinate)
    {
        return ((mWhitePieces && !Piece::GetPieceColor(ChessState::Get().GetPieceOnChessCoordinate(endCoordinate))) || (!mWhitePieces && Piece::GetPieceColor(ChessState::Get().GetPieceOnChessCoordinate(endCoordinate))));
    }

    bool Pawn::EnPassantPossible(ChessCoordinate startCoordinate, ChessCoordinate endCoordinate)
    {
      if((mWhitePieces && ChessState::Get().GetPieceOnChessCoordinate(startCoordinate) != whitePawn) || (!mWhitePieces && ChessState::Get().GetPieceOnChessCoordinate(startCoordinate) != blackPawn))
        return false;
      
      List<ChessCoordinate> lastMove = ChessState::Get().GetLastPlayedMove();
      if(lastMove.size()<2)return false;
      if(mWhitePieces && lastMove[0].rank == 7 && lastMove[1].rank == 5 && startCoordinate.rank == 5 && endCoordinate.rank == 6 
        && lastMove[0].file == lastMove[1].file
        && abs(startCoordinate.file - endCoordinate.file) == 1 && abs(startCoordinate.file - lastMove[0].file) == 1)
      {
        return true;
      }
      else if(!mWhitePieces && lastMove[0].rank == 2 && lastMove[1].rank == 4 && startCoordinate.rank == 4 && endCoordinate.rank == 3 
        && lastMove[0].file == lastMove[1].file
        && abs(startCoordinate.file - endCoordinate.file) == 1 && abs(startCoordinate.file - lastMove[0].file) == 1)
      {
        return true;
      }
        return false;
    }
}