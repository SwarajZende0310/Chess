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

  class Stage : public Object
  {
    public:
      Stage(Application* owningApp);
      
      virtual void Init();
      virtual void Render();
      void TickInternal(float deltaTime);

      sf::RenderWindow& GetWindow();
      sf::Vector2f GetSpriteScale();

      inline bool IsPieceMoved(){ return mPieceMoved;}
      inline void SetPieceMoved(bool moved){ mPieceMoved = moved; }

      template<typename HUDType, typename...Args>
      weak<HUDType> SpawnHUD(Args... args);

      bool HandleEvent(const std::optional<sf::Event> & event);
      virtual bool HandleEventInternal(const std::optional<sf::Event> & event);

      void BeginPlayInternal();

      Application* GetApplication(){ return mOwningApp;}
    
    protected:
      void RenderBoard();
      void RenderPieces();
      
      void RenderHUD(sf::RenderWindow & renderWindow);

      bool HandleBoardEvent(const std::optional<sf::Event> & event);

    private:

      bool CheckCorrectPieceSelected(char piece);
      bool MovePiece(char piece);

      bool CastlingPossible(ChessCoordinate kingCoordinate, ChessCoordinate rookCoordinate);
      void CastleKingSide(bool whitePiece);
      void CastleQueenSide(bool whitePiece);

      const sf::Vector2f ConvertChessCoordinateToPosition(const ChessCoordinate& chessCoordinate);
      ChessCoordinate ConvertPositionToChessCoordinate(const sf::Vector2i& position);

      shared<Board> SpawnBoard(const sf::Vector2f& boardStart, const sf::Vector2f& boardEnd);

      template<typename PieceType>
      shared<PieceType> SpawnPiece(bool whitePiece);

      shared<Piece> GetPieceContainer(char piece);

      char WhichPieceToPromote();

      int EndState();

      void RenderPossibleMoves();

      void RenderKingInCheck();

      void RenderLastPlayedMove();

      virtual void BeginPlay();

      Application* mOwningApp;

      shared<Board> mBoard;

      shared<King> mWhiteKing;
      shared<Queen> mWhiteQueen;
      shared<Rook> mWhiteRook;
      shared<Knight> mWhiteKnight;
      shared<Bishop> mWhiteBishop;
      shared<Pawn> mWhitePawn;
      
      shared<King> mBlackKing;
      shared<Queen> mBlackQueen;
      shared<Rook> mBlackRook;
      shared<Knight> mBlackKnight;
      shared<Bishop> mBlackBishop;
      shared<Pawn> mBlackPawn;

      bool mPieceMoved;

      int mPieceOffsetX;
      int mPieceOffsetY;

      bool mPieceSelected;
      ChessCoordinate mStartPose;
      ChessCoordinate mEndPose;

      bool mWhiteTurn;

      bool mMouseDragging;
      sf::Vector2i mMousePosition;

      bool mFlipBoard;

      bool mRenderPossibleMoves;
      sf::Color mPossibleMovesColor;

      sf::Color mKingInCheckColor;

      shared<HUD> mHUD;

      bool mBeginPlay;
  };

  template <typename PieceType>
  inline shared<PieceType> Stage::SpawnPiece(bool whitePiece)
  {
      shared<PieceType> newPiece{new PieceType{this,whitePiece}};
      return newPiece;
  }
  
  template <typename HUDType, typename... Args>
  inline weak<HUDType> Stage::SpawnHUD(Args... args)
  {
      mHUD = std::make_shared<HUDType>(args...);
      return std::static_pointer_cast<HUDType>(mHUD);
  }
}
