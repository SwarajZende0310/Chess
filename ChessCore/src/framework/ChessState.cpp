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
            mBlackPawns |= (1ULL << ( 8 * (row-1) + 8 * i));
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

    List<ChessCoordinate> ChessState::GetWhitePawnsPosition()
    {
        List<ChessCoordinate> pawns;
        pawns.reserve(8);
        for(int i = 0; i < 8; i++)
        {
            for(int j = 7; j>=0; j--)
            {
                if( (1ULL << ( (8 * i) + j)) & mWhitePawns)
                {
                    // ChessCoordinate chessCoordinate;
                    // chessCoordinate.rank = 7 - j;
                    // chessCoordinate.file = ConvertColToRank( i + 1 );

                    pawns.emplace_back(ChessCoordinate{ i + 1 , ConvertColToRank(7 - j + 1)});
                }
            }
        }
        return pawns;
    }

    List<ChessCoordinate> ChessState::GetWhiteKnightsPosition()
    {
        List<ChessCoordinate> knights;
        knights.reserve(2);
        for(int i = 0; i < 8; i++)
        {
            for(int j = 7; j>=0; j--)
            {
                if( (1ULL << ( (8 * i) + j)) & mWhiteKnights)
                {
                    knights.emplace_back(ChessCoordinate{ i + 1 , ConvertColToRank(7 - j + 1)});
                }
            }
        }
        return knights;
    }

    List<ChessCoordinate> ChessState::GetWhiteBishopsPosition()
    {
        List<ChessCoordinate> bishops;
        bishops.reserve(2);
        for(int i = 0; i < 8; i++)
        {
            for(int j = 7; j>=0; j--)
            {
                if( (1ULL << ( (8 * i) + j)) & mWhiteBishops)
                {
                    bishops.emplace_back(ChessCoordinate{ i + 1 , ConvertColToRank(7 - j + 1)});
                }
            }
        }
        return bishops;
    }

    List<ChessCoordinate> ChessState::GetWhiteRooksPosition()
    {
        List<ChessCoordinate> rooks;
        rooks.reserve(2);
        for(int i = 0; i < 8; i++)
        {
            for(int j = 7; j>=0; j--)
            {
                if( (1ULL << ( (8 * i) + j)) & mWhiteRooks)
                {
                    rooks.emplace_back(ChessCoordinate{ i + 1 , ConvertColToRank(7 - j + 1)});
                }
            }
        }
        return rooks;
    }

    ChessCoordinate ChessState::GetWhiteQueenPosition()
    {
        for(int i = 0; i < 8; i++)
        {
            for(int j = 7; j>=0; j--)
            {
                if( (1ULL << ( (8 * i) + j)) & mWhiteQueen)
                {
                    return ChessCoordinate{ i + 1 , ConvertColToRank(7 - j + 1)};
                }
            }
        }
        return ChessCoordinate{ -1, ConvertColToRank(-1)};
    }

    ChessCoordinate ChessState::GetWhiteKingPosition()
    {
        for(int i = 0; i < 8; i++)
        {
            for(int j = 7; j>=0; j--)
            {
                if( (1ULL << ( (8 * i) + j)) & mWhiteKing)
                {
                    return ChessCoordinate{ i + 1 , ConvertColToRank(7 - j + 1)};
                }
            }
        }
        return ChessCoordinate{ -1, ConvertColToRank(-1)};
    }

    List<ChessCoordinate> ChessState::GetBlackPawnsPosition()
    {
        List<ChessCoordinate> pawns;
        pawns.reserve(8);
        for(int i = 0; i < 8; i++)
        {
            for(int j = 7; j>=0; j--)
            {
                if( (1ULL << ( (8 * i) + j)) & mBlackPawns)
                {
                    pawns.emplace_back(ChessCoordinate{ i + 1 , ConvertColToRank(7 - j + 1)});
                }
            }
        }
        return pawns;
    }

    List<ChessCoordinate> ChessState::GetBlackKnightsPosition()
    {
        List<ChessCoordinate> knights;
        knights.reserve(2);
        for(int i = 0; i < 8; i++)
        {
            for(int j = 7; j>=0; j--)
            {
                if( (1ULL << ( (8 * i) + j)) & mBlackKnights)
                {
                    knights.emplace_back(ChessCoordinate{ i + 1 , ConvertColToRank(7 - j + 1)});
                }
            }
        }
        return knights;
    }

    List<ChessCoordinate> ChessState::GetBlackBishopsPosition()
    {
        List<ChessCoordinate> bishops;
        bishops.reserve(2);
        for(int i = 0; i < 8; i++)
        {
            for(int j = 7; j>=0; j--)
            {
                if( (1ULL << ( (8 * i) + j)) & mBlackBishops)
                {
                    bishops.emplace_back(ChessCoordinate{ i + 1 , ConvertColToRank(7 - j + 1)});
                }
            }
        }
        return bishops;
    }

    List<ChessCoordinate> ChessState::GetBlackRooksPosition()
    {
        List<ChessCoordinate> rooks;
        rooks.reserve(2);
        for(int i = 0; i < 8; i++)
        {
            for(int j = 7; j>=0; j--)
            {
                if( (1ULL << ( (8 * i) + j)) & mBlackRooks)
                {
                    rooks.emplace_back(ChessCoordinate{ i + 1 , ConvertColToRank(7 - j + 1)});
                }
            }
        }
        return rooks;
    }

    ChessCoordinate ChessState::GetBlackQueenPosition()
    {
        for(int i = 0; i < 8; i++)
        {
            for(int j = 7; j>=0; j--)
            {
                if( (1ULL << ( (8 * i) + j)) & mBlackQueen)
                {
                    return ChessCoordinate{ i + 1 , ConvertColToRank(7 - j + 1)};
                }
            }
        }
        return ChessCoordinate{ -1, ConvertColToRank(-1)};
    }

    ChessCoordinate ChessState::GetBlackKingPosition()
    {
        for(int i = 0; i < 8; i++)
        {
            for(int j = 7; j>=0; j--)
            {
                if( (1ULL << ( (8 * i) + j)) & mBlackKing)
                {
                    return ChessCoordinate{ i + 1 , ConvertColToRank(7 - j + 1)};
                }
            }
        }
        return ChessCoordinate{ -1, ConvertColToRank(-1)};
    }

    void ChessState::SetWhitePawnPosition(ChessCoordinate &start, ChessCoordinate &end)
    {
        if(start.file == 'n' || start.rank == -1 || end.file == 'n' || end.rank == -1) return;
        uint64_t currentPos =  1ULL << (8 * (start.rank - 1) + (7- ConvertRankToCol(start.file)));
        if(!(mWhitePawns & currentPos))return;

        // Unset the current high bit
        mWhitePawns ^= currentPos;

        // Set the end position bit
        mWhitePawns |= 1ULL << (8 * (end.rank - 1) + (7- ConvertRankToCol(end.file)));
    }

    void ChessState::SetWhiteKnightPosition(ChessCoordinate &start, ChessCoordinate &end)
    {
        if(start.file == 'n' || start.rank == -1 || end.file == 'n' || end.rank == -1) return;
        uint64_t currentPos =  (1ULL << (8 * (start.rank - 1) + (7- ConvertRankToCol(start.file))));
        if(!(mWhiteKnights & currentPos))return;

        // Unset the current high bit
        mWhiteKnights ^= currentPos;

        // Set the end position bit
        mWhiteKnights |= 1ULL << (8 * (end.rank - 1) + (7- ConvertRankToCol(end.file)));
    }

    void ChessState::SetWhiteBishopPosition(ChessCoordinate &start, ChessCoordinate &end)
    {
        if(start.file == 'n' || start.rank == -1 || end.file == 'n' || end.rank == -1) return;
        uint64_t currentPos =  (1ULL << (8 * (start.rank - 1) + (7- ConvertRankToCol(start.file))));
        if(!(mWhiteBishops & currentPos))return;

        // Unset the current high bit
        mWhiteBishops ^= currentPos;

        // Set the end position bit
        mWhiteBishops |= 1ULL << (8 * (end.rank - 1) + (7- ConvertRankToCol(end.file)));
    }

    void ChessState::SetWhiteRookPosition(ChessCoordinate &start, ChessCoordinate &end)
    {
        if(start.file == 'n' || start.rank == -1 || end.file == 'n' || end.rank == -1) return;
        uint64_t currentPos =  (1ULL << (8 * (start.rank - 1) + (7- ConvertRankToCol(start.file))));

        if(!(mWhiteRooks & currentPos))return;

        // Unset the current high bit
        mWhiteRooks ^= currentPos;

        // Set the end position bit
        mWhiteRooks |= 1ULL << (8 * (end.rank - 1) + (7- ConvertRankToCol(end.file)));
    }

    void ChessState::SetWhiteQueenPosition(ChessCoordinate &start, ChessCoordinate &end)
    {
        if(start.file == 'n' || start.rank == -1 || end.file == 'n' || end.rank == -1) return;
        uint64_t currentPos =  (1ULL << (8 * (start.rank - 1) + (7- ConvertRankToCol(start.file))));
        if(!(mWhiteQueen & currentPos))return;

        // Unset the current high bit
        mWhiteQueen ^= currentPos;

        // Set the end position bit
        mWhiteQueen |= 1ULL << (8 * (end.rank - 1) + (7- ConvertRankToCol(end.file)));
    }

    void ChessState::SetWhiteKingPosition(ChessCoordinate &start, ChessCoordinate &end)
    {
        if(start.file == 'n' || start.rank == -1 || end.file == 'n' || end.rank == -1) return;
        uint64_t currentPos =  (1ULL << (8 * (start.rank - 1) + (7- ConvertRankToCol(start.file))));
        if(!(mWhiteKing & currentPos))return;

        // Unset the current high bit
        mWhiteKing ^= currentPos;

        // Set the end position bit
        mWhiteKing |= 1ULL << (8 * (end.rank - 1) + (7- ConvertRankToCol(end.file)));
    }

    void ChessState::SetBlackPawnPosition(ChessCoordinate &start, ChessCoordinate &end)
    {
        if(start.file == 'n' || start.rank == -1 || end.file == 'n' || end.rank == -1) return;
        uint64_t currentPos =  (1ULL << (8 * (start.rank - 1) + (7- ConvertRankToCol(start.file))));
        if(!(mBlackPawns & currentPos))return;

        // Unset the current high bit
        mBlackPawns ^= currentPos;

        // Set the end position bit
        mBlackPawns |= 1ULL << (8 * (end.rank - 1) + (7- ConvertRankToCol(end.file)));
    }

    void ChessState::SetBlackKnightPosition(ChessCoordinate &start, ChessCoordinate &end)
    {
        if(start.file == 'n' || start.rank == -1 || end.file == 'n' || end.rank == -1) return;
        uint64_t currentPos =  (1ULL << (8 * (start.rank - 1) + (7- ConvertRankToCol(start.file))));
        if(!(mBlackKnights & currentPos))return;

        // Unset the current high bit
        mBlackKnights ^= currentPos;

        // Set the end position bit
        mBlackKnights |= 1ULL << (8 * (end.rank - 1) + (7- ConvertRankToCol(end.file)));
    }

    void ChessState::SetBlackBishopPosition(ChessCoordinate &start, ChessCoordinate &end)
    {
        if(start.file == 'n' || start.rank == -1 || end.file == 'n' || end.rank == -1) return;
        uint64_t currentPos =  (1ULL << (8 * (start.rank - 1) + (7- ConvertRankToCol(start.file))));
        if(!(mBlackBishops & currentPos))return;

        // Unset the current high bit
        mBlackBishops ^= currentPos;

        // Set the end position bit
        mBlackBishops |= 1ULL << (8 * (end.rank - 1) + (7- ConvertRankToCol(end.file)));
    }

    void ChessState::SetBlackRookPosition(ChessCoordinate &start, ChessCoordinate &end)
    {
        if(start.file == 'n' || start.rank == -1 || end.file == 'n' || end.rank == -1) return;
        uint64_t currentPos =  (1ULL << (8 * (start.rank - 1) + (7- ConvertRankToCol(start.file))));
        if(!(mBlackRooks & currentPos))return;

        // Unset the current high bit
        mBlackRooks ^= currentPos;

        // Set the end position bit
        mBlackRooks |= 1ULL << (8 * (end.rank - 1) + (7- ConvertRankToCol(end.file)));
    }

    void ChessState::SetBlackQueenPosition(ChessCoordinate &start, ChessCoordinate &end)
    {
        if(start.file == 'n' || start.rank == -1 || end.file == 'n' || end.rank == -1) return;
        uint64_t currentPos =  (1ULL << (8 * (start.rank - 1) + (7- ConvertRankToCol(start.file))));
        if(!(mBlackQueen & currentPos))return;

        // Unset the current high bit
        mBlackQueen ^= currentPos;

        // Set the end position bit
        mBlackQueen |= 1ULL << (8 * (end.rank - 1) + (7- ConvertRankToCol(end.file)));
    }

    void ChessState::SetBlackKingPosition(ChessCoordinate &start, ChessCoordinate &end)
    {
        if(start.file == 'n' || start.rank == -1 || end.file == 'n' || end.rank == -1) return;
        uint64_t currentPos =  (1ULL << (8 * (start.rank - 1) + (7- ConvertRankToCol(start.file))));
        if(!(mBlackKing & currentPos))return;

        // Unset the current high bit
        mBlackKing ^= currentPos;

        // Set the end position bit
        mBlackKing |= 1ULL << (8 * (end.rank - 1) + (7- ConvertRankToCol(end.file)));
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

    char ChessState::ConvertColToRank(int col)
    {
        if(col < 1 && col > 8) return 'n';
        return static_cast<char>('a' + col - 1) ;
    }
}