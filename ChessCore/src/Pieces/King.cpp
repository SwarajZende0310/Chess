/**
 * @file King.cpp
 * @brief Implementation of the King chess piece.
 */
#include "Pieces/King.h"
#include "framework/AssetManager.h"
#include "framework/Stage.h"
#include "framework/ChessState.h"

namespace chess
{
    /**
     * @brief Construct a king and load its assets.
     * @param owningStage Stage for render context and scaling.
     * @param whitePiece True for white king, false for black.
     */
    King::King(Stage *owningStage, bool whitePiece)
        :Piece{owningStage},
        mOwningStage{owningStage},
        mWhiteKingTexture{AssetManager::Get().LoadTexture("JohnPablok Cburnett Chess set/PNGs/With Shadow/1024px/w_king_png_shadow_1024px.png")},
        mBlackKingTexture{AssetManager::Get().LoadTexture("JohnPablok Cburnett Chess set/PNGs/With Shadow/1024px/b_king_png_shadow_1024px.png")},
        mWhiteKingSprite{*(mWhiteKingTexture)},
        mBlackKingSprite{*(mBlackKingTexture)},
        mWhitePieces{whitePiece}
    {
        mWhiteKingSprite.setScale(mOwningStage->GetSpriteScale() - sf::Vector2f{0.01,0.01});
        mBlackKingSprite.setScale(mOwningStage->GetSpriteScale() - sf::Vector2f{0.01,0.01});
    }

    /**
     * @brief Validate king move: one square any direction and not into check.
     * @param startCoordinate Start square.
     * @param endCoordinate Destination square.
     * @return true if destination is not attacked and movement is 1-square.
     */
    bool King::MovePossible(ChessCoordinate &startCoordinate, ChessCoordinate &endCoordinate)
    {
        int ranksForward = abs(endCoordinate.rank - startCoordinate.rank);
        int filesRightward = abs(endCoordinate.file - startCoordinate.file);

        if(((ranksForward == 1 && filesRightward == 1) || (ranksForward == 0 && filesRightward == 1 ) || (ranksForward == 1 && filesRightward == 0 )) && 
            (ChessState::Get().GetPieceOnChessCoordinate(endCoordinate) == invalid || isEnemy(endCoordinate)) && 
            ((mWhitePieces && (ChessState::Get().GetBlackAttackedSquares().find(endCoordinate) == ChessState::Get().GetBlackAttackedSquares().end())) || (!mWhitePieces && (ChessState::Get().GetWhiteAttackedSquares().find(endCoordinate) == ChessState::Get().GetWhiteAttackedSquares().end())) ) )
        {
            return true;
        }
        return false;
    }

    /**
     * @brief Apply the move to `ChessState` for the correct side.
     */
    void King::MakeMove(ChessCoordinate &startCoordinate, ChessCoordinate &endCoordinate)
    {
        if(mWhitePieces)
        {
            ChessState::Get().SetPiecePosition(whiteKing,startCoordinate,endCoordinate);
        }
        else
        {
            ChessState::Get().SetPiecePosition(blackKing,startCoordinate,endCoordinate);
        }
    }

    /**
     * @brief Draw the appropriate king sprite to the window.
     */
    void King::RenderPiece()
    {
        if(mWhitePieces)
        {
            mOwningStage->GetWindow().draw(mWhiteKingSprite);
        }
        else
        {
            mOwningStage->GetWindow().draw(mBlackKingSprite);
        }
    }

    /**
     * @brief Set the on-screen position of the king sprite.
     */
    void King::SetPieceLocation(const sf::Vector2f &newLocation, bool whitePieces)
    {
        if(whitePieces)
            mWhiteKingSprite.setPosition(newLocation);
        else
            mBlackKingSprite.setPosition(newLocation);
    }
    
    /**
     * @brief Set the rotation of the king sprite (unused).
     */
    void King::SetPieceRotation(float newRotation, bool whitePieces)
    {
        // sf::Angle newRot{newRotation};
        // mWhiteKingSprite.setRotation(newRot);
    }

    /**
     * @brief King moves to adjacent squares.
     * @param pieceCoordinate Current square of the king.
     * @return Adjacent squares that pass `MovePossible` (excludes castling).
     */
    List<ChessCoordinate> King::GetAllPossibleMoves(const ChessCoordinate pieceCoordinate)
    {
        List<ChessCoordinate> moves;
        moves.reserve(8);

        int offsetRank[8] = { -1,  -1,  1,  1,  1, -1,  0,  0};
        int offsetFile[8] = {  1,  -1, -1,  1,  0,  0,  1, -1};
        
        ChessCoordinate start{pieceCoordinate.rank,pieceCoordinate.file};
        for(int i = 0; i < 8; i++)
        {
            ChessCoordinate end{pieceCoordinate.rank, pieceCoordinate.file};
            end.rank += offsetRank[i];
            end.file += offsetFile[i];

            if(end.isValid() && MovePossible(start,end))
            {
                moves.emplace_back(end);
            }
        }
        return moves;
    }
    /**
     * @brief Check whether this king is currently in check.
     * @return true if current side's king square is in enemy attacked set.
     */
    bool King::IsInCheck()
    {
        Set<ChessCoordinate,ChessCoordinateHashFunction> attackedSquares = mWhitePieces ? ChessState::Get().GetBlackAttackedSquares() : ChessState::Get().GetWhiteAttackedSquares();
        if(mWhitePieces && attackedSquares.find(ChessState::Get().GetPiecePosiiton(whiteKing)[0]) != attackedSquares.end())
        {
            return true;
        }
        else if(!mWhitePieces && attackedSquares.find(ChessState::Get().GetPiecePosiiton(blackKing)[0]) != attackedSquares.end())
        {
            return true;
        }
        return false;
    }
    /**
     * @brief Get current sprite position for this king.
     */
    sf::Vector2f King::GetPieceLocation() const
    {
        if(mWhitePieces)
            return mWhiteKingSprite.getPosition();
        else
            return mBlackKingSprite.getPosition();
    }
    
    /**
     * @brief Get current sprite rotation (always 0 for now).
     */
    float King::GetPieceRotation() const
    {
        return 0.0f;
    }

    /**
     * @brief Center sprite origin based on its global bounds.
     */
    void King::CenterPivot()
    {
        sf::FloatRect bound ;
        if(mWhitePieces)
        {
            bound = mWhiteKingSprite.getGlobalBounds();
            mWhiteKingSprite.setOrigin({float(bound.position.x) ,float(bound.position.y)});
        }
        else
        {
            bound = mBlackKingSprite.getGlobalBounds();
            mBlackKingSprite.setOrigin({float(bound.position.x) ,float(bound.position.y)});
        }
    }
    /**
     * @brief Check if a target square contains an opponent piece.
     */
    bool chess::King::isEnemy(ChessCoordinate &endCoordinate)
    {
        return ((mWhitePieces && !Piece::GetPieceColor(ChessState::Get().GetPieceOnChessCoordinate(endCoordinate))) || (!mWhitePieces && Piece::GetPieceColor(ChessState::Get().GetPieceOnChessCoordinate(endCoordinate))));
    }
}