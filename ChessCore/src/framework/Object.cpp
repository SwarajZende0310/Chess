/**
 * @file Object.cpp
 * @brief Implementation of base Object lifetime and ID utilities.
 */
#include"framework/Object.h"

namespace chess
{
    unsigned int Object::uniqueIDCounter = 0;

    Object::Object()
        :mIsPendingDestroy{false},
        mUniqueID{NextAvailableID()}
    {

    }

    Object::~Object()
    {
    }

    void Object::Destroy()
    {

    }
    
    weak<Object> Object::GetWeakRef()
    {
        return weak_from_this();
    }

    weak<const Object> Object::GetWeakRef() const
    {
        return weak_from_this();
    }
    unsigned int Object::NextAvailableID()
    {
        return uniqueIDCounter++;
    }
}
