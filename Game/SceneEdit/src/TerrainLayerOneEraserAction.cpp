#include "TerrainLayerOneEraserAction.h"
#include "SceneManipulator.h"
#include "HitIndicator.h"
#include "TerrainSelections.h"

namespace WX{

    TerrainLayerOneEraserAction::TerrainLayerOneEraserAction(SceneManipulator* sceneManipulator)
        : PaintAction(sceneManipulator)
    {
        mCurrentGrids = new GridSelection(getTerrain(),sceneManipulator);
        mHintModified = new GridSelection(getTerrain(),sceneManipulator);
        mModifiedGrids = new GridSelection(getTerrain(),sceneManipulator);
    }
    //-----------------------------------------------------------------------
    TerrainLayerOneEraserAction::~TerrainLayerOneEraserAction()
    {
        delete mCurrentGrids;
        delete mHintModified;
        delete mModifiedGrids;
    }
    //-----------------------------------------------------------------------
    const String& TerrainLayerOneEraserAction::getName(void) const
    {
        static const String name = "TerrainLayerOneEraserAction";
        return name;
    }
    //-----------------------------------------------------------------------
    void TerrainLayerOneEraserAction::_buildHitIndicator(const Point& pt)
    {
        getSceneManipulator()->getHitIndicator("IntersectGrids")->setHitPoint(pt);
        getSceneManipulator()->getHitIndicator("IntersectPoint")->setHitPoint(pt);

        Ogre::Vector3 position;
        bool intersected = getSceneManipulator()->getTerrainIntersects(pt, position);
        if (!intersected)
        {
            mCurrentGrids->reset();
            return;
        }

        getSceneManipulator()->_buildSelection(mCurrentGrids, position.x, position.z);
    }
    //-----------------------------------------------------------------------
    void TerrainLayerOneEraserAction::_onActive(bool active)
    {
        if (!active)
        {
            mHintModified->apply();
            mHintModified->reset();
        }
    }
    //-----------------------------------------------------------------------
    void TerrainLayerOneEraserAction::_onMove(const Point& pt)
    {
        mHintModified->apply();
        mHintModified->reset();
        _buildHitIndicator(pt);
        _doErase(mHintModified);
    }
    //-----------------------------------------------------------------------
    void TerrainLayerOneEraserAction::_onBegin(const Point& pt)
    {
        mHintModified->apply();
        mHintModified->reset();
        mModifiedGrids->reset();
        _buildHitIndicator(pt);
        _doErase(mModifiedGrids);
    }
    //-----------------------------------------------------------------------
    void TerrainLayerOneEraserAction::_onDrag(const Point& pt)
    {
        _buildHitIndicator(pt);
        _doErase(mModifiedGrids);
    }
    //-----------------------------------------------------------------------
    void TerrainLayerOneEraserAction::_onEnd(const Point& pt, bool canceled)
    {
        /*if (canceled)
        {
            mModifiedGrids->apply();
        }*/
        doFinish(mModifiedGrids, canceled);
        mModifiedGrids->reset();
        _buildHitIndicator(pt);
    }
    //-----------------------------------------------------------------------
    void TerrainLayerOneEraserAction::_doErase(GridSelection* modified)
    {
        const GridSelection::GridMap& grids = mCurrentGrids->getGrids();

        bool anyModified = false;

        if (!grids.empty())
        {            
            for (GridSelection::GridMap::const_iterator it = grids.begin(); it != grids.end(); ++it)
            {
                const GridSelection::Grid& grid = it->second;
                TerrainData::GridInfo info = grid.info;

                if ( info.layers[1].pixmapId )
                {
                    info.layers[1].pixmapId = 0;
                    info.layers[1].orientation = 0;
                }

                if (info == grid.info)
                    continue;

                anyModified = true;
                modified->add(grid.x, grid.z);
                getTerrainData()->setGridInfo(grid.x, grid.z, info);
            }

            if (anyModified)
            {
                mCurrentGrids->notifyModified();
            }
        }
    }
}