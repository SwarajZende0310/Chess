/**
 * @file ChessState.cpp
 * @brief Implementation of bitboard-based chess state, move logging, and queries.
 */
#include"framework/ChessState.h"

namespace chess
{
    unique<ChessState> ChessState::mChessState{nullptr};
    /**
     * @brief Get the singleton instance of `ChessState`.
     *
     * Lazily constructs the instance on first use. The state holds bitboards
     * for all pieces, move history, and helper maps (e.g., first-move flags).
     * @return Reference to the global `ChessState`.
     */
    ChessState& ChessState::Get()
    {
        if(!mChessState)
        {
            mChessState = std::move(unique<ChessState>{new ChessState});
        }
        return *mChessState;
    }

    /**
     * @brief Reset all bitboards and state to the initial chess position.
     *
     * Clears move logs and first-move flags, then sets up all piece bitboards
     * to their standard starting squares. Also resets attacked-square maps.
     */
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

        // Initialise evrything to Zero
        mWhitePawns = 0;
        mWhiteBishops = 0;
        mWhiteKnights = 0;
        mWhiteRooks = 0;
        mWhiteQueen = 0;
        mWhiteKing = 0;

        mBlackPawns = 0;
        mBlackBishops = 0;
        mBlackKnights = 0;
        mBlackRooks = 0;
        mBlackQueen = 0;
        mBlackKing = 0;

        mWhiteAttackedSquares.clear();
        mBlackAttackedSquares.clear();
        mMovesPlayed.clear();
        mFirstMove.clear();

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

