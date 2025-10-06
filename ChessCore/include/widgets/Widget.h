/**
 * @file Widget.h
 * @brief Base UI widget with transform, visibility, and event hooks.
 */
#pragma once

#include<SFML/Graphics.hpp>
#include"framework/Object.h"

namespace chess
{
    /**
     * @brief Base class for UI widgets.
     *
     * Provides drawing pipeline entry (`NativeDraw`), transform utilities
     * (location/rotation), visibility control, bounds/center helpers, and
     * overridable hooks for custom draw and event handling.
     */
    class Widget : public Object
    {
        public:
            /**
             * @brief Engine draw entry. Respects visibility and calls `Draw`.
             */
            void NativeDraw(sf::RenderWindow& windowRef);

            /**
             * @brief Handle input events. Default returns false (unhandled).
             */
            virtual bool HandleEvent(const std::optional<sf::Event> &event);

            /** @brief Set widget position in window coordinates. */
            void SetWidgetLocation(sf::Vector2f newLocation);
            /** @brief Set widget rotation. */
            void SetWidgetRotation(sf::Angle newRotation);

            /** @brief Get current widget position. */
            sf::Vector2f GetWidgetLocation() { return mWidgetTransform.getPosition(); }
            /** @brief Get current widget rotation. */
            sf::Angle GetWidgetRotaiton() { return mWidgetTransform.getRotation(); }

            /** @brief Show/hide this widget. */
            void SetVisibility(bool newVisibility) { mIsVisible = newVisibility; }
            /** @brief Whether the widget is visible. */
            bool GetVisibilty() { return mIsVisible; }

            /** @brief Bound rectangle in window coordinates (implemented by derived). */
            virtual sf::FloatRect GetBound() = 0 ;
            /** @brief Convenience: center point of the current bounds. */
            sf::Vector2f GetCenterPosition();

        protected:
            /** @brief Construct a visible widget with identity transform. */
            Widget();

        private:
            /** @brief Derived classes implement actual drawing here. */
            virtual void Draw(sf::RenderWindow& windowRef);
            /** @brief Notification when location changes. */
            virtual void LocationUpdated(const sf::Vector2f& newLocation);
            /** @brief Notification when rotation changes. */
            virtual void RotationUpdated(const sf::Angle& newRotation);

            sf::Transformable mWidgetTransform; ///< Position/rotation state
            bool mIsVisible;                     ///< Visibility flag
    };
}