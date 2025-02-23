#include"framework/ChessState.h"
#include<memory>

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

        // Unset the current high bit
        pieceContainer ^= currentPos;

        // Set the end position bit
        pieceContainer |= 1ULL << (8 * (end.rank - 1) + (7- ConvertRankToCol(end.file)+1));
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
          mBlackKing{0}
    {
        ResetToStartPosition();
    }

    int ChessState::ConvertRankToCol(char rank)
    {
        if(rank < 'a' || rank > 'h') return -1;
        return static_cast<int>(rank - 'a' + 1);
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
    }

    char ChessState::ConvertColToRank(int col)
    {
        if(col < 1 && col > 8) return invalid;
        return static_cast<char>('a' + col - 1) ;
    }
}