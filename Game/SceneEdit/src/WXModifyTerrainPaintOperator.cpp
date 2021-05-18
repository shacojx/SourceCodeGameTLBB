#include "WXModifyTerrainPaintOperator.h"
#include "SceneManipulator.h"

#include "Core/Terrain.h"

#include <OgreTextureManager.h>

namespace WX
{    
    ModifyTerrainPaintOperator::ModifyTerrainPaintOperator(SceneManipulator* sceneManipulator)
        : mSceneManipulator(sceneManipulator)
        , mInfos()
    {
        assert(sceneManipulator);
    }
    //-----------------------------------------------------------------------
    ModifyTerrainPaintOperator::~ModifyTerrainPaintOperator()
    {
    }
    //-----------------------------------------------------------------------
    void ModifyTerrainPaintOperator::add(int x, int z, const TerrainData::GridInfo &oldGridInfo, const TerrainData::GridInfo &newGridInfo)
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

        // 得到当前gridinfo中的纹理名称

        Info info;
        info.x = x;
        info.z = z;
        fillUndoRedoInfo(oldGridInfo, info.oldGridInfo);
        fillUndoRedoInfo(newGridInfo, info.newGridInfo);

        mInfos.push_back(info);
    }
    //-----------------------------------------------------------------------
    bool ModifyTerrainPaintOperator::empty(void) const
    {
        return mInfos.empty();
    }
    //-----------------------------------------------------------------------
    const String& ModifyTerrainPaintOperator::getGroupName(void) const
    {
        static const String groupName = "TerrainPaint";
        return groupName;
    }
    //-----------------------------------------------------------------------
    String ModifyTerrainPaintOperator::getDescription(void) const
    {
        // TODO:
        return "";
    }
    //-----------------------------------------------------------------------
    String ModifyTerrainPaintOperator::getHelp(void) const
    {
        // TODO
        return "";
    }
    //-----------------------------------------------------------------------
    void ModifyTerrainPaintOperator::undo(void)
    {
        apply(&Info::oldGridInfo);
    }
    //-----------------------------------------------------------------------
    void ModifyTerrainPaintOperator::redo(void)
    {
        apply(&Info::newGridInfo);
    }
    //-----------------------------------------------------------------------
    void ModifyTerrainPaintOperator::fillUndoRedoInfo(const TerrainData::GridInfo &gridInfo, UndoRedoGridInfo &fillInfo)
    {
        // 保存redo undo所需的信息
        TerrainData *pTerrainData = mSceneManipulator->getTerrainData();

        /** 原来是只保存当前格子的纹理的id，现在要保存
        1 纹理名称
        2 纹理的上下左右坐标点
        3 纹理的方向
        */
        // 保存第一层纹理的信息
        int id = gridInfo.layers[0].pixmapId;

        if ( id == 0 )
        {
            fillInfo.mPaintInfo[0].mTexName.clear();
            fillInfo.mPaintInfo[0].left = 0.0f;
            fillInfo.mPaintInfo[0].right = 1.0f;
            fillInfo.mPaintInfo[0].top = 0.0f;
            fillInfo.mPaintInfo[0].bottom = 1.0f;
            fillInfo.mPaintInfo[0].orientation = 0;
        }
        else
        {
            fillInfo.mPaintInfo[0].mTexName = pTerrainData->mTextures[ pTerrainData->mPixmaps[id-1].textureId ];
            fillInfo.mPaintInfo[0].left = pTerrainData->mPixmaps[id-1].left;
            fillInfo.mPaintInfo[0].right = pTerrainData->mPixmaps[id-1].right;
            fillInfo.mPaintInfo[0].top = pTerrainData->mPixmaps[id-1].top;
            fillInfo.mPaintInfo[0].bottom = pTerrainData->mPixmaps[id-1].bottom;
            fillInfo.mPaintInfo[0].orientation = gridInfo.layers[0].orientation;
        }
        // 保存第二层纹理的信息（如果第二层有纹理的话）
        id = gridInfo.layers[1].pixmapId;
        if ( id > 0 )
        {
            fillInfo.mPaintInfo[1].mTexName = pTerrainData->mTextures[ pTerrainData->mPixmaps[id-1].textureId ];
            fillInfo.mPaintInfo[1].left = pTerrainData->mPixmaps[id-1].left;
            fillInfo.mPaintInfo[1].right = pTerrainData->mPixmaps[id-1].right;
            fillInfo.mPaintInfo[1].top = pTerrainData->mPixmaps[id-1].top;
            fillInfo.mPaintInfo[1].bottom = pTerrainData->mPixmaps[id-1].bottom;
            fillInfo.mPaintInfo[1].orientation = gridInfo.layers[1].orientation;
        }

    }
    //-----------------------------------------------------------------------
    void ModifyTerrainPaintOperator::apply(UndoRedoGridInfo Info::* undoRedo)
    {
        if (!mInfos.empty())
        {
            for (InfoList::const_iterator i = mInfos.begin(); i != mInfos.end(); ++i)
            {
                const Info& info = *i;
                assert(mSceneManipulator->getTerrainData()->isValidGrid(info.x, info.z));

                // 取出当前要进行undo 或 redo的格子的信息，对其中进行部分修改（当前格子的纹理，纹理的方向）
                const TerrainData::GridInfo &nowInfo = mSceneManipulator->getTerrainData()->getGridInfo(info.x, info.z);
                TerrainData::GridInfo newInfo = nowInfo;

                // 根据指针取出结构中的内容
                const UndoRedoGridInfo &undoRedoInfo = info.*undoRedo;

                // 如果第一层的纹理名称为空，说明还没贴纹理
                if (undoRedoInfo.mPaintInfo[0].mTexName.empty())
                {
                    newInfo.layers[0].pixmapId = 0;
                }
                else
                {
                    int layerOnePixmapId = mSceneManipulator->getTerrainData()->_registerPixmap(
                        undoRedoInfo.mPaintInfo[0].mTexName,
                        undoRedoInfo.mPaintInfo[0].left,
                        undoRedoInfo.mPaintInfo[0].top,
                        undoRedoInfo.mPaintInfo[0].right,
                        undoRedoInfo.mPaintInfo[0].bottom);

                    newInfo.layers[0].pixmapId = layerOnePixmapId;    
                }  

                newInfo.layers[0].orientation = undoRedoInfo.mPaintInfo[0].orientation;

                if (undoRedoInfo.mPaintInfo[1].mTexName.empty())
                {
                    newInfo.layers[1].pixmapId = 0;
                }
                else
                {
                    int layerTwoPixmapId = mSceneManipulator->getTerrainData()->_registerPixmap(
                        undoRedoInfo.mPaintInfo[1].mTexName,
                        undoRedoInfo.mPaintInfo[1].left,
                        undoRedoInfo.mPaintInfo[1].top,
                        undoRedoInfo.mPaintInfo[1].right,
                        undoRedoInfo.mPaintInfo[1].bottom);

                    newInfo.layers[1].pixmapId = layerTwoPixmapId;    
                }

                newInfo.layers[1].orientation = undoRedoInfo.mPaintInfo[1].orientation;

                mSceneManipulator->getTerrainData()->setGridInfo(info.x, info.z, newInfo);
            }

            mSceneManipulator->getTerrain()->notifyGridInfoModified(mMinX, mMinZ, mMaxX+1, mMaxZ+1);
        }
    }
}
