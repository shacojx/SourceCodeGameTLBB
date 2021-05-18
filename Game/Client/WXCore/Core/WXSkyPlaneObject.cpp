#include "WXSkyPlaneObject.h"
#include "WXObjectFactory.h"
#include "WXPropertyHelper.h"
#include "WXSystem.h"

#include <OgreSceneManager.h>
#include <OgreStringConverter.h>

namespace WX
{
    const String SkyPlaneObject::msType = "SkyPlane";
    const String SkyPlaneObject::msCategory = "Enviroment";

    //////////////////////////////////////////////////////////////////////////
    // Plane/String converter
    //////////////////////////////////////////////////////////////////////////

    static String toString(const Ogre::Plane& val)
    {
        return Ogre::StringConverter::toString(val.normal) + " | " +
               Ogre::StringConverter::toString(val.d);
    }

    static Ogre::Plane parsePlane(const String& val)
    {
        std::vector<String> vec = Ogre::StringUtil::split(val, "|");

        if (vec.size() != 2)
        {
            return Ogre::Plane(Ogre::Vector3::UNIT_Z, 0);
        }
        else
        {
            return Ogre::Plane(Ogre::StringConverter::parseVector3(vec[0]),
                               Ogre::StringConverter::parseReal(vec[1]));
        }
    }

    //////////////////////////////////////////////////////////////////////////
    // Factory
    //////////////////////////////////////////////////////////////////////////

    class SkyPlaneObject::Factory : public ObjectFactory
    {
    public:
        const String& getType(void) const
        {
            return msType;
        }

        Object* createInstance(void)
        {
            return new SkyPlaneObject;
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
                const SkyPlaneObject* object = static_cast<const SkyPlaneObject*>(source);
                return object->mMaterialName;
            }
            String doGetAsString(const void* source) const
            {
                const SkyPlaneObject* object = static_cast<const SkyPlaneObject*>(source);
                return object->mMaterialName;
            }
            void doSet(void* target, const Variant& value)
            {
                SkyPlaneObject* object = static_cast<SkyPlaneObject*>(target);
                object->mMaterialName = VariantCast<String>(value);
                object->updateRenderInstance();
            }
            void doSetAsString(void* target, const String& value)
            {
                SkyPlaneObject* object = static_cast<SkyPlaneObject*>(target);
                object->mMaterialName = value;
                object->updateRenderInstance();
            }
        };

        struct PlaneCmd : PropertyCommand
        {
            Variant doGet(const void* source) const
            {
                const SkyPlaneObject* object = static_cast<const SkyPlaneObject*>(source);
                return object->mPlane;
            }
            String doGetAsString(const void* source) const
            {
                const SkyPlaneObject* object = static_cast<const SkyPlaneObject*>(source);
                return toString(object->mPlane);
            }
            void doSet(void* target, const Variant& value)
            {
                SkyPlaneObject* object = static_cast<SkyPlaneObject*>(target);
                object->mPlane = VariantCast<Ogre::Plane>(value);
                object->updateRenderInstance();
            }
            void doSetAsString(void* target, const String& value)
            {
                SkyPlaneObject* object = static_cast<SkyPlaneObject*>(target);
                object->mPlane = parsePlane(value);
                object->updateRenderInstance();
            }
        };

