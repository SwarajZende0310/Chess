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
        int row = 7; //Seventh row from bottom
        for(int i = 7; i >= 0; i--)
        {
            mWhitePawns |= (1 << ( 8 * (row-1) + i));
        }

        row = 1;
        //White Rooks
        mWhiteRooks |= (1 << ( 8 * (row-1) + 0));
        mWhiteRooks |= (1 << ( 8 * (row-1) + 7));

        //White Knights
        mWhiteKnights |= (1 << ( 8 * (row-1) + 1));
        mWhiteKnights |= (1 << ( 8 * (row-1) + 6));

        //White Bishops
        mWhiteBishops |= (1 << ( 8 * (row-1) + 2));
        mWhiteBishops |= (1 << ( 8 * (row-1) + 5));

        //White Queen
        mWhiteQueen |= (1 << ( 8 * (row-1) + 4));

        //White King
        mWhiteKing |= (1 << ( 8 * (row-1) + 3));

        //Black Pawns
        row = 7;
        for(int i = 7; i >= 0; i--)
        {
            mBlackPawns |= (1 << ( 8 * (row-1) + 8 * i));
        }

        row = 1;
        //Black Rooks
        mBlackRooks |= (1 << ( 8 * (row-1) + 0));
        mBlackRooks |= (1 << ( 8 * (row-1) + 7));

        //Black Knights
        mBlackKnights |= (1 << ( 8 * (row-1) + 1));
        mBlackKnights |= (1 << ( 8 * (row-1) + 6));

        //Black Bishops
        mBlackBishops |= (1 << ( 8 * (row-1) + 2));
        mBlackBishops |= (1 << ( 8 * (row-1) + 5));

        //Black Queen
        mBlackQueen |= (1 << ( 8 * (row-1) + 4));

        //Black King
        mBlackKing |= (1 << ( 8 * (row-1) + 3));
    }

    List<ChessCoordinate> ChessState::GetWhitePawnsPosition()
    {
        List<ChessCoordinate> pawns;
        pawns.reserve(8);
        for(int i = 0; i < 8; i++)
        {
            for(int j = 7; j>=0; j--)
            {
                if( (1 << ( (8 * i) + j)) & mWhitePawns)
                {
                    // ChessCoordinate chessCoordinate;
                    // chessCoordinate.rank = 7 - j;
                    // chessCoordinate.file = ConvertColtoRank( i + 1 );

                    pawns.emplace_back(ChessCoordinate{ ConvertColtoRank(i+1) , 7 - j });
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
                if( (1 << ( (8 * i) + j)) & mWhiteKnights)
                {
                    knights.emplace_back(ChessCoordinate{ ConvertColtoRank(i+1) , 7 - j });
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
                if( (1 << ( (8 * i) + j)) & mWhiteBishops)
                {
                    bishops.emplace_back(ChessCoordinate{ ConvertColtoRank(i+1) , 7 - j });
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
                if( (1 << ( (8 * i) + j)) & mWhiteRooks)
                {
                    rooks.emplace_back(ChessCoordinate{ ConvertColtoRank(i+1) , 7 - j });
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
                if( (1 << ( (8 * i) + j)) & mWhiteQueen)
                {
                    return ChessCoordinate{ ConvertColtoRank(i+1) , 7 - j };
                }
            }
        }
        return ChessCoordinate{ ConvertColtoRank(-1), -1};
    }

    ChessCoordinate ChessState::GetWhiteKingPosition()
    {
        for(int i = 0; i < 8; i++)
        {
            for(int j = 7; j>=0; j--)
            {
                if( (1 << ( (8 * i) + j)) & mWhiteKing)
                {
                    return ChessCoordinate{ ConvertColtoRank(i+1) , 7 - j };
                }
            }
        }
        return ChessCoordinate{ ConvertColtoRank(-1), -1};
    }

    List<ChessCoordinate> ChessState::GetBlackPawnsPosition()
    {
        List<ChessCoordinate> pawns;
        pawns.reserve(8);
        for(int i = 0; i < 8; i++)
        {
            for(int j = 7; j>=0; j--)
            {
                if( (1 << ( (8 * i) + j)) & mBlackPawns)
                {
                    pawns.emplace_back(ChessCoordinate{ ConvertColtoRank(i+1) , 7 - j });
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
                if( (1 << ( (8 * i) + j)) & mBlackKnights)
                {
                    knights.emplace_back(ChessCoordinate{ ConvertColtoRank(i+1) , 7 - j });
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
                if( (1 << ( (8 * i) + j)) & mBlackBishops)
                {
                    bishops.emplace_back(ChessCoordinate{ ConvertColtoRank(i+1) , 7 - j });
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
                if( (1 << ( (8 * i) + j)) & mBlackRooks)
                {
                    rooks.emplace_back(ChessCoordinate{ ConvertColtoRank(i+1) , 7 - j });
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
                if( (1 << ( (8 * i) + j)) & mBlackQueen)
                {
                    return ChessCoordinate{ ConvertColtoRank(i+1) , 7 - j };
                }
            }
        }
        return ChessCoordinate{ ConvertColtoRank(-1), -1};
    }

    ChessCoordinate ChessState::GetBlackKingPosition()
    {
        for(int i = 0; i < 8; i++)
        {
            for(int j = 7; j>=0; j--)
            {
                if( (1 << ( (8 * i) + j)) & mWhiteKing)
                {
                    return ChessCoordinate{ ConvertColtoRank(i+1) , 7 - j };
                }
            }
        }
        return ChessCoordinate{ ConvertColtoRank(-1), -1};
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

    char ChessState::ConvertColtoRank(int row)
    {
        if(row < 1 && row > 8) return 'n';
        return static_cast<char>('a' + row - 1) ;
    }
}