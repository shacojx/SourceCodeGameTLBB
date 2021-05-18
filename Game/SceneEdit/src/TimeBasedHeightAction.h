#ifndef __TimeBasedHeightAction_H__
#define __TimeBasedHeightAction_H__

#include "HeightAction.h"
#include "TerrainSelections.h"

namespace WX {

    class JunctionSelection;

    class TimeBasedHeightAction : public HeightAction
    {
    public:
        TimeBasedHeightAction(SceneManipulator* sceneManipulator);
        ~TimeBasedHeightAction();

    protected:
        JunctionSelection* mModifiedJunctions;

        void _onMove(const Point& pt);
        void _onBegin(const Point& pt);
        void _onDrag(const Point& pt);
        void _onUpdate(const Point& pt, Real seconds);
        void _onEnd(const Point& pt, bool canceled);

        virtual void _prepareUpdate(const JunctionSelection& selection, Real seconds);
        virtual Real _computeHeight(const JunctionSelection::Junction& junction, Real seconds) = 0;
    };

}

#endif // __TimeBasedHeightAction_H__
