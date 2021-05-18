#include "WXLightObject.h"
#include "WXObjectFactory.h"
#include "WXPropertyHelper.h"
#include "WXSystem.h"

#include <OgreSceneManager.h>
#include <OgreStringConverter.h>

namespace WX
{
    const String LightObject::msType = "Light";
    const String LightObject::msCategory = "Light";

    //////////////////////////////////////////////////////////////////////////
    // Factory
    //////////////////////////////////////////////////////////////////////////

    class LightObject::Factory : public ObjectFactory
    {
    public:
        const String& getType(void) const
        {
            return msType;
        }

        Object* createInstance(void)
        {
            return new LightObject;
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Commands
    //////////////////////////////////////////////////////////////////////////

    namespace
    {
        static const Constant gsLightTypeConstants[] =
        {
            "point",        Ogre::Light::LT_POINT,
            "directional",  Ogre::Light::LT_DIRECTIONAL,
            "spotlight",    Ogre::Light::LT_SPOTLIGHT,
            NULL,
        };

        struct LightTypeCmd : PropertyCommand
        {
            Variant doGet(const void* source) const
            {
                const LightObject* object = static_cast<const LightObject*>(source);
                return object->mLightType;
            }

            String doGetAsString(const void* source) const
            {
                String result;

                const LightObject* object = static_cast<const LightObject*>(source);
                if (!mapEnumLabel(gsLightTypeConstants, object->mLightType, result))
                {
                    OGRE_EXCEPT(Ogre::Exception::ERR_RT_ASSERTION_FAILED,
                        "Unknown light type: " + Ogre::StringConverter::toString(object->mLightType),
                        "LightTypeCmd::doGet");
                }

                return result;
            }

            void doSet(void* target, const Variant& value)
            {
                LightObject* object = static_cast<LightObject*>(target);
                object->mLightType = VariantCast<Ogre::Light::LightTypes>(value);
                object->updateRenderInstance();
            }

            void doSetAsString(void* target, const String& value)
            {
                LightObject* object = static_cast<LightObject*>(target);
                int lightType;
                if (!mapEnumValue(gsLightTypeConstants, value, lightType, false))
                {
                    OGRE_EXCEPT(Ogre::Exception::ERR_RT_ASSERTION_FAILED,
                        "Unknown light type: " + value,
                        "LightTypeCmd::doSet");
                }

                object->mLightType = static_cast<Ogre::Light::LightTypes>(lightType);
                object->updateRenderInstance();
            }
        };

        template <Ogre::Vector3 LightObject::*PtrToMember>
        struct Vector3MemberCommand : PropertyCommand
        {
            Variant doGet(const void* source) const
            {
                const LightObject* object = static_cast<const LightObject*>(source);
                return object->*PtrToMember;
            }

            String doGetAsString(const void* source) const
            {
                const LightObject* object = static_cast<const LightObject*>(source);
                return Ogre::StringConverter::toString(object->*PtrToMember);
            }

            void doSet(void* target, const Variant& value)
            {
                LightObject* object = static_cast<LightObject*>(target);
                object->*PtrToMember = VariantCast<Ogre::Vector3>(value);
                object->updateRenderInstance();
            }

            void doSetAsString(void* target, const String& value)
            {
                LightObject* object = static_cast<LightObject*>(target);
                object->*PtrToMember = Ogre::StringConverter::parseVector3(value);
                object->updateRenderInstance();
            }
        };

        template <Ogre::ColourValue LightObject::*PtrToMember>
        struct ColourRGBMemberCommand : PropertyCommand
        {
            Variant doGet(const void* source) const
            {
                const LightObject* object = static_cast<const LightObject*>(source);
                return object->*PtrToMember;
            }

            String doGetAsString(const void* source) const
            {
                const LightObject* object = static_cast<const LightObject*>(source);
                return ColourRGB2String(object->*PtrToMember);
            }

            void doSet(void* target, const Variant& value)
            {
                LightObject* object = static_cast<LightObject*>(target);
                object->*PtrToMember = VariantCast<Ogre::ColourValue>(value);
                object->updateRenderInstance();
            }

            void doSetAsString(void* target, const String& value)
            {
                LightObject* object = static_cast<LightObject*>(target);
                object->*PtrToMember = String2ColourRGB(value);
                object->updateRenderInstance();
            }
        };

        typedef ColourRGBMemberCommand<&LightObject::mDiffuse> DiffuseCmd;
        typedef ColourRGBMemberCommand<&LightObject::mSpecular> SpecularCmd;
        typedef Vector3MemberCommand<&LightObject::mPosition> PositionCmd;
        typedef Vector3MemberCommand<&LightObject::mDirection> DirectionCmd;
        typedef ObjectMemberPropertyCommand<LightObject, Real, &LightObject::mRange> RangeCmd;
        typedef ObjectMemberPropertyCommand<LightObject, Real, &LightObject::mAttenuationConstant> AttenuationConstantCmd;
        typedef ObjectMemberPropertyCommand<LightObject, Real, &LightObject::mAttenuationLinear> AttenuationLinearCmd;
        typedef ObjectMemberPropertyCommand<LightObject, Real, &LightObject::mAttenuationQuadric> AttenuationQuadricCmd;
        typedef ObjectMemberPropertyCommand<LightObject, Real, &LightObject::mSpotOuterAngle> SpotOuterAngleCmd;
        typedef ObjectMemberPropertyCommand<LightObject, Real, &LightObject::mSpotInnerAngle> SpotInnerAngleCmd;
        typedef ObjectMemberPropertyCommand<LightObject, Real, &LightObject::mSpotFalloff> SpotFalloffCmd;
        typedef ObjectMemberPropertyCommand<LightObject, bool, &LightObject::mCastShadows> CastShadowsCmd;
        typedef ObjectMemberPropertyCommand<LightObject, bool, &LightObject::mStaticLight> StaticLightCmd;
    }

    //////////////////////////////////////////////////////////////////////////
    // LightObject class
    //////////////////////////////////////////////////////////////////////////


    LightObject::LightObject(void)
        : Object()
        , mBaseSceneNode(NULL)
        , mSceneNode(NULL)
        , mLight(NULL)
        , mLightType(Ogre::Light::LT_POINT)
        , mDiffuse(Ogre::ColourValue::White)
        , mSpecular(Ogre::ColourValue::Black)
        , mPosition(Ogre::Vector3::ZERO)
        , mDirection(Ogre::Vector3::UNIT_Z)
        , mRange(100000.0f)
        , mAttenuationConstant(1.0f)
        , mAttenuationLinear(0.0f)
        , mAttenuationQuadric(0.0f)
        , mSpotOuterAngle(40.0f)
        , mSpotInnerAngle(30.0f)
        , mSpotFalloff(1.0f)
        , mCastShadows(false)
        , mStaticLight(false)
    {
        if (createPropertyDictionary("Light"))
        {
            static LightTypeCmd lightTypeCmd;
            defineProperty(
                "type",
                "The type of light.",
                getEnumTypeString("LightType", gsLightTypeConstants),
                &lightTypeCmd,
                0);

            static DiffuseCmd diffuseCmd;
            defineProperty(
                "diffuse",
                "The colour of the diffuse light given off by this light.",
                "ColourRGB",
                &diffuseCmd,
                0);

            static SpecularCmd specularCmd;
            defineProperty(
                "specular",
                "The colour of the specular light given off by this light.",
                "ColourRGB",
                &specularCmd,
                0);

            static PositionCmd positionCmd;
            defineProperty(
                "position",
                "The position of the light.\n"
                "Applicable to point lights and spotlights only.",
                "Position",
                &positionCmd,
                0);

            static DirectionCmd directionCmd;
            defineProperty(
                "direction",
                "The direction of the light.\n"
                "Applicable to directional lights and spotlights only.",
                "Direction",
                &directionCmd,
                0);

            static RangeCmd rangeCmd;
            defineProperty(
                "range",
                "The absolute upper range of the light.\n"
                "Applicable to point lights and spotlights only.",
                "Real",
                &rangeCmd,
                0);

            static AttenuationConstantCmd attenuationConstantCmd;
            defineProperty(
                "attenuation.constant",
                "The constant factor in the attenuation formula.\n"
                "Applicable to point lights and spotlights only.",
                "Real",
                &attenuationConstantCmd,
                0);

            static AttenuationLinearCmd attenuationLinearCmd;
            defineProperty(
                "attenuation.linear",
                "The linear factor in the attenuation formula.\n"
                "Applicable to point lights and spotlights only.",
                "Real",
                &attenuationLinearCmd,
                0);

            static AttenuationQuadricCmd attenuationQuadricCmd;
            defineProperty(
                "attenuation.quadric",
                "The quadric factor in the attenuation formula.\n"
                "Applicable to point lights and spotlights only.",
                "Real",
                &attenuationQuadricCmd,
                0);

            static SpotOuterAngleCmd spotOuterAngleCmd;
            defineProperty(
                "spotlight.outer",
                "The angle covered by the spotlights outer cone.\n"
                "Applicable to spotlights only.",
                "Real",
                &spotOuterAngleCmd,
                0);

            static SpotInnerAngleCmd spotInnerAngleCmd;
            defineProperty(
                "spotlight.inner",
                "The angle covered by the spotlights inner cone.\n"
                "Applicable to spotlights only.",
                "Real",
                &spotInnerAngleCmd,
                0);

            static SpotFalloffCmd spotFalloffCmd;
            defineProperty(
                "spotlight.falloff",
                "The falloff between the inner and outer cones of the spotlight.\n"
                "Applicable to spotlights only.",
                "Real",
                &spotFalloffCmd,
                0);

            static CastShadowsCmd castShadowsCmd;
            defineProperty(
                "cast shadows",
                "Does this light cast shadows?",
                "Bool",
                &castShadowsCmd,
                0);

            static StaticLightCmd staticLightCmd;
            defineProperty(
                "static light",
                "This light is use as static, which will be use for calculate lightmap only.",
                "Bool",
                &staticLightCmd,
                0);
        }
    }
    //-----------------------------------------------------------------------
    LightObject::~LightObject()
    {
        destroyRenderInstance();
    }
    //-----------------------------------------------------------------------
    const String& LightObject::getType(void) const
    {
        return msType;
    }
    //-----------------------------------------------------------------------
    const String& LightObject::getCategory(void) const
    {
        return msCategory;
    }
    //-----------------------------------------------------------------------
    void LightObject::createRenderInstance(System* system)
    {
        assert(system);
        assert(!mBaseSceneNode);

        mBaseSceneNode = system->getBaseSceneNode();

        updateRenderInstance();
    }
    //-----------------------------------------------------------------------
    void LightObject::createLight(void)
    {
        assert(!mSceneNode);
        assert(!mLight);

        mSceneNode = mBaseSceneNode->createChildSceneNode();
        mLight = mSceneNode->getCreator()->createLight(mSceneNode->getName());
        mSceneNode->attachObject(mLight);
    }
    //-----------------------------------------------------------------------
    void LightObject::destroyLight(void)
    {
        if (mLight)
        {
            mLight->destroy();
            mLight = NULL;
        }
        if (mSceneNode)
        {
            mSceneNode->destroy();
            mSceneNode = NULL;
        }
    }
    //-----------------------------------------------------------------------
    void LightObject::destroyRenderInstance(void)
    {
        destroyLight();

        mBaseSceneNode = NULL;
    }
    //-----------------------------------------------------------------------
    void LightObject::updateRenderInstanceImpl(void)
    {
        assert(mLight);

        mLight->setCastShadows(mCastShadows);
        mLight->setType(mLightType);
        mLight->setDiffuseColour(mDiffuse);
        mLight->setSpecularColour(mSpecular);
        if (mLightType != Ogre::Light::LT_DIRECTIONAL)
        {
            mLight->setPosition(mPosition);
            mLight->setAttenuation(mRange, mAttenuationConstant, mAttenuationLinear, mAttenuationQuadric);
        }
        if (mLightType != Ogre::Light::LT_POINT)
        {
            mLight->setDirection(mDirection.normalisedCopy());
        }
        if (mLightType == Ogre::Light::LT_SPOTLIGHT)
        {
            mLight->setSpotlightRange(Ogre::Degree(mSpotInnerAngle), Ogre::Degree(mSpotOuterAngle), mSpotFalloff);
        }
    }
    //-----------------------------------------------------------------------
    void LightObject::updateRenderInstance(void)
    {
        if (mBaseSceneNode)
        {
            if (mStaticLight)
            {
                destroyLight();
            }
            else
            {
                if (!mLight)
                {
                    createLight();
                }

                updateRenderInstanceImpl();
            }
        }
    }
    //-----------------------------------------------------------------------
    uint LightObject::getPropertyFlags(const String& name) const
    {
        uint flags = Object::getPropertyFlags(name);

        if (name == "position" || name == "range" || Ogre::StringUtil::startsWith(name, "attenuation", false))
        {
            if (mLightType == Ogre::Light::LT_DIRECTIONAL)
                flags |= PF_READONLY;
        }
        else if (name == "direction")
        {
            if (mLightType == Ogre::Light::LT_POINT)
                flags |= PF_READONLY;
        }
        else if (Ogre::StringUtil::startsWith(name, "spotlight", false))
        {
            if (mLightType != Ogre::Light::LT_SPOTLIGHT)
                flags |= PF_READONLY;
        }

        return flags;
    }
    //-----------------------------------------------------------------------
    ObjectFactory* LightObject::getFactory(void)
    {
        static Factory factory;
        return &factory;
    }

}
