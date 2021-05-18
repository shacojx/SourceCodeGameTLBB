#ifndef __RaiseHeightAction_H__
#define __RaiseHeightAction_H__

#include "TimeBasedHeightAction.h"

namespace WX {

    class JunctionSelection;

    class RaiseHeightAction : public TimeBasedHeightAction
    {
    public:
        RaiseHeightAction(SceneManipulator* sceneManipulator);

        const String& getName(void) const;

    protected:
        Real mAdjust;

        void _prepareUpdate(const JunctionSelection& selection, Real seconds);
        Real _computeHeight(const JunctionSelection::Junction& junction, Real seconds);
    };

}

#endif // __RaiseHeightAction_H__