        typedef ObjectMemberPropertyCommand<SkyPlaneObject, bool, &SkyPlaneObject::mDrawFirst> DrawFirstCmd;
        typedef ObjectMemberPropertyCommand<SkyPlaneObject, Real, &SkyPlaneObject::mTiling> TilingCmd;
        typedef ObjectMemberPropertyCommand<SkyPlaneObject, Real, &SkyPlaneObject::mScale> ScaleCmd;
        typedef ObjectMemberPropertyCommand<SkyPlaneObject, Real, &SkyPlaneObject::mBow> BowCmd;
        typedef ObjectMemberPropertyCommand<SkyPlaneObject, int,  &SkyPlaneObject::mXsegments> XsegmentsCmd;
        typedef ObjectMemberPropertyCommand<SkyPlaneObject, int,  &SkyPlaneObject::mYsegments> YsegmentsCmd;
    }

    //////////////////////////////////////////////////////////////////////////
    // SkyPlaneObject class
    //////////////////////////////////////////////////////////////////////////
 
    SkyPlaneObject::SkyPlaneObject(void)
        : Object()
        , mSceneManager(NULL)
        ,mMaterialName("BaseWhite") 
        ,mScale(1000)
        ,mTiling(10)
        ,mBow(0)
        ,mXsegments(1)
        ,mYsegments(1)
        ,mDrawFirst(true)
    {
        if (createPropertyDictionary("SkyPlane"))
        {
            static MaterialNameCmd materialNameCmd;
            defineProperty(
                "material",
                "the name of material",
                "Material",
                &materialNameCmd,
                PF_PRIMARY);
            
            static DrawFirstCmd drawFirstCmd;
            defineProperty(
                "draw first",
                "whether the box is drawn before all other geometry in the scene.",
                "Bool",
                &drawFirstCmd,
                0);

            static BowCmd bowCmd;
            defineProperty(
                "bow",
                "bow of the plane.",
                "Real",
                &bowCmd,
                0);

            static ScaleCmd scaleCmd;
            defineProperty(
                "scale",
                "scale of the plane.",
                "Real",
                &scaleCmd,
                0);

            static TilingCmd tilingCmd;
            defineProperty(
                "tiling",
                "How many times to tile the texture(s) across the plane.",
                "Real",
                &tilingCmd,
                0);

            static PlaneCmd planeCmd;
            defineProperty(
                "plane",
                "the plane.",
                "Plane",
                &planeCmd,
                0);

            static XsegmentsCmd xsegmentsCmd;
            defineProperty(
                "x segments",
                "the number of xsegments the plane will have to it",
                "Integer",
                &xsegmentsCmd,
                0);

            static YsegmentsCmd ysegmentsCmd;
            defineProperty(
                "y segments",
                "the number of ysegments the plane will have to it",
                "Integer",
                &ysegmentsCmd,
                0);
        }
    }
    //-----------------------------------------------------------------------
    SkyPlaneObject::~SkyPlaneObject()
    {
        destroyRenderInstance();
    }
    //-----------------------------------------------------------------------
    const String& SkyPlaneObject::getType(void) const
    {
        return msType;
    }
    //-----------------------------------------------------------------------
    const String& SkyPlaneObject::getCategory(void) const
    {
        return msCategory;
    }
    //-----------------------------------------------------------------------
    void SkyPlaneObject::createRenderInstance(System* system)
    {
        assert(system);
        if (system->isEditable())
        {
            mSceneManager = system->getSceneManager();

            updateRenderInstanceImpl();
        }
    }
    //-----------------------------------------------------------------------
    void SkyPlaneObject::destroyRenderInstance(void)
    {
        if (mSceneManager)
        {
            mSceneManager->setSkyPlane(false, mPlane, mMaterialName);
            mSceneManager = NULL;
        }
    }
    //-----------------------------------------------------------------------
    void SkyPlaneObject::updateRenderInstanceImpl(void)
    {
        assert(mSceneManager);
        mSceneManager->setSkyPlane(true, mPlane, mMaterialName,
            mScale, mTiling, mDrawFirst, mBow, mXsegments, mYsegments);
    }
    //-----------------------------------------------------------------------
    void SkyPlaneObject::updateRenderInstance(void)
    {
        if (mSceneManager)
        {
            updateRenderInstanceImpl();
        }
    }
    //-----------------------------------------------------------------------
    ObjectFactory* SkyPlaneObject::getFactory(void)
    {
        static Factory factory;
        return &factory;
    }

}
