#include "LevelOffHeightAction.h"
#include "SceneManipulator.h"
#include "HitIndicator.h"
#include "TerrainSelections.h"

namespace WX {

    //////////////////////////////////////////////////////////////////////////

    LevelOffHeightAction::LevelOffHeightAction(SceneManipulator* sceneManipulator)
        : HeightAction(sceneManipulator)
        , mModifiedJunctions(NULL)
    {
        mModifiedJunctions = new JunctionSelection(getSceneManipulator()->getTerrain(),sceneManipulator);
    }

    LevelOffHeightAction::~LevelOffHeightAction()
    {
        delete mModifiedJunctions;
    }

    const String& LevelOffHeightAction::getName(void) const
    {
        static const String name = "LevelOffHeightAction";
        return name;
    }

    void LevelOffHeightAction::_doModify(const Point& pt)
    {
        Ogre::Vector3 position;
        bool intersected = getSceneManipulator()->getTerrainIntersects(pt, position);
        if (intersected)
        {
            bool anyModified = false;

            JunctionSelection* selection = static_cast<JunctionSelection*>(
                getSceneManipulator()->_getSelection("JunctionSelection"));
            getSceneManipulator()->_buildSelection(selection, position.x, position.z);

            const JunctionSelection::JunctionMap& junctions = selection->getJunctions();
            JunctionSelection::JunctionMap::const_iterator it;
            std::vector<TerrainInfo> terrainInfo;
            for (it = junctions.begin(); it != junctions.end(); ++it)
            {
                const JunctionSelection::Junction& junction = it->second;
                Real newHeight = position.y;
                if (newHeight != junction.height)
                {
                    anyModified = true;
                    mModifiedJunctions->add(junction.x, junction.z, 1);

                    TerrainInfo terrInfo;
                    terrInfo.x = junction.x;
                    terrInfo.z = junction.z;
                    terrInfo.oldHeight = junction.height;
                    terrInfo.nowHeight = newHeight;
                    terrainInfo.push_back(terrInfo);

                    getTerrainData()->setHeight(junction.x, junction.z, newHeight);
                }
            }

            getSceneManipulator()->_fireTerrainHeightChanged(terrainInfo);

            if (anyModified)
            {
                selection->notifyModified();
            }

            getSceneManipulator()->getHitIndicator("JunctionPoints")->refresh();
        }
        else
        {
            getSceneManipulator()->getHitIndicator("JunctionPoints")->setHitPoint(pt);
        }

        getSceneManipulator()->getHitIndicator("IntersectPoint")->setHitPoint(pt);
    }

    void LevelOffHeightAction::_onMove(const Point& pt)
    {
        getSceneManipulator()->getHitIndicator("JunctionPoints")->setHitPoint(pt);
        getSceneManipulator()->getHitIndicator("IntersectPoint")->setHitPoint(pt);
    }

    void LevelOffHeightAction::_onBegin(const Point& pt)
    {
        mModifiedJunctions->reset();
        _doModify(pt);
    }

    void LevelOffHeightAction::_onDrag(const Point& pt)
    {
        _doModify(pt);
    }

    void LevelOffHeightAction::_onEnd(const Point& pt, bool canceled)
    {
        doFinish(mModifiedJunctions, canceled);
        mModifiedJunctions->reset();
        getSceneManipulator()->getHitIndicator("JunctionPoints")->setHitPoint(pt);
        getSceneManipulator()->getHitIndicator("IntersectPoint")->setHitPoint(pt);
    }

}
