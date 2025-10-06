/**
 * @file ImageWidget.cpp
 * @brief Implementation of ImageWidget rendering and property updates.
 */
#include"widgets/ImageWidget.h"
#include"framework/AssetManager.h"

namespace chess
{
    /**
     * @brief Construct an image widget and load its texture.
     * @param imagePath Path to the image texture file.
     * @param imageScale Initial scale to apply to the sprite.
     */
    ImageWidget::ImageWidget(const std::string &imagePath, sf::Vector2f imageScale)
        :mImageTexture{AssetManager::Get().LoadTexture(imagePath)},
        mImageSprite{*(mImageTexture.get())}
    {
        mImageSprite.setScale(imageScale);
    }

    /**
     * @brief Set the sprite scale.
     */
    void ImageWidget::SetScale(sf::Vector2f newScale)
    {
        mImageSprite.setScale(newScale);
    }

    /**
     * @brief Get the screen-space bounds of the image sprite.
     */
    sf::FloatRect ImageWidget::GetBound()
    {
        return mImageSprite.getGlobalBounds();
    }

    /**
     * @brief Draw the image sprite to the window.
     */
    void ImageWidget::Draw(sf::RenderWindow& windowRef)
    {
        windowRef.draw(mImageSprite);
    }

    /**
     * @brief React to location updates by moving the sprite.
     */
    void ImageWidget::LocationUpdated(const sf::Vector2f& newLocation)
    {
        mImageSprite.setPosition(newLocation);
    }

    /**
     * @brief React to rotation updates by rotating the sprite.
     */
    void ImageWidget::RotationUpdated(const sf::Angle& newRotation)
    {
        mImageSprite.setRotation(newRotation);
    }
}