#include"framework/Stage.h"
#include"framework/Board.h"
#include"framework/Application.h"
#include"Pieces/King.h"

namespace chess
{
  Stage::Stage(Application* owningApp)
    :mOwningApp{owningApp},
    mBoard{}
  {
    SpawnBoard({0.f,0.f},{800.f,800.f});
    mWhiteKing = SpawnPiece<King>(true);
  }

  void Stage::Init()
  {
  }

  void Stage::Render()
  {
    mBoard->RefreshBoard();
    mWhiteKing->RenderPiece();
  }

  sf::RenderWindow &Stage::GetWindow()
  {
      return mOwningApp->GetWindow();
  }

  void Stage::RenderBoard()
  {
  }

  void Stage::RenderPieces()
  {
  }

  const sf::Vector2f &Stage::ConvertChessCoordinateToPosition(const ChessCoordinate &chessCoordinate)
  {
      return sf::Vector2f{};
  }

  shared<Board> Stage::SpawnBoard(const sf::Vector2f &boardStart, const sf::Vector2f &boardEnd)
  {
      mBoard = std::make_shared<Board>(this, boardStart, boardEnd); 
      return mBoard;
  }

  sf::Vector2f Stage::GetSpriteScale()
  {
      return mBoard->GetSpriteScale();
  }
  bool Stage::HandleEvent(const sf::Event &event)
  {
      return false;
  }
}