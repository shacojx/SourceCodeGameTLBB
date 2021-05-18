#include "DragHeightAction.h"
#include "SceneManipulator.h"
#include "HitIndicator.h"
#include "TerrainSelections.h"

namespace WX {

    //////////////////////////////////////////////////////////////////////////

    DragHeightAction::DragHeightAction(SceneManipulator* sceneManipulator)
        : HeightAction(sceneManipulator)
    {
    }

    const String& DragHeightAction::getName(void) const
    {
        static const String name = "DragHeightAction";
        return name;
    }

    void DragHeightAction::_onMove(const Point& pt)
    {
        getSceneManipulator()->getHitIndicator("JunctionPoints")->setHitPoint(pt);
        getSceneManipulator()->getHitIndicator("IntersectPoint")->setHitPoint(pt);
    }

    void DragHeightAction::_onBegin(const Point& pt)
    {
        getSceneManipulator()->makeTerrainHitInfo(mTerrainHitInfo, pt);
        getSceneManipulator()->getHitIndicator("JunctionPoints")->setHitPoint(pt);
        getSceneManipulator()->getHitIndicator("IntersectPoint")->setHitPoint(pt);
    }

    void DragHeightAction::_onDrag(const Point& pt)
    {
        if (!mTerrainHitInfo.hitted)
            return;

        Ogre::Ray ray = getSceneManipulator()->getWindowToViewportRay(mDragOrigin.x, pt.y);
        Real s = ray.getDirection().x*ray.getDirection().x + ray.getDirection().z*ray.getDirection().z;
        if (!s)
            return;

        Ogre::Vector3 v = mTerrainHitInfo.hitPosition - mTerrainHitInfo.position;
        Real adjust = Ogre::Math::Sqrt((v.x*v.x + v.z*v.z) / s) * ray.getDirection().y - v.y;

        JunctionSelection* selection = static_cast<JunctionSelection*>(
            getSceneManipulator()->_getSelection("JunctionSelection"));
        const JunctionSelection::JunctionMap& junctions = selection->getJunctions();
        JunctionSelection::JunctionMap::const_iterator it;
        std::vector<TerrainInfo> terrainInfo;
        for (it = junctions.begin(); it != junctions.end(); ++it)
        {
            const JunctionSelection::Junction& junction = it->second;
            Real newHeight = junction.height + adjust * junction.weight;

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

    void DragHeightAction::_onEnd(const Point& pt, bool canceled)
    {
        doFinish(static_cast<JunctionSelection*>(getSceneManipulator()->_getSelection("JunctionSelection")), canceled);
        getSceneManipulator()->getHitIndicator("JunctionPoints")->setHitPoint(pt);
        getSceneManipulator()->getHitIndicator("IntersectPoint")->setHitPoint(pt);
    }

}
