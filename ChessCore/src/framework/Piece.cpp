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

    bool Piece::IsKingPiece(PieceType piece)
    {
        return piece == PieceType::whiteKing || piece == PieceType::blackKing;
    }

    bool Piece::IsCapturableEnemy(bool whitePiece, PieceType targetPiece)
    {
        if(targetPiece == PieceType::invalid || IsKingPiece(targetPiece))
        {
            return false;
        }

        return whitePiece ? !GetPieceColor(targetPiece) : GetPieceColor(targetPiece);
    }
}
