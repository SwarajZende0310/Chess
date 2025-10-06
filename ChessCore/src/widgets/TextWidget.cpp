/**
 * @file TextWidget.cpp
 * @brief Implementation of TextWidget rendering and property updates.
 */
#include"widgets/TextWidget.h"
#include"framework/AssetManager.h"

namespace chess
{
    /**
     * @brief Construct a text widget and load its font.
     * @param textStr Initial text to display.
     * @param fontPath Font file path to load.
     * @param characterSize Initial text size.
     */
    TextWidget::TextWidget(const std::string &textStr, const std::string &fontPath, unsigned int characterSize)
        :mFont{AssetManager::Get().LoadFont(fontPath)},
        mText{*(mFont.get()),textStr,characterSize}    
    {
        
    }

    /**
     * @brief Set the character size of the text.
     */
    void TextWidget::SetTextSize(unsigned int textSize)
    {
        mText.setCharacterSize(textSize);
    }

    /**
     * @brief Update the displayed string.
     */
    void TextWidget::SetTextString(const std::string &newStr)
    {
        mText.setString(newStr);
    }

    /**
     * @brief Get the screen-space bounds of the text.
     */
    sf::FloatRect TextWidget::GetBound()
    {
        return mText.getGlobalBounds();
    }

    /**
     * @brief Draw the text to the given window.
     */
    void TextWidget::Draw(sf::RenderWindow &windowRef)
    {
        windowRef.draw(mText);
    }

    /**
     * @brief React to location updates by moving the text.
     */
    void TextWidget::LocationUpdated(const sf::Vector2f &newLocation)
    {
        mText.setPosition(newLocation);
    }
    
    /**
     * @brief React to rotation updates by rotating the text.
     */
    void TextWidget::RotationUpdated(const sf::Angle &newRotation)
    {
        mText.setRotation(newRotation);
    }
}