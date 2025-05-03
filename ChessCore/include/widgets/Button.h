#pragma once

#include"widgets/Widget.h"
#include"framework/Core.h"

namespace chess
{
    struct ButtonColor
    {
        sf::Color buttonDefaultColor;
        sf::Color buttonDownColor;
        sf::Color buttonHoverColor;
    };

    class Button : public Widget
    {
        public:
            Button(const std::string &textString = "Button", const std::string & buttonTexturePath = "UI/buttonBlue.png");

            virtual sf::FloatRect GetBound() override;

            void SetTextString(const std::string newString);
            std::string GetTextString()const;
            
            void SetColor(const ButtonColor newButtonColor);

            void SetTextSize(int textSize);

            virtual bool HandleEvent(const std::optional<sf::Event> &event)override;

            Delegate<> mOnButtonClicked;
        private:
            virtual void Draw(sf::RenderWindow& windowRef)override;
            virtual void LocationUpdated(const sf::Vector2f& newLocation)override;
            virtual void RotationUpdated(const sf::Angle& newRotation)override;
        
            void CenterText();
        
            shared<sf::Texture> mButtonTexture;
            sf::Sprite mButtonSprite;
                
            shared<sf::Font> mButtonFont;
            sf::Text mButtonText;
        
            ButtonColor mButtonColor;
        
            bool mIsButtonDown;
        
            void ButtonUp();
            void ButtonDown();
            void MouseHovered();
    };
}