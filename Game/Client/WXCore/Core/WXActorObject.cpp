#include "WXActorObject.h"
#include "WXObjectFactory.h"
#include "WXObjectProxy.h"
#include "WXActor.h"
#include "WXSystem.h"

#include <OgreSceneManager.h>
#include <OgreStringConverter.h>

namespace WX
{
    const String ActorObject::msType = "Actor";
    const String ActorObject::msCategory = "Actor";

    //////////////////////////////////////////////////////////////////////////
    // Factory
    //////////////////////////////////////////////////////////////////////////

    class ActorObject::Factory : public ObjectFactory
    {
    public:
        const String& getType(void) const
        {
            return msType;
        }

        Object* createInstance(void)
        {
            return new ActorObject;
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Commands
    //////////////////////////////////////////////////////////////////////////

    namespace
    {
        struct ActorNameCmd : PropertyCommand
        {
            Variant doGet(const void* source) const
            {
                const ActorObject* object = static_cast<const ActorObject*>(source);
                return object->getActorName();
            }

            String doGetAsString(const void* source) const
            {
                const ActorObject* object = static_cast<const ActorObject*>(source);
                return object->getActorName();
            }

            void doSet(void* target, const Variant& value)
            {
                ActorObject* object = static_cast<ActorObject*>(target);
                String actorName = VariantCast<String>(value);
                object->setActorName(actorName);
            }

            void doSetAsString(void* target, const String& value)
            {
                ActorObject* object = static_cast<ActorObject*>(target);
                object->setActorName(value);
            }
        };

    }

    //////////////////////////////////////////////////////////////////////////
    // ActorObject class
    //////////////////////////////////////////////////////////////////////////


    ActorObject::ActorObject(void)
        : PlacementObject()
        , mProxy(NULL)
        , mActor(NULL)
        , mActorName()
		, mSystem(NULL)
    {
        if (createPropertyDictionary("Actor"))
        {
            static ActorNameCmd actorNameCmd;
            defineProperty(
                "actor name",
                "The actor filename use to create this actor.",
                "ActorName",
                &actorNameCmd,
                PF_PRIMARY);

            addBaseProperty();
        }
    }
    //-----------------------------------------------------------------------
    ActorObject::~ActorObject()
    {
        destroyRenderInstance();
    }
    //-----------------------------------------------------------------------
    const String& ActorObject::getType(void) const
    {
        return msType;
    }
    //-----------------------------------------------------------------------
    const String& ActorObject::getCategory(void) const
    {
        return msCategory;
    }
    //-----------------------------------------------------------------------
    void ActorObject::createRenderInstance(System* system)
    {
        assert(system);
        assert(!mActor);
        assert(!mProxy);

		mSystem = system;

        PlacementObject::createRenderInstance(system);

        mProxy = new ObjectProxy(shared_from_this());

        if (!mActorName.empty())
        {
            createActor();
        }
    }
    //-----------------------------------------------------------------------
    void ActorObject::destroyRenderInstance(void)
    {
        if (mActor)
        {
            destroyActor();
        }

        if (mProxy)
        {
            delete mProxy;
            mProxy = NULL;
        }

        PlacementObject::destroyRenderInstance();
    }
    //-----------------------------------------------------------------------
    void ActorObject::setActorName(const String& actorName)
    {
        if (mActorName != actorName)
        {
            if (mActor)
            {
                destroyActor();
            }

            mActorName = actorName;

            if (mProxy && !mActorName.empty())
            {
				// i assume we only call this function in FairyEditor, so I set the parameter "editable" to false
                createActor();
            }
        }
    }
    //-----------------------------------------------------------------------
    void ActorObject::createActor(void)
    {
        assert(!mActor);
        assert(mProxy && !mActorName.empty());

        Ogre::SceneNode* parent = getSceneNode();
        assert(parent);
        Ogre::SceneManager* creator = parent->getCreator();
        assert(creator);

        mActor = new Actor(mSystem, parent->getName(), mActorName, parent);
        mActor->setUserObject(mProxy);
    }
    //-----------------------------------------------------------------------
    void ActorObject::destroyActor(void)
    {
        assert(mActor);

        mActor->setUserObject(NULL);
        delete mActor;
        mActor = NULL;
    }
    //-----------------------------------------------------------------------
    Variant ActorObject::getProperty(const String& name) const
    {
        if (name == "bounding box" && mActor)
        {
            return mActor->getBoundingBox();
        }

        return PlacementObject::getProperty(name);
    }
    //-----------------------------------------------------------------------
    String ActorObject::getPropertyAsString(const String& name) const
    {
        if (name == "bounding box" && mActor)
        {
            const Ogre::AxisAlignedBox& aabb = mActor->getBoundingBox();
            return Ogre::StringConverter::toString(aabb.getMinimum()) + ", " + 
                   Ogre::StringConverter::toString(aabb.getMaximum());
        }

        return PlacementObject::getPropertyAsString(name);
    }
    //-----------------------------------------------------------------------
    ObjectFactory* ActorObject::getFactory(void)
    {
        static Factory factory;
        return &factory;
    }

}
