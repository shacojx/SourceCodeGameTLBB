#ifndef __FAIRYENVIROMENTOBJECT_H__
#define __FAIRYENVIROMENTOBJECT_H__

#include "WXObject.h"

#include <OgreColourValue.h>
#include <OgreCommon.h>

namespace WX
{
    class ObjectFactory;

    class EnviromentObject : public Object
    {
    protected:
        class Factory;

    public:
        EnviromentObject(void);
        virtual ~EnviromentObject();

        const String& getType(void) const;
        const String& getCategory(void) const;

        void createRenderInstance(System* system);
        void destroyRenderInstance(void);
        void updateRenderInstance(void);

        uint getPropertyFlags(const String& name) const;

		void setWeatherParticleSystemPosition(const Ogre::Vector3& pos);

		void setRenderWeather(bool render);
		bool getRenderWeather(void)
		{
			return mRenderWeather;
		}

        //////////////////////////////////////////////////////////////////////////

        static const String msType;
        static const String msCategory;

        static ObjectFactory* getFactory(void);

    protected:
        Ogre::SceneManager* mSceneManager;
        Ogre::Viewport* mViewport;

		Ogre::ParticleSystem* mWeatherParticleSystem;

        void updateRenderInstanceImpl(void);

		void createWeatherParticleSystemNode(System* system);
		void destroyWeatherParticleSystemNode(void);

		void createWeatherParticleSystem(void);
		void destroyWeatherParticleSystem(void);
        
    public:     // Intend for direct access by property commands only, DO NOT access outside
        Ogre::ColourValue mAmbientLight;
        Ogre::ColourValue mBackgroundColour;
        Ogre::FogMode mFogMode;
        Ogre::ColourValue mFogColour;
        Ogre::Real mFogExpDensity;
        Ogre::Real mFogLinearStart;
        Ogre::Real mFogLinearEnd; 

		/// weather particle system
		Ogre::String mWeatherParticleSystemName;
		Ogre::SceneNode* mWeatherParticleSystemNode;
		bool mRenderWeather;
    };

}

#endif 
