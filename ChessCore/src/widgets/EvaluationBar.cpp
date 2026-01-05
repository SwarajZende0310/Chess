#include "widgets/EvaluationBar.h"

namespace chess
{
    EvaluationBar::EvaluationBar(const sf::Vector2f &size, 
                float currentEvaluation, 
                float maxEvaluation, 
                sf::Color positiveColor, 
                sf::Color negativeColor, 
                sf::Color backgroundColor, 
                sf::Color borderColor, 
                float borderThickness)
        :mSize{size},
        mBackground{mSize},
        mPositiveBar{{mSize.x, mSize.y / 2.f}},
        mNegativeBar{{mSize.x, mSize.y / 2.f}},
        mCurrentEvaluation{currentEvaluation},
        mMaxEvaluation{maxEvaluation},
        mPositiveColor{positiveColor},
        mNegativeColor{negativeColor},
        mFlip{false},
        mPositiveBarYOffset{mSize.y / 2.f}
    {
        mBackground.setSize(size);
        mBackground.setFillColor(backgroundColor);
        mBackground.setOutlineThickness(borderThickness);
        mBackground.setOutlineColor(borderColor);

        mPositiveBar.setFillColor(mPositiveColor);
        mPositiveBar.setPosition({0.f, mPositiveBarYOffset});

        mNegativeBar.setFillColor(mNegativeColor);
        mNegativeBar.setPosition({0.f, 0.f});

    }

    /**
    * @brief Update the current evaluation displayed on the bar.
    */
    void EvaluationBar::UpdateCurrentEvaluation(float value)
    {
        mCurrentEvaluation = value;
        value = std::min(std::abs(value), mMaxEvaluation / 2.f); // TO clamp to max Evaluation value
        float fillHeight = (((mMaxEvaluation / 2.f) + std::abs(value)) / mMaxEvaluation) * mSize.y;
        float negativeFillHeight = 0.f;
        float positiveFillHeight = 0.f;

        if(mCurrentEvaluation < 0.f)
        {
            negativeFillHeight = fillHeight;
            positiveFillHeight = mSize.y - negativeFillHeight;
        }
        else
        {
            positiveFillHeight = fillHeight;
            negativeFillHeight = mSize.y - positiveFillHeight;
        }
        mNegativeBar.setSize({mSize.x, negativeFillHeight});
        mPositiveBar.setSize({mSize.x, positiveFillHeight});

        // Adding offset from the main background widget for Positive bar
        sf::FloatRect mBackgroundBar = mBackground.getGlobalBounds();
        mPositiveBar.setPosition({mBackgroundBar.position.x, mBackgroundBar.position.y + negativeFillHeight});
    }

    /**
     * @brief Get the screen-space bounding rectangle for the bar.
     * @return FloatRect of the current bar.
     */
    sf::FloatRect EvaluationBar::GetBound()
    {
        return mBackground.getGlobalBounds();
    }

    /**
     * @brief Draw the bar to the window.
     */
    void EvaluationBar::Draw(sf::RenderWindow &windowRef)
    {
        windowRef.draw(mBackground);
        windowRef.draw(mNegativeBar);
        windowRef.draw(mPositiveBar);
    }

    /**
     * @brief React to widget location updates by moving the bar components.
     */
    void EvaluationBar::LocationUpdated(const sf::Vector2f &newLocation)
    {
        mBackground.setPosition(newLocation);
        mNegativeBar.setPosition(newLocation);
        mPositiveBar.setPosition(newLocation + sf::Vector2f(0.f, mPositiveBarYOffset));
    }

    /**
     * @brief React to widget rotation updates by rotating the bar components.
     */
    void EvaluationBar::RotationUpdated(const sf::Angle &newRotation)
    {
        mBackground.setRotation(newRotation);
        mNegativeBar.setRotation(newRotation);
        mPositiveBar.setRotation(newRotation);
    }
}