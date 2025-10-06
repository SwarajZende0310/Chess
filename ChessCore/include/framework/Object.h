/**
 * @file Object.h
 * @brief Base object with lifetime management and unique IDs.
 */
#pragma once

#include<memory>

#include"framework/Core.h"
#include"framework/Delegate.h"

namespace chess
{
    /**
     * @brief Base class providing destroy notifications and unique IDs.
     */
    class Object : public std::enable_shared_from_this<Object>
    {
        public:
            /**
             * @brief Construct a new Object and assign a unique ID.
             */
            Object();
            /**
             * @brief Virtual destructor to allow polymorphic deletion.
             */
            virtual ~Object();

            /**
             * @brief Mark object for destruction and broadcast `onDestroy`.
             */
            virtual void Destroy();
            /**
             * @brief Whether this object has been marked for destruction.
             */
            bool IsPendingDestroy() { return mIsPendingDestroy; }

            /**
             * @brief Get a weak reference to this object.
             */
            weak<Object> GetWeakRef();
            /**
             * @brief Get a const weak reference to this object.
             */
            weak<const Object> GetWeakRef()const;

            /**
             * @brief Delegate invoked when this object is destroyed.
             */
            Delegate<Object*> onDestroy;

            /**
             * @brief Unique, monotonically increasing ID assigned at construction.
             */
            unsigned int GetUniqueID() { return mUniqueID; }
        protected:

        private:
            bool mIsPendingDestroy; ///< True once `Destroy()` is called
            unsigned int mUniqueID; ///< Unique ID for this object

            static unsigned int uniqueIDCounter; ///< Global ID counter
            /**
             * @brief Get next available unique ID (increments counter).
             */
            static unsigned int NextAvailableID();
    };
}