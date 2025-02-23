#include "framework/Piece.h"

namespace chess
{
    Piece::Piece(Stage *owningStage)
        :mOwningSage{owningStage}
    {

    }

    bool Piece::GetPieceColor(char piece)
    {
        if(piece == whiteKing || piece == whiteQueen || piece == whiteRook || piece == whiteBishop || piece == whiteKnight || piece == whitePawn )
            return true;
        else 
            return false;
    }
}