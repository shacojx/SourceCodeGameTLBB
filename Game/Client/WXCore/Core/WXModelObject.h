#ifndef __FAIRYMODELOBJECT_H__
#define __FAIRYMODELOBJECT_H__

#include "WXPlacementObject.h"

namespace WX
{
    class Model;
    class ObjectFactory;
    class ObjectProxy;

    class ModelObject : public PlacementObject
    {
    protected:
        class Factory;

    public:
        ModelObject(void);
        virtual ~ModelObject();

        const String& getType(void) const;
        const String& getCategory(void) const;

        void createRenderInstance(System* system);
        void destroyRenderInstance(void);

        void processMessage(const String& msg, const Variant& params);

        Variant getProperty(const String& name) const;
        String getPropertyAsString(const String& name) const;

        //////////////////////////////////////////////////////////////////////////

        static const String msType;
        static const String msCategory;

        static ObjectFactory* getFactory(void);

        //////////////////////////////////////////////////////////////////////////

        Model* getModel(void) const
        {
            return mModel;
        }

        const String& getModelName(void) const
        {
            return mModelName;
        }

        void setModelName(const String& actorName);

        Real getAnimationLevel(void) const
        {
            return mAnimationLevel;
        }

        void setAnimationLevel(Real level);

        bool getCastShadows(void) const
        {
            return mCastShadows;
        }

        void setCastShadows(bool castShadows);

    protected:
        ObjectProxy* mProxy;
        Model* mModel;
        Ogre::String mModelName;
        Real mAnimationLevel;
        bool mCastShadows;
        System* mSystem;

        void createModel(void);
        void destroyModel(void);
    };

}

#endif 
