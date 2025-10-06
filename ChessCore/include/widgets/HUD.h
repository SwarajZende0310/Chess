/**
 * @file HUD.h
 * @brief Heads-up display base class with initialization and event handling.
 */
#pragma once

#include"widgets/Widget.h"
#include"framework/Object.h"

namespace chess
{
    /**
     * @brief Base class for game HUDs.
     *
     * Provides a one-time initialization flow via `NativeInit()` that calls
     * derived `Init()` exactly once, plus a `Draw()` contract and optional
     * `HandleEvent()` forwarding.
     */
    class HUD : public Object
    {
        public:
            /** @brief Draw the HUD to the render window. Pure virtual. */
            virtual void Draw(sf::RenderWindow & windowRef) = 0;
            /**
             * @brief Ensure one-time initialization and then call `Init()`.
             * @param windowRef Window reference for size/resources.
             */
            void NativeInit(sf::RenderWindow& windowRef);
            /** @brief Whether `Init()` has already been invoked. */
            bool HasInit(){ return mAlreadyInit; }
            /** @brief Handle input events. Default returns false. */
            virtual bool HandleEvent(const std::optional<sf::Event> &event);

        protected:
            /** @brief Construct HUD with not-yet-initialized state. */
            HUD();

        private:
            /** @brief Derived initialization hook called once by `NativeInit()`. */
            virtual void Init(const sf::RenderWindow& windowRef);
            bool mAlreadyInit; ///< One-time initialization flag
    };
}