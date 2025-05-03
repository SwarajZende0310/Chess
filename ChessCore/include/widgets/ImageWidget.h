#pragma once

#include"widgets/Widget.h"

namespace chess
{
    class ImageWidget : public Widget
    {
        public:
            ImageWidget(const std::string& imagePath, sf::Vector2f imageScale = { 1.f , 1.f } );

            void SetScale(sf::Vector2f newScale);

            virtual sf::FloatRect GetBound()override;
        protected:

        private:
            virtual void Draw(sf::RenderWindow& windowRef)override;
            virtual void LocationUpdated(const sf::Vector2f& newLocation)override;
            virtual void RotationUpdated(const sf::Angle& newRotation)override;

            shared<sf::Texture> mImageTexture;
            sf::Sprite mImageSprite;
    };
}