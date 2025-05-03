#pragma once

#include"widgets/Widget.h"

namespace chess
{
    class TextWidget : public Widget
    {
        public:
            TextWidget(const std::string& textStr,
                const std::string& fontPath = "fonts/kenvector_future.ttf",
                unsigned int characterSize = 30);

            void SetTextSize(unsigned int textSize);

            std::string GetTextString(){ return mText.getString(); }
            void SetTextString(const std::string& newStr);

            virtual sf::FloatRect GetBound()override;
        protected:

        private:
            virtual void Draw(sf::RenderWindow& windowRef)override;
            virtual void LocationUpdated(const sf::Vector2f& newLocation)override;
            virtual void RotationUpdated(const sf::Angle& newRotation)override;

            shared<sf::Font> mFont;
            sf::Text mText;
    };
}