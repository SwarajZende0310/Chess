#include"widgets/Button.h"
#include"framework/AssetManager.h"

namespace chess
{
    Button::Button(const std::string &textString, const std::string &buttonTexturePath)
        :mButtonTexture{AssetManager::Get().LoadTexture(buttonTexturePath)},
        mButtonSprite{*(mButtonTexture.get())},
        mButtonFont{AssetManager::Get().LoadFont("fonts/kenvector_future.ttf")},
        mButtonText{*(mButtonFont.get()),textString},
        mIsButtonDown{false}    
    {
        mButtonColor.buttonDefaultColor = sf::Color{128, 128, 128, 255};
        mButtonColor.buttonDownColor = sf::Color{64, 64, 64 , 255};
        mButtonColor.buttonHoverColor = sf::Color{190, 190, 190, 255};
        SetColor(mButtonColor);
        CenterText();
    }

    sf::FloatRect Button::GetBound()
    {
        return mButtonSprite.getGlobalBounds();
    }

    void Button::SetTextString(const std::string newString)
    {
        mButtonText.setString(newString);
        CenterText();
    }

    std::string Button::GetTextString() const
    {
        return mButtonText.getString();
    }

    void Button::SetColor(const ButtonColor newButtonColor)
    {
        mButtonColor = newButtonColor;
        mButtonSprite.setColor(mButtonColor.buttonDefaultColor);
    }
    
    void Button::SetTextSize(int textSize)
    {
        mButtonText.setCharacterSize(textSize);
        CenterText();
    }

    bool Button::HandleEvent(const std::optional<sf::Event> &event)
    {
        if(!GetVisibilty())return false;

        bool handled = false;
        sf::Vector2f mMousePosition;

        if(const auto* mouseReleased = event->getIf<sf::Event::MouseButtonReleased>())
        {
            mMousePosition = sf::Vector2f{float(mouseReleased->position.x), float(mouseReleased->position.y)};
            if(mouseReleased->button == sf::Mouse::Button::Left)
            {
                if(mButtonSprite.getGlobalBounds().contains(mMousePosition) && mIsButtonDown)
                {
                    handled = true;
                }
            }
            ButtonUp();
        }
        else if(const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>())
        {
            mMousePosition = sf::Vector2f{float(mousePressed->position.x), float(mousePressed->position.y)};
            if(mousePressed->button == sf::Mouse::Button::Left)
            {
                if(mButtonSprite.getGlobalBounds().contains(mMousePosition))
                {
                    ButtonDown();
                    handled = true;
                }
            }
        }
        else if(const auto* mouseMoved = event->getIf<sf::Event::MouseMoved>())
        {
            mMousePosition = sf::Vector2f{float(mouseMoved->position.x), float(mouseMoved->position.y)};
            if(!sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
            {
                if(mButtonSprite.getGlobalBounds().contains(mMousePosition))
                {
                    MouseHovered();
                    handled = true;
                }
                else
                {
                    ButtonUp();
                }
            }
        }
        return handled;
    }

    void Button::Draw(sf::RenderWindow &windowRef)
    {
        windowRef.draw(mButtonSprite);
        windowRef.draw(mButtonText);
    }

    void Button::LocationUpdated(const sf::Vector2f &newLocation)
    {
        mButtonSprite.setPosition(newLocation);
        mButtonText.setPosition(newLocation);
        CenterText();
    }

    void Button::RotationUpdated(const sf::Angle &newRotation)
    {
        mButtonSprite.setRotation(newRotation);
        mButtonText.setRotation(newRotation);
        CenterText();
    }
    
    void Button::CenterText()
    {
        sf::Vector2f widgetCenter = GetCenterPosition();
        sf::FloatRect textBound = mButtonText.getGlobalBounds();
        mButtonText.setPosition(widgetCenter - sf::Vector2f(textBound.size.x/2.f , textBound.size.y));
    }
    
    void Button::ButtonUp()
    {
        mIsButtonDown = false;
        mButtonSprite.setColor(mButtonColor.buttonDefaultColor);
    }
    
    void Button::ButtonDown()
    {
        mIsButtonDown = true;
        mButtonSprite.setColor(mButtonColor.buttonDownColor);
    }
    
    void Button::MouseHovered()
    {
        mButtonSprite.setColor(mButtonColor.buttonHoverColor);
    }
}
