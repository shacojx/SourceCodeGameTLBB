#include "LowerHeightAction.h"
#include "SceneManipulator.h"

namespace WX {

    //////////////////////////////////////////////////////////////////////////

    LowerHeightAction::LowerHeightAction(SceneManipulator* sceneManipulator)
        : TimeBasedHeightAction(sceneManipulator)
    {
    }

    const String& LowerHeightAction::getName(void) const
    {
        static const String name = "LowerHeightAction";
        return name;
    }

    void LowerHeightAction::_prepareUpdate(const JunctionSelection& selection, Real seconds)
    {
        mAdjust = - getSceneManipulator()->_getHeightAdjustSpeed() * seconds;
    }

    Real LowerHeightAction::_computeHeight(const JunctionSelection::Junction& junction, Real seconds)
    {
        return getTerrainData()->getHeight(junction.x, junction.z) + mAdjust * junction.weight;
    }

}
