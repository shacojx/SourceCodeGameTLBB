#include "WXSkyBoxObject.h"
#include "WXObjectFactory.h"
#include "WXPropertyHelper.h"
#include "WXSystem.h"

#include <OgreSceneManager.h>
#include <OgreStringConverter.h>

namespace WX
{
    const String SkyBoxObject::msType = "SkyBox";
    const String SkyBoxObject::msCategory = "Enviroment";

    //////////////////////////////////////////////////////////////////////////
    // Factory
    //////////////////////////////////////////////////////////////////////////

    class SkyBoxObject::Factory : public ObjectFactory
    {
    public:
        const String& getType(void) const
        {
            return msType;
        }

        Object* createInstance(void)
        {
            return new SkyBoxObject;
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Commands
    //////////////////////////////////////////////////////////////////////////
    //非命名名字空间
    namespace
    {
        struct MaterialNameCmd : PropertyCommand
        {
            Variant doGet(const void* source) const
            {
                const SkyBoxObject* object = static_cast<const SkyBoxObject*>(source);
                return object->mMaterialName;
            }
            String doGetAsString(const void* source) const
            {
                const SkyBoxObject* object = static_cast<const SkyBoxObject*>(source);
                return object->mMaterialName;
            }
            void doSet(void* target, const Variant& value)
            {
                SkyBoxObject* object = static_cast<SkyBoxObject*>(target);
                object->mMaterialName = VariantCast<String>(value);
                object->updateRenderInstance();
            }
            void doSetAsString(void* target, const String& value)
            {
                SkyBoxObject* object = static_cast<SkyBoxObject*>(target);
                object->mMaterialName = value;
                object->updateRenderInstance();
            }
        };

        struct OrientationCmd : PropertyCommand
        {
            Variant doGet(const void* source) const
            {
                const SkyBoxObject* object = static_cast<const SkyBoxObject*>(source);
                return object->mOrientation;
            }
            String doGetAsString(const void* source) const
            {
                const SkyBoxObject* object = static_cast<const SkyBoxObject*>(source);
                return Ogre::StringConverter::toString(object->mOrientation);
            }
            void doSet(void* target, const Variant& value)
            {
                SkyBoxObject* object = static_cast<SkyBoxObject*>(target);
                object->mOrientation = VariantCast<Ogre::Quaternion>(value);
                object->updateRenderInstance();
            }
            void doSetAsString(void* target, const String& value)
            {
                SkyBoxObject* object = static_cast<SkyBoxObject*>(target);
                object->mOrientation = Ogre::StringConverter::parseQuaternion(value);
                object->updateRenderInstance();
            }
        };

        typedef ObjectMemberPropertyCommand<SkyBoxObject, bool, &SkyBoxObject::mDrawFirst> DrawFirstCmd;
        typedef ObjectMemberPropertyCommand<SkyBoxObject, Real, &SkyBoxObject::mDistance> DistanceCmd;
    }

    //////////////////////////////////////////////////////////////////////////
    // SkyBoxObject class
    //////////////////////////////////////////////////////////////////////////


    SkyBoxObject::SkyBoxObject(void)
        : Object()
        , mSceneManager(NULL)
        , mMaterialName("BaseWhite") 
        ,mDistance(5000)
        ,mDrawFirst(true)
        ,mOrientation(Ogre::Quaternion::IDENTITY)
    {
        if (createPropertyDictionary("SkyBox"))
        {
            static MaterialNameCmd materialNameCmd;
            defineProperty(
                "material",
                "the name of material",
                "Material",
                &materialNameCmd,
                PF_PRIMARY);
            static DistanceCmd distanceCmd;
            defineProperty(
                "distance",
                "the distance of skybox.",
                "Real",    
                &distanceCmd,
                0);
            
            static DrawFirstCmd drawFirstCmd;
            defineProperty(
                "draw first",
                "whether the box is drawn before all other geometry in the scene.",
                "Bool",
                &drawFirstCmd,
                0);

            static OrientationCmd orientationCmd;
            defineProperty(
                "orientation",
                "Optional parameter to specify the orientation of the box",
                "Orientation",
                &orientationCmd,
                0);
        }
    }
    //-----------------------------------------------------------------------
    SkyBoxObject::~SkyBoxObject()
    {
        destroyRenderInstance();
    }
    //-----------------------------------------------------------------------
    const String& SkyBoxObject::getType(void) const
    {
        return msType;
    }
    //-----------------------------------------------------------------------
    const String& SkyBoxObject::getCategory(void) const
    {
        return msCategory;
    }
    //-----------------------------------------------------------------------
    void SkyBoxObject::createRenderInstance(System* system)
    {
        assert(system);
        if (system->isEditable())
        {
            mSceneManager = system->getSceneManager();

            updateRenderInstanceImpl();
        }
    }
    //-----------------------------------------------------------------------
    void SkyBoxObject::destroyRenderInstance(void)
    {
        if (mSceneManager)
            mSceneManager->setSkyBox(false, mMaterialName);
    }
    //-----------------------------------------------------------------------
    void SkyBoxObject::updateRenderInstanceImpl(void)
    {
        assert(mSceneManager);
        mSceneManager->setSkyBox(true,mMaterialName,mDistance,mDrawFirst,mOrientation);
    }
    //-----------------------------------------------------------------------
    void SkyBoxObject::updateRenderInstance(void)
    {
        if (mSceneManager)
        {
            updateRenderInstanceImpl();
        }
    }
    //-----------------------------------------------------------------------
    ObjectFactory* SkyBoxObject::getFactory(void)
    {
        static Factory factory;
        return &factory;
    }

}
