#include "TimeBasedHeightAction.h"
#include "SceneManipulator.h"
#include "HitIndicator.h"
#include "TerrainSelections.h"

namespace WX {

    //////////////////////////////////////////////////////////////////////////

    TimeBasedHeightAction::TimeBasedHeightAction(SceneManipulator* sceneManipulator)
        : HeightAction(sceneManipulator)
        , mModifiedJunctions(NULL)
    {
        mModifiedJunctions = new JunctionSelection(getSceneManipulator()->getTerrain(),sceneManipulator);
    }

    TimeBasedHeightAction::~TimeBasedHeightAction()
    {
        delete mModifiedJunctions;
    }

    void TimeBasedHeightAction::_onMove(const Point& pt)
    {
        getSceneManipulator()->getHitIndicator("JunctionPoints")->setHitPoint(pt);
        getSceneManipulator()->getHitIndicator("IntersectPoint")->setHitPoint(pt);
    }

    void TimeBasedHeightAction::_onBegin(const Point& pt)
    {
        getSceneManipulator()->getHitIndicator("JunctionPoints")->setHitPoint(pt);
        getSceneManipulator()->getHitIndicator("IntersectPoint")->setHitPoint(pt);
        mModifiedJunctions->reset();
    }

    void TimeBasedHeightAction::_onDrag(const Point& pt)
    {
        getSceneManipulator()->getHitIndicator("JunctionPoints")->setHitPoint(pt);
    }

    void TimeBasedHeightAction::_onUpdate(const Point& pt, Real seconds)
    {
        JunctionSelection* selection = static_cast<JunctionSelection*>(
            getSceneManipulator()->_getSelection("JunctionSelection"));
        _prepareUpdate(*selection, seconds);

        const JunctionSelection::JunctionMap& junctions = selection->getJunctions();
        JunctionSelection::JunctionMap::const_iterator it;
        std::vector<TerrainInfo> terrainInfo;
        for (it = junctions.begin(); it != junctions.end(); ++it)
        {
            const JunctionSelection::Junction& junction = it->second;
            mModifiedJunctions->add(junction.x, junction.z, 1);
            Real newHeight = _computeHeight(junction, seconds);

            float oldHeight = getTerrainData()->getHeight(junction.x,junction.z);

            TerrainInfo terrInfo;
            terrInfo.x = junction.x;
            terrInfo.z = junction.z;
            terrInfo.oldHeight = junction.height;
            terrInfo.nowHeight = newHeight;
            terrainInfo.push_back(terrInfo);

            getTerrainData()->setHeight(junction.x, junction.z, newHeight);
        }   
        getSceneManipulator()->_fireTerrainHeightChanged(terrainInfo);
        selection->notifyModified();
        getSceneManipulator()->getHitIndicator("JunctionPoints")->refresh();
        getSceneManipulator()->getHitIndicator("IntersectPoint")->setHitPoint(pt);
    }

    void TimeBasedHeightAction::_onEnd(const Point& pt, bool canceled)
    {
        doFinish(mModifiedJunctions, canceled);
        mModifiedJunctions->reset();
        getSceneManipulator()->getHitIndicator("JunctionPoints")->setHitPoint(pt);
        getSceneManipulator()->getHitIndicator("IntersectPoint")->setHitPoint(pt);
    }

    void TimeBasedHeightAction::_prepareUpdate(const JunctionSelection& selection, Real seconds)
    {
    }

}
