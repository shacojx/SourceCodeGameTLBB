#ifndef __FAIRYSKYPLANEOBJECT_H__
#define __FAIRYSKYPLANEOBJECT_H__

#include "WXObject.h"

#include <OgrePlane.h>

namespace WX
{
    class ObjectFactory;

    class SkyPlaneObject : public Object
    {
    protected:
        class Factory;

    public:
        SkyPlaneObject(void);
        virtual ~SkyPlaneObject();

        const String& getType(void) const;
        const String& getCategory(void) const;

        void createRenderInstance(System* system);
        void destroyRenderInstance(void);
        void updateRenderInstance(void);

        //////////////////////////////////////////////////////////////////////////

        static const String msType;
        static const String msCategory;

        static ObjectFactory* getFactory(void);
        //////////////////////////////////////////////////////////////////////////
   

    protected:
        Ogre::SceneManager* mSceneManager;

        void updateRenderInstanceImpl(void);

    public:     // Intend for direct access by property commands only, DO NOT access outside
        Ogre::Plane mPlane;
        Ogre::String mMaterialName;
        Ogre::Real mScale;
        Ogre::Real mTiling;
        bool mDrawFirst;
        Ogre::Real mBow;
        int mXsegments;
        int mYsegments;
    };

}

#endif 
