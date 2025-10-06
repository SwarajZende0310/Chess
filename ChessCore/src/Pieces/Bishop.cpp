/**
 * @file Bishop.cpp
 * @brief Implementation of the Bishop chess piece.
 */
#include "Pieces/Bishop.h"
#include "framework/AssetManager.h"
#include "framework/Stage.h"
#include "framework/ChessState.h"

namespace chess
{
    /**
     * @brief Construct a bishop and load its assets.
     * @param owningStage Stage for render context and scaling.
     * @param whitePiece True for white bishop, false for black.
     */
    Bishop::Bishop(Stage *owningStage, bool whitePiece)
        :Piece{owningStage},
        mOwningStage{owningStage},
        mWhiteBishopTexture{AssetManager::Get().LoadTexture("JohnPablok Cburnett Chess set/PNGs/With Shadow/1024px/w_bishop_png_shadow_1024px.png")},
        mBlackBishopTexture{AssetManager::Get().LoadTexture("JohnPablok Cburnett Chess set/PNGs/With Shadow/1024px/b_bishop_png_shadow_1024px.png")},
        mWhiteBishopSprite{*(mWhiteBishopTexture)},
        mBlackBishopSprite{*(mBlackBishopTexture)},
        mWhitePieces{whitePiece}
    {
        mWhiteBishopSprite.setScale(mOwningStage->GetSpriteScale() - sf::Vector2f{0.01,0.01});
        mBlackBishopSprite.setScale(mOwningStage->GetSpriteScale() - sf::Vector2f{0.01,0.01});
    }

    /**
     * @brief Validate diagonal bishop movement and capture rules.
     * @param startCoordinate Start square.
     * @param endCoordinate Destination square.
     * @return true if diagonal path is clear and destination is empty or enemy.
     */
    bool Bishop::MovePossible(ChessCoordinate &startCoordinate, ChessCoordinate &endCoordinate)
    {
        int ranksForward = abs(endCoordinate.rank - startCoordinate.rank);
        int filesRightward = abs(endCoordinate.file - startCoordinate.file);

        if((ranksForward == filesRightward) && 
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
    void Bishop::MakeMove(ChessCoordinate &startCoordinate, ChessCoordinate &endCoordinate)
    {
        if(mWhitePieces)
        {
            ChessState::Get().SetPiecePosition(whiteBishop,startCoordinate,endCoordinate);
        }
        else
        {
            ChessState::Get().SetPiecePosition(blackBishop,startCoordinate,endCoordinate);
        }
    }

    /**
     * @brief Draw the appropriate bishop sprite to the window.
     */
    void Bishop::RenderPiece()
    {
        if(mWhitePieces)
        {
            mOwningStage->GetWindow().draw(mWhiteBishopSprite);
        }
        else
        {
            mOwningStage->GetWindow().draw(mBlackBishopSprite);
        }
    }

    /**
     * @brief Set the on-screen position of the bishop sprite.
     */
    void Bishop::SetPieceLocation(const sf::Vector2f &newLocation, bool whitePieces)
    {
        if(whitePieces)
            mWhiteBishopSprite.setPosition(newLocation);
        else
            mBlackBishopSprite.setPosition(newLocation);
    }
    
    /**
     * @brief Set the rotation of the bishop sprite (unused).
     */
    void Bishop::SetPieceRotation(float newRotation, bool whitePieces)
    {
        // sf::Angle newRot{newRotation};
        // mWhiteBishopSprite.setRotation(newRot);
    }

    /**
     * @brief legal bishop moves along four diagonals from current pieceCoordinate.
     * @param pieceCoordinate Current square of the bishop.
     * @return All reachable squares until blocked; includes one capture per ray.
     */
    List<ChessCoordinate> Bishop::GetAllPossibleMoves(const ChessCoordinate pieceCoordinate)
    {
        List<ChessCoordinate> moves;
        moves.reserve(32);

        int offsetRank[4] = { -1,  -1,  1,  1};
        int offsetFile[4] = {  1,  -1, -1,  1};

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
     * @brief Get current sprite position for this bishop.
     */
    sf::Vector2f Bishop::GetPieceLocation() const
    {
        if(mWhitePieces)
            return mWhiteBishopSprite.getPosition();
        else
            return mBlackBishopSprite.getPosition();
    }
    
    /**
     * @brief Get current sprite rotation (always 0 for now).
     */
    float Bishop::GetPieceRotation() const
    {
        return 0.0f;
    }

    /**
     * @brief Center sprite origin based on its global bounds.
     */
    void Bishop::CenterPivot()
    {
        sf::FloatRect bound ;
        if(mWhitePieces)
        {
            bound = mWhiteBishopSprite.getGlobalBounds();
            mWhiteBishopSprite.setOrigin({float(bound.position.x) ,float(bound.position.y)});
        }
        else
        {
            bound = mBlackBishopSprite.getGlobalBounds();
            mBlackBishopSprite.setOrigin({float(bound.position.x) ,float(bound.position.y)});
        }
    }

    /**
     * @brief Check if a target square contains an opponent piece.
     */
    bool Bishop::isEnemy(ChessCoordinate &endCoordinate)
    {
        return ((mWhitePieces && !Piece::GetPieceColor(ChessState::Get().GetPieceOnChessCoordinate(endCoordinate))) || (!mWhitePieces && Piece::GetPieceColor(ChessState::Get().GetPieceOnChessCoordinate(endCoordinate))));
    }

    /**
     * @brief Determine if any piece lies between two diagonal squares.
     * @return true if a blocking piece exists, false if the path is clear.
     */
    bool Bishop::PiecesInBetweenPath(ChessCoordinate& startCoordinate, ChessCoordinate& endCoordinate)
    {
        int offsetX = (endCoordinate.file - startCoordinate.file) > 0 ? 1 : -1 ;
        int offsetY = (endCoordinate.rank - startCoordinate.rank) > 0 ? 1 : -1 ;

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