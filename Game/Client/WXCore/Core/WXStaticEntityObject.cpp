#include "WXStaticEntityObject.h"
#include "WXObjectFactory.h"
#include "WXObjectProxy.h"
#include "WXResources.h"
#include "WXResourceCollection.h"
#include "WXUtils.h"
#include "WXFlags.h"
#include "WXTransparentor.h"
#include "WXSystem.h"

#include "OgreExt/OgreAutoAnimationEntity.h"

#include <OgreSceneManager.h>
#include <OgreEntity.h>
#include <OgreStringConverter.h>
#include <OgreStaticGeometry.h>

namespace WX
{
    const String StaticEntityObject::msType = "StaticEntity";
    const String StaticEntityObject::msCategory = "StaticObject";

    //////////////////////////////////////////////////////////////////////////
    // Factory
    //////////////////////////////////////////////////////////////////////////

    class StaticEntityObject::Factory : public ObjectFactory
    {
    public:
        const String& getType(void) const
        {
            return msType;
        }

        Object* createInstance(void)
        {
            return new StaticEntityObject;
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Commands
    //////////////////////////////////////////////////////////////////////////

    namespace
    {
        struct MeshNameCmd : PropertyCommand
        {
            Variant doGet(const void* source) const
            {
                const StaticEntityObject* object = static_cast<const StaticEntityObject*>(source);
                return object->getMeshName();
            }

            String doGetAsString(const void* source) const
            {
                const StaticEntityObject* object = static_cast<const StaticEntityObject*>(source);
                return object->getMeshName();
            }

            void doSet(void* target, const Variant& value)
            {
                StaticEntityObject* object = static_cast<StaticEntityObject*>(target);
                String meshName = VariantCast<String>(value);
                object->setMeshName(meshName);
            }

            void doSetAsString(void* target, const String& value)
            {
                StaticEntityObject* object = static_cast<StaticEntityObject*>(target);
                object->setMeshName(value);
            }
        };

        struct AnimationLevelCmd : PropertyCommand
        {
            Variant doGet(const void* source) const
            {
                const StaticEntityObject* object = static_cast<const StaticEntityObject*>(source);
                return object->getAnimationLevel();
            }

            String doGetAsString(const void* source) const
            {
                const StaticEntityObject* object = static_cast<const StaticEntityObject*>(source);
                return Ogre::StringConverter::toString(object->getAnimationLevel());
            }

            void doSet(void* target, const Variant& value)
            {
                StaticEntityObject* object = static_cast<StaticEntityObject*>(target);
                Real animationLevel = VariantCast<Real>(value);
                object->setAnimationLevel(animationLevel);
            }

            void doSetAsString(void* target, const String& value)
            {
                StaticEntityObject* object = static_cast<StaticEntityObject*>(target);
                Real animationLevel = Ogre::StringConverter::parseReal(value);
                object->setAnimationLevel(animationLevel);
            }
        };

        struct TransparencyCmd : PropertyCommand
        {
            Variant doGet(const void* source) const
            {
                const StaticEntityObject* object = static_cast<const StaticEntityObject*>(source);
                return object->getTransparency();
            }

            String doGetAsString(const void* source) const
            {
                const StaticEntityObject* object = static_cast<const StaticEntityObject*>(source);
                return Ogre::StringConverter::toString(object->getTransparency());
            }

            void doSet(void* target, const Variant& value)
            {
                StaticEntityObject* object = static_cast<StaticEntityObject*>(target);
                Real transparency = VariantCast<Real>(value);
                object->setTransparency(transparency);
            }

            void doSetAsString(void* target, const String& value)
            {
                StaticEntityObject* object = static_cast<StaticEntityObject*>(target);
                Real transparency = Ogre::StringConverter::parseReal(value);
                object->setTransparency(transparency);
            }
        };

        struct CastShadowsCmd : PropertyCommand
        {
            Variant doGet(const void* source) const
            {
                const StaticEntityObject* object = static_cast<const StaticEntityObject*>(source);
                return object->getCastShadows();
            }

            String doGetAsString(const void* source) const
            {
                const StaticEntityObject* object = static_cast<const StaticEntityObject*>(source);
                return Ogre::StringConverter::toString(object->getCastShadows());
            }

            void doSet(void* target, const Variant& value)
            {
                StaticEntityObject* object = static_cast<StaticEntityObject*>(target);
                bool castShadows = VariantCast<bool>(value);
                object->setCastShadows(castShadows);
            }

            void doSetAsString(void* target, const String& value)
            {
                StaticEntityObject* object = static_cast<StaticEntityObject*>(target);
                bool castShadows = Ogre::StringConverter::parseBool(value);
                object->setCastShadows(castShadows);
            }
        };

        struct ReceiveShadowsCmd : PropertyCommand
        {
            Variant doGet(const void* source) const
            {
                const StaticEntityObject* object = static_cast<const StaticEntityObject*>(source);
                return object->getReceiveShadows();
            }

            String doGetAsString(const void* source) const
            {
                const StaticEntityObject* object = static_cast<const StaticEntityObject*>(source);
                return Ogre::StringConverter::toString(object->getReceiveShadows());
            }

            void doSet(void* target, const Variant& value)
            {
                StaticEntityObject* object = static_cast<StaticEntityObject*>(target);
                bool castShadows = VariantCast<bool>(value);
                object->setReceiveShadows(castShadows);
            }

            void doSetAsString(void* target, const String& value)
            {
                StaticEntityObject* object = static_cast<StaticEntityObject*>(target);
                bool castShadows = Ogre::StringConverter::parseBool(value);
                object->setReceiveShadows(castShadows);
            }
        };

        struct ReceiveDecalsCmd : PropertyCommand
        {
            Variant doGet(const void* source) const
            {
                const StaticEntityObject* object = static_cast<const StaticEntityObject*>(source);
                return object->getReceiveDecals();
            }

            String doGetAsString(const void* source) const
            {
                const StaticEntityObject* object = static_cast<const StaticEntityObject*>(source);
                return Ogre::StringConverter::toString(object->getReceiveDecals());
            }

            void doSet(void* target, const Variant& value)
            {
                StaticEntityObject* object = static_cast<StaticEntityObject*>(target);
                bool receiveDecals = VariantCast<bool>(value);
                object->setReceiveDecals(receiveDecals);
            }

            void doSetAsString(void* target, const String& value)
            {
                StaticEntityObject* object = static_cast<StaticEntityObject*>(target);
                bool receiveDecals = Ogre::StringConverter::parseBool(value);
                object->setReceiveDecals(receiveDecals);
            }
        };
    }

    //////////////////////////////////////////////////////////////////////////
    // StaticEntityObject class
    //////////////////////////////////////////////////////////////////////////


    StaticEntityObject::StaticEntityObject(void)
        : PlacementObject()
        , mSystem(NULL)
        , mProxy(NULL)
        , mEntity(NULL)
        , mMeshName()
        , mAnimationLevel(50)
        , mTransparency()
        , mCastShadows(false)
        , mReceiveShadows(false)
        , mReceiveDecals(false)
        , mEntityTransparentor()
        , mAnimationType(AT_DISABLE)
    {
        if (createPropertyDictionary("StaticEntity"))
        {
            static MeshNameCmd meshNameCmd;
            defineProperty(
                "mesh name",
                "The mesh filename use to create this static entity.",
                "MeshName",
                &meshNameCmd,
                PF_PRIMARY);

            addBaseProperty();

            static AnimationLevelCmd animationLevelCmd;
            defineProperty(
                "animation level",
                "The animation level of this object.",
                "Real",
                &animationLevelCmd,
                0);

            static TransparencyCmd transparencyCmd;
            defineProperty(
                "transparency",
                "Amount transparent of this object.",
                "Real",
                &transparencyCmd,
                0);

            static CastShadowsCmd castShadowsCmd;
            defineProperty(
                "cast shadows",
                "Does this object cast shadows?",
                "Bool",
                &castShadowsCmd,
                0);

            static ReceiveShadowsCmd receiveShadowsCmd;
            defineProperty(
                "receive shadows",
                "Does this object receive shadows?",
                "Bool",
                &receiveShadowsCmd,
                0);

            static ReceiveDecalsCmd receiveDecalsCmd;
            defineProperty(
                "receive decals",
                "Does this object receive decals?",
                "Bool",
                &receiveDecalsCmd,
                0);
        }
    }
    //-----------------------------------------------------------------------
    StaticEntityObject::~StaticEntityObject()
    {
        destroyRenderInstance();
    }
    //-----------------------------------------------------------------------
    const String& StaticEntityObject::getType(void) const
    {
        return msType;
    }
    //-----------------------------------------------------------------------
    const String& StaticEntityObject::getCategory(void) const
    {
        return msCategory;
    }
    //-----------------------------------------------------------------------
    void StaticEntityObject::queryReferenceResources(ResourceCollection* resourceCollection)
    {
        if (mEntity)
        {
            resourceCollection->add("Mesh", mEntity->getMesh()->getName());

            if (getAnimationStateSafely(mEntity, AUTO_ANIMATION_NAME))
            {
                resourceCollection->add("Skeleton", mEntity->getMesh()->getSkeletonName());
            }
        }
        else
        {
            resourceCollection->add("Mesh", mMeshName);
        }
    }
    //-----------------------------------------------------------------------
    void StaticEntityObject::queryBakableEntities(EntityList& entities)
    {
        assert(mEntity);

        if (!getAnimationStateSafely(mEntity, AUTO_ANIMATION_NAME))
        {
            entities.push_back(mEntity);
        }
    }
    //-----------------------------------------------------------------------
    void StaticEntityObject::bakeStaticGeometry(Ogre::StaticGeometry* staticGeometry, const EntityList& entity)
    {
        assert(mEntity);
        assert(!entity.empty());
        assert(entity.front() == mEntity);
        assert(!getAnimationStateSafely(mEntity, AUTO_ANIMATION_NAME));

        staticGeometry->addSceneNode(mEntity->getParentSceneNode());
        destroyRenderInstance();
    }
    //-----------------------------------------------------------------------
    void StaticEntityObject::createRenderInstance(System* system)
    {
        assert(system);
        assert(!mEntity);
        assert(!mProxy);

        PlacementObject::createRenderInstance(system);
        mSystem = system;

        mProxy = new ObjectProxy(shared_from_this());

        if (!mMeshName.empty())
        {
            createEntity();
        }
    }
    //-----------------------------------------------------------------------
    void StaticEntityObject::destroyRenderInstance(void)
    {
        if (mEntity)
        {
            destroyEntity();
        }

        if (mProxy)
        {
            delete mProxy;
            mProxy = NULL;
        }

        mSystem = NULL;

        PlacementObject::destroyRenderInstance();
    }
    //-----------------------------------------------------------------------
    void StaticEntityObject::setMeshName(const String& meshName)
    {
        if (mMeshName != meshName)
        {
            if (mEntity)
            {
                destroyEntity();
            }

            mMeshName = meshName;

            if (mProxy && !mMeshName.empty())
            {
                createEntity();
            }
        }
    }
    //-----------------------------------------------------------------------
    void StaticEntityObject::setAnimationLevel(Real level)
    {
        mAnimationLevel = level;
        updateAnimationType();
    }
    //-----------------------------------------------------------------------
    void StaticEntityObject::setTransparency(Real transparency)
    {
        // Clamp transparency to range [0, 1]
        if (transparency < 0)
            transparency = 0;
        else if (transparency > 1)
            transparency = 1;

        if (mTransparency != transparency)
        {
            mTransparency = transparency;

            updateTransparency();
        }
    }
    //-----------------------------------------------------------------------
    void StaticEntityObject::setCastShadows(bool castShadows)
    {
        if (mCastShadows != castShadows)
        {
            mCastShadows = castShadows;

            if (mEntity)
            {
                mEntity->setCastShadows(castShadows);
            }
        }
    }
    //-----------------------------------------------------------------------
    void StaticEntityObject::setReceiveShadows(bool receiveShadows)
    {
        if (mReceiveShadows != receiveShadows)
        {
            mReceiveShadows = receiveShadows;

            if (mEntity)
            {
                if (receiveShadows)
                {
                    mEntity->setRenderQueueGroup(Ogre::RENDER_QUEUE_WORLD_GEOMETRY_1);
                }
                else
                {
                    mEntity->setRenderQueueGroup(Ogre::RENDER_QUEUE_MAIN);
                }
            }
        }
    }
    //-----------------------------------------------------------------------
    void StaticEntityObject::setReceiveDecals(bool receiveDecals)
    {
        mReceiveDecals = receiveDecals;

        if (mEntity)
        {
            if (receiveDecals)
            {
                mEntity->addQueryFlags(OQF_DECAL_TARGET);
            }
            else
            {
                mEntity->removeQueryFlags(OQF_DECAL_TARGET);
            }
        }
    }
    //-----------------------------------------------------------------------
    void StaticEntityObject::createEntity(void)
    {
        assert(!mEntity);
        assert(mProxy && !mMeshName.empty());

        Ogre::SceneNode* parent = getSceneNode();
        assert(parent);
        Ogre::SceneManager* creator = parent->getCreator();
        assert(creator);

        // Load the mesh intelligently
        Ogre::MeshPtr mesh = loadMesh(
            mMeshName, Ogre::StringUtil::BLANK,
            Ogre::StringUtil::BLANK, DEFAULT_RESOURCE_GROUP_NAME);

        if (getAnimationSafely(mesh, AUTO_ANIMATION_NAME))
        {
            Ogre::NameValuePairList params;
            params["mesh"] = mesh->getName();
            Ogre::AutoAnimationEntity* autoAnimationEntity = static_cast<Ogre::AutoAnimationEntity*>(
                creator->createMovableObject(parent->getName(), Ogre::AutoAnimationEntityFactory::FACTORY_TYPE_NAME, &params));
            mEntity = autoAnimationEntity;
            convertEntityToHardwareSkinning(mEntity);
        }
        else
        {
            mEntity = creator->createEntity(parent->getName(), mesh->getName());
        }

        mEntity->setUserObject(mProxy);
        parent->attachObject(mEntity);
        mEntity->setCastShadows(mCastShadows);
        if (mReceiveDecals)
        {
            mEntity->addQueryFlags(OQF_DECAL_TARGET);
        }

        updateNormaliseNormals();
        updateTransparency();
        updateAnimationType();
    }
    //-----------------------------------------------------------------------
    void StaticEntityObject::destroyEntity(void)
    {
        assert(mEntity);

        if (mEntityTransparentor)
        {
            delete mEntityTransparentor;
            mEntityTransparentor = NULL;
        }

        mEntity->destroy();
        mEntity = NULL;
    }
    //-----------------------------------------------------------------------
    void StaticEntityObject::updateAnimationType(void)
    {
        if (mSystem && mEntity &&
            mEntity->getMovableType() == Ogre::AutoAnimationEntityFactory::FACTORY_TYPE_NAME)
        {
            AnimationType type = mSystem->_determineAnimationType(mAnimationLevel);
            if (mAnimationType != type)
            {
                mAnimationType = type;

                Ogre::AutoAnimationEntity* autoAnimationEntity = static_cast<Ogre::AutoAnimationEntity*>(mEntity);
                Ogre::AnimationState* as = autoAnimationEntity->getAnimationState(AUTO_ANIMATION_NAME);
                switch (mAnimationType)
                {
                case AT_DISABLE:
                    if (autoAnimationEntity->getAutoUpdateAnimationStateList().find(as) !=
                        autoAnimationEntity->getAutoUpdateAnimationStateList().end())
                    {
                        autoAnimationEntity->removeAutoUpdateAnimationState(AUTO_ANIMATION_NAME);
                    }
                    as->setEnabled(false);
                    break;

                case AT_FROZE:
                    if (autoAnimationEntity->getAutoUpdateAnimationStateList().find(as) !=
                        autoAnimationEntity->getAutoUpdateAnimationStateList().end())
                    {
                        autoAnimationEntity->removeAutoUpdateAnimationState(AUTO_ANIMATION_NAME);
                    }
                    else if (!as->getEnabled())
                    {
                        as->setTimePosition(Ogre::Math::UnitRandom() * as->getLength());
                        as->setEnabled(true);
                    }
                    break;

                case AT_FULL:
                    if (autoAnimationEntity->getAutoUpdateAnimationStateList().find(as) ==
                        autoAnimationEntity->getAutoUpdateAnimationStateList().end())
                    {
                        autoAnimationEntity->addAutoUpdateAnimationState(AUTO_ANIMATION_NAME,
                            as->getEnabled() ? as->getTimePosition() : - Ogre::Math::UnitRandom());
                    }
                    break;
                }
            }
        }
    }
    //-----------------------------------------------------------------------
    void StaticEntityObject::updateNormaliseNormals(void)
    {
        assert(mEntity);
        assert(mEntity->getParentSceneNode());

        mEntity->setNormaliseNormals(
            !mEntity->getParentSceneNode()->_getDerivedScale().positionEquals(Ogre::Vector3::UNIT_SCALE));
    }
    //-----------------------------------------------------------------------
    void StaticEntityObject::updateTransparency(void)
    {
        if (mEntityTransparentor)
        {
            mEntityTransparentor->apply(mTransparency);
            if (mTransparency <= 0)
            {
                delete mEntityTransparentor;
                mEntityTransparentor = 0;
            }
        }
        else if (mEntity && mTransparency > 0)
        {
            mEntityTransparentor = new EntityTransparentor(mEntity);
            mEntityTransparentor->apply(mTransparency);
        }
    }
    //-----------------------------------------------------------------------
    void StaticEntityObject::processMessage(const String& msg, const Variant& params)
    {
        if (msg == System::ANIMATION_QUALITY_CHANGED_MSG)
        {
            updateAnimationType();
        }

        PlacementObject::processMessage(msg, params);
    }
    //-----------------------------------------------------------------------
    bool StaticEntityObject::setProperty(const String& name, const Variant& value)
    {
        if (!PlacementObject::setProperty(name, value))
            return false;

        if (mEntity && name == "scale")
        {
            updateNormaliseNormals();
        }

        return true;
    }
    //-----------------------------------------------------------------------
    bool StaticEntityObject::setPropertyAsString(const String& name, const String& value)
    {
        if (!PlacementObject::setPropertyAsString(name, value))
            return false;

        if (mEntity && name == "scale")
        {
            updateNormaliseNormals();
        }

        return true;
    }
    //-----------------------------------------------------------------------
    Variant StaticEntityObject::getProperty(const String& name) const
    {
        if (name == "bounding box" && mEntity)
        {
            return mEntity->getBoundingBox();
        }

        return PlacementObject::getProperty(name);
    }
    //-----------------------------------------------------------------------
    String StaticEntityObject::getPropertyAsString(const String& name) const
    {
        if (name == "bounding box" && mEntity)
        {
            const Ogre::AxisAlignedBox& aabb = mEntity->getBoundingBox();
            return Ogre::StringConverter::toString(aabb.getMinimum()) + ", " + 
                   Ogre::StringConverter::toString(aabb.getMaximum());
        }

        return PlacementObject::getPropertyAsString(name);
    }
    //-----------------------------------------------------------------------
    ObjectFactory* StaticEntityObject::getFactory(void)
    {
        static Factory factory;
        return &factory;
    }

}
