#ifndef __LevelOffHeightAction_H__
#define __LevelOffHeightAction_H__

#include "HeightAction.h"
#include "TerrainSelections.h"

namespace WX {

    class JunctionSelection;

    class LevelOffHeightAction : public HeightAction
    {
    public:
        LevelOffHeightAction(SceneManipulator* sceneManipulator);
        ~LevelOffHeightAction();

        const String& getName(void) const;

    protected:
        JunctionSelection* mModifiedJunctions;

        void _onMove(const Point& pt);
        void _onBegin(const Point& pt);
        void _onDrag(const Point& pt);
        void _onEnd(const Point& pt, bool canceled);

        virtual void _doModify(const Point& pt);
    };

}

#endif // __LevelOffHeightAction_H__
