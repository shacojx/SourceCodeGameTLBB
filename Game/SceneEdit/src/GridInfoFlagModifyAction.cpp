#include "GridInfoFlagModifyAction.h"
#include "HitIndicator.h"
#include "SceneManipulator.h"
#include "TerrainSelections.h"
#include "GridInfoFlagModifyOperator.h"
#include "WXOperatorManager.h"

#include <OgreVector3.h>

#include "Core/TerrainData.h"

namespace WX {

    //////////////////////////////////////////////////////////////////////////
    
    GridInfoFlagModifyAction::GridInfoFlagModifyAction(SceneManipulator* sceneManipulator) :
    mModifiedFlag(0),
    mHitIndicatorName(""),
    Action(sceneManipulator)
    {

    }

    GridInfoFlagModifyAction::~GridInfoFlagModifyAction()
    {
    }

    void GridInfoFlagModifyAction::_onMove(const Point& pt)
    {
        getSceneManipulator()->getHitIndicator("JunctionPoints")->setHitPoint(pt);
        getSceneManipulator()->getHitIndicator(mHitIndicatorName)->setHitPoint(pt);
    }

    void GridInfoFlagModifyAction::_onBegin(const Point& pt)
    {
        getSceneManipulator()->getHitIndicator("JunctionPoints")->setHitPoint(pt);
        getSceneManipulator()->getHitIndicator(mHitIndicatorName)->setHitPoint(pt);

        _modifyFlag(pt);
    }

    void GridInfoFlagModifyAction::_onEnd(const Point& pt, bool canceled)
    {     
        _doFinish(static_cast<GridSelection*>(getSceneManipulator()->_getSelection("GridSelection")), canceled);
        getSceneManipulator()->getHitIndicator(mHitIndicatorName)->setHitPoint(pt);
    }

    void GridInfoFlagModifyAction::_modifyFlag(const Point& pt)
    {
        GridSelection* selection = static_cast<GridSelection*>(
            getSceneManipulator()->_getSelection("GridSelection"));

        Ogre::Vector3 position;
        bool intersected = getSceneManipulator()->getTerrainIntersects(pt.x, pt.y, position);

        if (intersected)
            getSceneManipulator()->_buildSelection(selection, position.x, position.z);

        const GridSelection::GridMap& grids = selection->getGrids();

        GridSelection::GridMap::const_iterator it;
        for (it = grids.begin(); it != grids.end(); ++it)
        {
            const GridSelection::Grid& grid = it->second;
            TerrainData::GridInfo info = grid.info;
            info.flags ^= mModifiedFlag;

            getTerrainData()->setGridInfo(grid.x, grid.z, info);
        }

        selection->notifyModified();    
    }

    void GridInfoFlagModifyAction::_doFinish(GridSelection* origin, bool canceled)
    {
        assert(origin);

        if (canceled)
        {
            origin->apply();
        }
        else
        {
            if (!origin->empty())
            {
                std::auto_ptr<FlipTerrainDiagonalOperator> op(new FlipTerrainDiagonalOperator(getSceneManipulator()));

                const GridSelection::GridMap& grids = origin->getGrids();
                GridSelection::GridMap::const_iterator it;
                for (it = grids.begin(); it != grids.end(); ++it)
                {
                    const GridSelection::Grid& grid = it->second;
                    uint newFlipFlag = getTerrainData()->getGridInfo(grid.x, grid.z).flags;
                    uint oldFlipFlag = grid.info.flags;
                    if (newFlipFlag != oldFlipFlag)
                    {
                        op->add(grid.x, grid.z, oldFlipFlag, newFlipFlag);
                    }
                }

                if (!op->empty())
                    getOperatorManager()->addOperator(op.release());
            }
        }
    }

    //////////////////////////////////////////////////////////////////////////
    FlipDiagonalAction::FlipDiagonalAction(SceneManipulator* sceneManipulator) : 
    GridInfoFlagModifyAction(sceneManipulator)
    {
        mModifiedFlag = TerrainData::GridInfo::FlipDiagonal;
        mHitIndicatorName = "FlipDiagonalGrids";
    }

    const String & FlipDiagonalAction::getName() const 
    {
        static const String name = "FlipDiagonalAction";
        return name;
    }

    //////////////////////////////////////////////////////////////////////////
    DisableDecalAction::DisableDecalAction(SceneManipulator* sceneManipulator) : 
    GridInfoFlagModifyAction(sceneManipulator)
    {
        mModifiedFlag = TerrainData::GridInfo::DisableDecal;
        mHitIndicatorName = "DisableDecalGrids";
    }

    const String & DisableDecalAction::getName() const 
    {
        static const String name = "DisableDecalAction";
        return name;
    }
}