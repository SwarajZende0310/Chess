/**
 * @file Core.h
 * @brief Common core utilities, aliases, constants, and chess data structures.
 *
 * Provides STL alias templates, logging macro, chess piece identifiers,
 * game state constants, and lightweight data types like `ChessCoordinate`
 * and `PlayedMove` used across the engine.
 */
#pragma once

#include<stdio.h>
#include<vector>
#include<memory>
#include<map>
#include<set>
#include<unordered_map>
#include<unordered_set>
#include<utility>
#include<cmath>

namespace chess
{
    /**
     * @brief Unique ownership smart pointer alias.
     */
    template<typename T>
    using unique = std::unique_ptr<T>;

    /**
     * @brief Shared ownership smart pointer alias.
     */
    template<typename T>
    using shared = std::shared_ptr<T>;
    
    /**
     * @brief Weak reference smart pointer alias.
     */
    template<typename T>
    using weak = std::weak_ptr<T>;

    /**
     * @brief Dynamic array alias.
     */
    template<typename T>
    using List = std::vector<T>;

    /**
     * @brief Ordered map alias.
     */
    template<typename keyType,typename valType,typename Pr=std::less<keyType>>
    using Map = std::map<keyType,valType,Pr>;

    /**
     * @brief Hash map alias.
     */
    template<typename keyType,typename valType,typename Hasher=std::hash<keyType>>
    using Dictionary = std::unordered_map<keyType,valType,Hasher>;

    /**
     * @brief Hash set alias.
     */
    template<typename keyType,typename Hasher=std::hash<keyType>>
    using Set = std::unordered_set<keyType,Hasher>;

    /**
     * @brief Simple printf-style logging macro.
     */
    #define LOG(M, ...) printf(M "\n", ##__VA_ARGS__)

    // White Piece Representation
    /** @name White piece identifiers */
    ///@{
    #define whitePawn 'P'
    #define whiteBishop 'B'
    #define whiteKnight 'N'
    #define whiteRook 'R'
    #define whiteQueen 'Q'
    #define whiteKing 'K'
    ///@}

    // Black Piece Representation
    /** @name Black piece identifiers */
    ///@{
    #define blackPawn 'p'
    #define blackBishop 'b'
    #define blackKnight 'n'
    #define blackRook 'r'
    #define blackQueen 'q'
    #define blackKing 'k'
    ///@}

    /** @brief Invalid piece identifier. */
    #define invalid 'i'

    // Game End State
    /** @name Game end state codes */
    ///@{
    #define WhiteWon  1
    #define DRAW      0
    #define BlackWon -1
    #define Ongoing   2
    ///@}

    // Castling states
    /** @name Castling state flags */
    ///@{
    #define NoCastling 0
    #define KingSide 1
    #define QueenSide 2
    ///@}

    // Invalid piece container
    /** @brief Helper constant representing an all-ones 64-bit value. */
    static uint64_t UINT64_MAX_VALUE = UINT64_C(0xFFFFFFFFFFFFFFFF);

    /**
     * @brief Board coordinate type (rank 1..8, file 'a'..'h').
     */
    struct ChessCoordinate
    {
        public:
        /**
         * @brief Construct a valid coordinate.
         * @param _rank Rank (1..8)
         * @param _file File ('a'..'h')
         */
        ChessCoordinate(int _rank, char _file)
            :rank{_rank},
            file{_file}
        {

        }

        /**
         * @brief Construct an invalid coordinate.
         */
        ChessCoordinate()
            :rank{invalid},
            file{invalid}
        {

        }

        int rank;  ///< Rank (1..8) or 'invalid'
        char file; ///< File ('a'..'h') or 'invalid'

        /**
         * @brief Check if the coordinate is within board bounds.
         * @return true if in [1..8] x ['a'..'h']
         */
        bool isValid()
        {
            return (rank > 0 && rank < 9) && (file >= 'a' && file <= 'h');
        }
    };

    /**
     * @brief Equality operator for ChessCoordinate.
     */
    static bool operator==(const ChessCoordinate& lhs, const ChessCoordinate& rhs)
    {
        return lhs.file == rhs.file && lhs.rank == rhs.rank;
    }

    /**
     * @brief Hash functor for ChessCoordinate for use in unordered containers.
     */
    struct ChessCoordinateHashFunction
    {
        public:
            std::size_t operator()(const ChessCoordinate& coordinate)const
            {
                return std::hash<int>()(coordinate.rank) ^ std::hash<char>()(coordinate.file);
            }
    };

    /**
     * @brief Record of a move that was played, including captures and castling.
     */
    struct PlayedMove
    {
        public:
        /**
         * @brief Construct an empty move (no piece, invalid coordinates).
         */
        PlayedMove()
            :mPiece{invalid},
            mStartCoordinate{},
            mEndCoordinate{},
            mCapturedPiece{invalid},
            mCapturedPieceCoordinate{},
            mCastling{NoCastling}
        {

        }

        /**
         * @brief Construct a fully specified move record.
         * @param piece Piece identifier
         * @param start Start coordinate
         * @param end End coordinate
         * @param capturedPiece Captured piece identifier (or 'invalid')
         * @param capturedCoordinate Coordinate of captured piece
         * @param castling Castling flag (NoCastling/KingSide/QueenSide)
         */
        PlayedMove(char piece, ChessCoordinate start, ChessCoordinate end, char capturedPiece, ChessCoordinate capturedCoordinate,int castling)
            :mPiece{piece},
            mStartCoordinate{start.rank,start.file},
            mEndCoordinate{end.rank,end.file},
            mCapturedPiece{capturedPiece},
            mCapturedPieceCoordinate{capturedCoordinate.rank,capturedCoordinate.file},
            mCastling{castling}
            {

            }
        
        char mPiece;                     ///< Piece that moved
        ChessCoordinate mStartCoordinate;///< Starting coordinate
        ChessCoordinate mEndCoordinate;  ///< Ending coordinate

        char mCapturedPiece;             ///< Captured piece ('invalid' if none)
        ChessCoordinate mCapturedPieceCoordinate; ///< Coordinate of captured piece

        int mCastling;                   ///< Castling flag
    };
}