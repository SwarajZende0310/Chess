#include"framework/ChessState.h"

namespace chess
{
    unique<ChessState> ChessState::mChessState{nullptr};
    ChessState& ChessState::Get()
    {
        if(!mChessState)
        {
            mChessState = std::move(unique<ChessState>{new ChessState});
        }
        return *mChessState;
    }

    void ChessState::ResetToStartPosition()
    {

        /*
        Start position of Chess Piece :
        
        White(Upper case)/Black(Lower case)
        R/r = Rook
        N/n = Knight
        B/b = Bishop
        Q/q = Queen
        K/k = King

        rnbqkbnr
        pppppppp
        xxxxxxxx
        xxxxxxxx
        xxxxxxxx
        xxxxxxxx
        pppppppp
        RNBQKBNR
        */

        //Setting bits of uint64_t to show presence of piece

        //White Pawns
        int row = 2; //Seventh row from bottom
        for(int i = 7; i >= 0; i--)
        {
            mWhitePawns |= (1ULL << ( 8 * (row-1) + i));
        }

        row = 1;
        //White Rooks
        mWhiteRooks |= (1ULL << ( 8 * (row-1) + 0));
        mWhiteRooks |= (1ULL << ( 8 * (row-1) + 7));

        //White Knights
        mWhiteKnights |= (1ULL << ( 8 * (row-1) + 1));
        mWhiteKnights |= (1ULL << ( 8 * (row-1) + 6));

        //White Bishops
        mWhiteBishops |= (1ULL << ( 8 * (row-1) + 2));
        mWhiteBishops |= (1ULL << ( 8 * (row-1) + 5));

        //White Queen
        mWhiteQueen |= (1ULL << ( 8 * (row-1) + 4));

        //White King
        mWhiteKing |= (1ULL << ( 8 * (row-1) + 3));

        //Black Pawns
        row = 7;
        for(int i = 7; i >= 0; i--)
        {
            mBlackPawns |= (1ULL << ( 8 * (row-1) + i));
        }

        row = 8;
        //Black Rooks
        mBlackRooks |= (1ULL << ( 8 * (row-1) + 0));
        mBlackRooks |= (1ULL << ( 8 * (row-1) + 7));

        //Black Knights
        mBlackKnights |= (1ULL << ( 8 * (row-1) + 1));
        mBlackKnights |= (1ULL << ( 8 * (row-1) + 6));

        //Black Bishops
        mBlackBishops |= (1ULL << ( 8 * (row-1) + 2));
        mBlackBishops |= (1ULL << ( 8 * (row-1) + 5));

        //Black Queen
        mBlackQueen |= (1ULL << ( 8 * (row-1) + 4));

        //Black King
        mBlackKing |= (1ULL << ( 8 * (row-1) + 3));
    }

    List<ChessCoordinate> ChessState::GetPiecePosiiton(char piece)
    {
        List<ChessCoordinate> position;
        position.reserve(8);
        uint64_t pieceContainer = GetPieceContainer(piece);

        for(int i = 0; i < 8; i++)
        {
            for(int j = 7; j>=0; j--)
            {
                if( (1ULL << ( (8 * i) + j)) & pieceContainer)
                {
                    position.emplace_back(ChessCoordinate{ i + 1 , ConvertColToRank(7 - j + 1)});
                }
            }
        }

        return position;
    }

    void ChessState::SetPiecePosition(char piece, ChessCoordinate &start, ChessCoordinate &end)
    {
        uint64_t& pieceContainer = GetPieceContainer(piece);
        if(start.file == invalid || start.rank == -1 || end.file == invalid || end.rank == -1) return;
        uint64_t currentPos =  1ULL << (8 * (start.rank - 1) + (7- ConvertRankToCol(start.file)+1));
        if(!(pieceContainer & currentPos))return;
        bool pieceRemoved = false;

        // Remove other piece if already there in position where we are moving or Enpassant played
        if(ChessState::Get().GetPieceOnChessCoordinate(end) != invalid || ( (piece == whitePawn || piece == blackPawn) && abs(start.file - end.file) == 1 ))
        {
            char endPiece = ChessState::Get().GetPieceOnChessCoordinate(end);
            if(endPiece == invalid)
            {
                // Enpassant move
                pieceRemoved = true;
                mRemovedPiecePosition = ChessCoordinate{start.rank,end.file};
                mRemovedPiece = GetPieceOnChessCoordinate(mRemovedPiecePosition);
            }
            else
            {
                pieceRemoved = true;
                mRemovedPiece = endPiece;
                mRemovedPiecePosition = end;
            }
            ChessState::Get().RemovePiece(mRemovedPiece, mRemovedPiecePosition);
        }

        // Unset the current high bit
        pieceContainer ^= currentPos;

        // Set the end position bit
        pieceContainer |= 1ULL << (8 * (end.rank - 1) + (7- ConvertRankToCol(end.file)+1));

        // Updating for undoing move if needed
        mLastPieceMoved = piece;
        mLastMovedStartMove = start;
        mLastMovedEndMove = end;
        mRemovedPieceLastMove = pieceRemoved;

        UpdateAttackedSquare();
    }

