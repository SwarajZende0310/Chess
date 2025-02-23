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
    mPieceOffsetY{8},
    mPieceSelected{false},
    mStartPose{-1,invalid},
    mEndPose{-1,invalid},
    mWhiteTurn{true}
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

  bool Stage::HandleEvent(const std::optional<sf::Event> &event)
  {
      bool handled = false;

      if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>())
      {
          if (mouseButtonPressed->button == sf::Mouse::Button::Left)
          {
              if(!mPieceSelected)
              {
                mStartPose = ConvertPositionToChessCoordinate({mouseButtonPressed->position.x,mouseButtonPressed->position.y});
                mPieceSelected = CheckCorrectPieceSelected(ChessState::Get().GetPieceOnChessCoordinate(mStartPose));
                handled = true;
              }
              else
              {
                mEndPose = ConvertPositionToChessCoordinate({mouseButtonPressed->position.x,mouseButtonPressed->position.y});
                char piece = ChessState::Get().GetPieceOnChessCoordinate(mStartPose);
                // Change chess state and render everything again
                if(MovePiece(piece))
                {
                  SetPieceMoved(true);
                  mPieceSelected = false;
                }
                handled = true;
              }
          }
          else if(mouseButtonPressed->button == sf::Mouse::Button::Right)
          {
            mPieceSelected = false;
            handled = true;
          }
      }
      return handled;
  }
  
  void Stage::RenderBoard()
  {
    mBoard->RefreshBoard();
  }

  void Stage::RenderPieces()
  {
    char whitePieces[6] = {whiteKing,whiteQueen,whiteRook,whiteBishop,whiteKnight,whitePawn};
    char blackPieces[6] = {blackKing,blackQueen,blackRook,blackBishop,blackKnight,blackPawn};

    // Render White Pieces
    for(int i = 0; i < 6; i++)
    {
      List<ChessCoordinate> coordinates = ChessState::Get().GetPiecePosiiton(whitePieces[i]);
      for(auto &coordinate : coordinates)
      {
        GetPieceContainer(whitePieces[i])->SetPieceLocation(ConvertChessCoordinateToPosition(coordinate),true);
        GetPieceContainer(whitePieces[i])->RenderPiece();
      }
    }

    // Render Black Pieces
    for(int i = 0; i < 6; i++)
    {
      List<ChessCoordinate> coordinates = ChessState::Get().GetPiecePosiiton(blackPieces[i]);
      for(auto &coordinate : coordinates)
      {
        GetPieceContainer(blackPieces[i])->SetPieceLocation(ConvertChessCoordinateToPosition(coordinate),false);
        GetPieceContainer(blackPieces[i])->RenderPiece();
      }
    }
  }

  bool Stage::CheckCorrectPieceSelected(char piece)
  {
      if((mWhiteTurn && (piece==whiteKing || piece==whiteQueen || piece==whiteRook || piece==whiteKnight || piece==whiteBishop || piece==whitePawn)) 
          || (!mWhiteTurn && (piece==blackKing || piece==blackQueen || piece==blackRook || piece==blackKnight || piece==blackBishop || piece==blackPawn)))
          return true;
      return false;
  }
  
  bool Stage::MovePiece(char piece) 
  {
    shared<Piece> piecePointer = GetPieceContainer(piece); 
    // Determine whose turn and valid move
    if((mWhiteTurn && piecePointer->GetPieceColor() && piecePointer->MovePossible(mStartPose,mEndPose)) || 
      (!mWhiteTurn && !piecePointer->GetPieceColor() && piecePointer->MovePossible(mStartPose,mEndPose)))
    {
      piecePointer->MakeMove(mStartPose, mEndPose);
      mWhiteTurn = !mWhiteTurn;
      return true;
    }  
    return false;
  }

  const sf::Vector2f Stage::ConvertChessCoordinateToPosition(const ChessCoordinate &chessCoordinate)
  {
      int row = chessCoordinate.rank - 1;
      int col = chessCoordinate.file - 'a';

      return sf::Vector2f{mBoard->GetSquareOffsetX() * (col) + mPieceOffsetX, mBoard->GetSquareOffsetY() * (7 - row) + mPieceOffsetY};
  }

  ChessCoordinate Stage::ConvertPositionToChessCoordinate(const sf::Vector2i &position)
  {
      int row = position.y / mBoard->GetSquareOffsetX();
      int col = position.x / mBoard->GetSquareOffsetY(); 
      return ChessCoordinate{7 - row + 1, (char)(col + 'a')};
  }

  shared<Board> Stage::SpawnBoard(const sf::Vector2f &boardStart, const sf::Vector2f &boardEnd)
  {
      mBoard = std::make_shared<Board>(this, boardStart, boardEnd); 
      return mBoard;
  }

  shared<Piece> Stage::GetPieceContainer(char piece)
  {
      switch (piece)
        {
        case whitePawn:
            return mWhitePawn;
        case whiteBishop:
            return mWhiteBishop;
        case whiteKnight:
            return mWhiteKnight;
        case whiteRook:
            return mWhiteRook;
        case whiteQueen:
            return mWhiteQueen;
        case whiteKing:
            return mWhiteKing;
        case blackPawn:
            return mBlackPawn;
        case blackBishop:
            return mBlackBishop;
        case blackKnight:
            return mBlackKnight;
        case blackRook:
            return mBlackRook;
        case blackQueen:
            return mBlackQueen;
        case blackKing:
            return mBlackKing;
        }
      return nullptr;
  }
  sf::Vector2f Stage::GetSpriteScale()
  {
      return mBoard->GetSpriteScale();
  }
}