#ifndef __FAIRYSTATICENTITYOBJECT_H__
#define __FAIRYSTATICENTITYOBJECT_H__

#include "WXPlacementObject.h"

namespace WX
{
    class ObjectFactory;
    class ObjectProxy;

    class EntityTransparentor;

    class StaticEntityObject : public PlacementObject
    {
    protected:
        class Factory;

    public:
        StaticEntityObject(void);
        virtual ~StaticEntityObject();

        const String& getType(void) const;
        const String& getCategory(void) const;

        void queryReferenceResources(ResourceCollection* resourceCollection);

        void queryBakableEntities(EntityList& entities);
        void bakeStaticGeometry(Ogre::StaticGeometry* staticGeometry, const EntityList& entities);

        void createRenderInstance(System* system);
        void destroyRenderInstance(void);

        void processMessage(const String& msg, const Variant& params);

        bool setProperty(const String& name, const Variant& value);
        bool setPropertyAsString(const String& name, const String& value);
        Variant getProperty(const String& name) const;
        String getPropertyAsString(const String& name) const;

        //////////////////////////////////////////////////////////////////////////

        static const String msType;
        static const String msCategory;

        static ObjectFactory* getFactory(void);

        //////////////////////////////////////////////////////////////////////////

        Ogre::Entity* getEntity(void) const
        {
            return mEntity;
        }

        const String& getMeshName(void) const
        {
            return mMeshName;
        }

        void setMeshName(const String& meshName);

        Real getAnimationLevel(void) const
        {
            return mAnimationLevel;
        }

        void setAnimationLevel(Real level);

        void setTransparency(Real transparency);

        Real getTransparency(void) const
        {
            return mTransparency;
        }

        bool getCastShadows(void) const
        {
            return mCastShadows;
        }

        void setCastShadows(bool castShadows);

        bool getReceiveShadows(void) const
        {
            return mReceiveShadows;
        }

        void setReceiveShadows(bool receiveShadows);

        bool getReceiveDecals(void) const
        {
            return mReceiveDecals;
        }

        void setReceiveDecals(bool receiveDecals);

    protected:
        System* mSystem;
        ObjectProxy* mProxy;
        Ogre::Entity* mEntity;
        Ogre::String mMeshName;
        Ogre::Real mAnimationLevel;
        Ogre::Real mTransparency;
        bool mCastShadows;
        bool mReceiveShadows;
        bool mReceiveDecals;

        EntityTransparentor* mEntityTransparentor;
        AnimationType mAnimationType;

        void createEntity(void);
        void destroyEntity(void);
        void updateAnimationType(void);
        void updateTransparency(void);
        void updateNormaliseNormals(void);
    };

}

#endif 