    void ChessState::UndoLastMove()
    {
        if(!mLastMovedStartMove.isValid() || !mLastMovedEndMove.isValid())return;

        // Spawning removed piece
        if(mRemovedPieceLastMove && mRemovedPiece != invalid && mRemovedPiecePosition.isValid())
        {
            SpawnPiece(mRemovedPiece,mRemovedPiecePosition);
        }
        // reseting to previous position
        SetPiecePosition(mLastPieceMoved,mLastMovedEndMove,mLastMovedStartMove);
        
        // Reseting to invalid state
        mRemovedPieceLastMove = false;
        mRemovedPiece = invalid;
        mRemovedPiecePosition.file = invalid;
        mRemovedPiecePosition.rank = -1;
        mLastPieceMoved = invalid;
        mLastMovedStartMove.file = invalid;
        mLastMovedStartMove.rank = -1;
        mLastMovedEndMove.file = invalid;
        mLastMovedEndMove.rank = -1;
    }

    char ChessState::GetPieceOnChessCoordinate(ChessCoordinate coordinate)
    {
        uint64_t currentPos =  (1ULL << (8 * (coordinate.rank - 1) + (7- ConvertRankToCol(coordinate.file)+1)));
        
        if(mWhitePawns & currentPos)return whitePawn;
        else if( mWhiteRooks & currentPos)return whiteRook;
        else if( mWhiteKnights & currentPos)return whiteKnight;
        else if( mWhiteBishops & currentPos)return whiteBishop;
        else if( mWhiteQueen & currentPos)return whiteQueen;
        else if( mWhiteKing & currentPos)return whiteKing;

        else if(mBlackPawns & currentPos)return blackPawn;
        else if( mBlackRooks & currentPos)return blackRook;
        else if( mBlackKnights & currentPos)return blackKnight;
        else if( mBlackBishops & currentPos)return blackBishop;
        else if( mBlackQueen & currentPos)return blackQueen;
        else if( mBlackKing & currentPos)return blackKing;

        return invalid;
    }

    void ChessState::RemovePiece(char piece, ChessCoordinate &position)
    {
        if(piece == invalid || position.file == invalid || position.rank == -1)return;
        uint64_t& pieceContainer = GetPieceContainer(piece);
        uint64_t currentPos =  1ULL << (8 * (position.rank - 1) + (7- ConvertRankToCol(position.file)+1));
        if(!(pieceContainer & currentPos))return;

        currentPos = ~(currentPos);

        pieceContainer &= currentPos;
    }

    bool ChessState::KingInCheck(bool white)
    {
        UpdateAttackedSquare();
        if(white)
        {
            return mBlackAttackedSquares.find(GetPiecePosiiton(whiteKing)[0]) != mBlackAttackedSquares.end() ;
        }
        else
        {
            return mWhiteAttackedSquares.find(GetPiecePosiiton(blackKing)[0]) != mWhiteAttackedSquares.end() ;
        }
        return false;
    }

    List<ChessCoordinate> ChessState::GetLastPlayedMove()
    {
        return {mLastMovedStartMove,mLastMovedEndMove};
    }
    
    int ChessState::GetPieceCount(char piece)
    {
        int pieceCount = 0;
        uint64_t pieceContainer = GetPieceContainer(piece);

        for(int i = 0; i < 64 ; i++)
        {
            if(pieceContainer & 1ULL << i)
                pieceCount++;
        }

        return pieceCount;
    }
    
