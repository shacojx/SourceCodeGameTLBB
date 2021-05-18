#include "WXEnviromentObject.h"
#include "WXObjectFactory.h"
#include "WXPropertyHelper.h"
#include "WXSystem.h"

#include <OgreSceneManager.h>
#include <OgreStringConverter.h>
#include <OgreCamera.h>
#include <OgreViewport.h>
#include <OgreParticleSystem.h>

namespace WX
{
    const String EnviromentObject::msType = "Enviroment";
    const String EnviromentObject::msCategory = "Enviroment";

    //////////////////////////////////////////////////////////////////////////
    // Factory
    //////////////////////////////////////////////////////////////////////////

    class EnviromentObject::Factory : public ObjectFactory
    {
    public:
        const String& getType(void) const
        {
            return msType;
        }

        Object* createInstance(void)
        {
            return new EnviromentObject;
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Commands
    //////////////////////////////////////////////////////////////////////////
    namespace
    {  
        static const Constant gsFogModeConstants[] =
        {
            "exp",    Ogre::FOG_EXP,
            "exp2",   Ogre::FOG_EXP2,
            "linear", Ogre::FOG_LINEAR,
            "none",   Ogre::FOG_NONE,
             NULL,
        };

        struct FogModeCmd : PropertyCommand
        {
            Variant doGet(const void* source) const
            {
                const EnviromentObject* object = static_cast<const EnviromentObject*>(source);
                return object->mFogMode;
            }

            String doGetAsString(const void* source) const
            {
                String result;

                const EnviromentObject* object = static_cast<const EnviromentObject*>(source);
                if (!mapEnumLabel(gsFogModeConstants, object->mFogMode, result))
                {
                    OGRE_EXCEPT(Ogre::Exception::ERR_RT_ASSERTION_FAILED,
                        "Unknown fog mode: " + Ogre::StringConverter::toString(object->mFogMode),
                        "FogModeCmd::doGet");
                }

                return result;
            }

            void doSet(void* target, const Variant& value)
            {
                EnviromentObject* object = static_cast<EnviromentObject*>(target);
                object->mFogMode = VariantCast<Ogre::FogMode>(value);
                object->updateRenderInstance();
            }

            void doSetAsString(void* target, const String& value)
            {
                EnviromentObject* object = static_cast<EnviromentObject*>(target);
                int fogMode;
                if (!mapEnumValue(gsFogModeConstants, value, fogMode, false))
                {
                    OGRE_EXCEPT(Ogre::Exception::ERR_RT_ASSERTION_FAILED,
                        "Unknown fog mode: " + value,
                        "FogModeCmd::doSet");
                }

                object->mFogMode = static_cast<Ogre::FogMode>(fogMode);
                object->updateRenderInstance();
            }
        };

        template <Ogre::ColourValue EnviromentObject::*PtrToMember>
        struct ColourRGBMemberCommand : PropertyCommand
        {
            Variant doGet(const void* source) const
            {
                const EnviromentObject* object = static_cast<const EnviromentObject*>(source);
                return object->*PtrToMember;
            }

            String doGetAsString(const void* source) const
            {
                const EnviromentObject* object = static_cast<const EnviromentObject*>(source);
                return ColourRGB2String(object->*PtrToMember);
            }

            void doSet(void* target, const Variant& value)
            {
                EnviromentObject* object = static_cast<EnviromentObject*>(target);
                object->*PtrToMember = VariantCast<Ogre::ColourValue>(value);
                object->updateRenderInstance();
            }

            void doSetAsString(void* target, const String& value)
            {
                EnviromentObject* object = static_cast<EnviromentObject*>(target);
                object->*PtrToMember = String2ColourRGB(value);
                object->updateRenderInstance();
            }
        };

		struct RenderWeatherCmd : PropertyCommand
		{
			Variant doGet(const void* source) const
			{
				const EnviromentObject* object = static_cast<const EnviromentObject*>(source);
				return object->mRenderWeather;
			}

			String doGetAsString(const void* source) const
			{
				const EnviromentObject* object = static_cast<const EnviromentObject*>(source);
				return Ogre::StringConverter::toString(object->mRenderWeather);
			}

			void doSet(void* target, const Variant& value)
			{
				EnviromentObject* object = static_cast<EnviromentObject*>(target);
				object->setRenderWeather( VariantCast<bool>(value) );
			}

			void doSetAsString(void* target, const String& value)
			{
				EnviromentObject* object = static_cast<EnviromentObject*>(target);

				object->setRenderWeather( Ogre::StringConverter::parseBool(value) );
			}
		};

        typedef ColourRGBMemberCommand<&EnviromentObject::mAmbientLight> AmbientLightCmd;
        typedef ColourRGBMemberCommand<&EnviromentObject::mBackgroundColour> BackgroundColourCmd;
        typedef ColourRGBMemberCommand<&EnviromentObject::mFogColour> FogColourCmd;
        typedef ObjectMemberPropertyCommand<EnviromentObject,Real,&EnviromentObject::mFogExpDensity> FogExpDensityCmd;    
        typedef ObjectMemberPropertyCommand<EnviromentObject,Real,&EnviromentObject::mFogLinearStart> FogLinearStartCmd;
        typedef ObjectMemberPropertyCommand<EnviromentObject,Real,&EnviromentObject::mFogLinearEnd> FogLinearEndCmd;

		typedef ObjectMemberPropertyCommand<EnviromentObject, String, &EnviromentObject::mWeatherParticleSystemName> WeatherParticleSystemNameCmd;
    }

    //////////////////////////////////////////////////////////////////////////
    // EnviromentObject class
    //////////////////////////////////////////////////////////////////////////

    EnviromentObject::EnviromentObject(void)
        : Object()
        , mSceneManager(NULL)
        , mViewport(NULL)
        , mFogMode(Ogre::FOG_NONE)
        , mFogExpDensity(0.001f)
        , mFogLinearStart(0.0f)
        , mFogLinearEnd(1.0f)
        , mAmbientLight(Ogre::ColourValue::Black)
        , mBackgroundColour(Ogre::ColourValue::Black)
        , mFogColour(Ogre::ColourValue::White)
		, mWeatherParticleSystemName("")
		, mWeatherParticleSystem(NULL)
		, mWeatherParticleSystemNode(NULL)
		, mRenderWeather(true)
    {
        if (createPropertyDictionary("Enviroment"))
        {
            static AmbientLightCmd ambientColourCmd;
            defineProperty(
                "ambient",
                "The ambient light level to be used for the scene.",
                "ColourRGB",
                &ambientColourCmd,
                0);     

            static BackgroundColourCmd backgroundCmd;
            defineProperty(
                "background",
                "The background colour of the scene.",
                "ColourRGB",
                &backgroundCmd,
                0);     

            static FogModeCmd fogModeCmd;
            defineProperty(
                "fog.mode",
                "The fog mode",
                getEnumTypeString("FogMode", gsFogModeConstants),
                &fogModeCmd,
                0);

            static FogColourCmd fogColourCmd;
            defineProperty(
                "fog.colour",
                "The colour of the fog.",
                "ColourRGB",
                &fogColourCmd,
                0);     

            static FogExpDensityCmd expDensityCmd;
            defineProperty(
                "fog.exp density",
                "The density of the fog in exp or exp2 mode.",
                "Real",
                &expDensityCmd,
                0);    

            static FogLinearStartCmd linearStartCmd;
            defineProperty(
                "fog.linear start",
                "Distance in world units at which linear fog starts to encroach.",
                "Real",
                &linearStartCmd,
                0);    

            static FogLinearEndCmd linearEndCmd;
            defineProperty(
                "fog.linear end",
                "Distance in world units at which linear fog becomes completely opaque.",
                "Real",
                &linearEndCmd,
                0);    

			static RenderWeatherCmd renderWeatherCmd;
			defineProperty(
				"weather.render weather",
				"if this option is true, the weather particle system will be rendered.",
				"Bool",
				&renderWeatherCmd
				);

			static WeatherParticleSystemNameCmd weatherCmd;
			defineProperty(
				"weather.particle system name",
				"The name of the particle system that used for scene weather.",
				"String",
				&weatherCmd
				);
        }
    }
    //-----------------------------------------------------------------------
    EnviromentObject::~EnviromentObject()
    {
        destroyRenderInstance();
    }
    //-----------------------------------------------------------------------
    const String& EnviromentObject::getType(void) const
    {
        return msType;
    }
    //-----------------------------------------------------------------------
    const String& EnviromentObject::getCategory(void) const
    {
        return msCategory;
    }
    //-----------------------------------------------------------------------
    void EnviromentObject::createRenderInstance(System* system)
    {
        assert(system);
        mSceneManager = system->getSceneManager();
        mViewport = system->getViewport();

        updateRenderInstanceImpl();

		createWeatherParticleSystemNode(system);
		createWeatherParticleSystem();
    }
    //-----------------------------------------------------------------------
    void EnviromentObject::destroyRenderInstance(void)
    {
		destroyWeatherParticleSystem();
		destroyWeatherParticleSystemNode();

        if (mSceneManager)
        {
            mSceneManager->setFog(Ogre::FOG_NONE);
            mSceneManager->setAmbientLight(Ogre::ColourValue::Black);
            mSceneManager = NULL;
        }
        if (mViewport)
        {
            mViewport->setBackgroundColour(Ogre::ColourValue::Black);
            mViewport = NULL;
        }
    }
    //-----------------------------------------------------------------------
    void EnviromentObject::updateRenderInstanceImpl(void)
    {
        assert(mSceneManager);   
        assert(mViewport);

        mSceneManager->setFog(mFogMode, mFogColour, mFogExpDensity, mFogLinearStart, mFogLinearEnd);
        mSceneManager->setAmbientLight(mAmbientLight);
        mViewport->setBackgroundColour(mFogMode == Ogre::FOG_NONE ? mBackgroundColour : mFogColour);
    }
    //-----------------------------------------------------------------------
    void EnviromentObject::updateRenderInstance(void)
    {
        if (mSceneManager && mViewport)
        {
            updateRenderInstanceImpl();
        }

		createWeatherParticleSystem();
    }
    //-----------------------------------------------------------------------
    uint EnviromentObject::getPropertyFlags(const String& name) const
    {
        uint flags = Object::getPropertyFlags(name);

        if (name == "fog.exp density")
        {
            if (mFogMode != Ogre::FOG_EXP && mFogMode != Ogre::FOG_EXP2)
                flags |= PF_READONLY;
        }
        else if (name == "fog.linear start" || name == "fog.linear end")
        {
            if (mFogMode != Ogre::FOG_LINEAR )
                flags |= PF_READONLY;
        }
        else if (name == "background")
        {
            if (mFogMode != Ogre::FOG_NONE)
                flags |= PF_READONLY;
        }

        return flags;
    }
    //-----------------------------------------------------------------------
    ObjectFactory* EnviromentObject::getFactory(void)
    {
        static Factory factory;
        return &factory;
    }
	//-----------------------------------------------------------------------
	void EnviromentObject::createWeatherParticleSystemNode(System* system)
	{
		assert (system);

		if ( NULL == mWeatherParticleSystemNode )
		{
			mWeatherParticleSystemNode = system->getBaseSceneNode()->createChildSceneNode();
			assert (mWeatherParticleSystemNode);
		}
	}
	//-----------------------------------------------------------------------
	void EnviromentObject::destroyWeatherParticleSystemNode(void)
	{
		if (mWeatherParticleSystemNode)
		{
			mWeatherParticleSystemNode->destroy();
			mWeatherParticleSystemNode = NULL;
		}
	}
	//-----------------------------------------------------------------------
	void EnviromentObject::createWeatherParticleSystem(void)
	{
		if ( false == mWeatherParticleSystemName.empty() && mWeatherParticleSystemNode )
		{
			if (mWeatherParticleSystem)
			{
				destroyWeatherParticleSystem();
			}

			mWeatherParticleSystem = mWeatherParticleSystemNode->getCreator()->
                createParticleSystem(mWeatherParticleSystemNode->getName(), mWeatherParticleSystemName);

			if (mWeatherParticleSystem)
			{
				mWeatherParticleSystem->fastForward(5.0f);
				mWeatherParticleSystemNode->attachObject(mWeatherParticleSystem);

				mWeatherParticleSystemNode->setVisible(mRenderWeather);
			}
		}
		// if the particle system name is empty,we will delete the exist particle system
		else
		{
			if (mWeatherParticleSystem)
			{
				destroyWeatherParticleSystem();
			}
		}
	}
	//-----------------------------------------------------------------------
	void EnviromentObject::destroyWeatherParticleSystem(void)
	{
		if (mWeatherParticleSystem)
		{
			mWeatherParticleSystem->destroy();
			mWeatherParticleSystem = NULL;		
		}		
	}
	//-----------------------------------------------------------------------
	void EnviromentObject::setWeatherParticleSystemPosition(const Ogre::Vector3& pos)
	{
		if (mWeatherParticleSystemNode)
			mWeatherParticleSystemNode->setPosition(pos);
	}
	//-----------------------------------------------------------------------
	void EnviromentObject::setRenderWeather(bool render)
	{
		mRenderWeather = render;

		if (mWeatherParticleSystemNode)
			mWeatherParticleSystemNode->setVisible(mRenderWeather);
	}
}
