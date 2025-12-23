/**
 * @file Piece.cpp
 * @brief Implementation of base Piece utilities shared by specific pieces.
 */
#include "framework/Piece.h"

namespace chess
{
    Piece::Piece(Stage *owningStage)
        :mOwningSage{owningStage}
    {

    }

    bool Piece::GetPieceColor(PieceType piece)
    {
        if(static_cast<int>(piece) > 0)
            return true;
        else
            return false;
    }
}