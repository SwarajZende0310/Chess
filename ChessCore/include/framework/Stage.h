#pragma once

#include <SFML/Graphics.hpp>
#include "framework/Core.h"

namespace chess
{
  class Application;
  class Board;
  class Actor;
  class King;
  class Pawn;
  class Knight;
  class Bishop;
  class Rook;
  class Queen;
  class Stage
  {
    public:
      Stage(Application* owningApp);
      
      virtual void Init();
      virtual void Render();

      sf::RenderWindow& GetWindow();
      sf::Vector2f GetSpriteScale();

      inline bool IsPieceMoved(){ return mPieceMoved;}
      inline void SetPieceMoved(bool moved){ mPieceMoved = moved; }
      
    private:
      void RenderBoard();
      void RenderPieces();

      const sf::Vector2f ConvertChessCoordinateToPosition(const ChessCoordinate& chessCoordinate);

      shared<Board> SpawnBoard(const sf::Vector2f& boardStart, const sf::Vector2f& boardEnd);

      template<typename PieceType>
      shared<PieceType> SpawnPiece(bool whitePiece);

      bool HandleEvent(const sf::Event& event);

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
  };

  template <typename PieceType>
  inline shared<PieceType> Stage::SpawnPiece(bool whitePiece)
  {
      shared<PieceType> newPiece{new PieceType{this,whitePiece}};
      return newPiece;
  }
}
