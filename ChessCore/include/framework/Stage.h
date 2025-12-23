/**
 * @file Stage.h
 * @brief Base class for game stages/levels in the Chess game
 * 
 * This file contains the declaration of the Stage class which serves as the base
 * class for different game levels or stages in the Chess game, providing common
 * functionality for rendering, event handling, and game logic.
 */

#pragma once

#include <SFML/Graphics.hpp>
#include "framework/Core.h"
#include "framework/Object.h"

namespace chess
{
  class Application;
  class Board;
  class Piece;
  class King;
  class Pawn;
  class Knight;
  class Bishop;
  class Rook;
  class Queen;
  class HUD;

  /**
   * @brief Base class for game stages/levels
   * 
   * The Stage class provides the foundation for different game states in the Chess application.
   * It handles rendering, input processing, and game logic for a specific game state.
   */
  class Stage : public Object
  {
    public:
      /**
       * @brief Construct a new Stage
       * 
       * @param owningApp Pointer to the parent Application
       */
      Stage(Application* owningApp);
      
      /**
       * @brief Initialize the stage
       */
      virtual void Init();
      
      /**
       * @brief Render the stage by Render the HUD
       */
      virtual void Render();
      
      /**
       * @brief Internal tick function called by the application
       * 
       * @param deltaTime Time elapsed since the last frame in seconds
       */
      void TickInternal(float deltaTime);

      /**
       * @brief Get the render window from the application
       * 
       * @return sf::RenderWindow& Reference to the render window
       */
      sf::RenderWindow& GetWindow();
      
      /**
       * @brief Get the scale factor for sprites
       * 
       * @return sf::Vector2f The scale factor as a 2D vector
       */
      sf::Vector2f GetSpriteScale();

      /**
       * @brief Check if a piece has been moved
       * 
       * @return true if a piece has been moved, false otherwise
       */
      inline bool IsPieceMoved(){ return mPieceMoved;}
      
      /**
       * @brief Set whether a piece has been moved
       * 
       * @param moved Whether a piece has been moved
       */
      inline void SetPieceMoved(bool moved){ mPieceMoved = moved; }

      /**
       * @brief Spawn a HUD element
       * 
       * @tparam HUDType Type of HUD to spawn
       * @tparam Args Argument types for HUD constructor
       * @param args Arguments for HUD constructor
       * @return weak<HUDType> Weak pointer to the created HUD
       */
      template<typename HUDType, typename...Args>
      weak<HUDType> SpawnHUD(Args... args);

      /**
       * @brief Handle an SFML event
       * 
       * @param event The SFML event to handle
       * @return true if the event was handled, false otherwise
       */
      bool HandleEvent(const std::optional<sf::Event> & event);
      
      /**
       * @brief Internal event handler that can be overridden by derived classes
       * 
       * @param event The SFML event to handle
       * @return true if the event was handled, false otherwise
       */
      virtual bool HandleEventInternal(const std::optional<sf::Event> & event);

      /**
       * @brief Internal function to begin play
       */
      void BeginPlayInternal();

      /**
       * @brief Get the parent Application
       * 
       * @return Application* Pointer to the parent Application
       */
      Application* GetApplication(){ return mOwningApp; }
    
    protected:
      /**
       * @brief Render the chess board
       */
      void RenderBoard();
      
      /**
       * @brief Render all chess pieces
       */
      void RenderPieces();
      
      /**
       * @brief Render the HUD elements
       * 
       * @param renderWindow The render window to draw to
       */
      void RenderHUD(sf::RenderWindow & renderWindow);

      /**
       * @brief Handle board-related events
       * 
       * @param event The SFML event to handle
       * @return true if the event was handled, false otherwise
       */
      bool HandleBoardEvent(const std::optional<sf::Event> & event);

    private:
      /**
       * @brief Check if the correct piece is selected
       * 
       * @param piece The piece to check
       * @return true if the correct piece is selected, false otherwise
       */
      bool CheckCorrectPieceSelected(PieceType piece);
      
      /**
       * @brief Move a piece on the board
       * 
       * @param piece The piece to move
       * @return true if the move was successful, false otherwise
       */
      bool MovePiece(PieceType piece);

      /**
       * @brief Check if castling is possible
       * 
       * @param kingCoordinate The king's position
       * @param rookCoordinate The rook's position
       * @return true if castling is possible, false otherwise
       */
      bool CastlingPossible(ChessCoordinate kingCoordinate, ChessCoordinate rookCoordinate);
      
      /**
       * @brief Perform kingside castling
       * 
       * @param whitePiece Whether the king is white
       */
      void CastleKingSide(bool whitePiece);
      
      /**
       * @brief Perform queenside castling
       * 
       * @param whitePiece Whether the king is white
       */
      void CastleQueenSide(bool whitePiece);

