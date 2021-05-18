#include "SmoothHeightAction.h"
#include "SceneManipulator.h"

namespace WX {

    //////////////////////////////////////////////////////////////////////////

    SmoothHeightAction::SmoothHeightAction(SceneManipulator* sceneManipulator)
        : TimeBasedHeightAction(sceneManipulator)
    {
    }

    const String& SmoothHeightAction::getName(void) const
    {
        static const String name = "SmoothHeightAction";
        return name;
    }

    void SmoothHeightAction::_onBegin(const Point& pt)
    {
        TimeBasedHeightAction::_onBegin(pt);

        computeAverageHeight();
    }

    void SmoothHeightAction::_onDrag(const Point& pt)
    {
        TimeBasedHeightAction::_onDrag(pt);

        computeAverageHeight();
    }

    void SmoothHeightAction::computeAverageHeight(void)
    {
        Real totalHeight = 0;
        Real totalWeight = 0;
        JunctionSelection* selection = static_cast<JunctionSelection*>(
            getSceneManipulator()->_getSelection("JunctionSelection"));
        const JunctionSelection::JunctionMap& junctions = selection->getJunctions();
        JunctionSelection::JunctionMap::const_iterator it;
        for (it = junctions.begin(); it != junctions.end(); ++it)
        {
            const JunctionSelection::Junction& junction = it->second;
            totalWeight += junction.weight;
            totalHeight += getTerrainData()->getHeight(junction.x, junction.z) * junction.weight;
        }
        mAverageHeight = totalWeight ? totalHeight / totalWeight : 0;
    }

    Real SmoothHeightAction::_computeHeight(const JunctionSelection::Junction& junction, Real seconds)
    {
        Real height = getTerrainData()->getHeight(junction.x, junction.z);
        Real diff = mAverageHeight - height;
        Real secondsRequest = Ogre::Math::Abs(diff * junction.weight / getSceneManipulator()->_getHeightAdjustSpeed());
        if (secondsRequest < seconds)
            return mAverageHeight;
        else
            return height + diff * seconds / secondsRequest;
    }

}
