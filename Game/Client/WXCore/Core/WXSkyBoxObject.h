#ifndef __FAIRYSKYBOXOBJECT_H__
#define __FAIRYSKYBOXOBJECT_H__

#include "WXObject.h"

#include <OgreQuaternion.h>

namespace WX
{
    class ObjectFactory;

    class SkyBoxObject : public Object
    {
    protected:
        class Factory;

    public:
        SkyBoxObject(void);
        virtual ~SkyBoxObject();

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
        Ogre::String mMaterialName;
        Ogre::Real mDistance;
        bool mDrawFirst;
        Ogre::Quaternion mOrientation;

    };

}

#endif 
