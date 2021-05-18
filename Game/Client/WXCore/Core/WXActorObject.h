#ifndef __FAIRYACTOROBJECT_H__
#define __FAIRYACTOROBJECT_H__

#include "WXPlacementObject.h"

namespace WX
{
    class Actor;
    class ObjectFactory;
    class ObjectProxy;
	class System;

    class ActorObject : public PlacementObject
    {
    protected:
        class Factory;

    public:
        ActorObject(void);
        virtual ~ActorObject();

        const String& getType(void) const;
        const String& getCategory(void) const;

        void createRenderInstance(System* system);
        void destroyRenderInstance(void);

        Variant getProperty(const String& name) const;
        String getPropertyAsString(const String& name) const;

        //////////////////////////////////////////////////////////////////////////

        static const String msType;
        static const String msCategory;

        static ObjectFactory* getFactory(void);

        //////////////////////////////////////////////////////////////////////////

        Actor* getActor(void) const
        {
            return mActor;
        }

        const String& getActorName(void) const
        {
            return mActorName;
        }

        void setActorName(const String& actorName);

    protected:
        ObjectProxy* mProxy;
        Actor* mActor;
        Ogre::String mActorName;
		System *mSystem;

        void createActor(void);
        void destroyActor(void);
    };

}

#endif 
