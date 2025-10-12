/**
 * @file Button.cpp
 * @brief Implementation of the Button widget (drawing, events, helpers).
 */
#include"widgets/Button.h"
#include"framework/AssetManager.h"

namespace chess
{
    /**
     * @brief Construct a button with text and a textured background.
     * @param textString Initial text label to display.
     * @param buttonTexturePath Texture file used for the button sprite.
     */
    Button::Button(const std::string &textString, const std::string &buttonTexturePath)
        :mButtonTexture{AssetManager::Get().LoadTexture(buttonTexturePath)},
        mButtonSprite{*(mButtonTexture.get())},
        mButtonFont{AssetManager::Get().LoadFont("fonts/kenvector_future.ttf")},
        mButtonText{*(mButtonFont.get()),textString},
        mIsButtonDown{false}    
    {
        SetColor(mButtonColor);
        CenterText();
    }

    /**
     * @brief Get the screen-space bounding rectangle for the button sprite.
     * @return FloatRect of the current button sprite bounds.
     */
    sf::FloatRect Button::GetBound()
    {
        return mButtonSprite.getGlobalBounds();
    }

    /**
     * @brief Update the button's text label and re-center it.
     * @param newString New string to set on the text object.
     */
    void Button::SetTextString(const std::string newString)
    {
        mButtonText.setString(newString);
        CenterText();
    }

    /**
     * @brief Read the current text label displayed on the button.
     */
    std::string Button::GetTextString() const
    {
        return mButtonText.getString();
    }

    /**
     * @brief Set the default/hover/down colors of the button and apply default color.
     * @param newButtonColor Color triplet for different button states.
     */
    void Button::SetColor(const ButtonColor newButtonColor)
    {
        mButtonColor = newButtonColor;
        mButtonSprite.setColor(mButtonColor.buttonDefaultColor);
    }
    
    /**
     * @brief Change the text character size and re-center.
     */
    void Button::SetTextSize(int textSize)
    {
        mButtonText.setCharacterSize(textSize);
        CenterText();
    }

    /**
     * @brief Handle mouse press/release/move events for click and hover behavior.
     *
     * Emits `mOnButtonClicked` on a complete click inside bounds.
     * @param event Optional SFML event to process.
     * @return true if the button handled the event; false otherwise.
     */
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
                    mOnButtonClicked.Broadcast();
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

    /**
     * @brief Draw the button sprite and text to the window.
     */
    void Button::Draw(sf::RenderWindow &windowRef)
    {
        windowRef.draw(mButtonSprite);
        windowRef.draw(mButtonText);
    }

    /**
     * @brief React to widget location updates and keep text centered over the sprite.
     */
    void Button::LocationUpdated(const sf::Vector2f &newLocation)
    {
        mButtonSprite.setPosition(newLocation);
        mButtonText.setPosition(newLocation);
        CenterText();
    }

    /**
     * @brief React to widget rotation changes and keep text aligned.
     */
    void Button::RotationUpdated(const sf::Angle &newRotation)
    {
        mButtonSprite.setRotation(newRotation);
        mButtonText.setRotation(newRotation);
        CenterText();
    }
    
    /**
     * @brief Center the text within the button sprite's bounds.
     */
    void Button::CenterText()
    {
        sf::Vector2f widgetCenter = GetCenterPosition();
        sf::FloatRect textBound = mButtonText.getGlobalBounds();
        mButtonText.setPosition(widgetCenter - sf::Vector2f(textBound.size.x/2.f , textBound.size.y));
    }
    
    /**
     * @brief Set visual state to "up" (not pressed) and apply default color.
     */
    void Button::ButtonUp()
    {
        mIsButtonDown = false;
        mButtonSprite.setColor(mButtonColor.buttonDefaultColor);
    }
    
    /**
     * @brief Set visual state to "down" (pressed) and apply down color.
     */
    void Button::ButtonDown()
    {
        mIsButtonDown = true;
        mButtonSprite.setColor(mButtonColor.buttonDownColor);
    }
    
    /**
     * @brief Apply hover color when the mouse moves over the button.
     */
    void Button::MouseHovered()
    {
        mButtonSprite.setColor(mButtonColor.buttonHoverColor);
    }
}