      /**
       * @brief Convert chess coordinates to screen position
       * 
       * @param chessCoordinate The chess coordinate to convert
       * @return const sf::Vector2f The screen position
       */
      const sf::Vector2f ConvertChessCoordinateToPosition(const ChessCoordinate& chessCoordinate);
      
      /**
       * @brief Convert screen position to chess coordinates
       * 
       * @param position The screen position
       * @return ChessCoordinate The chess coordinate
       */
      ChessCoordinate ConvertPositionToChessCoordinate(const sf::Vector2i& position);

      /**
       * @brief Spawn the chess board
       * 
       * @param boardStart The top-left corner of the board
       * @param boardEnd The bottom-right corner of the board
       * @return shared<Board> Shared pointer to the created board
       */
      shared<Board> SpawnBoard(const sf::Vector2f& boardStart, const sf::Vector2f& boardEnd);

      /**
       * @brief Spawn a chess piece
       * 
       * @tparam PieceType Type of piece to spawn
       * @param whitePiece Whether the piece is white
       * @return shared<PieceType> Shared pointer to the created piece
       */
      template<typename PieceType>
      shared<PieceType> SpawnPiece(bool whitePiece);

      /**
       * @brief Get the container for a piece
       * 
       * @param piece The piece to get the container for
       * @return shared<Piece> Shared pointer to the piece container
       */
      shared<Piece> GetPieceContainer(PieceType piece);

      /**
       * @brief Determine which piece to promote a pawn to
       * 
       * @return PieceType to promote to
       */
      PieceType WhichPieceToPromote();

      /**
       * @brief Check the current game state which might be Ongoing, WhiteWin, BlackWin, Draw
       * 
       * @return int The end state code
       */
      GameState EndState();

      /**
       * @brief Render possible moves for the selected piece
       */
      void RenderPossibleMoves();

      /**
       * @brief Render the king in check indicator
       */
      void RenderKingInCheck();

      /**
       * @brief Render the last played move
       */
      void RenderLastPlayedMove();

      /**
       * @brief Called when the stage begins play
       */
      virtual void BeginPlay();

      Application* mOwningApp;      ///< Pointer to the parent Application
      shared<Board> mBoard;         ///< The chess board

      // White pieces
      shared<King> mWhiteKing;      ///< White king
      shared<Queen> mWhiteQueen;    ///< White queen
      shared<Rook> mWhiteRook;      ///< White rooks
      shared<Knight> mWhiteKnight;  ///< White knights
      shared<Bishop> mWhiteBishop;  ///< White bishops
      shared<Pawn> mWhitePawn;      ///< White pawns
      
      // Black pieces
      shared<King> mBlackKing;      ///< Black king
      shared<Queen> mBlackQueen;    ///< Black queen
      shared<Rook> mBlackRook;      ///< Black rooks
      shared<Knight> mBlackKnight;  ///< Black knights
      shared<Bishop> mBlackBishop;  ///< Black bishops
      shared<Pawn> mBlackPawn;      ///< Black pawns

      // Game state flags
      bool mPieceMoved;             ///< Whether a piece has been moved

      int mPieceOffsetX;            ///< X offset of the piece
      int mPieceOffsetY;            ///< Y offset of the piece

      bool mPieceSelected;          ///< Whether a piece is selected
      ChessCoordinate mStartPose;   ///< Start position of the piece
      ChessCoordinate mEndPose;     ///< End position of the piece

      bool mWhiteTurn;              ///< Whether it's white's turn

      bool mMouseDragging;          ///< Whether the mouse is dragging
      sf::Vector2i mMousePosition;  ///< Mouse position

      bool mFlipBoard;              ///< Whether the board is flipped

      bool mRenderPossibleMoves;    ///< Whether to render possible moves
      sf::Color mPossibleMovesColor;///< Color of possible moves

      sf::Color mKingInCheckColor;  ///< Color of king in check

      shared<HUD> mHUD;             ///< The HUD

      bool mBeginPlay;              ///< Whether the stage has begun play
  };

  /**
   * @brief Spawn a chess piece
   * 
   * @tparam PieceType Type of piece to spawn
   * @param whitePiece Whether the piece is white
   * @return shared<PieceType> Shared pointer to the created piece
   */
  template<typename PieceType>
  inline shared<PieceType> Stage::SpawnPiece(bool whitePiece)
  {
      shared<PieceType> newPiece{new PieceType{this,whitePiece}};
      return newPiece;
  }
  
  /**
   * @brief Spawn a HUD element
   * 
   * @tparam HUDType Type of HUD to spawn
   * @param args Arguments for HUD constructor
   * @return weak<HUDType> Weak pointer to the created HUD
   */
  template <typename HUDType, typename... Args>
  inline weak<HUDType> Stage::SpawnHUD(Args... args)
  {
      mHUD = std::make_shared<HUDType>(args...);
      return std::static_pointer_cast<HUDType>(mHUD);
  }
}
