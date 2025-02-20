#include"framework/Stage.h"
#include"framework/Board.h"
#include"framework/Application.h"
#include"framework/ChessState.h"
#include"Pieces/King.h"

namespace chess
{
  Stage::Stage(Application* owningApp)
    :mOwningApp{owningApp},
    mBoard{}
  {
    SpawnBoard({0.f,0.f},{800.f,800.f});
    ChessState::Get().ResetToStartPosition();
    mWhiteKing = SpawnPiece<King>(true);
    mBlackKing = SpawnPiece<King>(false);
  }

  void Stage::Init()
  {
  }

  void Stage::Render()
  {
    RenderBoard();
    RenderPieces();
  }

  sf::RenderWindow &Stage::GetWindow()
  {
      return mOwningApp->GetWindow();
  }

  void Stage::RenderBoard()
  {
    mBoard->RefreshBoard();
  }

  void Stage::RenderPieces()
  {
    mWhiteKing->SetPieceLocation(ConvertChessCoordinateToPosition(ChessState::Get().GetWhiteKingPosition()), true);
    mWhiteKing->RenderPiece();

    mBlackKing->SetPieceLocation(ConvertChessCoordinateToPosition(ChessState::Get().GetBlackKingPosition()),false);
    mBlackKing->RenderPiece();
  }

  const sf::Vector2f Stage::ConvertChessCoordinateToPosition(const ChessCoordinate &chessCoordinate)
  {
      int row = chessCoordinate.rank - 1;
      int col = chessCoordinate.file - 'a';

      return sf::Vector2f{mBoard->GetSquareOffsetX() * (col), mBoard->GetSquareOffsetY() * (7 - row)};
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