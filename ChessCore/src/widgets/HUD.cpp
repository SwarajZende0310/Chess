/**
 * @file HUD.cpp
 * @brief Implementation of HUD initialization and event handling.
 */
#include"widgets/HUD.h"

namespace chess
{
    /**
     * @brief Initialize HUD once per window lifetime.
     * Calls user-defined `Init` on first invocation only.
     */
    void HUD::NativeInit(sf::RenderWindow &windowRef)
    {
        if(!mAlreadyInit)
        {
            mAlreadyInit = true;
            Init(windowRef);
        }
    }

    /**
     * @brief Handle HUD-level events.
     * @return true if an event was handled.
     */
    bool HUD::HandleEvent(const std::optional<sf::Event> &event)
    {
        return false;
    }

    /**
     * @brief Construct the HUD (deferred Init via `NativeInit`).
     */
    HUD::HUD()
        :mAlreadyInit{false}
    {

    }
    
    /**
     * @brief User hook to build HUD content.
     * @param windowRef Reference window for sizing/placement.
     */
    void HUD::Init(const sf::RenderWindow &windowRef)
    {

    }
}