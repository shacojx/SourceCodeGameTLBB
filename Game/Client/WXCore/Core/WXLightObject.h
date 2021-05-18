#ifndef __FAIRYLIGHTOBJECT_H__
#define __FAIRYLIGHTOBJECT_H__

#include "WXObject.h"
#include <OgreLight.h>

namespace WX
{
    class ObjectFactory;

    class LightObject : public Object
    {
    protected:
        class Factory;

    public:
        LightObject(void);
        virtual ~LightObject();

        const String& getType(void) const;
        const String& getCategory(void) const;

        void createRenderInstance(System* system);
        void destroyRenderInstance(void);
        void updateRenderInstance(void);

        uint getPropertyFlags(const String& name) const;

        //////////////////////////////////////////////////////////////////////////

        static const String msType;
        static const String msCategory;

        static ObjectFactory* getFactory(void);

        //////////////////////////////////////////////////////////////////////////

        Ogre::Light* getLight(void) const
        {
            return mLight;
        }    

    protected:
        Ogre::SceneNode* mBaseSceneNode;
        Ogre::SceneNode* mSceneNode;
        Ogre::Light* mLight;

        void createLight(void);
        void destroyLight(void);
        void updateRenderInstanceImpl(void);

    public:     // Intend for direct access by property commands only, DO NOT access outside
        Ogre::Light::LightTypes mLightType;
        Ogre::ColourValue mDiffuse;
        Ogre::ColourValue mSpecular;
        Ogre::Vector3 mPosition;
        Ogre::Vector3 mDirection;
        Ogre::Real mRange;
        Ogre::Real mAttenuationConstant;
        Ogre::Real mAttenuationLinear;
        Ogre::Real mAttenuationQuadric;
        Ogre::Real mSpotOuterAngle;
        Ogre::Real mSpotInnerAngle;
        Ogre::Real mSpotFalloff;
        bool mCastShadows;
        bool mStaticLight;
    };

}

#endif 