#include"widgets/Widget.h"

namespace chess
{
    void Widget::NativeDraw(sf::RenderWindow &windowRef)
    {
        if(mIsVisible)
        {
            Draw(windowRef);
        }
    }

    bool Widget::HandleEvent(const std::optional<sf::Event> &event)
    {
        return false;
    }

    void Widget::SetWidgetLocation(sf::Vector2f newLocation)
    {
        mWidgetTransform.setPosition(newLocation);
        LocationUpdated(newLocation);
    }

    void Widget::SetWidgetRotation(sf::Angle newRotation)
    {
        mWidgetTransform.setRotation(newRotation);
        RotationUpdated(newRotation);
    }

    sf::Vector2f Widget::GetCenterPosition()
    {
        sf::FloatRect bound = GetBound();
        return sf::Vector2f(bound.position.x + bound.size.x / 2.f , bound.position.y + bound.size.y / 2.f);
    }

    Widget::Widget()
        :mIsVisible{true},
        mWidgetTransform{}
    {
    }

    void Widget::Draw(sf::RenderWindow &windowRef)
    {
    }

    void Widget::LocationUpdated(const sf::Vector2f &newLocation)
    {
    }

    void Widget::RotationUpdated(const sf::Angle &newRotation)
    {
    }
}