#ifndef __GridInfoFlagModifyAction_H__
#define __GridInfoFlagModifyAction_H__

#include "Action.h"

namespace WX {

    class GridSelection;

    class GridInfoFlagModifyAction : public Action
    {
    public:
        GridInfoFlagModifyAction(SceneManipulator* sceneManipulator);

        ~GridInfoFlagModifyAction();

    protected:

        void _onMove(const Point& pt);
        void _onBegin(const Point& pt);
        void _onEnd(const Point& pt, bool canceled);

        void _modifyFlag(const Point& pt);
        void _doFinish(GridSelection* origin, bool canceled);

    protected:

        unsigned int mModifiedFlag;
        Ogre::String mHitIndicatorName;
    };

    //////////////////////////////////////////////////////////////////////////
    class FlipDiagonalAction : public GridInfoFlagModifyAction
    {
    public:
        FlipDiagonalAction(SceneManipulator* sceneManipulator);

        const String& getName(void) const;
    };

    //////////////////////////////////////////////////////////////////////////
    class DisableDecalAction : public GridInfoFlagModifyAction
    {
    public:
        DisableDecalAction(SceneManipulator* sceneManipulator);

        const String& getName(void) const;
    };
}

#endif // __GridInfoFlagModifyAction_H__