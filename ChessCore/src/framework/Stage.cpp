#include"framework/Stage.h"
#include"framework/Board.h"
#include"framework/Application.h"
#include"framework/ChessState.h"
#include"Pieces/King.h"
#include"Pieces/Queen.h"
#include"Pieces/Rook.h"
#include"Pieces/Knight.h"
#include"Pieces/Bishop.h"
#include"Pieces/Pawn.h"

namespace chess
{
  Stage::Stage(Application* owningApp)
    :mOwningApp{owningApp},
    mBoard{},
    mWhiteKing{},
    mWhiteQueen{},
    mWhiteRook{},
    mWhiteKnight{},
    mWhiteBishop{},
    mWhitePawn{},
    mBlackKing{},
    mBlackQueen{},
    mBlackRook{},
    mBlackKnight{},
    mBlackBishop{},
    mBlackPawn{},
    mPieceMoved{true},
    mPieceOffsetX{10},
    mPieceOffsetY{8}
  {
    SpawnBoard({0.f,0.f},{800.f,800.f});
    ChessState::Get().ResetToStartPosition();

    mWhiteKing = SpawnPiece<King>(true);
    mWhiteQueen = SpawnPiece<Queen>(true);
    mWhiteRook = SpawnPiece<Rook>(true);
    mWhiteKnight = SpawnPiece<Knight>(true);
    mWhiteBishop = SpawnPiece<Bishop>(true);
    mWhitePawn = SpawnPiece<Pawn>(true);
    
    mBlackKing = SpawnPiece<King>(false);
    mBlackQueen = SpawnPiece<Queen>(false);
    mBlackRook = SpawnPiece<Rook>(false);
    mBlackKnight = SpawnPiece<Knight>(false);
    mBlackBishop = SpawnPiece<Bishop>(false);
    mBlackPawn = SpawnPiece<Pawn>(false);
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
    // Render White Pieces
    // Render Pawns
    for(auto &coordinate : ChessState::Get().GetWhitePawnsPosition())
    {
      mWhitePawn->SetPieceLocation(ConvertChessCoordinateToPosition(coordinate),true);
      mWhitePawn->RenderPiece();
    }

    // Render Bishops
    for(auto &coordinate : ChessState::Get().GetWhiteBishopsPosition())
    {
      mWhiteBishop->SetPieceLocation(ConvertChessCoordinateToPosition(coordinate),true);
      mWhiteBishop->RenderPiece();
    }

    // Render Knights
    for(auto &coordinate : ChessState::Get().GetWhiteKnightsPosition())
    {
      mWhiteKnight->SetPieceLocation(ConvertChessCoordinateToPosition(coordinate),true);
      mWhiteKnight->RenderPiece();
    }

    // Render Rooks
    for(auto &coordinate : ChessState::Get().GetWhiteRooksPosition())
    {
      mWhiteRook->SetPieceLocation(ConvertChessCoordinateToPosition(coordinate),true);
      mWhiteRook->RenderPiece();
    }

    // Render Queen
    ChessCoordinate queenPose = ChessState::Get().GetWhiteQueenPosition();
    if(queenPose.file != 'n' && queenPose.rank != -1)
    {
      mWhiteQueen->SetPieceLocation(ConvertChessCoordinateToPosition(queenPose),true);
      mWhiteQueen->RenderPiece();
    }

    mWhiteKing->SetPieceLocation(ConvertChessCoordinateToPosition(ChessState::Get().GetWhiteKingPosition()), true);
    mWhiteKing->RenderPiece();

    // Render Black Pieces
    // Render Pawns
    for(auto &coordinate : ChessState::Get().GetBlackPawnsPosition())
    {
      mBlackPawn->SetPieceLocation(ConvertChessCoordinateToPosition(coordinate),false);
      mBlackPawn->RenderPiece();
    }

    // Render Bishops
    for(auto &coordinate : ChessState::Get().GetBlackBishopsPosition())
    {
      mBlackBishop->SetPieceLocation(ConvertChessCoordinateToPosition(coordinate),false);
      mBlackBishop->RenderPiece();
    }

    // Render Knights
    for(auto &coordinate : ChessState::Get().GetBlackKnightsPosition())
    {
      mBlackKnight->SetPieceLocation(ConvertChessCoordinateToPosition(coordinate),false);
      mBlackKnight->RenderPiece();
    }

    // Render Rooks
    for(auto &coordinate : ChessState::Get().GetBlackRooksPosition())
    {
      mBlackRook->SetPieceLocation(ConvertChessCoordinateToPosition(coordinate),false);
      mBlackRook->RenderPiece();
    }

    // Render Queen
    queenPose = ChessState::Get().GetBlackQueenPosition();
    if(queenPose.file != 'n' && queenPose.rank != -1)
    {
      mBlackQueen->SetPieceLocation(ConvertChessCoordinateToPosition(queenPose),false);
      mBlackQueen->RenderPiece();
    }

    mBlackKing->SetPieceLocation(ConvertChessCoordinateToPosition(ChessState::Get().GetBlackKingPosition()),false);
    mBlackKing->RenderPiece();
  }

  const sf::Vector2f Stage::ConvertChessCoordinateToPosition(const ChessCoordinate &chessCoordinate)
  {
      int row = chessCoordinate.rank - 1;
      int col = chessCoordinate.file - 'a';

      return sf::Vector2f{mBoard->GetSquareOffsetX() * (col) + mPieceOffsetX, mBoard->GetSquareOffsetY() * (7 - row) + mPieceOffsetY};
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