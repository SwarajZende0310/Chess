#pragma once

#include"framework/Core.h"

namespace chess
{
    class ChessState
    {
        public:
            static ChessState& Get();

            void ResetToStartPosition();
            
            List<ChessCoordinate> GetPiecePosiiton(char piece);

            void SetPiecePosition(char piece, ChessCoordinate& start, ChessCoordinate& end,bool log = true);

            bool UndoLastMove();

            char GetPieceOnChessCoordinate(ChessCoordinate coordinate);

            void RemovePiece(char piece,ChessCoordinate& position);

            Set<ChessCoordinate,ChessCoordinateHashFunction> GetWhiteAttackedSquares(){ return mWhiteAttackedSquares; }
            Set<ChessCoordinate,ChessCoordinateHashFunction> GetBlackAttackedSquares(){ return mBlackAttackedSquares; }

            bool KingInCheck(bool white);

            void SpawnPiece(char piece, ChessCoordinate& position);

            List<ChessCoordinate> GetLastPlayedMove();

            int GetPieceCount(char piece);

            bool IsFirstMove(ChessCoordinate coordinate);

            int GetMovesWithoutCapture();

        protected:
            ChessState();

        private:
            uint64_t& GetPieceContainer(char piece); 

            char ConvertColToRank(int row);
            int ConvertRankToCol(char col);

            void UpdateAttackedSquare();

            static unique<ChessState> mChessState;
            // White Pieces
            uint64_t mWhitePawns;
            uint64_t mWhiteKnights;
            uint64_t mWhiteBishops;
            uint64_t mWhiteRooks;
            uint64_t mWhiteQueen;
            uint64_t mWhiteKing;

            // Black Pieces
            uint64_t mBlackPawns;
            uint64_t mBlackKnights;
            uint64_t mBlackBishops;
            uint64_t mBlackRooks;
            uint64_t mBlackQueen;
            uint64_t mBlackKing;

            Set<ChessCoordinate,ChessCoordinateHashFunction> mWhiteAttackedSquares;
            Set<ChessCoordinate,ChessCoordinateHashFunction> mBlackAttackedSquares;

            List<PlayedMove> mMovesPlayed;

            Dictionary<ChessCoordinate,bool,ChessCoordinateHashFunction> mFirstMove;

            int mMovesWithoutCapture;
    };
}