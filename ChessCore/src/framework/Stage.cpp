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
#include"widgets/HUD.h"

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
    mMousePosition{-1,-1},
    mFlipBoard{false},
    mRenderPossibleMoves{true},
    mPossibleMovesColor{201,201,201,90},
    mKingInCheckColor{150,0,0,100},
    mBeginPlay{false}
  {
    SpawnBoard({100.f,100.f},{800.f,800.f});
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
    RenderHUD(mOwningApp->GetWindow());
  }

  void Stage::TickInternal(float deltaTime)
  {
    if(mHUD && !mHUD->HasInit())
    {
      mHUD->NativeInit(mOwningApp->GetWindow());
    }
  }
  sf::RenderWindow &Stage::GetWindow()
  {
      return mOwningApp->GetWindow();
  }

  bool Stage::HandleEvent(const std::optional<sf::Event> &event)
  {
    bool handled = false;

    handled = HandleEventInternal(event) && handled;

    if(mHUD)
    {
      handled = mHUD->HandleEvent(event) && handled;
    }
    return handled;
  }

  bool Stage::HandleEventInternal(const std::optional<sf::Event> &event)
  {
      return false;
  }

  void Stage::BeginPlayInternal()
  {
    if(!mBeginPlay)
    {
      mBeginPlay = true;
      BeginPlay();
    }
  }

  void Stage::RenderBoard()
  {
    mBoard->RefreshBoard();
    RenderLastPlayedMove();
  }

  void Stage::RenderPieces()
  {
    // Render red if king in check
    RenderKingInCheck();

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

    // Render Possible moves if piece Selected
    if(mRenderPossibleMoves && mPieceSelected)
    {
      RenderPossibleMoves();
    }

    if(mMouseDragging && mPieceSelected && mStartPose.isValid())
    {
      char piece = ChessState::Get().GetPieceOnChessCoordinate(mStartPose);
      if(piece != invalid)
      {
        GetPieceContainer(piece)->SetPieceLocation({float(mMousePosition.x - mBoard->GetSquareOffsetX()/2.f) ,float(mMousePosition.y - mBoard->GetSquareOffsetY()/2.f)},GetPieceContainer(piece)->GetPieceColor());
        GetPieceContainer(piece)->RenderPiece();
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
    if(piece == invalid)return false;

    shared<Piece> piecePointer = GetPieceContainer(piece); 

    // Check for castling
    if(CastlingPossible(mStartPose,mEndPose))
    {
      if(mEndPose.file - mStartPose.file > 0)
      {
        CastleKingSide(mWhiteTurn);
      }
      else if(mEndPose.file - mStartPose.file < 0)
      {
        CastleQueenSide(mWhiteTurn);
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
      
      // Check for promotion
      ChessCoordinate pawnToPromote = mWhiteTurn ? mWhitePawn->PawnToPromote() : mBlackPawn->PawnToPromote();
      if(pawnToPromote.isValid())
      {
        ChessState::Get().RemovePiece(mWhiteTurn ? whitePawn : blackPawn, pawnToPromote);
        ChessState::Get().SpawnPiece(WhichPieceToPromote(), pawnToPromote);
      }
      mWhiteTurn = !mWhiteTurn;
      return true;
    }  
    return false;
  }

  bool Stage::CastlingPossible(ChessCoordinate kingCoordinate, ChessCoordinate rookCoordinate)
  {
      if(kingCoordinate == rookCoordinate)return false;

      int offsetFile = (rookCoordinate.file - kingCoordinate.file) > 0 ? 1 :  -1;
      ChessCoordinate rookEndCoordinate{mWhiteTurn ? 1 : 8 ,offsetFile > 0 ? 'h' : 'a'};

      if(mWhiteTurn)
      {
        if(ChessState::Get().GetPieceOnChessCoordinate(kingCoordinate) != whiteKing 
          || abs(rookCoordinate.file - kingCoordinate.file) < 2
          || !ChessState::Get().IsFirstMove(ChessState::Get().GetPiecePosiiton(whiteKing)[0])
          || (offsetFile == 1 && !ChessState::Get().IsFirstMove(ChessCoordinate{1,'h'}))
          || (offsetFile == -1 && !ChessState::Get().IsFirstMove(ChessCoordinate{1,'a'}))
          || mWhiteKing->IsInCheck())
            return false;
        
        Set<ChessCoordinate,ChessCoordinateHashFunction> blackAttackedSquares = ChessState::Get().GetBlackAttackedSquares();
        ChessCoordinate iter{kingCoordinate.rank,kingCoordinate.file};
        iter.file += offsetFile;

        while(iter.isValid() && !(iter == rookEndCoordinate))
        {
          if(ChessState::Get().GetPieceOnChessCoordinate(iter) != invalid || blackAttackedSquares.find(iter) != blackAttackedSquares.end())
            return false;
          iter.file += offsetFile;
        }
        return true;
      }
      else
      {
        if(ChessState::Get().GetPieceOnChessCoordinate(kingCoordinate) != blackKing 
          || abs(rookCoordinate.file - kingCoordinate.file) < 2
          || !ChessState::Get().IsFirstMove(ChessState::Get().GetPiecePosiiton(blackKing)[0])
          || (offsetFile == 1 && !ChessState::Get().IsFirstMove(ChessCoordinate{8,'h'}))
          || (offsetFile == -1 && !ChessState::Get().IsFirstMove(ChessCoordinate{8,'a'}))
          || mBlackKing->IsInCheck())
            return false;
        
        Set<ChessCoordinate,ChessCoordinateHashFunction> whiteAttackedSquares = ChessState::Get().GetWhiteAttackedSquares();
        ChessCoordinate iter{kingCoordinate.rank,kingCoordinate.file};
        iter.file += offsetFile;

        while(iter.isValid() && !(iter == rookEndCoordinate))
        {
          if(ChessState::Get().GetPieceOnChessCoordinate(iter) != invalid || whiteAttackedSquares.find(iter) != whiteAttackedSquares.end())
            return false;
          iter.file += offsetFile;
        }
        return true;
      }
      return false;
  }
  
  void Stage::CastleKingSide(bool whitePiece)
  {
      if(whitePiece)
      {
        ChessCoordinate kingCoordinateStart = ChessCoordinate{1,'e'};
        ChessCoordinate kingCoordinateEnd = ChessCoordinate{1,'g'};
        ChessCoordinate rookCoordinateStart = ChessCoordinate{1,'h'};
        ChessCoordinate rookCoordinateEnd = ChessCoordinate{1,'f'};

        mWhiteKing->MakeMove(kingCoordinateStart,kingCoordinateEnd);
        ChessState::Get().SetPiecePosition(whiteRook,rookCoordinateStart,rookCoordinateEnd,false);
      }
      else
      {
        ChessCoordinate kingCoordinateStart = ChessCoordinate{8,'e'};
        ChessCoordinate kingCoordinateEnd = ChessCoordinate{8,'g'};
        ChessCoordinate rookCoordinateStart = ChessCoordinate{8,'h'};
        ChessCoordinate rookCoordinateEnd = ChessCoordinate{8,'f'};

        mBlackKing->MakeMove(kingCoordinateStart,kingCoordinateEnd);
        ChessState::Get().SetPiecePosition(blackRook,rookCoordinateStart,rookCoordinateEnd,false);
      }
  }

  void Stage::CastleQueenSide(bool whitePiece)
  {
    if(whitePiece)
    {
      ChessCoordinate kingCoordinateStart = ChessCoordinate{1,'e'};
      ChessCoordinate kingCoordinateEnd = ChessCoordinate{1,'c'};
      ChessCoordinate rookCoordinateStart = ChessCoordinate{1,'a'};
      ChessCoordinate rookCoordinateEnd = ChessCoordinate{1,'d'};

      mWhiteKing->MakeMove(kingCoordinateStart,kingCoordinateEnd);
      ChessState::Get().SetPiecePosition(whiteRook,rookCoordinateStart,rookCoordinateEnd,false);
    }
    else
    {
      ChessCoordinate kingCoordinateStart = ChessCoordinate{8,'e'};
      ChessCoordinate kingCoordinateEnd = ChessCoordinate{8,'c'};
      ChessCoordinate rookCoordinateStart = ChessCoordinate{8,'a'};
      ChessCoordinate rookCoordinateEnd = ChessCoordinate{8,'d'};

      mBlackKing->MakeMove(kingCoordinateStart,kingCoordinateEnd);
      ChessState::Get().SetPiecePosition(blackRook,rookCoordinateStart,rookCoordinateEnd,false);
    }
  }

  const sf::Vector2f Stage::ConvertChessCoordinateToPosition(const ChessCoordinate &chessCoordinate)
  {
      int row = chessCoordinate.rank - 1;
      int col = chessCoordinate.file - 'a';

      if(mFlipBoard)//Black's perspective
      {
        return sf::Vector2f{mBoard->GetBoardStart().x + mBoard->GetSquareOffsetX() * (7 - col) + mPieceOffsetX, mBoard->GetBoardStart().y + mBoard->GetSquareOffsetY() * (row) + mPieceOffsetY};
      }
      // White's perspective
      return sf::Vector2f{mBoard->GetBoardStart().x + mBoard->GetSquareOffsetX() * (col) + mPieceOffsetX, mBoard->GetBoardStart().y + mBoard->GetSquareOffsetY() * (7 - row) + mPieceOffsetY};
  }

  ChessCoordinate Stage::ConvertPositionToChessCoordinate(const sf::Vector2i &position)
  {
      if(position.x < mBoard->GetBoardStart().x || position.y < mBoard->GetBoardStart().y 
        || position.x > mBoard->GetBoardDimensions().x + mBoard->GetBoardStart().x
        || position.y > mBoard->GetBoardDimensions().y + mBoard->GetBoardStart().y) 
        return ChessCoordinate{invalid,invalid};
      
      int row =  (position.y - mBoard->GetBoardStart().y) / mBoard->GetSquareOffsetX();
      int col =  (position.x - mBoard->GetBoardStart().x) / mBoard->GetSquareOffsetY(); 

      return mFlipBoard ? ChessCoordinate{row + 1, (char)((7 - col ) + 'a')} : ChessCoordinate{7 - row + 1, (char)(col + 'a')};
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
  
  char Stage::WhichPieceToPromote()
  {
      // TODO :: Implement an ask to which piece to promote
      return mWhiteTurn ? whiteQueen : blackQueen;
  }

  int Stage::EndState()
  {
      bool ongoing = false; 
      if(mWhiteTurn)
      {
        bool whiteKingInCheck = mWhiteKing->IsInCheck();
        // Check for all possible moves for white Pieces
        // White kings moves
        List<ChessCoordinate> startCoordinate = ChessState::Get().GetPiecePosiiton(whiteKing);
        List<ChessCoordinate> moves = mWhiteKing->GetAllPossibleMoves(startCoordinate[0]);
        
        if(moves.size() > 0)
          ongoing = true;

        char whitePieces[5] = {whitePawn, whiteKnight, whiteBishop, whiteRook, whiteQueen};

        for(int p = 0; p < 5 ; p++)
        {
          shared<Piece> pieceContainer = GetPieceContainer(whitePieces[p]);
          startCoordinate = ChessState::Get().GetPiecePosiiton(whitePieces[p]);
          for(int i = 0; !ongoing && i < startCoordinate.size(); i++)
          {
            moves = pieceContainer->GetAllPossibleMoves(startCoordinate[i]);
            for(int j = 0; j < moves.size(); j++ )
            {
              ChessState::Get().SetPiecePosition(whitePieces[p],startCoordinate[i],moves[j]);
              if(!mWhiteKing->IsInCheck())
                ongoing = true;
              ChessState::Get().UndoLastMove();
            }
          }
        }

        // Check for draw 
        if(ongoing)
        {
          //Check for 50 move rule
          if(ChessState::Get().GetMovesWithoutCapture() >= 100 )return DRAW;  

          // Check if enough checkmating material available
          if(ChessState::Get().GetPieceCount(whiteQueen) || ChessState::Get().GetPieceCount(blackQueen) 
            || ChessState::Get().GetPieceCount(whiteRook) || ChessState::Get().GetPieceCount(blackRook)
            || ChessState::Get().GetPieceCount(whitePawn) || ChessState::Get().GetPieceCount(blackPawn)
            || ChessState::Get().GetPieceCount(whiteBishop) == 2 || ChessState::Get().GetPieceCount(blackBishop) == 2
            || (ChessState::Get().GetPieceCount(whiteBishop) && ChessState::Get().GetPieceCount(whiteKnight))
            || (ChessState::Get().GetPieceCount(blackBishop) && ChessState::Get().GetPieceCount(blackKnight)) )
            {
              return Ongoing;
            }
          return DRAW;
        }
        else
        {
          return whiteKingInCheck ? BlackWon : DRAW;
        }
      }
      // Black's turn
      else
      {
        bool blackKingInCheck = mBlackKing->IsInCheck();
        // Check for all possible moves for black Pieces
        // Black kings moves
        List<ChessCoordinate> startCoordinate = ChessState::Get().GetPiecePosiiton(blackKing);
        List<ChessCoordinate> moves = mBlackKing->GetAllPossibleMoves(startCoordinate[0]);
        
        if(moves.size() > 0)
          ongoing = true;

        char blackPieces[5] = {blackPawn, blackKnight, blackBishop, blackRook, blackQueen};

        for(int p = 0; p < 5 ; p++)
        {
          shared<Piece> pieceContainer = GetPieceContainer(blackPieces[p]);
          startCoordinate = ChessState::Get().GetPiecePosiiton(blackPieces[p]);
          for(int i = 0; !ongoing && i < startCoordinate.size(); i++)
          {
            moves = pieceContainer->GetAllPossibleMoves(startCoordinate[i]);
            for(int j = 0; j < moves.size(); j++ )
            {
              ChessState::Get().SetPiecePosition(blackPieces[p],startCoordinate[i],moves[j]);
              if(!mBlackKing->IsInCheck())
                ongoing = true;
              ChessState::Get().UndoLastMove();
            }
          }
        }

        // Check for draw 
        if(ongoing)
        {
          //Check for 50 move rule
          if(ChessState::Get().GetMovesWithoutCapture() >= 100 )return DRAW;

          // Check if enough checkmating material available
          if(ChessState::Get().GetPieceCount(whiteQueen) || ChessState::Get().GetPieceCount(blackQueen)
            || ChessState::Get().GetPieceCount(whiteRook) || ChessState::Get().GetPieceCount(blackRook)
            || ChessState::Get().GetPieceCount(whitePawn) || ChessState::Get().GetPieceCount(blackPawn)
            || ChessState::Get().GetPieceCount(whiteBishop) == 2 || ChessState::Get().GetPieceCount(blackBishop) == 2
            || (ChessState::Get().GetPieceCount(whiteBishop) && ChessState::Get().GetPieceCount(whiteKnight))
            || (ChessState::Get().GetPieceCount(blackBishop) && ChessState::Get().GetPieceCount(blackKnight)) )
            {
              return Ongoing;
            }
          return DRAW;
        }
        else
        {
          return blackKingInCheck ? WhiteWon : DRAW;
        }
      }
    return DRAW;
  }

  void Stage::RenderPossibleMoves()
  {
    char piece = ChessState::Get().GetPieceOnChessCoordinate(mStartPose);
    if(piece == invalid)return;

    sf::CircleShape circle{mBoard->GetSquareOffsetY()/6.f};

    shared<Piece> piecePointer = GetPieceContainer(piece);
    for(auto move : piecePointer->GetAllPossibleMoves(mStartPose))
    {
      ChessState::Get().SetPiecePosition(piece,mStartPose,move);
      bool kingInCheck = ChessState::Get().KingInCheck(mWhiteTurn);
      ChessState::Get().UndoLastMove();
      if(!kingInCheck)
      {
        if(ChessState::Get().GetPieceOnChessCoordinate(move) == invalid)
        {
          circle.setRadius(mBoard->GetSquareOffsetY()/6.f);
          circle.setOutlineThickness(0.f);
          circle.setPosition(sf::Vector2f{mBoard->GetSquareOffsetX()/4.f , mBoard->GetSquareOffsetY()/4.f} + ConvertChessCoordinateToPosition(move));
          circle.setFillColor(mPossibleMovesColor);
        }
        else//If move is a capture
        {
          circle.setRadius(mBoard->GetSquareOffsetY()/2.6f);
          circle.setOutlineColor(mPossibleMovesColor);
          circle.setOutlineThickness(10.f);
          circle.setFillColor(sf::Color{0,0,0,0});
          circle.setPosition(ConvertChessCoordinateToPosition(move));
        }
        mOwningApp->GetWindow().draw(circle);
      }
    }
  }

  void Stage::RenderKingInCheck()
  {
    if(ChessState::Get().KingInCheck(mWhiteTurn))
    {
      sf::RectangleShape rect{sf::Vector2f{mBoard->GetSquareOffsetX(),mBoard->GetSquareOffsetY()}};
      rect.setFillColor(mKingInCheckColor);
      rect.setPosition(ConvertChessCoordinateToPosition(mWhiteTurn ? ChessState::Get().GetPiecePosiiton(whiteKing)[0] : ChessState::Get().GetPiecePosiiton(blackKing)[0] ) + sf::Vector2f{-10.f,-10.f});
      mOwningApp->GetWindow().draw(rect);
    }
    
  }

  void Stage::RenderLastPlayedMove()
  {
    List<ChessCoordinate> lastMove = ChessState::Get().GetLastPlayedMove();
    if(lastMove.size() < 2) return;

    sf::RectangleShape rect{{mBoard->GetSquareOffsetX(),mBoard->GetSquareOffsetY()}};
    rect.setFillColor({255,255,255,20});
    
    rect.setPosition(ConvertChessCoordinateToPosition(lastMove[0]) - sf::Vector2f{10.f,8.f});
    mOwningApp->GetWindow().draw(rect);

    rect.setPosition(ConvertChessCoordinateToPosition(lastMove[1]) - sf::Vector2f{10.f,8.f});
    mOwningApp->GetWindow().draw(rect);
  }

  void Stage::RenderHUD(sf::RenderWindow &renderWindow)
  {
    if(mHUD)
    {
      mHUD->Draw(renderWindow);
    }
  }

  bool Stage::HandleBoardEvent(const std::optional<sf::Event> &event)
  {
      bool handled = false;

      if(mPieceMoved)
      {
        // Check if current position is an End Position
        int currState = EndState();
        if(currState != Ongoing)
        {
          if(currState == DRAW)
          {
            LOG("DRAW");
          }
          else if(currState == WhiteWon)
          {
            LOG("White Won");
          }
          else if(currState == BlackWon)
          {
            LOG("Black Won");
          }
          mOwningApp->ReachedEndState(currState);
        }
      }
      SetPieceMoved(false);

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
                }
                mPieceSelected = false;
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
          SetPieceMoved(true);
          mPieceSelected = false;
        }

        handled = true;
      }
      else if (const auto* keyPress = event->getIf<sf::Event::KeyPressed>())
      {
        if(keyPress->scancode == sf::Keyboard::Scan::Left && ChessState::Get().UndoLastMove())
        {
          SetPieceMoved(true);
          mWhiteTurn = !mWhiteTurn;
        }
        else if(keyPress->scancode == sf::Keyboard::Scan::F)
        {
          mFlipBoard = !mFlipBoard;
        }
        else if(keyPress->scancode == sf::Keyboard::Scan::R)
        {
          mRenderPossibleMoves = !mRenderPossibleMoves;
        }
      }

      return handled;
  }

  void Stage::BeginPlay()
  {

  }

  sf::Vector2f Stage::GetSpriteScale()
  {
      return mBoard->GetSpriteScale();
  }
}