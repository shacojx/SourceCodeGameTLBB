#ifndef __SmoothHeightAction_H__
#define __SmoothHeightAction_H__

#include "TimeBasedHeightAction.h"

namespace WX {

    class JunctionSelection;

    class SmoothHeightAction : public TimeBasedHeightAction
    {
    public:
        SmoothHeightAction(SceneManipulator* sceneManipulator);

        const String& getName(void) const;

    protected:
        Real mAverageHeight;

        void computeAverageHeight(void);

        void _onBegin(const Point& pt);
        void _onDrag(const Point& pt);
        Real _computeHeight(const JunctionSelection::Junction& junction, Real seconds);
    };

}

#endif // __SmoothHeightAction_H__
