#pragma once

#include<SFML/Graphics.hpp>

namespace chess
{
    class Widget
    {
        public:
            void NativeDraw(sf::RenderWindow& windowRef);
            virtual bool HandleEvent(std::optional<sf::Event> &event);

            void SetWidgetLocation(sf::Vector2f newLocation);
            void SetWidgetRotation(sf::Angle newRotation);

            sf::Vector2f GetWidgetLocation() { return mWidgetTransform.getPosition(); }
            sf::Angle GetWidgetRotaiton() { return mWidgetTransform.getRotation(); }

            void SetVisibility(bool newVisibility) { mIsVisible = newVisibility; }
            bool GetVisibilty() { return mIsVisible; }

            virtual sf::FloatRect GetBound() = 0 ;
            sf::Vector2f GetCenterPosition();

        protected:
            Widget();

        private:
            virtual void Draw(sf::RenderWindow& windowRef);
            virtual void LocationUpdated(const sf::Vector2f& newLocation);
            virtual void RoationUpdated(const sf::Angle& newRotation);

            sf::Transformable mWidgetTransform;
            bool mIsVisible;
    };
}