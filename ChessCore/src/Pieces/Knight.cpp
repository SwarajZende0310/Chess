/**
 * @file Knight.cpp
 * @brief Implementation of the Knight chess piece.
 */
#include "Pieces/Knight.h"
#include "framework/AssetManager.h"
#include "framework/Stage.h"
#include "framework/ChessState.h"

namespace chess
{
    /**
     * @brief Construct a knight and load its assets.
     * @param owningStage Stage for render context and scaling.
     * @param whitePiece True for white knight, false for black.
     */
    Knight::Knight(Stage *owningStage, bool whitePiece)
        :Piece{owningStage},
        mOwningStage{owningStage},
        mWhiteKnightTexture{AssetManager::Get().LoadTexture("JohnPablok Cburnett Chess set/PNGs/With Shadow/1024px/w_knight_png_shadow_1024px.png")},
        mBlackKnightTexture{AssetManager::Get().LoadTexture("JohnPablok Cburnett Chess set/PNGs/With Shadow/1024px/b_knight_png_shadow_1024px.png")},
        mWhiteKnightSprite{*(mWhiteKnightTexture)},
        mBlackKnightSprite{*(mBlackKnightTexture)},
        mWhitePieces{whitePiece}
    {
        mWhiteKnightSprite.setScale(mOwningStage->GetSpriteScale() - sf::Vector2f{0.01,0.01});
        mBlackKnightSprite.setScale(mOwningStage->GetSpriteScale() - sf::Vector2f{0.01,0.01});
    }

    /**
     * @brief Validate L-shaped knight movement and capture rules.
     * @param startCoordinate Start square.
     * @param endCoordinate Destination square.
     * @return true if an (2,1) move and destination is empty or enemy.
     */
    bool Knight::MovePossible(ChessCoordinate &startCoordinate, ChessCoordinate &endCoordinate)
    {
        int ranksForward = endCoordinate.rank - startCoordinate.rank;
        int filesRightward = endCoordinate.file - startCoordinate.file;
        
        //Knight moves forward and backward
        if((((ranksForward == 2 || ranksForward == -2) && (filesRightward == 1 || filesRightward == -1))  ||
            ((filesRightward == 2 || filesRightward == -2) && (ranksForward == 1 || ranksForward == -1))) && 
            (ChessState::Get().GetPieceOnChessCoordinate(endCoordinate) == invalid || isEnemy(endCoordinate))) //Knight moves sidewards
        {
            return true;
        }
        return false;
    }

    /**
     * @brief Apply the move to `ChessState` for the correct side.
     */
    void Knight::MakeMove(ChessCoordinate &startCoordinate, ChessCoordinate &endCoordinate)
    {
        if(mWhitePieces)
        {
            ChessState::Get().SetPiecePosition(whiteKnight,startCoordinate,endCoordinate);
        }
        else
        {
            ChessState::Get().SetPiecePosition(blackKnight,startCoordinate,endCoordinate);
        }
    }

    /**
     * @brief Draw the appropriate knight sprite to the window.
     */
    void Knight::RenderPiece()
    {
        if(mWhitePieces)
        {
            mOwningStage->GetWindow().draw(mWhiteKnightSprite);
        }
        else
        {
            mOwningStage->GetWindow().draw(mBlackKnightSprite);
        }
    }

    /**
     * @brief Set the on-screen position of the knight sprite.
     */
    void Knight::SetPieceLocation(const sf::Vector2f &newLocation, bool whitePieces)
    {
        if(whitePieces)
            mWhiteKnightSprite.setPosition(newLocation);
        else
            mBlackKnightSprite.setPosition(newLocation);
    }
    
    /**
     * @brief Set the rotation of the knight sprite (unused).
     */
    void Knight::SetPieceRotation(float newRotation, bool whitePieces)
    {
        // sf::Angle newRot{newRotation};
        // mWhiteKnightSprite.setRotation(newRot);
    }

    /**
     * @brief Generate pseudo-legal knight moves to 8 potential targets.
     * @param pieceCoordinate Current square of the knight.
     * @return Valid target squares that pass `MovePossible`.
     */
    List<ChessCoordinate> Knight::GetAllPossibleMoves(const ChessCoordinate pieceCoordinate)
    {
        List<ChessCoordinate> moves;
        moves.reserve(8);

        int offsetRank[8] = {  2,   2, -2, -2,  1, -1,  1, -1};
        int offsetFile[8] = {  1,  -1,  1, -1,  2,  2, -2, -2};

        ChessCoordinate start{pieceCoordinate.rank, pieceCoordinate.file};

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
     * @brief Get current sprite position for this knight.
     */
    sf::Vector2f Knight::GetPieceLocation() const
    {
        if(mWhitePieces)
            return mWhiteKnightSprite.getPosition();
        else
            return mBlackKnightSprite.getPosition();
    }
    
    /**
     * @brief Get current sprite rotation (always 0 for now).
     */
    float Knight::GetPieceRotation() const
    {
        return 0.0f;
    }

    /**
     * @brief Center sprite origin based on its global bounds.
     */
    void Knight::CenterPivot()
    {
        sf::FloatRect bound ;
        if(mWhitePieces)
        {
            bound = mWhiteKnightSprite.getGlobalBounds();
            mWhiteKnightSprite.setOrigin({float(bound.position.x) ,float(bound.position.y)});
        }
        else
        {
            bound = mBlackKnightSprite.getGlobalBounds();
            mBlackKnightSprite.setOrigin({float(bound.position.x) ,float(bound.position.y)});
        }
    }
    /**
     * @brief Check if a target square contains an opponent piece.
     */
    bool Knight::isEnemy(ChessCoordinate &endCoordinate)
    {
        return ((mWhitePieces && !Piece::GetPieceColor(ChessState::Get().GetPieceOnChessCoordinate(endCoordinate))) || (!mWhitePieces && Piece::GetPieceColor(ChessState::Get().GetPieceOnChessCoordinate(endCoordinate))));
    }
}