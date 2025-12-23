/**
 * @file Queen.cpp
 * @brief Implementation of the Queen chess piece.
 */
#include "Pieces/Queen.h"
#include "framework/AssetManager.h"
#include "framework/Stage.h"
#include "framework/ChessState.h"

namespace chess
{
    /**
     * @brief Construct a queen and load its assets.
     * @param owningStage Stage for render context and scaling.
     * @param whitePiece True for white queen, false for black.
     */
    Queen::Queen(Stage *owningStage, bool whitePiece)
        :Piece{owningStage},
        mOwningStage{owningStage},
        mWhiteQueenTexture{AssetManager::Get().LoadTexture("JohnPablok Cburnett Chess set/PNGs/With Shadow/1024px/w_queen_png_shadow_1024px.png")},
        mBlackQueenTexture{AssetManager::Get().LoadTexture("JohnPablok Cburnett Chess set/PNGs/With Shadow/1024px/b_queen_png_shadow_1024px.png")},
        mWhiteQueenSprite{*(mWhiteQueenTexture)},
        mBlackQueenSprite{*(mBlackQueenTexture)},
        mWhitePieces{whitePiece}
    {
        mWhiteQueenSprite.setScale(mOwningStage->GetSpriteScale() - sf::Vector2f{0.01,0.01});
        mBlackQueenSprite.setScale(mOwningStage->GetSpriteScale() - sf::Vector2f{0.01,0.01});
    }

    /**
     * @brief Validate queen moves (rook or bishop pattern) and capture rules.
     * @param startCoordinate Start square.
     * @param endCoordinate Destination square.
     * @return true if path is clear and destination is empty or enemy.
     */
    bool Queen::MovePossible(ChessCoordinate &startCoordinate, ChessCoordinate &endCoordinate)
    {
        int ranksForward = abs(endCoordinate.rank - startCoordinate.rank);
        int filesRightward = abs(endCoordinate.file - startCoordinate.file);

        if(((ranksForward == filesRightward) || (ranksForward == 0 && filesRightward > 0 ) || (ranksForward > 0 && filesRightward == 0 )) && 
            !PiecesInBetweenPath(startCoordinate,endCoordinate) &&
            (ChessState::Get().GetPieceOnChessCoordinate(endCoordinate) == PieceType::invalid || isEnemy(endCoordinate)))
        {
            return true;
        }
        return false;
    }

    /**
     * @brief Apply the move to `ChessState` for the correct side.
     */
    void Queen::MakeMove(ChessCoordinate &startCoordinate, ChessCoordinate &endCoordinate)
    {
        if(mWhitePieces)
        {
            ChessState::Get().SetPiecePosition(PieceType::whiteQueen,startCoordinate,endCoordinate);
        }
        else
        {
            ChessState::Get().SetPiecePosition(PieceType::blackQueen,startCoordinate,endCoordinate);
        }
    }

    /**
     * @brief Draw the appropriate queen sprite to the window.
     */
    void Queen::RenderPiece()
    {
        if(mWhitePieces)
        {
            mOwningStage->GetWindow().draw(mWhiteQueenSprite);
        }
        else
        {
            mOwningStage->GetWindow().draw(mBlackQueenSprite);
        }
    }

    /**
     * @brief Set the on-screen position of the queen sprite.
     */
    void Queen::SetPieceLocation(const sf::Vector2f &newLocation, bool whitePieces)
    {
        if(whitePieces)
            mWhiteQueenSprite.setPosition(newLocation + sf::Vector2f{-6.f,0.f});
        else
            mBlackQueenSprite.setPosition(newLocation + sf::Vector2f{-6.f,0.f});
    }
    
    /**
     * @brief Set the rotation of the queen sprite (unused).
     */
    void Queen::SetPieceRotation(float newRotation, bool whitePieces)
    {
        // sf::Angle newRot{newRotation};
        // mWhiteQueenSprite.setRotation(newRot);
    }

    /**
     * @brief Legal queen moves in 8 directions.
     * @param pieceCoordinate Current square of the queen.
     * @return All reachable squares until blocked; includes one capture per ray.
     */
    List<ChessCoordinate> Queen::GetAllPossibleMoves(const ChessCoordinate pieceCoordinate)
    {
        List<ChessCoordinate> moves;
        moves.reserve(32);

        int offsetRank[8] = { -1,  -1,  1,  1,  0,   0,  1, -1};
        int offsetFile[8] = {  1,  -1, -1,  1,  1,  -1,  0,  0};

        for(int i = 0; i < 8; i++)
        {
            ChessCoordinate iter{pieceCoordinate.rank, pieceCoordinate.file};
            iter.rank += offsetRank[i];
            iter.file += offsetFile[i];

            while(iter.isValid())
            {
                if(ChessState::Get().GetPieceOnChessCoordinate(iter) != PieceType::invalid)
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
     * @brief Get current sprite position for this queen.
     */
    sf::Vector2f Queen::GetPieceLocation() const
    {
        if(mWhitePieces)
            return mWhiteQueenSprite.getPosition();
        else
            return mBlackQueenSprite.getPosition();
    }
    
    /**
     * @brief Get current sprite rotation (always 0 for now).
     */
    float Queen::GetPieceRotation() const
    {
        return 0.0f;
    }

    /**
     * @brief Center sprite origin based on its global bounds.
     */
    void Queen::CenterPivot()
    {
        sf::FloatRect bound ;
        if(mWhitePieces)
        {
            bound = mWhiteQueenSprite.getGlobalBounds();
            mWhiteQueenSprite.setOrigin({float(bound.position.x) ,float(bound.position.y)});
        }
        else
        {
            bound = mBlackQueenSprite.getGlobalBounds();
            mBlackQueenSprite.setOrigin({float(bound.position.x) ,float(bound.position.y)});
        }
    }
    /**
     * @brief Check if a target square contains an opponent piece.
     */
    bool Queen::isEnemy(ChessCoordinate &endCoordinate)
    {
        return ((mWhitePieces && !Piece::GetPieceColor(ChessState::Get().GetPieceOnChessCoordinate(endCoordinate))) || (!mWhitePieces && Piece::GetPieceColor(ChessState::Get().GetPieceOnChessCoordinate(endCoordinate))));
    }

    /**
     * @brief Determine if any piece blocks the path to destination.
     * Normalizes direction and steps one square at a time until end.
     */
    bool Queen::PiecesInBetweenPath(ChessCoordinate &startCoordinate, ChessCoordinate &endCoordinate)
    {
        int offsetX = (endCoordinate.file - startCoordinate.file);
        int offsetY = (endCoordinate.rank - startCoordinate.rank);

        if(offsetX > 0)
            offsetX = 1;
        else if(offsetX < 0)
            offsetX = -1;

        if(offsetY > 0)
            offsetY = 1;
        else if(offsetY < 0)
            offsetY = -1;

        ChessCoordinate iterator{startCoordinate.rank,startCoordinate.file};
        iterator.file += offsetX;
        iterator.rank += offsetY;

        while(iterator.isValid() && !(iterator == endCoordinate))
        {
            if(ChessState::Get().GetPieceOnChessCoordinate(iterator) != PieceType::invalid)
            {
                return true;
            }
            iterator.file += offsetX;
            iterator.rank += offsetY;
        }
        return false;
    }
}