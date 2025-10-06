/**
 * @file Rook.cpp
 * @brief Implementation of the Rook chess piece.
 */
#include "Pieces/Rook.h"
#include "framework/AssetManager.h"
#include "framework/Stage.h"
#include "framework/ChessState.h"

namespace chess
{
    /**
     * @brief Construct a rook and load its assets.
     * @param owningStage Stage for render context and scaling.
     * @param whitePiece True for white rook, false for black.
     */
    Rook::Rook(Stage *owningStage, bool whitePiece)
        :Piece{owningStage},
        mOwningStage{owningStage},
        mWhiteRookTexture{AssetManager::Get().LoadTexture("JohnPablok Cburnett Chess set/PNGs/With Shadow/1024px/w_rook_png_shadow_1024px.png")},
        mBlackRookTexture{AssetManager::Get().LoadTexture("JohnPablok Cburnett Chess set/PNGs/With Shadow/1024px/b_rook_png_shadow_1024px.png")},
        mWhiteRookSprite{*(mWhiteRookTexture)},
        mBlackRookSprite{*(mBlackRookTexture)},
        mWhitePieces{whitePiece}
    {
        mWhiteRookSprite.setScale(mOwningStage->GetSpriteScale() - sf::Vector2f{0.01,0.01});
        mBlackRookSprite.setScale(mOwningStage->GetSpriteScale() - sf::Vector2f{0.01,0.01});
    }

    /**
     * @brief Validate rook movement (ranks/files) and capture rules.
     * @param startCoordinate Start square.
     * @param endCoordinate Destination square.
     * @return true if path is clear and destination is empty or enemy.
     */
    bool Rook::MovePossible(ChessCoordinate &startCoordinate, ChessCoordinate &endCoordinate)
    {
        int ranksForward = abs(endCoordinate.rank - startCoordinate.rank);
        int filesRightward = abs(endCoordinate.file - startCoordinate.file);

        if(((ranksForward == 0 && filesRightward > 0 ) || (ranksForward > 0 && filesRightward == 0 )) &&
            !PiecesInBetweenPath(startCoordinate,endCoordinate) &&
            (ChessState::Get().GetPieceOnChessCoordinate(endCoordinate) == invalid || isEnemy(endCoordinate)))
            {
                return true;
            }
        return false;
    }

    /**
     * @brief Apply the move to `ChessState` for the correct side.
     */
    void Rook::MakeMove(ChessCoordinate &startCoordinate, ChessCoordinate &endCoordinate)
    {
        if(mWhitePieces)
        {
            ChessState::Get().SetPiecePosition(whiteRook,startCoordinate,endCoordinate);
        }
        else
        {
            ChessState::Get().SetPiecePosition(blackRook,startCoordinate,endCoordinate);
        }
    }

    /**
     * @brief Draw the appropriate rook sprite to the window.
     */
    void Rook::RenderPiece()
    {
        if(mWhitePieces)
        {
            mOwningStage->GetWindow().draw(mWhiteRookSprite);
        }
        else
        {
            mOwningStage->GetWindow().draw(mBlackRookSprite);
        }
    }

    /**
     * @brief Set the on-screen position of the rook sprite.
     */
    void Rook::SetPieceLocation(const sf::Vector2f &newLocation, bool whitePieces)
    {
        if(whitePieces)
            mWhiteRookSprite.setPosition(newLocation);
        else
            mBlackRookSprite.setPosition(newLocation);
    }
    
    /**
     * @brief Set the rotation of the rook sprite (unused).
     */
    void Rook::SetPieceRotation(float newRotation, bool whitePieces)
    {
        // sf::Angle newRot{newRotation};
        // mWhiteRookSprite.setRotation(newRot);
    }

    /**
     * @brief Legal rook moves along ranks/files.
     * @param pieceCoordinate Current square of the rook.
     * @return All reachable squares until blocked; includes one capture per ray.
     */
    List<ChessCoordinate> Rook::GetAllPossibleMoves(const ChessCoordinate pieceCoordinate)
    {
        List<ChessCoordinate> moves;
        moves.reserve(32);

        int offsetRank[4] = {  0,   0,  1, -1};
        int offsetFile[4] = {  1,  -1,  0,  0};

        for(int i = 0; i < 4; i++)
        {
            ChessCoordinate iter{pieceCoordinate.rank, pieceCoordinate.file};
            iter.rank += offsetRank[i];
            iter.file += offsetFile[i];

            while(iter.isValid())
            {
                if(ChessState::Get().GetPieceOnChessCoordinate(iter) != invalid)
                {
                    if(isEnemy(iter))
                        moves.emplace_back(iter);
                    break;
                }
                moves.emplace_back(iter);
                iter.rank += offsetRank[i];
                iter.file += offsetFile[i];
            }
        }
        return moves;
    }
    
    /**
     * @brief Get current sprite position for this rook.
     */
    sf::Vector2f Rook::GetPieceLocation() const
    {
        if(mWhitePieces)
            return mWhiteRookSprite.getPosition();
        else
            return mBlackRookSprite.getPosition();
    }
    
    /**
     * @brief Get current sprite rotation (always 0 for now).
     */
    float Rook::GetPieceRotation() const
    {
        return 0.0f;
    }

    /**
     * @brief Center sprite origin based on its global bounds.
     */
    void Rook::CenterPivot()
    {
        sf::FloatRect bound ;
        if(mWhitePieces)
        {
            bound = mWhiteRookSprite.getGlobalBounds();
            mWhiteRookSprite.setOrigin({float(bound.position.x) ,float(bound.position.y)});
        }
        else
        {
            bound = mBlackRookSprite.getGlobalBounds();
            mBlackRookSprite.setOrigin({float(bound.position.x) ,float(bound.position.y)});
        }
    }
    /**
     * @brief Check if a target square contains an opponent piece.
     */
    bool chess::Rook::isEnemy(ChessCoordinate &endCoordinate)
    {
        return ((mWhitePieces && !Piece::GetPieceColor(ChessState::Get().GetPieceOnChessCoordinate(endCoordinate))) || (!mWhitePieces && Piece::GetPieceColor(ChessState::Get().GetPieceOnChessCoordinate(endCoordinate))));
    }

    /**
     * @brief Determine if any piece blocks the path to destination.
     * Normalizes direction along one axis and steps until end.
     */
    bool chess::Rook::PiecesInBetweenPath(ChessCoordinate &startCoordinate, ChessCoordinate &endCoordinate)
    {
        int offsetX = (endCoordinate.file - startCoordinate.file);
        int offsetY = (endCoordinate.rank - startCoordinate.rank);

        if(offsetX == 0)offsetY = offsetY > 0 ? 1 : -1;
        if(offsetY == 0)offsetX = offsetX > 0 ? 1 : -1;

        ChessCoordinate iterator{startCoordinate.rank,startCoordinate.file};
        iterator.file += offsetX;
        iterator.rank += offsetY;

        while(iterator.isValid() && !(iterator == endCoordinate))
        {
            if(ChessState::Get().GetPieceOnChessCoordinate(iterator) != invalid)
            {
                return true;
            }
            iterator.file += offsetX;
            iterator.rank += offsetY;
        }
        return false;
    }
}
