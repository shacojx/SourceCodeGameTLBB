#include "RaiseHeightAction.h"
#include "SceneManipulator.h"

namespace WX {

    //////////////////////////////////////////////////////////////////////////

    RaiseHeightAction::RaiseHeightAction(SceneManipulator* sceneManipulator)
        : TimeBasedHeightAction(sceneManipulator)
    {
    }

    const String& RaiseHeightAction::getName(void) const
    {
        static const String name = "RaiseHeightAction";
        return name;
    }

    void RaiseHeightAction::_prepareUpdate(const JunctionSelection& selection, Real seconds)
    {
        mAdjust = + getSceneManipulator()->_getHeightAdjustSpeed() * seconds;
    }

    Real RaiseHeightAction::_computeHeight(const JunctionSelection::Junction& junction, Real seconds)
    {
        return getTerrainData()->getHeight(junction.x, junction.z) + mAdjust * junction.weight;
    }

}
