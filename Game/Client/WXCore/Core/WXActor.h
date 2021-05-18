#ifndef __FAIRYACTOR_H__
#define __FAIRYACTOR_H__

#include "WXPrerequisites.h"

// ----------------------------------------------------------------------------
// forward declares
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// classes
// ----------------------------------------------------------------------------

namespace WX {

	class System;
	class LogicModel;

    class Actor
    {
    protected:
        typedef LogicModel Impl;

        Impl* mImpl;

    public:
    //    Actor(Ogre::SceneManager* creator, const String& name, bool editable);
		Actor( System *system, const String& name, const String& modelName, Ogre::SceneNode* node );
        virtual ~Actor();

        Impl* getImpl(void) const
        {
            return mImpl;
        }

        Ogre::SceneNode* getSceneNode(void) const;

        Ogre::AxisAlignedBox getBoundingBox(void) const;

        void setUserObject(Ogre::UserDefinedObject* userObject);
        Ogre::UserDefinedObject* getUserObject(void) const;
    };

}

#endif 
