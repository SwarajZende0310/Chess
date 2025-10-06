#pragma once
#include<functional>
#include"framework/Core.h"

namespace chess
{
    class Object;

    /**
     * @brief Lightweight multicast delegate for member function callbacks.
     *
     * Stores weak references to objects and invokes the bound member function
     * if the object is still alive. Expired callbacks are pruned on broadcast.
     */
    template<typename ...Args>
    class Delegate
    {
        public:
            /**
             * @brief Bind an object's member function as a callback.
             * @tparam ClassName Class of the object
             * @param obj Weak pointer to the object instance
             * @param callback Member function pointer to invoke on broadcast
             */
            template<typename ClassName>
            void BindAction(weak<Object>obj, void(ClassName::*callback)(Args...))
            {
                std::function<bool(Args...)> callbackFunc = [obj, callback](Args ...args ) -> bool
                {
                    if(!obj.expired())
                    {
                        (static_cast<ClassName*>(obj.lock().get())->*callback)(args...);
                        return true;
                    }
                    return false;
                };
                mCallbacks.push_back(callbackFunc);
            }

            /**
             * @brief Invoke all bound callbacks with the provided arguments.
             *
             * Removes any callbacks whose objects have expired.
             */
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