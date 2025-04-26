#pragma once

#include<memory>

#include"framework/Core.h"
#include"framework/Delegate.h"

namespace chess
{
    class Object : public std::enable_shared_from_this<Object>
    {
        public:
            Object();
            virtual ~Object();

            virtual void Destroy();
            bool IsPendingDestroy() { return mIsPendingDestroy; }

            weak<Object> GetWeakRef();
            weak<const Object> GetWeakRef()const;

            Delegate<Object*> onDestroy;

            unsigned int GetUniqueID() { return mUniqueID; }
        protected:

        private:
            bool mIsPendingDestroy;
            unsigned int mUniqueID;

            static unsigned int uniqueIDCounter;
            static unsigned int NextAvailableID();
    };
}