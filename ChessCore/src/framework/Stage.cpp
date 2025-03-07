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
    mWhiteTurn{true},
    mMouseDragging{false},
    mMousePosition{-1,-1}
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

  void Stage::TickInternal(float deltaTime)
  {

  }
  sf::RenderWindow &Stage::GetWindow()
  {
      return mOwningApp->GetWindow();
  }

  bool Stage::HandleEvent(const std::optional<sf::Event> &event)
  {
      bool handled = false;

      if(const auto* mouseMoved = event->getIf<sf::Event::MouseMoved>())
      {
        mMousePosition = mouseMoved->position;
      }
      if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>())
      {
          if (mouseButtonPressed->button == sf::Mouse::Button::Left)
          {      
              mMouseDragging = true;     
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
                else if(ChessState::Get().GetPieceOnChessCoordinate(mEndPose) == invalid)
                {
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
      else if( const auto* mouseButtonReleased = event->getIf<sf::Event::MouseButtonReleased>())
      {
        mMouseDragging = false;
        mEndPose = ConvertPositionToChessCoordinate({mouseButtonReleased->position.x,mouseButtonReleased->position.y});
        char piece = ChessState::Get().GetPieceOnChessCoordinate(mStartPose);
        if(MovePiece(piece))
        {
          mPieceSelected = false;
        }

        handled = true;
      }
      else if (const auto* keyPress = event->getIf<sf::Event::KeyPressed>())
      {
        if(keyPress->scancode == sf::Keyboard::Scan::Left)
        {
          ChessState::Get().UndoLastMove();
          SetPieceMoved(true);
          mWhiteTurn = !mWhiteTurn;
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
        // If piece is picked and mouse is dragging dont render
        if(mWhiteTurn && mPieceSelected && mMouseDragging && mStartPose.isValid() && coordinate == mStartPose)
          continue;
        else
        {
          GetPieceContainer(whitePieces[i])->SetPieceLocation(ConvertChessCoordinateToPosition(coordinate),true);
          GetPieceContainer(whitePieces[i])->RenderPiece();
        }
      }
    }

    // Render Black Pieces
    for(int i = 0; i < 6; i++)
    {
      List<ChessCoordinate> coordinates = ChessState::Get().GetPiecePosiiton(blackPieces[i]);
      for(auto &coordinate : coordinates)
      {
        // If piece is picked and mouse is dragging dont render
        if(!mWhiteTurn && mPieceSelected && mMouseDragging && mStartPose.isValid() && coordinate == mStartPose)
          continue;
        else
        {
          GetPieceContainer(blackPieces[i])->SetPieceLocation(ConvertChessCoordinateToPosition(coordinate),false);
          GetPieceContainer(blackPieces[i])->RenderPiece();
        }
      }
    }

    if(mMouseDragging && mPieceSelected && mStartPose.isValid())
    {
      char piece = ChessState::Get().GetPieceOnChessCoordinate(mStartPose);
      GetPieceContainer(piece)->SetPieceLocation({float(mMousePosition.x - mBoard->GetSquareOffsetX()/2.f) ,float(mMousePosition.y - mBoard->GetSquareOffsetY()/2.f)},GetPieceContainer(piece)->GetPieceColor());
      GetPieceContainer(piece)->RenderPiece();
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
    if(piece == invalid)return false;

    shared<Piece> piecePointer = GetPieceContainer(piece); 

    // Check for castling
    if(CastlingPossible(mStartPose,mEndPose))
    {
      if(mWhiteTurn)
      {
        if(mEndPose.file == 'h')
        {
          mEndPose.file = 'g';
          mWhiteKing->MakeMove(mStartPose, mEndPose);
          mStartPose.file = 'h';
          mEndPose.file = 'f';
          mWhiteRook->MakeMove(mStartPose, mEndPose);
        }
        else if(mEndPose.file == 'a')
        {
          mEndPose.file = 'c';
          mWhiteKing->MakeMove(mStartPose, mEndPose);
          mStartPose.file = 'a';
          mEndPose.file = 'd';
          mWhiteRook->MakeMove(mStartPose, mEndPose);
        }
      }
      else
      {
        if(mEndPose.file == 'h')
        {
          mEndPose.file = 'g';
          mBlackKing->MakeMove(mStartPose, mEndPose);
          mStartPose.file = 'h';
          mEndPose.file = 'f';
          mBlackRook->MakeMove(mStartPose, mEndPose);
        }
        else if(mEndPose.file == 'a')
        {
          mEndPose.file = 'c';
          mBlackKing->MakeMove(mStartPose, mEndPose);
          mStartPose.file = 'a';
          mEndPose.file = 'd';
          mBlackRook->MakeMove(mStartPose, mEndPose);
        }
      }
      mWhiteTurn = !mWhiteTurn;
      return true;
    }

    // Determine whose turn and valid move
    if((mWhiteTurn && piecePointer->GetPieceColor() && piecePointer->MovePossible(mStartPose,mEndPose)) || 
      (!mWhiteTurn && !piecePointer->GetPieceColor() && piecePointer->MovePossible(mStartPose,mEndPose)))
    {
      piecePointer->MakeMove(mStartPose, mEndPose);
      bool kingInCheck = mWhiteTurn ? mWhiteKing->IsInCheck() : mBlackKing->IsInCheck();
      
      if(kingInCheck)// If King in Check after making move then illegal move
      {
        ChessState::Get().UndoLastMove();
        return false;
      }
      mWhiteTurn = !mWhiteTurn;
      return true;
    }  
    return false;
  }

  bool Stage::CastlingPossible(ChessCoordinate kingCoordinate, ChessCoordinate rookCoordinate)
  {
      if(kingCoordinate == rookCoordinate)return false;

      if(mWhiteTurn)
      {
        if(ChessState::Get().GetPieceOnChessCoordinate(kingCoordinate) != whiteKing 
          || ChessState::Get().GetPieceOnChessCoordinate(rookCoordinate) != whiteRook
          || !mWhiteKing->IsFirstMove() 
          || !mWhiteRook->IsFirstMove(rookCoordinate))
            return false;

        int offsetFile = (rookCoordinate.file - kingCoordinate.file) > 0 ? 1 :  -1;
        
        Set<ChessCoordinate,ChessCoordinateHashFunction> blackAttackedSquares = ChessState::Get().GetBlackAttackedSquares();
        ChessCoordinate iter{kingCoordinate.rank,kingCoordinate.file};
        iter.file += offsetFile;

        while(iter.isValid() && !(iter == rookCoordinate))
        {
          if(ChessState::Get().GetPieceOnChessCoordinate(iter) != invalid && blackAttackedSquares.find(iter) != blackAttackedSquares.end())
            return false;
          iter.file += offsetFile;
        }
        return true;
      }
      else
      {
        if(ChessState::Get().GetPieceOnChessCoordinate(kingCoordinate) != blackKing 
        || ChessState::Get().GetPieceOnChessCoordinate(rookCoordinate) != blackRook
          || !mBlackKing->IsFirstMove() 
          || !mBlackRook->IsFirstMove(rookCoordinate))
            return false;

        int offsetFile = (rookCoordinate.file - kingCoordinate.file) > 0 ? 1 :  -1;
        
        Set<ChessCoordinate,ChessCoordinateHashFunction> whiteAttackedSquares = ChessState::Get().GetWhiteAttackedSquares();
        ChessCoordinate iter{kingCoordinate.rank,kingCoordinate.file};
        iter.file += offsetFile;

        while(iter.isValid() && !(iter == rookCoordinate))
        {
          if(ChessState::Get().GetPieceOnChessCoordinate(iter) != invalid && whiteAttackedSquares.find(iter) != whiteAttackedSquares.end())
            return false;
          iter.file += offsetFile;
        }
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