#pragma once

#include<stdio.h>
#include<vector>
#include<memory>
#include<map>
#include<set>
#include<unordered_map>
#include<unordered_set>
#include<utility>

namespace chess
{
    template<typename T>
    using unique = std::unique_ptr<T>;

    template<typename T>
    using shared = std::shared_ptr<T>;
    
    template<typename T>
    using weak = std::weak_ptr<T>;

    template<typename T>
    using List = std::vector<T>;

    template<typename keyType,typename valType,typename Pr=std::less<keyType>>
    using Map = std::map<keyType,valType,Pr>;

    template<typename keyType,typename valType,typename Hasher=std::hash<keyType>>
    using Dictionary = std::unordered_map<keyType,valType,Hasher>;

    template<typename T>
    using Set = std::unordered_set<T>;

    #define LOG(M, ...) printf(M "\n", ##__VA_ARGS__)

    // White Piece Representation
    #define whitePawn 'P'
    #define whiteBishop 'B'
    #define whiteKnight 'N'
    #define whiteRook 'R'
    #define whiteQueen 'Q'
    #define whiteKing 'K'

    // Black Piece Representation
    #define blackPawn 'p'
    #define blackBishop 'b'
    #define blackKnight 'n'
    #define blackRook 'r'
    #define blackQueen 'q'
    #define blackKing 'k'

    #define invalid 'i'

    struct ChessCoordinate
    {
        public:
        ChessCoordinate(int _rank, char _file)
            :rank{_rank},
            file{_file}
        {

        }
        int rank;
        char file;
    };

    static bool operator==(const ChessCoordinate& lhs, const ChessCoordinate& rhs)
    {
        return lhs.file == rhs.file && lhs.rank == rhs.rank;
    }

    struct ChessCoordinateHashFunction
    {
        public:
            std::size_t operator()(const ChessCoordinate& coordinate)const
            {
                return std::hash<int>()(coordinate.rank) ^ std::hash<char>()(coordinate.file);
            }
    };
}