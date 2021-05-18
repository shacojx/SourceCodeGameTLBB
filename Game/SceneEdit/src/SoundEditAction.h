#ifndef __SoundEditAction_H__
#define __SoundEditAction_H__

#include "Action.h"

#include <OgreVector3.h>

namespace Ogre
{
    class SceneNode;
    class Entity;
}

namespace WX
{
    class SceneManipulator;

    class SoundEditAction : public Action
    {
    public:

        SoundEditAction(SceneManipulator* manipulator);
        ~SoundEditAction();

        const String& getName(void) const;

        void setParameter(const String& name, const String& value);

    protected:

        void _onActive(bool active);

        void _onMove(const Point& pt);
        void _onBegin(const Point& pt){}
        void _onDrag(const Point& pt){}
        void _onEnd(const Point& pt, bool canceled);

        void _createIndicatorInstance(void);
        void _destroyIndicatorInstance(void);

        void _createSoundEntity(int gridX, int gridZ);

        void _clearAllSoundEntities(void);

        void _deleteSoundEntity(size_t index);

    protected:

        Ogre::Entity* mSoundMovingEntity;
        Ogre::SceneNode* mSoundNode;

        Ogre::Vector3 mCreatePos;

        typedef std::vector<Ogre::Entity*> SoundEntities;
        SoundEntities mSoundEntities;

        bool mFirstInit;

        int mNextNameIndex;

        bool mShowSoundEntity;

        Ogre::Entity* mSoundRadiusEntity;
    };
}

#endif