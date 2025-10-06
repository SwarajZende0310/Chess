/**
 * @file ImageWidget.h
 * @brief Image/sprite widget with scaling and transform support.
 */
#pragma once

#include"widgets/Widget.h"

namespace chess
{
    /**
     * @brief Displays a textured sprite as a widget.
     */
    class ImageWidget : public Widget
    {
        public:
            /**
             * @brief Construct an ImageWidget and load its texture.
             * @param imagePath Path to the image asset.
             * @param imageScale Initial sprite scale.
             */
            ImageWidget(const std::string& imagePath, sf::Vector2f imageScale = { 1.f , 1.f } );

            /** @brief Set sprite scale. */
            void SetScale(sf::Vector2f newScale);

            /** @brief Bounding rect of the sprite in window coordinates. */
            virtual sf::FloatRect GetBound()override;
        protected:

        private:
            /** @brief Draw the sprite. */
            virtual void Draw(sf::RenderWindow& windowRef)override;
            /** @brief Update location on move. */
            virtual void LocationUpdated(const sf::Vector2f& newLocation)override;
            /** @brief Update rotation on change. */
            virtual void RotationUpdated(const sf::Angle& newRotation)override;

            shared<sf::Texture> mImageTexture; ///< Loaded image texture
            sf::Sprite mImageSprite;           ///< Rendered sprite
    };
}