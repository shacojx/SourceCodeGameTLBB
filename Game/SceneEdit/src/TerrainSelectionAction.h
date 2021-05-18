#ifndef __TerrainSelectionAction_H__
#define __TerrainSelectionAction_H__

#include "Action.h"
#include <OgreVector3.h>

namespace WX {
    
	class TerrainSelectionGrids;

    class TerrainSelectionAction : public Action
    {
    public:
        TerrainSelectionAction(SceneManipulator* sceneManipulator);

        ~TerrainSelectionAction();

        virtual void setParameter(const String& name, const String& value);
        virtual String getParameter(const String& name) const;
    
        const String& getName(void) const;

        void CopyHeightInfo( bool copy = true )     {   mPasteHeightInfo = copy; }
        void CopyTextureInfo( bool copy = true )    {   mPasteTextureInfo = copy; }
        void CopyDiagonalInfo( bool copy = true )   {   mPasteDiagonalInfo = copy; }
       

    protected:
        virtual void _onActive(bool active);
        virtual void _onMove(const Point& pt);
        virtual void _onBegin(const Point& pt);
        virtual void _onDrag(const Point& pt);
        virtual void _onEnd(const Point& pt, bool canceled);

        void getJunctionInfoAsString(void) const;
        void getGridTextureInfoAsString(void) const;
        void getGridDiagonalInfoAsString(void) const;

        void pasteTerrainInfo(void);

		void beginRotateTerrain();

    protected:

        /// 选择地形时的指示器
        TerrainSelectionGrids *mTerrainSelectionGrids;
        /// 当前的选择模式（单选或复选）
        Ogre::String mCurrentMode;

        /// 三个字符串，用于保存当前选择地形的信息
        mutable Ogre::String mJunctionHeightInfoString;
        mutable Ogre::String mGridTextureInfoString;
        mutable Ogre::String mGridDiagonalInfoString;

        /// 当前要拷贝时的鼠标位置
        Ogre::Vector3 mCurrentMousePos;

        /// 当前这次粘贴要粘贴哪些信息
        bool mPasteHeightInfo;
        bool mPasteTextureInfo;
        bool mPasteDiagonalInfo;

		/// 是否已经按下了ctrl+C了，也就是说是否有了拷贝信息，如果已经拷贝了，地形块就可以跟着移动
		/// 也可以对地形块进行旋转
		bool mAfterCopy;
		int mRotateCount;

		/*typedef std::vector<float> HeightArray;
		mutable HeightArray mHeightArray;
		HeightArray mRotatedHeightArray;

		mutable int mMaxJunctionX;
		mutable int mMaxJunctionZ;*/
    };
}

#endif // __TerrainSelectionAction_H__