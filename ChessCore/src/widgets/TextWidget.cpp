#include"widgets/TextWidget.h"
#include"framework/AssetManager.h"

namespace chess
{
    TextWidget::TextWidget(const std::string &textStr, const std::string &fontPath, unsigned int characterSize)
        :mFont{AssetManager::Get().LoadFont(fontPath)},
        mText{*(mFont.get()),textStr,characterSize}    
    {
        
    }

    void TextWidget::SetTextSize(unsigned int textSize)
    {
        mText.setCharacterSize(textSize);
    }

    void TextWidget::SetTextString(const std::string &newStr)
    {
        mText.setString(newStr);
    }

    sf::FloatRect TextWidget::GetBound()
    {
        return mText.getGlobalBounds();
    }

    void TextWidget::Draw(sf::RenderWindow &windowRef)
    {
        windowRef.draw(mText);
    }

    void TextWidget::LocationUpdated(const sf::Vector2f &newLocation)
    {
        mText.setPosition(newLocation);
    }
    
    void TextWidget::RotationUpdated(const sf::Angle &newRotation)
    {
        mText.setRotation(newRotation);
    }
}