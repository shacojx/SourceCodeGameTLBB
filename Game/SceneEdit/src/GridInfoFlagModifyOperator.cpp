#include "GridInfoFlagModifyOperator.h"
#include "SceneManipulator.h"
#include "HitIndicator.h"

#include "Core/Terrain.h"

namespace WX {

    GridInfoFlagModifyOperator::GridInfoFlagModifyOperator(SceneManipulator* sceneManipulator)
        : mSceneManipulator(sceneManipulator), mInfos(),
          mIndicatorName(""), mGroupName("")
    {
        assert(sceneManipulator);
    }
    //-----------------------------------------------------------------------
    GridInfoFlagModifyOperator::~GridInfoFlagModifyOperator()
    {
    }
    //-----------------------------------------------------------------------
    void GridInfoFlagModifyOperator::add(int x, int z, uint oldFlipFlag, uint newFlipFlag)
    {
        assert(mSceneManipulator->getTerrainData()->isValidGrid(x, z));

        if (mInfos.empty())
        {
            mMinX = mMaxX = x;
            mMinZ = mMaxZ = z;
        }
        else
        {
            if (mMinX > x)
                mMinX = x;
            else if (mMaxX < x)
                mMaxX = x;

            if (mMinZ > z)
                mMinZ = z;
            else if (mMaxZ < z)
                mMaxZ = z;
        }

        Info info;
        info.x = x;
        info.z = z;
        info.oldFlipFlag = oldFlipFlag;
        info.newFlipFlag = newFlipFlag;

        mInfos.push_back(info);
    }
    //-----------------------------------------------------------------------
    bool GridInfoFlagModifyOperator::empty(void) const
    {
        return mInfos.empty();
    }
    //-----------------------------------------------------------------------
    const String& GridInfoFlagModifyOperator::getGroupName(void) const
    {
        static const String groupName = mGroupName;
        return groupName;
    }
    //-----------------------------------------------------------------------
    String GridInfoFlagModifyOperator::getDescription(void) const
    {
        // TODO:
        return "";
    }
    //-----------------------------------------------------------------------
    String GridInfoFlagModifyOperator::getHelp(void) const
    {
        // TODO
        return "";
    }
    //-----------------------------------------------------------------------
    void GridInfoFlagModifyOperator::undo(void)
    {
        apply(&Info::oldFlipFlag);
    }
    //-----------------------------------------------------------------------
    void GridInfoFlagModifyOperator::redo(void)
    {
        apply(&Info::newFlipFlag);
    }

    //-----------------------------------------------------------------------
    void GridInfoFlagModifyOperator::apply(uint Info::* flipFlag)
    {
        if (!mInfos.empty())
        {
            for (InfoList::const_iterator i = mInfos.begin(); i != mInfos.end(); ++i)
            {
                const Info& info = *i;
                assert(mSceneManipulator->getTerrainData()->isValidGrid(info.x, info.z));

                TerrainData::GridInfo gridinfo = mSceneManipulator->getTerrainData()->getGridInfo(info.x, info.z);
                gridinfo.flags = info.*flipFlag;

                mSceneManipulator->getTerrainData()->setGridInfo(info.x, info.z, gridinfo);
            }
            mSceneManipulator->getTerrain()->notifyGridInfoModified(mMinX, mMinZ, mMaxX+1, mMaxZ+1);
        }
        mSceneManipulator->getHitIndicator(mIndicatorName)->refresh();
    }

    //////////////////////////////////////////////////////////////////////////
    FlipTerrainDiagonalOperator::FlipTerrainDiagonalOperator(SceneManipulator* sceneManipulator) :
    GridInfoFlagModifyOperator(sceneManipulator)
    {
        mIndicatorName = "FlipDiagonalGrids";
        mGroupName = "FlipDiagonal";
    }

    //////////////////////////////////////////////////////////////////////////
    DisableDecalOperator::DisableDecalOperator(SceneManipulator* sceneManipulator) :
    GridInfoFlagModifyOperator(sceneManipulator)
    {
        mIndicatorName = "DisableDecalGrids";
        mGroupName = "DisableDecal";
    }
}