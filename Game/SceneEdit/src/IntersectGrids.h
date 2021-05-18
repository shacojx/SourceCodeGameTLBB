#ifndef __IntersectGrids_H__
#define __IntersectGrids_H__

#include "HitIndicator.h"
#include "TerrainSelections.h"

#include <OgreSimpleRenderable.h>
#include <OgreVector2.h>
#include <OgreSceneNode.h>

#include <list>

namespace WX {
    
    class Projector;

    typedef std::vector<Ogre::Vector3> PositionArray;

    /** 保存所有的指示方块，并进行渲染
    */
    class IntersectGrid : public Ogre::SimpleRenderable
    {
    public:
        IntersectGrid();
        ~IntersectGrid();

        /// 填充顶点缓冲区
        void fillPosition( const PositionArray &posArray );

        Ogre::Real getBoundingRadius() const;

        Ogre::Real getSquaredViewDepth(const Ogre::Camera*) const;

        void setGridColour( const Ogre::ColourValue &colour )
        {
            mGridsColour = colour;
        }        

    protected:

        Ogre::HardwareVertexBufferSharedPtr vbuf;

        float mRadius;

        size_t mCurrentVertexCount;

        Ogre::ColourValue mGridsColour;

    protected:

        void _createBuffer(void);

        /// 创建方块特用的材质（半透明，不接受光照）
        Ogre::MaterialPtr _createIntersectGridMaterial(void);        
    };

    /** 方块指示器，用于填充纹理时在地形上的指示
    */
    class IntersectGrids : public HitIndicator
    {       

    protected:

        /// 用于挂接操作指示面片的SceneNode
        Ogre::SceneNode *mIntersectGridsNode;

        /// 方块renderable
        IntersectGrid *intersectGrids;

        /// 保存当前刷子范围的顶点信息
        PositionArray mPosArray;

    public:
        IntersectGrids(SceneManipulator* sceneManipulator);
        ~IntersectGrids();

        const String& getName(void) const;

        void hide(void);
        void refresh(void);

    protected:

        void _setHitPoint(Real x, Real y);
    };

    /** 翻转对角线操作的指示器
    */
    class FlipDiagonalGrids : public IntersectGrids
    {
    public:
        FlipDiagonalGrids(SceneManipulator* sceneManipulator);

        const String& getName(void) const;
    };  

    /** Disable Decal操作的指示器
    */
    class DisableDecalGrids : public IntersectGrids
    {
    public:
        DisableDecalGrids(SceneManipulator* sceneManipulator);

        ~DisableDecalGrids();

        const String& getName(void) const;

        void hide(void);

    protected:

        virtual void _setHitPoint(Real x, Real y);

        void _createProjector(void);

        void _destroyProjector(void);

        void _setProjcetorSize(void);

    protected:

        Projector* mProjector;
        Ogre::SceneNode* mProjectorNode;
    };

    /** 用于地形选择的指示器 
        与普通的指示器的差别在于，可以复选多个地形
    */
    class TerrainSelectionGrids : public IntersectGrids
    {
    public:

        TerrainSelectionGrids(SceneManipulator* sceneManipulator);
        ~TerrainSelectionGrids();

        const String& getName(void) const;

        virtual void reshape(void);

        virtual void refresh(void);

        void setCurrentGridSelection( bool multiSelecting = true );

        void addCurrentGridSelection( bool multiSelecting  = true );

        GridSelection *getTotalGridSelection(void)
        {
            return mTotalGridSelection;
        }

        JunctionSelection *getTotalJunctionSelection(void)
        {
            return mTotalJunctionSelection;
        }

        void show(void);

		int getMinX(void)
		{
			return mMinX;
		}

		int getMinZ(void)
		{
			return mMinZ;
		}

		int getMaxX(void)	{	return mMaxX;	}
		int getMaxZ(void)	{	return mMaxZ;	}

		void setSceneNodePos(const Ogre::Vector3 &pos)
		{
			mIntersectGridsNode->setPosition(pos);
		}

		void rotateSceneNode(void)
		{
			mIntersectGridsNode->rotate(Ogre::Vector3::UNIT_Y, Ogre::Degree(90.0f));
		}

		void setOriginSceneNodeOrientation(void)
		{
			mIntersectGridsNode->setOrientation(Ogre::Quaternion::IDENTITY);
		}
        
    protected:

        void _setHitPoint(Real x, Real y);

        void _clearGridSelections(void);		

    protected:

        // 每次鼠标选择地形时的鼠标原点
        Ogre::Vector2 mOriginPos;

        // 地形选择数组，用于保存同时选中的多组地形
        typedef std::list<GridSelection *> GridSelections;
        GridSelections mGridSelectionList;

        // 当前进行选择的地形块       
        GridSelection *mCurrentSelection;

        GridSelection *mTotalGridSelection;
        JunctionSelection *mTotalJunctionSelection;

		int mMinX;
		int mMinZ;

		int mMaxX;
		int mMaxZ;
    };

}

#endif // __IntersectGrids_H__
