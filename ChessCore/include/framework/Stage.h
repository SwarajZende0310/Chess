#pragma once

#include <SFML/Graphics.hpp>
#include"framework/Core.h"

namespace chess
{
  class Application;
  class Board;
  class Actor;
  class Stage
  {
    public:
      Stage(Application* owningApp);
      
      virtual void Init();
      virtual void Render();

      sf::RenderWindow& GetWindow();
    private:
      void RenderBoard();
      void RenderPieces();

      const sf::Vector2f& ConvertChessCoordinateToPosition(const ChessCoordinate& chessCoordinate);

      shared<Board> SpawnBoard(const sf::Vector2f& boardStart, const sf::Vector2f& boardEnd);

      template<typename PieceType>
      shared<PieceType> SpawnPiece(const sf::Vector2f& piecePosition);

      bool HandleEvent(const sf::Event& event);

      Application* mOwningApp;

      shared<Board> mBoard;
      // shared<Actor> mPieces[12];
  };

  template <typename PieceType>
  inline shared<PieceType> Stage::SpawnPiece(const sf::Vector2f &piecePosition)
  {
      return shared<PieceType>();
  }
}
