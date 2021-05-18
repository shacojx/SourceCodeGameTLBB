#include "WXSkyDomeObject.h"
#include "WXObjectFactory.h"
#include "WXPropertyHelper.h"
#include "WXSystem.h"

#include <OgreSceneManager.h>
#include <OgreStringConverter.h>

namespace WX
{
    const String SkyDomeObject::msType = "SkyDome";
    const String SkyDomeObject::msCategory = "Enviroment";

    //////////////////////////////////////////////////////////////////////////
    // Factory
    //////////////////////////////////////////////////////////////////////////

    class SkyDomeObject::Factory : public ObjectFactory
    {
    public:
        const String& getType(void) const
        {
            return msType;
        }

        Object* createInstance(void)
        {
            return new SkyDomeObject;
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
                const SkyDomeObject* object = static_cast<const SkyDomeObject*>(source);
                return object->mMaterialName;
            }
            String doGetAsString(const void* source) const
            {
                const SkyDomeObject* object = static_cast<const SkyDomeObject*>(source);
                return object->mMaterialName;
            }
            void doSet(void* target, const Variant& value)
            {
                SkyDomeObject* object = static_cast<SkyDomeObject*>(target);
                object->mMaterialName = VariantCast<String>(value);
                object->updateRenderInstance();
            }
            void doSetAsString(void* target, const String& value)
            {
                SkyDomeObject* object = static_cast<SkyDomeObject*>(target);
                object->mMaterialName = value;
                object->updateRenderInstance();
            }
        };

        struct OrientationCmd : PropertyCommand
        {
            Variant doGet(const void* source) const
            {
                const SkyDomeObject* object = static_cast<const SkyDomeObject*>(source);
                return object->mOrientation;
            }
            String doGetAsString(const void* source) const
            {
                const SkyDomeObject* object = static_cast<const SkyDomeObject*>(source);
                return Ogre::StringConverter::toString(object->mOrientation);
            }
            void doSet(void* target, const Variant& value)
            {
                SkyDomeObject* object = static_cast<SkyDomeObject*>(target);
                object->mOrientation = VariantCast<Ogre::Quaternion>(value);
                object->updateRenderInstance();
            }
            void doSetAsString(void* target, const String& value)
            {
                SkyDomeObject* object = static_cast<SkyDomeObject*>(target);
                object->mOrientation = Ogre::StringConverter::parseQuaternion(value);
                object->updateRenderInstance();
            }
        };

        typedef ObjectMemberPropertyCommand<SkyDomeObject, bool, &SkyDomeObject::mDrawFirst> DrawFirstCmd;
        typedef ObjectMemberPropertyCommand<SkyDomeObject, Real, &SkyDomeObject::mDistance> DistanceCmd;
        typedef ObjectMemberPropertyCommand<SkyDomeObject, Real, &SkyDomeObject::mCurvature> CurvatureCmd;
        typedef ObjectMemberPropertyCommand<SkyDomeObject, Real, &SkyDomeObject::mTiling> TilingCmd;
        typedef ObjectMemberPropertyCommand<SkyDomeObject, int,  &SkyDomeObject::mXsegments> XsegmentsCmd;
        typedef ObjectMemberPropertyCommand<SkyDomeObject, int,  &SkyDomeObject::mYsegments> YsegmentsCmd;
        typedef ObjectMemberPropertyCommand<SkyDomeObject, int,  &SkyDomeObject::mYsegments_keep> Ysegments_keepCmd;
    }

    //////////////////////////////////////////////////////////////////////////
    // SkyDomeObject class
    //////////////////////////////////////////////////////////////////////////
    
    SkyDomeObject::SkyDomeObject(void)
        : Object()
        , mSceneManager(NULL)
        , mMaterialName("BaseWhite") 
        ,mCurvature(10)
        ,mTiling(8)
        ,mDistance(4000)
        ,mDrawFirst(true)
        ,mOrientation(Ogre::Quaternion::IDENTITY)
        ,mXsegments(16)
        ,mYsegments(16)
        ,mYsegments_keep(-1)
    {
        if (createPropertyDictionary("SkyDome"))
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
                "the distance of SkyDome.",
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
                "Optional parameter to specify the orientation of the box.",
                "Orientation",
                &orientationCmd,
                0);

            static CurvatureCmd curvatureCmd;
            defineProperty(
                "curvature",
                "The curvature of the dome.",
                "Real",
                &curvatureCmd,
                0);
            
            static TilingCmd tilingCmd;
            defineProperty(
                "tiling",
                "How many times to tile the texture(s) across the dome.",
                "Real",
                &tilingCmd,
                0);

            static XsegmentsCmd xsegmentsCmd;
            defineProperty(
                "x segments",
                "the number of xsegments the dome will have to it",
                "Integer",
                &xsegmentsCmd,
                0);

            static YsegmentsCmd ysegmentsCmd;
            defineProperty(
                "y segments",
                "the number of ysegments the dome will have to it",
                "Integer",
                &ysegmentsCmd,
                0);

            static Ysegments_keepCmd ysegments_keepCmd;
            defineProperty(
                "y segments keep",
                "the keep of ysegments",
                "Integer",
                &ysegments_keepCmd,
                0);

        }
    }
    //-----------------------------------------------------------------------
    SkyDomeObject::~SkyDomeObject()
    {
        destroyRenderInstance();
    }
    //-----------------------------------------------------------------------
    const String& SkyDomeObject::getType(void) const
    {
        return msType;
    }
    //-----------------------------------------------------------------------
    const String& SkyDomeObject::getCategory(void) const
    {
        return msCategory;
    }
    //-----------------------------------------------------------------------
    void SkyDomeObject::createRenderInstance(System* system)
    {
        assert(system);
        if (system->isEditable())
        {
            mSceneManager = system->getSceneManager();

            updateRenderInstanceImpl();
        }
    }
    //-----------------------------------------------------------------------
    void SkyDomeObject::destroyRenderInstance(void)
    {
        if (mSceneManager)
        {
            mSceneManager->setSkyDome(false, mMaterialName);
        }
    }
    //-----------------------------------------------------------------------
    void SkyDomeObject::updateRenderInstanceImpl(void)
    {
        assert(mSceneManager);
        mSceneManager->setSkyDome(true, mMaterialName,
            mCurvature, mTiling, mDistance, mDrawFirst,
            mOrientation, mXsegments, mYsegments, mYsegments_keep);
    }
    //-----------------------------------------------------------------------
    void SkyDomeObject::updateRenderInstance(void)
    {
        if (mSceneManager)
        {
            updateRenderInstanceImpl();
        }
    }
    //-----------------------------------------------------------------------
    ObjectFactory* SkyDomeObject::getFactory(void)
    {
        static Factory factory;
        return &factory;
    }

}
