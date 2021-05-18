#ifndef __FAIRYPARTICLESYSTEMOBJECT_H__
#define __FAIRYPARTICLESYSTEMOBJECT_H__

#include "WXPlacementObject.h"

namespace WX {

    class ObjectFactory;

    class ParticleSystemObject : public PlacementObject
    {
    protected:
        class Factory;

    public:
        ParticleSystemObject(void);
        virtual ~ParticleSystemObject();

        const String& getType(void) const;
        const String& getCategory(void) const;

        void createRenderInstance(System* system);
        void destroyRenderInstance(void);

        //////////////////////////////////////////////////////////////////////////

        static const String msType;
        static const String msCategory;

        static ObjectFactory* getFactory(void);

        void setParticleSystemName( const String &name );

        const String &getParticleSystemName(void) const
        {
            return mParticleSystemName;
        }

        //////////////////////////////////////////////////////////////////////////
        Ogre::ParticleSystem * getParticleSystem(void)
        {
            return mParticleSystem;
        }

		void setParticleSystem(Ogre::ParticleSystem *system)
		{
			assert (system);
			mParticleSystem = system;
		}

    protected:
        String mParticleSystemName;

        Ogre::ParticleSystem *mParticleSystem;

    protected:

        void createParticleSystem(void);
        void destroyParticleSystem(void);
    };
}

#endif // 
