#ifndef __FAIRYMODIFYTERRAINPAINTOPERATOR_H__
#define __FAIRYMODIFYTERRAINPAINTOPERATOR_H__

#include "WXOperator.h"
#include "Core/TerrainData.h"

namespace WX
{
    class SceneManipulator;

    /**
    这个类用于记录对地形纹理进行的修改，包括第一层的橡皮擦，自动纹理拼接，简单画刷等操作（从PaintAction派生的操作），
    用于redo，undo，大部分内容参考了ModifyTerrainHeightOperator类
    */
    class ModifyTerrainPaintOperator : public Operator
    {
    public:

        /** undo redo 要保存的纹理信息
        */
        struct PaintInfo
        {
            String mTexName;
            Real left;
            Real top;
            Real right;
            Real bottom;
            uchar orientation;

            PaintInfo()
                : left(0)
                , top(0)
                , right(1)
                , bottom(1)
                , orientation(0)
            {                
            }

            PaintInfo(String textureName, Real left, Real top, Real right, Real bottom, uchar orientation)
                : mTexName(textureName)
                , left(left)
                , top(top)
                , right(right)
                , bottom(bottom)
                , orientation(orientation)
            {
            }

            explicit PaintInfo(String textureName)
                : mTexName(textureName)
                , left(0)
                , top(0)
                , right(1)
                , bottom(1)
                , orientation(0)
            {
            }

            bool operator== (const PaintInfo& rhs) const
            {
                return mTexName == rhs.mTexName &&
                    left == rhs.left && right == rhs.right &&
                    top == rhs.top && bottom == rhs.bottom && orientation == rhs.orientation;
            }

            bool operator!= (const PaintInfo& rhs) const
            {
                return !(*this == rhs);
            }
        };

        enum {
            NumLayers = 2,
        };

        struct UndoRedoGridInfo
        {
            // 每个格要保存上下两层的纹理信息
            PaintInfo mPaintInfo[NumLayers];
        };

        ModifyTerrainPaintOperator(SceneManipulator* sceneManipulator);
        ~ModifyTerrainPaintOperator();

        void add(int x, int z, const TerrainData::GridInfo &oldGridInfo, const TerrainData::GridInfo &newGridInfo);
        bool empty(void) const;

        /** @copydoc Operator::getGroupName */
        const String& getGroupName(void) const;

        /** @copydoc Operator::getDescription */
        String getDescription(void) const;

        /** @copydoc Operator::getHelp */
        String getHelp(void) const;

        /** @copydoc Operator::undo */
        void undo(void);

        /** @copydoc Operator::redo */
        void redo(void);

    protected:
        SceneManipulator* mSceneManipulator;

        // 每次进行画刷修改时要保存的信息
        struct Info
        {
            int x;
            int z;
            UndoRedoGridInfo oldGridInfo; // 当前格子中的纹理信息 
            UndoRedoGridInfo newGridInfo; // 对当前格子所要做的纹理修改
        };

        typedef std::vector<Info> InfoList;

        InfoList mInfos;
        int mMinX, mMaxX, mMinZ, mMaxZ;

        void apply(UndoRedoGridInfo Info::* undoRedo);

        void fillUndoRedoInfo(const TerrainData::GridInfo &gridInfo, UndoRedoGridInfo &fillInfo);
    };
}

#endif 
