/**
 * @file EvaluationBar.h
 * @brief Widget that displays Current Evaluation through a Bar.
 */
#pragma once

#include "widgets/Widget.h"

namespace chess
{
    /**
     * @brief A drawable Widget that displays Current Evaluation through a Bar.
     * 
     * Draws a vertical bar that fills proportionally to the current evaluation value.
     */
    class EvaluationBar : public Widget
    {
        public:
            /**
             * @brief Construct a EvaluationBar and load Background.
             * @param size Initial size of the bar.
             * @param currentEvaluation Current evaluation of the Board.
             * @param maxEvaluation Maximum evaluation value.
             * @param positiveColor Color for positive evaluations.
             * @param negativeColor Color for negative evaluations.
             * @param backgroundColor Background color of the bar.
             * @param borderColor Border color of the bar.
             * @param borderThickness Thickness of the border.
             */
            EvaluationBar(const sf::Vector2f& size, 
                       float currentEvaluation = 0.f,
                       float maxEvaluation = 10.f,
                       sf::Color positiveColor = sf::Color::White,
                       sf::Color negativeColor = sf::Color::Black,
                       sf::Color backgroundColor = sf::Color::Transparent,
                       sf::Color borderColor = sf::Color::White,
                       float borderThickness = 1.f);
            ~EvaluationBar() = default;

            /**
             * @brief Update the current evaluation for the bar.
             * @param value New value to display
             */
            void UpdateCurrentEvaluation(float value);

            /**
             * @brief Bounding rect of the label in window coordinates.
             * @return Bounding rect.
             */
            virtual sf::FloatRect GetBound()override;

        protected:

        private:
            /**
             * @brief Draw the Bar.
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
            virtual void RotationUpdated(const sf::Angle&  newRotation)override;
            
            sf::Vector2f mSize;                ///< Size of the evaluation bar
            sf::RectangleShape mBackground;    ///< Background rectangle
            sf::RectangleShape mPositiveBar;   ///< Positive evaluation bar
            sf::RectangleShape mNegativeBar;   ///< Negative evaluation bar
            float mCurrentEvaluation;          ///< Current evaluation value
            float mMaxEvaluation;              ///< Maximum evaluation value
            sf::Color mPositiveColor;          ///< Color for positive evaluations
            sf::Color mNegativeColor;          ///< Color for negative evaluations
            bool mFlip;                        ///< Whether to flip the bar orientation
            float mPositiveBarYOffset;         ///< Positive evaluation bar Y offset
    };
}