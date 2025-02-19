#pragma once

#include"framework/Core.h"

namespace chess
{
    class ChessState
    {
        public:
            static ChessState& Get();

            void ResetToStartPosition();

            List<ChessCoordinate> GetWhitePawnsPosition();
            List<ChessCoordinate> GetWhiteKnightsPosition();
            List<ChessCoordinate> GetWhiteBishopsPosition();
            List<ChessCoordinate> GetWhiteRooksPosition();
            ChessCoordinate GetWhiteQueenPosition();
            ChessCoordinate GetWhiteKingPosition();

            List<ChessCoordinate> GetBlackPawnsPosition();
            List<ChessCoordinate> GetBlackKnightsPosition();
            List<ChessCoordinate> GetBlackBishopsPosition();
            List<ChessCoordinate> GetBlackRooksPosition();
            ChessCoordinate GetBlackQueenPosition();
            ChessCoordinate GetBlackKingPosition();

        protected:
            ChessState();

        private:
            char ConvertColtoRank(int row);
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
    };
}