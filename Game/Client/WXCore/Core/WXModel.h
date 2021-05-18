#ifndef __FAIRYACTOR_H__
#define __FAIRYACTOR_H__

#include "WXPrerequisites.h"
#include <OgreAxisAlignedBox.h>

// ----------------------------------------------------------------------------
// classes
// ----------------------------------------------------------------------------

namespace WX {

    class ModelSerializer;

    class Model
    {
        friend ModelSerializer;
    protected:
        typedef std::list<Ogre::SceneNode*> SceneNodeList;
        typedef std::list<Ogre::MovableObject*> ObjectList;
        typedef std::list<Ogre::AnimationState*> AnimationStateList;

        Ogre::SceneManager* mCreator;
        Ogre::SceneNode* mParent;
        Ogre::UserDefinedObject* mUserObject;
        SceneNodeList mSceneNodes;
        ObjectList mMasterObjects;
        ObjectList mMovables;
        AnimationStateList mAnimationStates;
        Ogre::AxisAlignedBox mBoundingBox;
        bool mCastShadows;
        AnimationType mAnimationType;
        Ogre::Controller<Real>* mController;

    public:
        Model(Ogre::SceneNode* parent, const String& resourceName, const String& resourceGroup);
        virtual ~Model();

        void setUserObject(Ogre::UserDefinedObject* userObject);
        Ogre::UserDefinedObject* getUserObject(void) const;

        /// Gets the bounding box
        const Ogre::AxisAlignedBox& getBoundingBox(void) const;

        void setCastShadows(bool castShadows);

        bool getCastShadows(void) const
        {
            return mCastShadows;
        }

        void setAnimationType(AnimationType animationType);

        AnimationType getAnimationType(void) const
        {
            return mAnimationType;
        }

        /// Modifies the time position
        void addTime(Real offset);

        bool existsObject(Ogre::MovableObject* object) const;

    protected:
        void create(const String& resourceName, const String& resourceGroup);
        void cleanup();

        void updateCastShadows(void);
        void updateAnimationType(void);
        void enableAnimationStates(void);
        void disableAnimationStates(void);
        void createController(void);
        void destroyController(void);

        static void addTime(AnimationStateList& animationStates, Real offset);
    };

}

#endif // 