    ChessState::ChessState()
        : mWhitePawns{0},
          mWhiteKnights{0},
          mWhiteBishops{0},
          mWhiteRooks{0},
          mWhiteQueen{0},
          mWhiteKing{0},
          mBlackPawns{0},
          mBlackKnights{0},
          mBlackBishops{0},
          mBlackRooks{0},
          mBlackQueen{0},
          mBlackKing{0},
          mWhiteAttackedSquares{},
          mBlackAttackedSquares{},
          mRemovedPiece{invalid},
          mRemovedPiecePosition{-1, invalid},
          mRemovedPieceLastMove{false},
          mLastPieceMoved{invalid},
          mLastMovedStartMove{-1, invalid},
          mLastMovedEndMove{-1, invalid}
    {
        ResetToStartPosition();
    }

    int ChessState::ConvertRankToCol(char rank)
    {
        if(rank < 'a' || rank > 'h') return -1;
        return static_cast<int>(rank - 'a' + 1);
    }

    void ChessState::UpdateAttackedSquare()
    {
        mWhiteAttackedSquares.clear();
        mBlackAttackedSquares.clear();

        // white pawn attacked squares
        for(auto coordinate : GetPiecePosiiton(whitePawn))
        {
            ChessCoordinate left = ChessCoordinate{coordinate.rank + 1, (char)(coordinate.file - 1)};
            ChessCoordinate right = ChessCoordinate{coordinate.rank + 1, (char)(coordinate.file + 1)};

            if(left.isValid())mWhiteAttackedSquares.insert(left);
            if(right.isValid())mWhiteAttackedSquares.insert(right);
        }

        // white Knight attacked squares
        for(auto coordinate : GetPiecePosiiton(whiteKnight))
        {
            ChessCoordinate forwardLeft = ChessCoordinate{coordinate.rank + 2, (char)(coordinate.file - 1)};
            ChessCoordinate forwardRight = ChessCoordinate{coordinate.rank + 2, (char)(coordinate.file + 1)};

            ChessCoordinate backwardLeft = ChessCoordinate{coordinate.rank - 2, (char)(coordinate.file - 1)};
            ChessCoordinate backwardRight = ChessCoordinate{coordinate.rank - 2, (char)(coordinate.file + 1)};

            ChessCoordinate leftUp = ChessCoordinate{coordinate.rank + 1, (char)(coordinate.file - 2)};
            ChessCoordinate leftDown = ChessCoordinate{coordinate.rank - 1, (char)(coordinate.file - 2)};

            ChessCoordinate rightUp = ChessCoordinate{coordinate.rank + 1, (char)(coordinate.file + 2)};
            ChessCoordinate rightDown = ChessCoordinate{coordinate.rank - 1, (char)(coordinate.file + 2)};

            if(forwardLeft.isValid())mWhiteAttackedSquares.insert(forwardLeft);
            if(forwardRight.isValid())mWhiteAttackedSquares.insert(forwardRight);

            if(backwardLeft.isValid())mWhiteAttackedSquares.insert(backwardLeft);
            if(backwardRight.isValid())mWhiteAttackedSquares.insert(backwardRight);

            if(leftUp.isValid())mWhiteAttackedSquares.insert(leftUp);
            if(leftDown.isValid())mWhiteAttackedSquares.insert(leftDown);

            if(rightUp.isValid())mWhiteAttackedSquares.insert(rightUp);
            if(rightDown.isValid())mWhiteAttackedSquares.insert(rightDown);
        }

        // wihte bishop attacked squares
        for(auto startCoordinate : GetPiecePosiiton(whiteBishop))
        {
            int offsetRank[] = { 1, 1,-1,-1};
            int offsetFile[] = { 1,-1, 1,-1};
            for(int i = 0; i < 4; i++)
            {
                ChessCoordinate iter = ChessCoordinate{startCoordinate.rank + offsetRank[i], (char)(startCoordinate.file + offsetFile[i])};
                while(iter.isValid() && (ChessState::Get().GetPieceOnChessCoordinate(iter) == invalid || ChessState::Get().GetPieceOnChessCoordinate(iter) == blackKing))
                {
                    mWhiteAttackedSquares.insert(iter);
                    iter.file += offsetFile[i];
                    iter.rank += offsetRank[i];
                }
                if(iter.isValid()) mWhiteAttackedSquares.insert(iter);
            }
        }

        // white Rook attacked squares
        for(auto startCoordinate : GetPiecePosiiton(whiteRook))
        {
            int offsetRank[] = { 0, 0, 1,-1};
            int offsetFile[] = { 1,-1, 0, 0};
            for(int i = 0; i < 4; i++)
            {
                ChessCoordinate iter = ChessCoordinate{startCoordinate.rank + offsetRank[i],(char) (startCoordinate.file + offsetFile[i])};
                while(iter.isValid() && (ChessState::Get().GetPieceOnChessCoordinate(iter) == invalid || ChessState::Get().GetPieceOnChessCoordinate(iter) == blackKing))
                {
                    mWhiteAttackedSquares.insert(iter);
                    iter.file += offsetFile[i];
                    iter.rank += offsetRank[i];
                }
                if(iter.isValid()) mWhiteAttackedSquares.insert(iter);
            }
        }

        // white Queen attacked squares
        for(auto startCoordinate : GetPiecePosiiton(whiteQueen))
        {
            int offsetRank[] = { 0, 0, 1,-1, 1, 1,-1,-1};
            int offsetFile[] = { 1,-1, 0, 0, 1,-1, 1,-1};
            for(int i = 0; i < 8; i++)
            {
                ChessCoordinate iter = ChessCoordinate{startCoordinate.rank + offsetRank[i], (char)(startCoordinate.file + offsetFile[i])};
                while(iter.isValid() && (ChessState::Get().GetPieceOnChessCoordinate(iter) == invalid || ChessState::Get().GetPieceOnChessCoordinate(iter) == blackKing) )
                {
                    mWhiteAttackedSquares.insert(iter);
                    iter.file += offsetFile[i];
                    iter.rank += offsetRank[i];
                }
                if(iter.isValid()) mWhiteAttackedSquares.insert(iter);
            }
        }

        // white King attacked squares
        for(auto startCoordinate : GetPiecePosiiton(whiteKing))
        {
            int offsetRank[] = { 0, 0, 1,-1, 1, 1,-1,-1};
            int offsetFile[] = { 1,-1, 0, 0, 1,-1, 1,-1};
            for(int i = 0; i < 8; i++)
            {
                ChessCoordinate iter = ChessCoordinate{startCoordinate.rank + offsetRank[i], (char)(startCoordinate.file + offsetFile[i])};
                if(iter.isValid()) mWhiteAttackedSquares.insert(iter);
            }
        }

        //Black pieces
        // black pawn attacked squares
        for(auto coordinate : GetPiecePosiiton(blackPawn))
        {
            ChessCoordinate left = ChessCoordinate{coordinate.rank - 1, (char)(coordinate.file - 1)};
            ChessCoordinate right = ChessCoordinate{coordinate.rank - 1, (char)(coordinate.file + 1)};

            if(left.isValid())mBlackAttackedSquares.insert(left);
            if(right.isValid())mBlackAttackedSquares.insert(right);
        }

        // black Knight attacked squares
        for(auto coordinate : GetPiecePosiiton(blackKnight))
        {
            ChessCoordinate forwardLeft = ChessCoordinate{coordinate.rank + 2, (char)(coordinate.file - 1)};
            ChessCoordinate forwardRight = ChessCoordinate{coordinate.rank + 2, (char)(coordinate.file + 1)};

            ChessCoordinate backwardLeft = ChessCoordinate{coordinate.rank - 2, (char)(coordinate.file - 1)};
            ChessCoordinate backwardRight = ChessCoordinate{coordinate.rank - 2, (char)(coordinate.file + 1)};

            ChessCoordinate leftUp = ChessCoordinate{coordinate.rank + 1, (char)(coordinate.file - 2)};
            ChessCoordinate leftDown = ChessCoordinate{coordinate.rank - 1, (char)(coordinate.file - 2)};

            ChessCoordinate rightUp = ChessCoordinate{coordinate.rank + 1, (char)(coordinate.file + 2)};
            ChessCoordinate rightDown = ChessCoordinate{coordinate.rank - 1, (char)(coordinate.file + 2)};

            if(forwardLeft.isValid())mBlackAttackedSquares.insert(forwardLeft);
            if(forwardRight.isValid())mBlackAttackedSquares.insert(forwardRight);

            if(backwardLeft.isValid())mBlackAttackedSquares.insert(backwardLeft);
            if(backwardRight.isValid())mBlackAttackedSquares.insert(backwardRight);

            if(leftUp.isValid())mBlackAttackedSquares.insert(leftUp);
            if(leftDown.isValid())mBlackAttackedSquares.insert(leftDown);

            if(rightUp.isValid())mBlackAttackedSquares.insert(rightUp);
            if(rightDown.isValid())mBlackAttackedSquares.insert(rightDown);
        }

        // black bishop attacked squares
        for(auto startCoordinate : GetPiecePosiiton(blackBishop))
        {
            int offsetRank[] = { 1, 1,-1,-1};
            int offsetFile[] = { 1,-1, 1,-1};
            for(int i = 0; i < 4; i++)
            {
                ChessCoordinate iter = ChessCoordinate{startCoordinate.rank + offsetRank[i], (char)(startCoordinate.file + offsetFile[i])};
                while(iter.isValid() && (ChessState::Get().GetPieceOnChessCoordinate(iter) == invalid || ChessState::Get().GetPieceOnChessCoordinate(iter) == whiteKing) )
                {
                    mBlackAttackedSquares.insert(iter);
                    iter.file += offsetFile[i];
                    iter.rank += offsetRank[i];
                }
                if(iter.isValid()) mBlackAttackedSquares.insert(iter);
            }
        }

        // black Rook attacked squares
        for(auto startCoordinate : GetPiecePosiiton(blackRook))
        {
            int offsetRank[] = { 0, 0, 1,-1};
            int offsetFile[] = { 1,-1, 0, 0};
            for(int i = 0; i < 4; i++)
            {
                ChessCoordinate iter = ChessCoordinate{startCoordinate.rank + offsetRank[i],(char) (startCoordinate.file + offsetFile[i])};
                while(iter.isValid() && (ChessState::Get().GetPieceOnChessCoordinate(iter) == invalid || ChessState::Get().GetPieceOnChessCoordinate(iter) == whiteKing) )
                {
                    mBlackAttackedSquares.insert(iter);
                    iter.file += offsetFile[i];
                    iter.rank += offsetRank[i];
                }
                if(iter.isValid()) mBlackAttackedSquares.insert(iter);
            }
        }

        // black Queen attacked squares
        for(auto startCoordinate : GetPiecePosiiton(blackQueen))
        {
            int offsetRank[] = { 0, 0, 1,-1, 1, 1,-1,-1};
            int offsetFile[] = { 1,-1, 0, 0, 1,-1, 1,-1};
            for(int i = 0; i < 8; i++)
            {
                ChessCoordinate iter = ChessCoordinate{startCoordinate.rank + offsetRank[i], (char)(startCoordinate.file + offsetFile[i])};
                while(iter.isValid() && (ChessState::Get().GetPieceOnChessCoordinate(iter) == invalid || ChessState::Get().GetPieceOnChessCoordinate(iter) == whiteKing) )
                {
                    mBlackAttackedSquares.insert(iter);
                    iter.file += offsetFile[i];
                    iter.rank += offsetRank[i];
                }
                if(iter.isValid()) mBlackAttackedSquares.insert(iter);
            }
        }

        // black King attacked squares
        for(auto startCoordinate : GetPiecePosiiton(blackKing))
        {
            int offsetRank[] = { 0, 0, 1,-1, 1, 1,-1,-1};
            int offsetFile[] = { 1,-1, 0, 0, 1,-1, 1,-1};
            for(int i = 0; i < 8; i++)
            {
                ChessCoordinate iter = ChessCoordinate{startCoordinate.rank + offsetRank[i], (char)(startCoordinate.file + offsetFile[i])};
                if(iter.isValid()) mBlackAttackedSquares.insert(iter);
            }
        }
    }

