#pragma once
#include<functional>
#include"framework/Core.h"

namespace chess
{
    class Object;

    template<typename ...Args>
    class Delegate
    {
        public:
            template<typename ClassName>
            void BindAction(weak<Object>obj, void(ClassName::*callback)(Args...))
            {
                std::function<bool(Args...)> callbackFunc = [obj, callback](Args ...args ) -> bool
                {
                    // Bypassing the check for whether obj is expired as Stage has only one weak reference
                    // TODO remove this and give stage a shared reference
                    (static_cast<ClassName*>(obj.lock().get())->*callback)(args...);
                    return true;
                    //
                    if(!obj.expired())
                    {
                        (static_cast<ClassName*>(obj.lock().get())->*callback)(args...);
                        return true;
                    }
                    return false;
                };
                mCallbacks.push_back(callbackFunc);
            }

            void Broadcast(Args ... args)
            {
                for(auto iter = mCallbacks.begin(); iter != mCallbacks.end();)
                {
                    if((*iter)(args...))
                    {
                        ++iter;
                    }
                    else
                    {
                        iter = mCallbacks.erase(iter);
                    }
                }
            }
        private:
            List<std::function<bool(Args ...)>> mCallbacks; 
    };
}