        // Set first move to true
        mFirstMove.clear();
        for(int i = 0; i < 8; i++)
        {
            mFirstMove[ChessCoordinate{1,char('a'+i)}] = true;
            mFirstMove[ChessCoordinate{2,char('a'+i)}] = true;
            mFirstMove[ChessCoordinate{7,char('a'+i)}] = true;
            mFirstMove[ChessCoordinate{8,char('a'+i)}] = true;
        }
    }

    /**
     * @brief Collect coordinates of all pieces of a given type.
     * @param piece One of the defined piece constants (e.g., whitePawn).
     * @return List of board coordinates where the piece currently exists.
     */
    List<ChessCoordinate> ChessState::GetPiecePosiiton(PieceType piece)
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

    /**
     * @brief Move a piece from start to end, updating bitboards and logs.
     *
     * Handles captures (including en passant), castling flags, bitboard
     * updates, and move history logging. Also updates first-move map and
     * attacked squares.
     * @param piece Piece identifier.
     * @param start Start coordinate (must be valid and contain the piece).
     * @param end Destination coordinate (must be valid).
     * @param log If true, records the move into history and updates flags.
     */
    void ChessState::SetPiecePosition(PieceType piece, ChessCoordinate &start, ChessCoordinate &end,bool log)
    {
        uint64_t& pieceContainer = GetPieceContainer(piece);
        if(start.file == -1 || start.rank == -1 || end.file == -1 || end.rank == -1) return;
        uint64_t currentPos =  1ULL << (8 * (start.rank - 1) + (7- ConvertRankToCol(start.file)+1));
        if(!(pieceContainer & currentPos))return;
        PlayedMove move;

        // Remove other piece if already there in position where we are moving or Enpassant played
        if(ChessState::Get().GetPieceOnChessCoordinate(end) != PieceType::invalid
            || ( piece == PieceType::whitePawn && abs(start.file - end.file) == 1 && (end.rank - start.rank) > 0 ) 
            || ( piece == PieceType::blackPawn && abs(start.file - end.file) == 1 && (end.rank - start.rank) < 0 ) )
        {
            PieceType endPiece = ChessState::Get().GetPieceOnChessCoordinate(end);
            if(endPiece == PieceType::invalid)
            {
                // Enpassant move
                move.mCapturedPieceCoordinate = ChessCoordinate{start.rank,end.file};
                move.mCapturedPiece = GetPieceOnChessCoordinate(move.mCapturedPieceCoordinate);
            }
            else
            {
                move.mCapturedPiece = endPiece;
                move.mCapturedPieceCoordinate = end;
            }
            ChessState::Get().RemovePiece(move.mCapturedPiece, move.mCapturedPieceCoordinate);
        }

        // Check for castling
        if((piece == PieceType::whiteKing || piece == PieceType::blackKing) && abs(start.file - end.file) > 1 )
        {
            move.mCastling = end.file - start.file > 0 ? CastlingState::KingSide : CastlingState::QueenSide;
        }

        // Unset the current high bit
        pieceContainer ^= currentPos;

        // Set the end position bit
        pieceContainer |= 1ULL << (8 * (end.rank - 1) + (7- ConvertRankToCol(end.file)+1));

        // Updating moves
        move.mPiece = piece;
        move.mStartCoordinate = start;
        move.mEndCoordinate = end;
        if(log)
        {
            mFirstMove[end] = false;
            mMovesPlayed.emplace_back(move);
        }    

        UpdateAttackedSquare();
    }

    /**
     * @brief Undo the last move from history and restore the previous state.
     *
     * Restores captured pieces, reverses castling if needed, resets first-move
     * flags and positions, and decrements counters accordingly.
     * @return true if a move was undone; false if history is empty or invalid.
     */
    bool ChessState::UndoLastMove()
    {
        if(mMovesPlayed.size() == 0) return false;
        PlayedMove LastMove = mMovesPlayed.back();
        mMovesPlayed.pop_back();

        if(!LastMove.mStartCoordinate.isValid() || !LastMove.mEndCoordinate.isValid())return false;

        // Undoing castling
        if(LastMove.mCastling != CastlingState::NoCastling)
        {
            if(LastMove.mPiece == PieceType::whiteKing)
            {
                ChessCoordinate kingCoordinateEnd = ChessCoordinate{1,'e'};
                ChessCoordinate kingCoordinateStart = ChessCoordinate{1,'g'};
                ChessCoordinate rookCoordinateEnd = ChessCoordinate{1,'h'};
                ChessCoordinate rookCoordinateStart = ChessCoordinate{1,'f'};
                if(LastMove.mCastling == CastlingState::KingSide)
                {
                    SetPiecePosition(PieceType::whiteKing,kingCoordinateStart,kingCoordinateEnd,false);
                    SetPiecePosition(PieceType::whiteRook,rookCoordinateStart,rookCoordinateEnd,false);
                }
                else if(LastMove.mCastling == CastlingState::QueenSide)
                {
                    kingCoordinateStart.file = 'c';
                    rookCoordinateStart.file = 'd';
                    rookCoordinateEnd.file = 'a';
                    SetPiecePosition(PieceType::whiteKing,kingCoordinateStart,kingCoordinateEnd,false);
                    SetPiecePosition(PieceType::whiteRook,rookCoordinateStart,rookCoordinateEnd,false);
                }
                mFirstMove[kingCoordinateEnd] = true;
                mFirstMove[rookCoordinateEnd] = true;
            }
            else if(LastMove.mPiece == PieceType::blackKing)
            {
                ChessCoordinate kingCoordinateEnd = ChessCoordinate{8,'e'};
                ChessCoordinate kingCoordinateStart = ChessCoordinate{8,'g'};
                ChessCoordinate rookCoordinateEnd = ChessCoordinate{8,'h'};
                ChessCoordinate rookCoordinateStart = ChessCoordinate{8,'f'};
                if(LastMove.mCastling == CastlingState::KingSide)
                {
                    SetPiecePosition(PieceType::blackKing,kingCoordinateStart,kingCoordinateEnd,false);
                    SetPiecePosition(PieceType::blackRook,rookCoordinateStart,rookCoordinateEnd,false);
                }
                else if(LastMove.mCastling == CastlingState::QueenSide)
                {
                    kingCoordinateStart.file = 'c';
                    rookCoordinateStart.file = 'd';
                    rookCoordinateEnd.file = 'a';
                    SetPiecePosition(PieceType::blackKing,kingCoordinateStart,kingCoordinateEnd,false);
                    SetPiecePosition(PieceType::blackRook,rookCoordinateStart,rookCoordinateEnd,false);  
                }
                mFirstMove[kingCoordinateEnd] = true;
                mFirstMove[rookCoordinateEnd] = true;
            }
            mMovesWithoutCapture -=1;
            return true;
        }

        // Spawning removed piece
        if(LastMove.mCapturedPiece != PieceType::invalid && LastMove.mCapturedPieceCoordinate.isValid())
        {
            SpawnPiece(LastMove.mCapturedPiece,LastMove.mCapturedPieceCoordinate);
        }

        // reseting to previous position
        SetPiecePosition(LastMove.mPiece,LastMove.mEndCoordinate,LastMove.mStartCoordinate,false);
        return true;
    }

    /**
     * @brief Query the piece occupying a coordinate.
     * @param coordinate Board coordinate to check.
     * @return Piece identifier at the square, or `invalid` if empty.
     */
    PieceType ChessState::GetPieceOnChessCoordinate(ChessCoordinate coordinate)
    {
        uint64_t currentPos =  (1ULL << (8 * (coordinate.rank - 1) + (7- ConvertRankToCol(coordinate.file)+1)));
        
        if(mWhitePawns & currentPos)return PieceType::whitePawn;
        else if( mWhiteRooks & currentPos)return PieceType::whiteRook;
        else if( mWhiteKnights & currentPos)return PieceType::whiteKnight;
        else if( mWhiteBishops & currentPos)return PieceType::whiteBishop;
        else if( mWhiteQueen & currentPos)return PieceType::whiteQueen;
        else if( mWhiteKing & currentPos)return PieceType::whiteKing;

        else if(mBlackPawns & currentPos)return PieceType::blackPawn;
        else if( mBlackRooks & currentPos)return PieceType::blackRook;
        else if( mBlackKnights & currentPos)return PieceType::blackKnight;
        else if( mBlackBishops & currentPos)return PieceType::blackBishop;
        else if( mBlackQueen & currentPos)return PieceType::blackQueen;
        else if( mBlackKing & currentPos)return PieceType::blackKing;

        return PieceType::invalid;
    }

    /**
     * @brief Remove a specific piece from a board position.
     *
     * Clears the high bit for the position in the piece's bitboard if present.
     * @param piece Piece identifier to remove.
     * @param position The coordinate from which to remove the piece.
     */
    void ChessState::RemovePiece(PieceType piece, ChessCoordinate &position)
    {
        if(piece == PieceType::invalid || position.file == -1 || position.rank == -1)return;
        uint64_t& pieceContainer = GetPieceContainer(piece);
        uint64_t currentPos =  1ULL << (8 * (position.rank - 1) + (7- ConvertRankToCol(position.file)+1));
        if(!(pieceContainer & currentPos))return;

        currentPos = ~(currentPos);

        pieceContainer &= currentPos;
    }

    /**
     * @brief Determine whether the given side's king is in check.
     * @param white True to test white's king; false to test black's king.
     * @return true if the king is currently attacked.
     */
    bool ChessState::KingInCheck(bool white)
    {
        UpdateAttackedSquare();
        if(white)
        {
            return mBlackAttackedSquares.find(GetPiecePosiiton(PieceType::whiteKing)[0]) != mBlackAttackedSquares.end() ;
        }
        else
        {
            return mWhiteAttackedSquares.find(GetPiecePosiiton(PieceType::blackKing)[0]) != mWhiteAttackedSquares.end() ;
        }
        return false;
    }

    /**
     * @brief Get the start and end coordinates of the last move.
     * @return A list with two coordinates {start, end}, or empty if none.
     */
    List<ChessCoordinate> ChessState::GetLastPlayedMove()
    {
        if(mMovesPlayed.size() == 0)return {};
        return {mMovesPlayed.back().mStartCoordinate,mMovesPlayed.back().mEndCoordinate};
    }
    
    /**
     * @brief Count the number of pieces of a given type on the board.
     * @param piece Piece identifier (whitePawn, blackQueen, etc.).
     * @return Number of set bits in the corresponding bitboard.
     */
    int ChessState::GetPieceCount(PieceType piece)
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

    /**
     * @brief Check first-move flag for a coordinate (used for pawns/king/rook).
     * @param coordinate Target coordinate to query.
     * @return true if the piece at that coordinate has not moved yet.
     */
    bool ChessState::IsFirstMove(ChessCoordinate coordinate)
    {
        if(mFirstMove.count(coordinate) > 0)
        {
            return mFirstMove[coordinate];
        }
        return false;
    }

    /**
     * @brief Compute the number of moves since the last capture or pawn move.
     * @return Count of consecutive non-capturing, non-pawn moves.
     */
    int ChessState::GetMovesWithoutCapture()
    {
        // Iterate through played moves and check for pawn moves or piece captures
        int moveCount = 0;
        for(int i = 0; i < mMovesPlayed.size(); i++)
        {
            PlayedMove move = mMovesPlayed[i];
            moveCount = (move.mCapturedPiece != PieceType::invalid || move.mPiece == PieceType::whitePawn || move.mPiece == PieceType::blackPawn) ? 0 : moveCount + 1 ;
        }
        return moveCount;
    }

    /**
     * @brief Construct `ChessState` with zeroed bitboards and initialize.
     *
     * Initializes all containers and calls `ResetToStartPosition()`.
     */
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
          mMovesPlayed{},
          mFirstMove{}
    {
        ResetToStartPosition();
    }

    /**
     * @brief Convert rank letter 'a'..'h' to 1-based file number.
     * @param rank File letter.
     * @return Integer 1..8 or -1 if out of range.
     */
    int ChessState::ConvertRankToCol(char rank)
    {
        if(rank < 'a' || rank > 'h') return -1;
        return static_cast<int>(rank - 'a' + 1);
    }

    /**
     * @brief Recompute white/black attacked squares using current bitboards.
     *
     * Expands attack rays and step moves for each piece type to fill the
     * attacked-squares sets. Kings' immediate neighborhoods are included.
     */
    void ChessState::UpdateAttackedSquare()
    {
        mWhiteAttackedSquares.clear();
        mBlackAttackedSquares.clear();

        // white pawn attacked squares
        for(auto coordinate : GetPiecePosiiton(PieceType::whitePawn))
        {
            ChessCoordinate left = ChessCoordinate{coordinate.rank + 1, (char)(coordinate.file - 1)};
            ChessCoordinate right = ChessCoordinate{coordinate.rank + 1, (char)(coordinate.file + 1)};

            if(left.isValid())mWhiteAttackedSquares.insert(left);
            if(right.isValid())mWhiteAttackedSquares.insert(right);
        }

        // white Knight attacked squares
        for(auto coordinate : GetPiecePosiiton(PieceType::whiteKnight))
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
        for(auto startCoordinate : GetPiecePosiiton(PieceType::whiteBishop))
        {
            int offsetRank[] = { 1, 1,-1,-1};
            int offsetFile[] = { 1,-1, 1,-1};
            for(int i = 0; i < 4; i++)
            {
                ChessCoordinate iter = ChessCoordinate{startCoordinate.rank + offsetRank[i], (char)(startCoordinate.file + offsetFile[i])};
                while(iter.isValid() && (ChessState::Get().GetPieceOnChessCoordinate(iter) == PieceType::invalid || ChessState::Get().GetPieceOnChessCoordinate(iter) == PieceType::blackKing))
                {
                    mWhiteAttackedSquares.insert(iter);
                    iter.file += offsetFile[i];
                    iter.rank += offsetRank[i];
                }
                if(iter.isValid()) mWhiteAttackedSquares.insert(iter);
            }
        }

        // white Rook attacked squares
        for(auto startCoordinate : GetPiecePosiiton(PieceType::whiteRook))
        {
            int offsetRank[] = { 0, 0, 1,-1};
            int offsetFile[] = { 1,-1, 0, 0};
            for(int i = 0; i < 4; i++)
            {
                ChessCoordinate iter = ChessCoordinate{startCoordinate.rank + offsetRank[i],(char) (startCoordinate.file + offsetFile[i])};
                while(iter.isValid() && (ChessState::Get().GetPieceOnChessCoordinate(iter) == PieceType::invalid || ChessState::Get().GetPieceOnChessCoordinate(iter) == PieceType::blackKing))
                {
                    mWhiteAttackedSquares.insert(iter);
                    iter.file += offsetFile[i];
                    iter.rank += offsetRank[i];
                }
                if(iter.isValid()) mWhiteAttackedSquares.insert(iter);
            }
        }

        // white Queen attacked squares
        for(auto startCoordinate : GetPiecePosiiton(PieceType::whiteQueen))
        {
            int offsetRank[] = { 0, 0, 1,-1, 1, 1,-1,-1};
            int offsetFile[] = { 1,-1, 0, 0, 1,-1, 1,-1};
            for(int i = 0; i < 8; i++)
            {
                ChessCoordinate iter = ChessCoordinate{startCoordinate.rank + offsetRank[i], (char)(startCoordinate.file + offsetFile[i])};
                while(iter.isValid() && (ChessState::Get().GetPieceOnChessCoordinate(iter) == PieceType::invalid || ChessState::Get().GetPieceOnChessCoordinate(iter) == PieceType::blackKing) )
                {
                    mWhiteAttackedSquares.insert(iter);
                    iter.file += offsetFile[i];
                    iter.rank += offsetRank[i];
                }
                if(iter.isValid()) mWhiteAttackedSquares.insert(iter);
            }
        }

        // white King attacked squares
        for(auto startCoordinate : GetPiecePosiiton(PieceType::whiteKing))
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
        for(auto coordinate : GetPiecePosiiton(PieceType::blackPawn))
        {
            ChessCoordinate left = ChessCoordinate{coordinate.rank - 1, (char)(coordinate.file - 1)};
            ChessCoordinate right = ChessCoordinate{coordinate.rank - 1, (char)(coordinate.file + 1)};

            if(left.isValid())mBlackAttackedSquares.insert(left);
            if(right.isValid())mBlackAttackedSquares.insert(right);
        }

        // black Knight attacked squares
        for(auto coordinate : GetPiecePosiiton(PieceType::blackKnight))
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
        for(auto startCoordinate : GetPiecePosiiton(PieceType::blackBishop))
        {
            int offsetRank[] = { 1, 1,-1,-1};
            int offsetFile[] = { 1,-1, 1,-1};
            for(int i = 0; i < 4; i++)
            {
                ChessCoordinate iter = ChessCoordinate{startCoordinate.rank + offsetRank[i], (char)(startCoordinate.file + offsetFile[i])};
                while(iter.isValid() && (ChessState::Get().GetPieceOnChessCoordinate(iter) == PieceType::invalid || ChessState::Get().GetPieceOnChessCoordinate(iter) == PieceType::whiteKing) )
                {
                    mBlackAttackedSquares.insert(iter);
                    iter.file += offsetFile[i];
                    iter.rank += offsetRank[i];
                }
                if(iter.isValid()) mBlackAttackedSquares.insert(iter);
            }
        }

        // black Rook attacked squares
        for(auto startCoordinate : GetPiecePosiiton(PieceType::blackRook))
        {
            int offsetRank[] = { 0, 0, 1,-1};
            int offsetFile[] = { 1,-1, 0, 0};
            for(int i = 0; i < 4; i++)
            {
                ChessCoordinate iter = ChessCoordinate{startCoordinate.rank + offsetRank[i],(char) (startCoordinate.file + offsetFile[i])};
                while(iter.isValid() && (ChessState::Get().GetPieceOnChessCoordinate(iter) == PieceType::invalid || ChessState::Get().GetPieceOnChessCoordinate(iter) == PieceType::whiteKing) )
                {
                    mBlackAttackedSquares.insert(iter);
                    iter.file += offsetFile[i];
                    iter.rank += offsetRank[i];
                }
                if(iter.isValid()) mBlackAttackedSquares.insert(iter);
            }
        }

        // black Queen attacked squares
        for(auto startCoordinate : GetPiecePosiiton(PieceType::blackQueen))
        {
            int offsetRank[] = { 0, 0, 1,-1, 1, 1,-1,-1};
            int offsetFile[] = { 1,-1, 0, 0, 1,-1, 1,-1};
            for(int i = 0; i < 8; i++)
            {
                ChessCoordinate iter = ChessCoordinate{startCoordinate.rank + offsetRank[i], (char)(startCoordinate.file + offsetFile[i])};
                while(iter.isValid() && (ChessState::Get().GetPieceOnChessCoordinate(iter) == PieceType::invalid || ChessState::Get().GetPieceOnChessCoordinate(iter) == PieceType::whiteKing) )
                {
                    mBlackAttackedSquares.insert(iter);
                    iter.file += offsetFile[i];
                    iter.rank += offsetRank[i];
                }
                if(iter.isValid()) mBlackAttackedSquares.insert(iter);
            }
        }

        // black King attacked squares
        for(auto startCoordinate : GetPiecePosiiton(PieceType::blackKing))
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

    /**
     * @brief Place a piece on the board at the given coordinate.
     * @param piece Piece to place.
     * @param position Target square (must be valid).
     */
    void ChessState::SpawnPiece(PieceType piece, ChessCoordinate &position)
    {
        uint64_t& pieceContainer = GetPieceContainer(piece);
        if(!position.isValid()) return;

        // Set the end position bit
        pieceContainer |= 1ULL << (8 * (position.rank - 1) + (7- ConvertRankToCol(position.file)+1));
    }

    /**
     * @brief Get a reference to the bitboard container for a piece type.
     * @param piece Piece identifier.
     * @return Reference to the corresponding uint64_t bitboard.
     */
    uint64_t &ChessState::GetPieceContainer(PieceType piece)
    {
        switch (piece)
        {
        case PieceType::whitePawn:
            return mWhitePawns;
        case PieceType::whiteBishop:
            return mWhiteBishops;
        case PieceType::whiteKnight:
            return mWhiteKnights;
        case PieceType::whiteRook:
            return mWhiteRooks;
        case PieceType::whiteQueen:
            return mWhiteQueen;
        case PieceType::whiteKing:
            return mWhiteKing;
        case PieceType::blackPawn:
            return mBlackPawns;
        case PieceType::blackBishop:
            return mBlackBishops;
        case PieceType::blackKnight:
            return mBlackKnights;
        case PieceType::blackRook:
            return mBlackRooks;
        case PieceType::blackQueen:
            return mBlackQueen;
        case PieceType::blackKing:
            return mBlackKing;
        }
        return UINT64_MAX_VALUE;
    }

    /**
     * @brief Convert 1-based file number to rank letter 'a'..'h'.
     * @param col 1..8 column value.
     * @return Rank letter or `invalid` if out of range.
     */
    char ChessState::ConvertColToRank(int col)
    {
        if(col < 1 && col > 8) return -1;
        return static_cast<char>('a' + col - 1) ;
    }
}