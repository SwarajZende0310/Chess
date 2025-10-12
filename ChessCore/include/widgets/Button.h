/**
 * @file Button.h
 * @brief Clickable button widget with texture, label, colors, and events.
 */
#pragma once

#include"widgets/Widget.h"
#include"framework/Core.h"

namespace chess
{
    /**
     * @brief Colors used by a `Button` in different interaction states.
     */
    struct ButtonColor
    {
        sf::Color buttonDefaultColor{128, 128, 128, 100}; ///< Idle color
        sf::Color buttonDownColor{64, 64, 64 , 255};      ///< Mouse-down color
        sf::Color buttonHoverColor{190, 190, 190, 255};   ///< Hover color
    };

    /**
     * @brief A textured button with centered text and click handling.
     *
     * Supports hover/press visual feedback, text updates, and a click
     * delegate `mOnButtonClicked` broadcast on release within bounds.
     */
    class Button : public Widget
    {
        public:
            /**
             * @brief Construct a Button and load its resources.
             * @param textString Initial button label.
             * @param buttonTexturePath Texture path for the button background.
             */
            Button(const std::string &textString = "Button", const std::string & buttonTexturePath = "UI/buttonBlue.png");

            /** @brief Bounding rect of the button in window coordinates. */
            virtual sf::FloatRect GetBound() override;

            /** @brief Set the button label text and re-center it. */
            void SetTextString(const std::string newString);
            /** @brief Get the current button label text. */
            std::string GetTextString()const;
            
            /** @brief Update default/hover/down colors and apply default. */
            void SetColor(const ButtonColor newButtonColor);

            /** @brief Set text size and re-center it. */
            void SetTextSize(int textSize);

            /** @brief Handle mouse press/release/move for hover and clicks. */
            virtual bool HandleEvent(const std::optional<sf::Event> &event)override;

            Delegate<> mOnButtonClicked; ///< Fired on a valid click release
        private:
            /** @brief Draw sprite and text. */
            virtual void Draw(sf::RenderWindow& windowRef)override;
            /** @brief Update sprite/text position on move. */
            virtual void LocationUpdated(const sf::Vector2f& newLocation)override;
            /** @brief Update rotation and re-center text. */
            virtual void RotationUpdated(const sf::Angle& newRotation)override;
        
            /** @brief Center text within the button's current bounds. */
            void CenterText();
        
            shared<sf::Texture> mButtonTexture; ///< Button background texture
            sf::Sprite mButtonSprite;           ///< Button background sprite
                
            shared<sf::Font> mButtonFont; ///< Font for label
            sf::Text mButtonText;         ///< Text label
        
            ButtonColor mButtonColor; ///< Interaction colors
        
            bool mIsButtonDown; ///< True while left mouse is pressed inside
        
            /** @brief Reset state on mouse up and apply default color. */
            void ButtonUp();
            /** @brief Set pressed state and color. */
            void ButtonDown();
            /** @brief Apply hover color while mouse is over. */
            void MouseHovered();
    };
}