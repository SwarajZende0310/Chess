#pragma once

#include"widgets/Widget.h"
#include"framework/Object.h"

namespace chess
{
    class HUD : public Object
    {
        public:
            virtual void Draw(sf::RenderWindow & windowRef) = 0;
            void NativeInit(sf::RenderWindow& windowRef);
            bool HasInit(){ return mAlreadyInit; }
            virtual bool HandleEvent(const std::optional<sf::Event> &event);

        protected:
            HUD();

        private:
            virtual void Init(const sf::RenderWindow& windowRef);
            bool mAlreadyInit;
    };
}