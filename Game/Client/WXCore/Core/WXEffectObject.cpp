/********************************************************************
filename:   WXEffectObject.cpp

purpose:    the object that wrap the effect,so we can put the effect in the scene
*********************************************************************/

#include "WXEffectObject.h"
#include "WXObjectFactory.h"
#include "EffectSystem/WXEffectManager.h"
#include "EffectSystem/WXEffect.h"

#include <OgreSceneManager.h>
#include <OgreStringConverter.h>
#include <OgreSceneNode.h>

namespace WX {

    const String EffectObject::msType = "Effect";
    const String EffectObject::msCategory = "StaticObject";

    //////////////////////////////////////////////////////////////////////////
    // Factory
    //////////////////////////////////////////////////////////////////////////

    class EffectObject::Factory : public ObjectFactory
    {
    public:
        const String& getType(void) const
        {
            return msType;
        }

        Object* createInstance(void)
        {
            return new EffectObject;
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Commands
    //////////////////////////////////////////////////////////////////////////

    namespace
    {
        struct EffectNameCmd : PropertyCommand
        {
            Variant doGet(const void* source) const
            {
                const EffectObject* object = static_cast<const EffectObject*>(source);
                return object->getEffectName();
            }

            String doGetAsString(const void* source) const
            {
                const EffectObject* object = static_cast<const EffectObject*>(source);
                return object->getEffectName();
            }

            void doSet(void* target, const Variant& value)
            {
                EffectObject* object = static_cast<EffectObject*>(target);
                String effectName = VariantCast<String>(value);
                object->setEffectName(effectName);
            }

            void doSetAsString(void* target, const String& value)
            {
                EffectObject* object = static_cast<EffectObject*>(target);
                object->setEffectName(value);
            }
        };

    }


    //////////////////////////////////////////////////////////////////////////
    // EffectObject class
    //////////////////////////////////////////////////////////////////////////
    EffectObject::EffectObject(void)
        : PlacementObject(),
        mEffect(NULL)
    {
        if (createPropertyDictionary("Effect"))
        {
            static EffectNameCmd effectNameCmd;
            defineProperty(
                "effect name",
                "The effect template name use to create this effect object.",
                "effect template name",
                &effectNameCmd,
                PF_PRIMARY);

            addBaseProperty();
        }
    }
    //-----------------------------------------------------------------------
    EffectObject::~EffectObject()
    {
        destroyRenderInstance();
    }
    //-----------------------------------------------------------------------
    const String& EffectObject::getType(void) const
    {
        return msType;
    }
    //-----------------------------------------------------------------------
    const String& EffectObject::getCategory(void) const
    {
        return msCategory;
    }
    //-----------------------------------------------------------------------
    void EffectObject::createRenderInstance(System* system)
    {
        assert(system);

        PlacementObject::createRenderInstance(system);

        if (!mEffectName.empty())
        {
            createEffect();
        }
    }
    //-----------------------------------------------------------------------
    void EffectObject::destroyRenderInstance(void)
    {
        if (mEffect)
        {
            destroyEffect();
        }
       
        PlacementObject::destroyRenderInstance();
    }
    //-----------------------------------------------------------------------
    void EffectObject::createEffect(void)
    {
        assert( mEffect == NULL );
        assert(!mEffectName.empty());

        Ogre::SceneNode* parent = getSceneNode();

		if (parent)
		{
			mEffect = EffectManager::getSingleton().createEffect( mEffectName );
			assert(mEffect);

            WX::TransformInfos tempTransformInfos;
            WX::TransformInfo tempTransformInfo(Ogre::Vector3::ZERO);
            tempTransformInfos.push_back(tempTransformInfo);
            mEffect->setExtraTransformInfos(tempTransformInfos);

			// the scene node of placement object will be the parent scene node of
			// effect object, so the position of the scene node will be the position of
			// the effect.
			mEffect->createSceneNode(parent);	
		}
    }
    //-----------------------------------------------------------------------
    void EffectObject::destroyEffect(void)
    {
        assert(mEffect);

        EffectManager::getSingleton().destroyEffect(mEffect);

        mEffect = NULL;
    }
    //-----------------------------------------------------------------------
    void EffectObject::setEffectName( const String &name )
    {
        if (mEffectName != name)
        {
            if (mEffect)
            {
                destroyEffect();
            }

            mEffectName = name;

            if (!mEffectName.empty())
            {
                createEffect();
            }
        }
    }
    //-----------------------------------------------------------------------
    ObjectFactory* EffectObject::getFactory(void)
    {
        static Factory factory;
        return &factory;
    }
}
