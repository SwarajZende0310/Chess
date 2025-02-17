#include"framework/Stage.h"
#include"framework/Board.h"
#include"framework/Application.h"

namespace chess
{
  Stage::Stage(Application* owningApp)
    :mOwningApp{owningApp},
    mBoard{}
  {
    SpawnBoard({0.f,0.f},{800.f,800.f});
  }

  void Stage::Init()
  {
  }
  void Stage::Render()
  {
    mBoard->RefreshBoard();
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

  bool Stage::HandleEvent(const sf::Event &event)
  {
      return false;
  }
}