#include "SetTerrainHeightAction.h"
#include "SceneManipulator.h"
#include "HitIndicator.h"
#include "TerrainSelections.h"

#include <OgreStringConverter.h>

namespace WX {

    //////////////////////////////////////////////////////////////////////////

    SetTerrainHeightAction::SetTerrainHeightAction(SceneManipulator* sceneManipulator)
        : HeightAction(sceneManipulator), mTerrainHeight(1000), mTerrainHeightIncrement(0)
    {
        mModifiedJunctions = new JunctionSelection(getSceneManipulator()->getTerrain(),sceneManipulator);
    }

    SetTerrainHeightAction::~SetTerrainHeightAction()
    {
        if (mModifiedJunctions)
        {
            delete mModifiedJunctions;
            mModifiedJunctions = 0;
        }
    }

    const String& SetTerrainHeightAction::getName(void) const
    {
        static const String name = "SetTerrainHeightAction";
        return name;
    }

    void SetTerrainHeightAction::_onMove(const Point& pt)
    {
        getSceneManipulator()->getHitIndicator("JunctionPoints")->setHitPoint(pt);
        getSceneManipulator()->getHitIndicator("IntersectPoint")->setHitPoint(pt);
    }

    void SetTerrainHeightAction::_onBegin(const Point& pt)
    {
        mModifiedJunctions->reset();
		mTerrainHeight += mTerrainHeightIncrement;
        _doModify(pt);
    }

    void SetTerrainHeightAction::_onDrag(const Point& pt)
    {
        _doModify(pt);
    }

    void SetTerrainHeightAction::_onEnd(const Point& pt, bool canceled)
    {
        doFinish(mModifiedJunctions, canceled);
        mModifiedJunctions->reset();
        getSceneManipulator()->getHitIndicator("JunctionPoints")->setHitPoint(pt);
        getSceneManipulator()->getHitIndicator("IntersectPoint")->setHitPoint(pt);
    }

    void SetTerrainHeightAction::_doModify(const Point& pt)
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
                Real newHeight = mTerrainHeight;
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

    void SetTerrainHeightAction::setParameter(const String& name, const String& value)
    {
        if (name == "%TerrainHeight")
        {
            mTerrainHeight = Ogre::StringConverter::parseInt(value);
        } 
		else if (name == "%TerrainHeightIncrement")
		{
			mTerrainHeightIncrement = Ogre::StringConverter::parseInt(value);
		}
        else
        {
            Action::setParameter(name, value);
        }
    }

    String SetTerrainHeightAction::getParameter(const String& name) const
    {
        if (name == "%TerrainHeight")
        {
            return Ogre::StringConverter::toString(mTerrainHeight);
        } 
		else if (name == "%TerrainHeightIncrement")
		{
			return Ogre::StringConverter::toString(mTerrainHeightIncrement);
		}
        
        return Action::getParameter(name);        
    }
}
