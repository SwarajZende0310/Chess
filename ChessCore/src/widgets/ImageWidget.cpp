#include"widgets/ImageWidget.h"
#include"framework/AssetManager.h"

namespace chess
{
    ImageWidget::ImageWidget(const std::string &imagePath, sf::Vector2f imageScale)
        :mImageTexture{AssetManager::Get().LoadTexture(imagePath)},
        mImageSprite{*(mImageTexture.get())}
    {
        mImageSprite.setScale(imageScale);
    }

    void ImageWidget::SetScale(sf::Vector2f newScale)
    {
        mImageSprite.setScale(newScale);
    }

    sf::FloatRect ImageWidget::GetBound()
    {
        mImageSprite.getGlobalBounds();
    }

    void ImageWidget::Draw(sf::RenderWindow& windowRef)
    {
        windowRef.draw(mImageSprite);
    }

    void ImageWidget::LocationUpdated(const sf::Vector2f& newLocation)
    {
        mImageSprite.setPosition(newLocation);
    }

    void ImageWidget::RotationUpdated(const sf::Angle& newRotation)
    {
        mImageSprite.setRotation(newRotation);
    }
}