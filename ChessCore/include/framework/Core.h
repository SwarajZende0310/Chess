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
#include <fmt/format.h>

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

    /** @enum PieceType
    * @brief A strongly typed enum class representing PieceTypes.
    * 
    * This enum is used to identify different PieceTypes.
    */
    enum class PieceType
    {
        invalid = 0,
        whitePawn = 1,
        whiteBishop = 2,
        whiteKnight = 3,
        whiteRook = 4,
        whiteQueen = 5,
        whiteKing = 6,
        blackPawn = -1,
        blackBishop = -2,
        blackKnight = -3,
        blackRook = -4,
        blackQueen = -5,
        blackKing = -6
    };

    /** @enum GameState
    * @brief A strongly typed enum class representing Game Current State.
    * 
    * This enum is used to identify current state of the ongoing chess game.
    */
    enum class GameState
    {
        WhiteWon,
        Draw,
        BlackWon,
        Ongoing
    };

    /** @enum CastlingState
    * @brief A strongly typed enum class representing Castling Possiblities.
    * 
    * This enum is used to identify castling of king.
    */
    enum class CastlingState
    {
        NoCastling = 0,
        KingSide = 1,
        QueenSide = 2
    };

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
            :rank{-1},
            file{-1}
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
            :mPiece{PieceType::invalid},
            mStartCoordinate{},
            mEndCoordinate{},
            mCapturedPiece{PieceType::invalid},
            mCapturedPieceCoordinate{},
            mCastling{CastlingState::NoCastling}
        {

        }

        /**
         * @brief Construct a fully specified move record.
         * @param piece Piece identifier
         * @param start Start coordinate
         * @param end End coordinate
         * @param capturedPiece Captured piece identifier (or 'invalid')
         * @param capturedCoordinate Coordinate of captured piece
         * @param castling CastlingState flag (NoCastling/KingSide/QueenSide)
         */
        PlayedMove(PieceType piece, ChessCoordinate start, ChessCoordinate end, PieceType capturedPiece, ChessCoordinate capturedCoordinate,CastlingState castling)
            :mPiece{piece},
            mStartCoordinate{start.rank,start.file},
            mEndCoordinate{end.rank,end.file},
            mCapturedPiece{capturedPiece},
            mCapturedPieceCoordinate{capturedCoordinate.rank,capturedCoordinate.file},
            mCastling{castling}
            {

            }
        
        PieceType mPiece;                ///< Piece that moved
        ChessCoordinate mStartCoordinate;///< Starting coordinate
        ChessCoordinate mEndCoordinate;  ///< Ending coordinate

        PieceType mCapturedPiece;                 ///< Captured piece ('invalid' if none)
        ChessCoordinate mCapturedPieceCoordinate; ///< Coordinate of captured piece

        CastlingState mCastling;                   ///< CastlingState flag
    };
}