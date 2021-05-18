#include "WXModifyTerrainHeightOperator.h"
#include "SceneManipulator.h"
#include "HitIndicator.h"

#include "Core/TerrainData.h"
#include "Core/Terrain.h"

namespace WX
{

    ModifyTerrainHeightOperator::ModifyTerrainHeightOperator(SceneManipulator* sceneManipulator)
        : mSceneManipulator(sceneManipulator)
        , mInfos()
    {
        assert(sceneManipulator);
    }
    //-----------------------------------------------------------------------
    ModifyTerrainHeightOperator::~ModifyTerrainHeightOperator()
    {
    }
    //-----------------------------------------------------------------------
    void ModifyTerrainHeightOperator::add(int x, int z, Real oldHeight, Real newHeight)
    {
        assert(mSceneManipulator->getTerrainData()->isValidJunction(x, z));

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
        info.oldHeight = oldHeight;
        info.newHeight = newHeight;

        mInfos.push_back(info);
    }
    //-----------------------------------------------------------------------
    bool ModifyTerrainHeightOperator::empty(void) const
    {
        return mInfos.empty();
    }
    //-----------------------------------------------------------------------
    const String& ModifyTerrainHeightOperator::getGroupName(void) const
    {
        static const String groupName = "TerrainHeight";
        return groupName;
    }
    //-----------------------------------------------------------------------
    String ModifyTerrainHeightOperator::getDescription(void) const
    {
        // TODO:
        return "";
    }
    //-----------------------------------------------------------------------
    String ModifyTerrainHeightOperator::getHelp(void) const
    {
        // TODO
        return "";
    }
    //-----------------------------------------------------------------------
    void ModifyTerrainHeightOperator::undo(void)
    {
        apply(&Info::oldHeight);
    }
    //-----------------------------------------------------------------------
    void ModifyTerrainHeightOperator::redo(void)
    {
        apply(&Info::newHeight);
    }
    //-----------------------------------------------------------------------
    void ModifyTerrainHeightOperator::apply(Real Info::* height)
    {
        if (!mInfos.empty())
        {
            std::vector<TerrainInfo> terrainInfo;
            for (InfoList::const_iterator i = mInfos.begin(); i != mInfos.end(); ++i)
            {
                const Info& info = *i;
                assert(mSceneManipulator->getTerrainData()->isValidJunction(info.x, info.z));
                
                if( (info.*height) == info.newHeight)
                {
                    TerrainInfo terrInfo;
                    terrInfo.x = info.x;
                    terrInfo.z = info.z;
                    terrInfo.oldHeight = info.oldHeight;
                    terrInfo.nowHeight = info.*height;
                    terrainInfo.push_back(terrInfo);
                }
                else
                {
                    TerrainInfo terrInfo;
                    terrInfo.x = info.x;
                    terrInfo.z = info.z;
                    terrInfo.oldHeight = info.newHeight;
                    terrInfo.nowHeight = info.*height;
                    terrainInfo.push_back(terrInfo);
                }

                mSceneManipulator->getTerrainData()->setHeight(info.x, info.z, info.*height);
            }

            mSceneManipulator->_fireTerrainHeightChanged(terrainInfo);
            mSceneManipulator->getTerrain()->notifyHeightModified(mMinX, mMinZ, mMaxX+1, mMaxZ+1);
        }

        // redo或undo后刷新指示器的位置
        mSceneManipulator->getHitIndicator("JunctionPoints")->refresh();
    }

}
