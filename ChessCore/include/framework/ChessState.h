/**
 * @file ChessState.h
 * @brief Bitboard-based chess state tracking and queries.
 */
#pragma once

#include"framework/Core.h"

namespace chess
{
    /**
     * @brief Singleton that manages the board state using bitboards.
     *
     * Provides piece placement, move logging/undo, attacked squares, and
     * convenience queries like king-in-check, first move flags, and counts.
     */
    class ChessState
    {
        public:
            /** @brief Get the global singleton instance. */
            static ChessState& Get();

            /** @brief Reset to the standard initial chess position. */
            void ResetToStartPosition();
            
            /** @brief Get all coordinates where a given piece currently exists. */
            List<ChessCoordinate> GetPiecePosiiton(PieceType piece);

            /**
             * @brief Move a piece from start to end and optionally log the move.
             * @param piece Piece identifier
             * @param start Start coordinate
             * @param end End coordinate
             * @param log If true, append to move history
             */
            void SetPiecePosition(PieceType piece, ChessCoordinate& start, ChessCoordinate& end,bool log = true);

            /** @brief Undo the last logged move if available. */
            bool UndoLastMove();

            /** @brief Get the piece occupying a coordinate or 'invalid'. */
            PieceType GetPieceOnChessCoordinate(ChessCoordinate coordinate);

            /** @brief Remove a specific piece at the position. */
            void RemovePiece(PieceType piece,ChessCoordinate& position);

            /** @brief Squares attacked by white pieces. */
            Set<ChessCoordinate,ChessCoordinateHashFunction> GetWhiteAttackedSquares(){ return mWhiteAttackedSquares; }
            /** @brief Squares attacked by black pieces. */
            Set<ChessCoordinate,ChessCoordinateHashFunction> GetBlackAttackedSquares(){ return mBlackAttackedSquares; }

            /** @brief True if the side-to-query's king is in check. */
            bool KingInCheck(bool white);

            /** @brief Place a new piece onto the board. */
            void SpawnPiece(PieceType piece, ChessCoordinate& position);

            /** @brief Get the last move as [from, to] if available. */
            List<ChessCoordinate> GetLastPlayedMove();

            /** @brief Count of specific piece type currently on the board. */
            int GetPieceCount(PieceType piece);

            /** @brief Whether the piece on this square has not yet moved. */
            bool IsFirstMove(ChessCoordinate coordinate);

            /** @brief Half-move clock (for 50-move rule). */
            int GetMovesWithoutCapture();

        protected:
            /** @brief Construct hidden for singleton pattern. */
            ChessState();

        private:
            /** @brief Get the bitboard container reference for a piece. */
            uint64_t& GetPieceContainer(PieceType piece); 

            /** @brief Convert a file char to rank index. */
            char ConvertColToRank(int row);
            /** @brief Convert a rank index to file char. */
            int ConvertRankToCol(char col);

            /** @brief Recompute attacked squares for both sides. */
            void UpdateAttackedSquare();

            static unique<ChessState> mChessState; ///< Singleton instance
            // White Pieces
            uint64_t mWhitePawns;    ///< Bitboard of white pawns
            uint64_t mWhiteKnights;  ///< Bitboard of white knights
            uint64_t mWhiteBishops;  ///< Bitboard of white bishops
            uint64_t mWhiteRooks;    ///< Bitboard of white rooks
            uint64_t mWhiteQueen;    ///< Bitboard of white queens
            uint64_t mWhiteKing;     ///< Bitboard of white king

            // Black Pieces
            uint64_t mBlackPawns;    ///< Bitboard of black pawns
            uint64_t mBlackKnights;  ///< Bitboard of black knights
            uint64_t mBlackBishops;  ///< Bitboard of black bishops
            uint64_t mBlackRooks;    ///< Bitboard of black rooks
            uint64_t mBlackQueen;    ///< Bitboard of black queens
            uint64_t mBlackKing;     ///< Bitboard of black king

            Set<ChessCoordinate,ChessCoordinateHashFunction> mWhiteAttackedSquares; ///< White attacked squares
            Set<ChessCoordinate,ChessCoordinateHashFunction> mBlackAttackedSquares; ///< Black attacked squares

            List<PlayedMove> mMovesPlayed; ///< Move history

            Dictionary<ChessCoordinate,bool,ChessCoordinateHashFunction> mFirstMove; ///< First-move flags per square

            int mMovesWithoutCapture; ///< i dont remember what this is for
    };
}