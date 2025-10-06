/**
 * @file Widget.cpp
 * @brief Implementation of base Widget drawing and transform helpers.
 */
#include"widgets/Widget.h"

namespace chess
{
    /**
     * @brief Internal draw wrapper that respects visibility.
     * Calls `Draw()` only if `mIsVisible` is true.
     */
    void Widget::NativeDraw(sf::RenderWindow &windowRef)
    {
        if(mIsVisible)
        {
            Draw(windowRef);
        }
    }

    /**
     * @brief Default event handler for widgets.
     * @return false by default; override in subclasses to handle events.
     */
    bool Widget::HandleEvent(const std::optional<sf::Event> &event)
    {
        return false;
    }

    /**
     * @brief Set world-space widget position and notify subclass hook.
     */
    void Widget::SetWidgetLocation(sf::Vector2f newLocation)
    {
        mWidgetTransform.setPosition(newLocation);
        LocationUpdated(newLocation);
    }

    /**
     * @brief Set widget rotation and notify subclass hook.
     */
    void Widget::SetWidgetRotation(sf::Angle newRotation)
    {
        mWidgetTransform.setRotation(newRotation);
        RotationUpdated(newRotation);
    }

    /**
     * @brief Compute the center position based on current `GetBound()`.
     * @return Center point of the widget bounds.
     */
    sf::Vector2f Widget::GetCenterPosition()
    {
        sf::FloatRect bound = GetBound();
        return sf::Vector2f(bound.position.x + bound.size.x / 2.f , bound.position.y + bound.size.y / 2.f);
    }

    /**
     * @brief Construct a visible widget with default transform.
     */
    Widget::Widget()
        :mIsVisible{true},
        mWidgetTransform{}
    {
    }

    /**
     * @brief Default Draw does nothing; override in subclasses.
     */
    void Widget::Draw(sf::RenderWindow &windowRef)
    {
    }

    /**
     * @brief Hook called after `SetWidgetLocation()` is invoked.
     */
    void Widget::LocationUpdated(const sf::Vector2f &newLocation)
    {
    }

    /**
     * @brief Hook called after `SetWidgetRotation()` is invoked.
     */
    void Widget::RotationUpdated(const sf::Angle &newRotation)
    {
    }
}