    void ChessState::SpawnPiece(char piece, ChessCoordinate &position)
    {
        uint64_t& pieceContainer = GetPieceContainer(piece);
        if(!position.isValid()) return;

        // Set the end position bit
        pieceContainer |= 1ULL << (8 * (position.rank - 1) + (7- ConvertRankToCol(position.file)+1));
    }

    uint64_t &ChessState::GetPieceContainer(char piece)
    {
        switch (piece)
        {
        case whitePawn:
            return mWhitePawns;
        case whiteBishop:
            return mWhiteBishops;
        case whiteKnight:
            return mWhiteKnights;
        case whiteRook:
            return mWhiteRooks;
        case whiteQueen:
            return mWhiteQueen;
        case whiteKing:
            return mWhiteKing;
        case blackPawn:
            return mBlackPawns;
        case blackBishop:
            return mBlackBishops;
        case blackKnight:
            return mBlackKnights;
        case blackRook:
            return mBlackRooks;
        case blackQueen:
            return mBlackQueen;
        case blackKing:
            return mBlackKing;
        }
        return UINT64_MAX_VALUE;
    }

    char ChessState::ConvertColToRank(int col)
    {
        if(col < 1 && col > 8) return invalid;
        return static_cast<char>('a' + col - 1) ;
    }
}