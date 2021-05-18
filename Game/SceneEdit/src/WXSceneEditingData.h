/********************************************************************
filename:   WXSceneEditingData.h

purpose:    保存场景编辑时需要保存的一些信息，例如reshape地形前地形的
            大小，用于下次打开同一场景时可以正确地显示reshape地形网格
            线。
            保存出去的文件的后缀名是.SED，表示scene editing data
*********************************************************************/

#ifndef _SCENEEDITINGDATA_H_
#define _SCENEEDITINGDATA_H_

#include "Core/WXPrerequisites.h"

namespace WX {
    
    /// 保存地形reshape的大小，比如说left缩小了3，right缩小了4等信息,用于地形网格线的显示
    struct TerrainReshapeSize
    {
        TerrainReshapeSize() :
        left(0), right(0), top(0), bottom(0)
        {
        }

        TerrainReshapeSize(int l, int r, int t, int b) :
        left(l), right(r), top(t), bottom(b)
        {
        }

        int left;
        int right;
        int top;
        int bottom;
    };

    class SceneEditingData
    {
    public:

        SceneEditingData();
        ~SceneEditingData();

        void saveData(const String& fileName);
        void readData(const String& fileName);

        void setTerrainReshapeSize(int left, int right, int top, int bottom)
        {
            mTerrainReshapeSize.left = left;
            mTerrainReshapeSize.right = right;
            mTerrainReshapeSize.top = top;
            mTerrainReshapeSize.bottom = bottom;

            mDataChanged = true;
        }

        const TerrainReshapeSize& getTerrainReshapeSize(void) const
        {
            return mTerrainReshapeSize;
        }

        bool isDataChanged(void)
        {
            return mDataChanged;
        }

    private:

        void saveTerrainReshapeSize(std::ofstream& of);

    private:

        TerrainReshapeSize mTerrainReshapeSize;

        /// 数据是否被改变，如果没被改变，保存场景时就不需要保存.SED文件
        bool mDataChanged;
    };
}

#endif