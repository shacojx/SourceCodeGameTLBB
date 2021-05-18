/********************************************************************
    filename:   WXMaterialInstance.h
    
    purpose:    Keep a copy of origin and derived material, for
                dealing with material adjustment.
*********************************************************************/

#ifndef __FAIRYMATERIALINSTANCE_H__
#define __FAIRYMATERIALINSTANCE_H__

#include "WXPrerequisites.h"

#include <OgreMaterial.h>

namespace WX {

    class MaterialInstance
    {
    public:
        MaterialInstance(const Ogre::MaterialPtr& origin);
        ~MaterialInstance();

        const Ogre::MaterialPtr& getOrigin(void) const
        {
            return mOrigin;
        }

        const Ogre::MaterialPtr& getDerived(void) const
        {
            return mDerived;
        }

    protected:
        Ogre::MaterialPtr mOrigin;
        Ogre::MaterialPtr mDerived;
    };

}

#endif // 
