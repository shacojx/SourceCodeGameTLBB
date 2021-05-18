/********************************************************************
    filename:   WXMaterialInstance.cpp
    
    purpose:    Keep a copy of origin and derived material, for
                dealing with material adjustment.
*********************************************************************/

#include "WXMaterialInstance.h"

#include <OgreMaterialManager.h>
#include <OgreStringConverter.h>

namespace WX {

    MaterialInstance::MaterialInstance(const Ogre::MaterialPtr& origin)
        : mOrigin(origin)
        , mDerived()
    {
        // Generate material name
        Ogre::String newName;

        // Avoid name collision
        do {
            newName = origin->getName() + Ogre::StringConverter::toString(Ogre::Math::UnitRandom());
        } while (Ogre::MaterialManager::getSingleton().resourceExists(newName));

        // Clone material
        mDerived = mOrigin->clone(newName);
    }

    MaterialInstance::~MaterialInstance()
    {
        if (!mDerived.isNull())
        {
            Ogre::MaterialManager *mm = Ogre::MaterialManager::getSingletonPtr();
            if (mm)
            {
                mm->remove(mDerived->getHandle());
            }
            mDerived.setNull();
        }
    }

}
