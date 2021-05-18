#ifndef __LowerHeightAction_H__
#define __LowerHeightAction_H__

#include "TimeBasedHeightAction.h"

namespace WX {

    class JunctionSelection;

    class LowerHeightAction : public TimeBasedHeightAction
    {
    public:
        LowerHeightAction(SceneManipulator* sceneManipulator);

        const String& getName(void) const;

    protected:
        Real mAdjust;

        void _prepareUpdate(const JunctionSelection& selection, Real seconds);
        Real _computeHeight(const JunctionSelection::Junction& junction, Real seconds);
    };

}

#endif // __LowerHeightAction_H__
