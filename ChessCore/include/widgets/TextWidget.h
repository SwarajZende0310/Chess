/**
 * @file TextWidget.h
 * @brief Simple text label widget with configurable font and size.
 */
#pragma once

#include"widgets/Widget.h"

namespace chess
{
    /**
     * @brief A drawable text label widget.
     *
     * Loads a font, renders text, and exposes helpers to set size and string.
     */
    class TextWidget : public Widget
    {
        public:
            /**
             * @brief Construct a TextWidget and load its font.
             * @param textStr Initial text string.
             * @param fontPath Path to the font asset.
             * @param characterSize Initial character size.
             */
            TextWidget(const std::string& textStr,
                const std::string& fontPath = "fonts/kenvector_future.ttf",
                unsigned int characterSize = 30);

            /**
             * @brief Set the character size of the text.
             * @param textSize New character size.
             */
            void SetTextSize(unsigned int textSize);

            /**
             * @brief Get current text string.
             * @return Current text string.
             */
            std::string GetTextString(){ return mText.getString(); }
            /**
             * @brief Set the text string content.
             * @param newStr New text string.
             */
            void SetTextString(const std::string& newStr);

            /**
             * @brief Bounding rect of the label in window coordinates.
             * @return Bounding rect.
             */
            virtual sf::FloatRect GetBound()override;
        protected:

        private:
            /**
             * @brief Draw the text.
             * @param windowRef Window to draw to.
             */
            virtual void Draw(sf::RenderWindow& windowRef)override;
            /**
             * @brief Update location on move.
             * @param newLocation New location.
             */
            virtual void LocationUpdated(const sf::Vector2f& newLocation)override;
            /**
             * @brief Update rotation on change.
             * @param newRotation New rotation.
             */
            virtual void RotationUpdated(const sf::Angle& newRotation)override;

            shared<sf::Font> mFont; ///< Loaded font resource
            sf::Text mText;         ///< Rendered text object
    };
}