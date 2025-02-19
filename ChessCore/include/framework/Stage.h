#pragma once

#include <SFML/Graphics.hpp>
#include "framework/Core.h"

namespace chess
{
  class Application;
  class Board;
  class Actor;
  class King;
  class Stage
  {
    public:
      Stage(Application* owningApp);
      
      virtual void Init();
      virtual void Render();

      sf::RenderWindow& GetWindow();
      sf::Vector2f GetSpriteScale();
      
    private:
      void RenderBoard();
      void RenderPieces();

      const sf::Vector2f& ConvertChessCoordinateToPosition(const ChessCoordinate& chessCoordinate);

      shared<Board> SpawnBoard(const sf::Vector2f& boardStart, const sf::Vector2f& boardEnd);

      template<typename PieceType>
      shared<PieceType> SpawnPiece(bool whitePiece);

      bool HandleEvent(const sf::Event& event);

      Application* mOwningApp;

      shared<Board> mBoard;
      // shared<Actor> mPieces[12];

      shared<King> mWhiteKing;
  };

  template <typename PieceType>
  inline shared<PieceType> Stage::SpawnPiece(bool whitePiece)
  {
      shared<PieceType> newPiece{new PieceType{this,whitePiece}};
      return newPiece;
  }
}
