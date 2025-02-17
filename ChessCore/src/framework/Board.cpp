#include"framework/Board.h"
#include"framework/AssetManager.h" 
#include"framework/Stage.h"

namespace chess
{
  Board::Board(Stage *owningStage, const sf::Vector2f &boardStart, const sf::Vector2f &boardEnd)
    :mOwingStage{owningStage},
    mBoardStart{boardStart},
    mBoardEnd{boardEnd},
    mWhiteTexture{AssetManager::Get().LoadTexture("JohnPablok Cburnett Chess set/PNGs/With Shadow/1024px/square brown dark_png_shadow_1024px.png")},
    mBlackTexture{AssetManager::Get().LoadTexture("JohnPablok Cburnett Chess set/PNGs/With Shadow/1024px/square brown light_png_shadow_1024px.png")},
    mWhiteSquaresSprite{*(mWhiteTexture)},
    mBlackSquaresSprite{*(mBlackTexture)},
    mScaleX{1.f},
    mScaleY{1.f},
    mOffsetX{102},
    mOffsetY{102}
  {
    Calculate_Square_Offset();
    RefreshBoard();
  }

  void Board::RefreshBoard()
  {
    mBlackSquaresSprite.setScale({mScaleX,mScaleY});
    mWhiteSquaresSprite.setScale({mScaleX,mScaleY});

    //Rendering complete board
    for(int i = 0; i < 8 ; i++)
    {
      bool light_square = i % 2 == 0 ? false : true;
      for(int j = 0; j < 8 ; j++)
      {
        if(light_square)
        {
          RenderWhiteSquare({ mBoardStart.x + i * mOffsetX , mBoardStart.y + j * mOffsetY });
        }
        else
        {
          RenderBlackSquare({ mBoardStart.x + i * mOffsetX , mBoardStart.y + j * mOffsetY });
        }
        light_square = !light_square;
      }
    }
  }

  void Board::RenderBlackSquare(const sf::Vector2f &position)
  {
    mBlackSquaresSprite.setPosition(position);
    mOwingStage->GetWindow().draw(mBlackSquaresSprite);
  }

  void Board::RenderWhiteSquare(const sf::Vector2f &position)
  {
    mWhiteSquaresSprite.setPosition(position);
    mOwingStage->GetWindow().draw(mWhiteSquaresSprite);
  }

  const sf::Vector2f &Board::GetSquareBound(const ChessCoordinate &chessCoordinate)
  {
      // TODO: insert return statement here
      return sf::Vector2f{};
  }

  void Board::Calculate_Square_Offset()
  {
    int length = mBoardEnd.y - mBoardStart.y;
    int width = mBoardEnd.x - mBoardStart.x;

    mOffsetX = length/8;
    mOffsetY = width/8;

    sf::Vector2u squareSpriteBound = mWhiteSquaresSprite.getTexture().getSize(); // Same for Both Light and Dark Sqauares

    //Scaling value of Square Sprite
    mScaleX = mOffsetX/squareSpriteBound.x;
    mScaleY = mOffsetY/squareSpriteBound.y;
  }
}