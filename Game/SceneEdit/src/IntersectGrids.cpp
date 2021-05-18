#include "IntersectGrids.h"
#include "TerrainSelections.h"
#include "SceneManipulator.h"

#include <OgreHardwareVertexBuffer.h>
#include <OgreHardwareIndexBuffer.h>
#include <OgreHardwareBufferManager.h>
#include <OgreRoot.h>
#include <OgreCamera.h>
#include <OgreMaterialManager.h>

#include "Core/WXProjector.h"

namespace WX{

    
    IntersectGrid::IntersectGrid()
    {
        // 缓冲区初始大小为54个顶点（9个格子）
        mCurrentVertexCount = 6*9;

		mRenderOp.operationType = Ogre::RenderOperation::OT_TRIANGLE_LIST;
        mRenderOp.useIndexes = false; 

        _createBuffer();

        this->setMaterial(_createIntersectGridMaterial()->getName()); 
        this->setPolygonModeOverrideable(false);
    }

    IntersectGrid::~IntersectGrid()
    {
        if ( mRenderOp.vertexData )
        {
            delete mRenderOp.vertexData;
            mRenderOp.vertexData = 0;
        }
    }

    void 
    IntersectGrid::fillPosition( const PositionArray &posArray )
    {
        size_t posCount = posArray.size();

        if ( posCount <= 0 )
            return;

    //    if ( posCount > mCurrentVertexCount )
    //   {   
            // 如果缓冲区不够用了，就扩容一倍
        //    mCurrentVertexCount = posCount<<1;
            // 如果不每帧创建缓冲区的话，由于lock时用的是discard，所以上次缓冲区中的内容还是会被显示
            mCurrentVertexCount = posCount;
            _createBuffer();
     //   }

        Ogre::Vector3 vmax = posArray[posCount-1];
        Ogre::Vector3 vmin = posArray[0];

        vmin.y -= 100.0f;
        vmax.y += 100.0f;

        Real sqLen = std::max(vmax.squaredLength(), vmin.squaredLength());
        mRadius = Ogre::Math::Sqrt(sqLen);//

        mBox.setExtents(vmin,vmax);//

        getParentNode()->needUpdate();//

        float *vertexPos = static_cast<float*>(vbuf->lock(Ogre::HardwareBuffer::HBL_DISCARD));

        for (size_t i = 0; i < posCount; ++i)
        {
            *vertexPos++ = posArray[i].x;
            *vertexPos++ = posArray[i].y+1;
            *vertexPos++ = posArray[i].z;

            Ogre::RGBA *pCol;
            pCol = static_cast<Ogre::RGBA*>(static_cast<void*>(vertexPos));
            // 用diffuse来做为alpha来源
            Ogre::Root::getSingleton().convertColourValue(mGridsColour, pCol++ ) ;

            vertexPos = static_cast<float*>(static_cast<void*>(pCol));
        }

        vbuf->unlock();     
    }

    Real
    IntersectGrid::getBoundingRadius() const
    {
        return mRadius;
    }

    Real 
    IntersectGrid::getSquaredViewDepth(const Ogre::Camera* cam) const
    {
        Ogre::Vector3 min, max, mid, dist;

        min = mBox.getMinimum();
        max = mBox.getMaximum();
        mid = ((min - max) * 0.5) + min;
        dist = cam->getDerivedPosition() - mid;

        return dist.squaredLength();
    }

    void
    IntersectGrid::_createBuffer()
    {
        // 先把上次创建的缓冲区删掉
        if ( mRenderOp.vertexData )
        {
            delete mRenderOp.vertexData;
            mRenderOp.vertexData = 0;
        }

        if ( mCurrentVertexCount <= 0 )
            return;

        mRenderOp.vertexData = new Ogre::VertexData;

        mRenderOp.vertexData->vertexCount = mCurrentVertexCount;
        mRenderOp.vertexData->vertexStart = 0;
        Ogre::VertexDeclaration *decl = mRenderOp.vertexData->vertexDeclaration;

        size_t offset = 0;

        decl->addElement(0, offset, Ogre::VET_FLOAT3, Ogre::VES_POSITION);

        offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);
        decl->addElement(0, offset, Ogre::VET_COLOUR, Ogre::VES_DIFFUSE);
        
        vbuf = 
            Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
            decl->getVertexSize(0),
            mRenderOp.vertexData->vertexCount,
            Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);

        mRenderOp.vertexData->vertexBufferBinding->setBinding(0, vbuf);
    }

    Ogre::MaterialPtr
    IntersectGrid::_createIntersectGridMaterial(void)
    {
        String name = "IntersectGridMaterial";

        Ogre::ResourcePtr res = Ogre::MaterialManager::getSingleton().getByName(name);
        if (!res.isNull())
            return res;

        Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().create(name, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
       
        material->setLightingEnabled(false);
        material->setSceneBlending(Ogre::SBF_SOURCE_ALPHA, Ogre::SBF_ONE_MINUS_SOURCE_ALPHA);
        material->setDepthWriteEnabled(false);
        // Use wireframe polygon mode
        material->getTechnique(0)->getPass(0)->setPolygonMode(Ogre::PM_WIREFRAME);

        return material;
    }

    //////////////////////////////////////////////////////////////////////////
    // IntersectGrids                                                /////
    //////////////////////////////////////////////////////////////////////////
    IntersectGrids::IntersectGrids(SceneManipulator* sceneManipulator)
        : HitIndicator(sceneManipulator)
    {
        mIntersectGridsNode = getIndicatorRootSceneNode()->createChildSceneNode();
        intersectGrids = new IntersectGrid;
        intersectGrids->setQueryFlags(0);
        intersectGrids->setGridColour( Ogre::ColourValue(1.0, 1.0, 1.0, 0.3) );
        mIntersectGridsNode->attachObject(intersectGrids);
    }

    IntersectGrids::~IntersectGrids()
    {
		if (mIntersectGridsNode)
		{
			mIntersectGridsNode->destroy();
		}

        if ( intersectGrids )
        {
            delete intersectGrids;
            intersectGrids = NULL;
        }
    }   

    const String&
    IntersectGrids::getName(void) const
    {
        static const String name = "IntersectGrids";
        return name;
    }

    void
    IntersectGrids::hide(void)
    {     
        mIntersectGridsNode->setVisible(false);
    }

    void
    IntersectGrids::_setHitPoint(Real x, Real y)
    {
        GridSelection* selection = 
            static_cast<GridSelection *>(getSceneManipulator()->_getSelection("GridSelection"));

        Ogre::Vector3 position;
        bool intersected = getSceneManipulator()->getTerrainIntersects(x, y, position);

        mIntersectGridsNode->setVisible(intersected);

        if (!intersected)
        {
            selection->reset();
            hide();
            return;
        }
        getSceneManipulator()->_buildSelection(selection, position.x, position.z);

        refresh();
    }

    void
    IntersectGrids::refresh(void)
    {

        GridSelection* selection = static_cast<GridSelection *>(
            getSceneManipulator()->_getSelection("GridSelection"));

        const GridSelection::GridMap &mGrids = selection->getGrids();
       
        TerrainData *terrainData = getTerrainData();

        if (!mGrids.empty())
        {
            GridSelection::GridMap::const_iterator it = mGrids.begin();

            while (it != mGrids.end())
            {        
                // 取得这个grid的四个角的坐标
                int minx = it->second.x;
                int minz = it->second.z;
                int maxx = minx+1;
                int maxz = minz+1;

                // 判断是否翻转了，因为翻转了会改变顶点的高度
                if ( terrainData->getGridInfo(minx, minz).flags & TerrainData::GridInfo::FlipDiagonal )
                {
                    // 第一个三角形
                    mPosArray.push_back(terrainData->_getPosition(minx,minz)-mIntersectGridsNode->getPosition());
                    mPosArray.push_back(terrainData->_getPosition(minx,maxz)-mIntersectGridsNode->getPosition());
                    mPosArray.push_back(terrainData->_getPosition(maxx,maxz)-mIntersectGridsNode->getPosition());

                    // 第二个三角形
                    mPosArray.push_back(terrainData->_getPosition(maxx,minz)-mIntersectGridsNode->getPosition());
                    mPosArray.push_back(terrainData->_getPosition(minx,minz)-mIntersectGridsNode->getPosition());
                    mPosArray.push_back(terrainData->_getPosition(maxx,maxz)-mIntersectGridsNode->getPosition());
                }
                else
                {
                    // 第一个三角形
                    mPosArray.push_back(terrainData->_getPosition(minx,minz)-mIntersectGridsNode->getPosition());
                    mPosArray.push_back(terrainData->_getPosition(minx,maxz)-mIntersectGridsNode->getPosition());
                    mPosArray.push_back(terrainData->_getPosition(maxx,minz)-mIntersectGridsNode->getPosition());

                    // 第二个三角形
                    mPosArray.push_back(terrainData->_getPosition(maxx,minz)-mIntersectGridsNode->getPosition());
                    mPosArray.push_back(terrainData->_getPosition(minx,maxz)-mIntersectGridsNode->getPosition());
                    mPosArray.push_back(terrainData->_getPosition(maxx,maxz)-mIntersectGridsNode->getPosition());
                }                

                ++it;
            } 

            intersectGrids->fillPosition(mPosArray);

            mPosArray.clear();
        }

    }

    //////////////////////////////////////////////////////////////////////////
    // FlipDiagonalGrids                                                /////
    //////////////////////////////////////////////////////////////////////////
    FlipDiagonalGrids::FlipDiagonalGrids(SceneManipulator* sceneManipulator) :
    IntersectGrids(sceneManipulator)
    {
        intersectGrids->setGridColour( Ogre::ColourValue(1.0, 0.0, 0.0, 1.0) );
    }

    const String& FlipDiagonalGrids::getName(void) const
    {
        static const String name = "FlipDiagonalGrids";
        return name;
    }    

    //////////////////////////////////////////////////////////////////////////
    // DisableDecalGrids                                                /////
    //////////////////////////////////////////////////////////////////////////
    DisableDecalGrids::DisableDecalGrids(SceneManipulator* sceneManipulator) :
    IntersectGrids(sceneManipulator),
    mProjector(NULL), mProjectorNode(NULL)
    {
        intersectGrids->setGridColour( Ogre::ColourValue(1.0, 0.0, 0.0, 1.0) );
    }

    DisableDecalGrids::~DisableDecalGrids()
    {
        _destroyProjector();
    }

    const String& DisableDecalGrids::getName(void) const
    {
        static const String name = "DisableDecalGrids";
        return name;
    }

    void DisableDecalGrids::_setHitPoint(Real x, Real y)
    {
        GridSelection* selection = 
            static_cast<GridSelection *>(getSceneManipulator()->_getSelection("GridSelection"));

        Ogre::Vector3 position;
        bool intersected = getSceneManipulator()->getTerrainIntersects(x, y, position);

        mIntersectGridsNode->setVisible(intersected);

        if (!intersected)
        {
            selection->reset();
            hide();
            return;
        }
        getSceneManipulator()->_buildSelection(selection, position.x, position.z);

        refresh();

        // 配置projector
        _createProjector();    
        mProjectorNode->setPosition(position.x, position.y + 400, position.z);

        if (mProjectorNode)
            mProjectorNode->setVisible(intersected);

        _setProjcetorSize();
    }

    void DisableDecalGrids::_createProjector(void)
    {
        if (NULL == mProjector)
        {
            mProjectorNode = getIndicatorRootSceneNode()->createChildSceneNode();

            mProjector = static_cast<Projector*>
                ( getSceneManipulator()->getSceneManager()->
                createMovableObject(mProjectorNode->getName(), ProjectorFactory::FACTORY_TYPE_NAME) );

            mProjector->setProjectionType(Ogre::PT_ORTHOGRAPHIC);
            mProjector->setTerrain( getSceneManipulator()->getTerrain() );
            mProjector->setAspectRatio(1);
            mProjector->setShowFrustum(false);
            mProjector->setEnableClipPlanes(false);
            mProjector->setPatchMaterial("DisableDecalProjector");

            mProjectorNode->attachObject(mProjector);
            mProjectorNode->setDirection(Ogre::Vector3::NEGATIVE_UNIT_Y, mIntersectGridsNode->TS_WORLD);
        }
    }

    void DisableDecalGrids::_destroyProjector(void)
    {
        if (mProjector)
        {
            Ogre::SceneNode* parent = mProjector->getParentSceneNode();
            assert(parent);
            Ogre::SceneManager* creator = parent->getCreator();
            assert(creator);

            assert (parent == mProjectorNode);

            parent->detachObject(mProjector->getName());

            getSceneManipulator()->getSceneManager()->destroyMovableObject(mProjector);

            mProjector = NULL;

            if (mProjectorNode)
            {
                mProjectorNode->destroy();
            }
        }
    }

    void DisableDecalGrids::_setProjcetorSize(void)
    {
        assert( mProjector );

        size_t xBrushSize = getSceneManipulator()->getBrushXSize() * getSceneManipulator()->getTerrainData()->mScale.x;
        size_t zBrushSize = getSceneManipulator()->getBrushZSize() * getSceneManipulator()->getTerrainData()->mScale.z;

        Real nearClip = mProjector->getNearClipDistance();

        float projectorSize = 
            (xBrushSize > zBrushSize ? xBrushSize : zBrushSize);

        if (nearClip > 0.0f)
        {
            mProjector->setFOVy( Ogre::Math::ATan( projectorSize / (nearClip * 2) ) * 2.0f );
        }
        else
        {
            OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS,
                "the near clip distance of projector must be larger than zero.",
                "DisableDecalGrids::_setProjcetorSize");
        }

    }

    void DisableDecalGrids::hide()
    {
        IntersectGrids::hide();
        if (mProjectorNode)
            mProjectorNode->setVisible(false);
    }
    //////////////////////////////////////////////////////////////////////////
    // TerrainSelectionGrids                                             ////
    //////////////////////////////////////////////////////////////////////////
    TerrainSelectionGrids::TerrainSelectionGrids(SceneManipulator* sceneManipulator)
        : IntersectGrids(sceneManipulator), 
        mOriginPos(Ogre::Vector2::ZERO),
        mCurrentSelection(NULL), mTotalGridSelection(NULL), mTotalJunctionSelection(NULL),
		mMinX(65535), mMinZ(65535), mMaxX(0), mMaxZ(0)
    {
        mTotalGridSelection = new GridSelection(sceneManipulator->getTerrain(),sceneManipulator);
        mTotalJunctionSelection = new JunctionSelection(sceneManipulator->getTerrain(),sceneManipulator);
    }

    TerrainSelectionGrids::~TerrainSelectionGrids()
    {
        _clearGridSelections();

        if (mTotalGridSelection)
        {
            delete mTotalGridSelection;
            mTotalGridSelection = 0;
        }

        if (mTotalJunctionSelection)
        {
            delete mTotalJunctionSelection;
            mTotalJunctionSelection = 0;
        }
    }

    const String& TerrainSelectionGrids::getName(void) const
    {
        static const String name = "TerrainSelectionGrids";
        return name;
    }

    void TerrainSelectionGrids::_setHitPoint(Real x, Real y)
    {
        GridSelection* selection = mCurrentSelection;

        bool intersected = false;

        // 如果原点为0，说明正点击左键（可能是单选，也可能是复选）
        if ( mOriginPos == Ogre::Vector2::ZERO )
        {
            mOriginPos.x = x;
            mOriginPos.y = y;
        }

        // 计算出当前选择的四个屏幕坐标点
        Real left = std::min(x, mOriginPos.x);
        Real top = std::min(y, mOriginPos.y);
        Real right = std::max(x, mOriginPos.x);
        Real bottom = std::max(y, mOriginPos.y);

        Ogre::Vector3 firstPos, lastPos;

        // 计算出实际的相交点
        intersected = getSceneManipulator()->getTerrainIntersects(left, top, firstPos);
        intersected = getSceneManipulator()->getTerrainIntersects(right, bottom, lastPos);

        std::pair<int, int> minGrid = getSceneManipulator()->getTerrainData()->getGrid(firstPos.x, firstPos.z);
        std::pair<int, int> maxGrid = getSceneManipulator()->getTerrainData()->getGrid(lastPos.x, lastPos.z);

		int mini = 0;
		int maxi = 0;
		int minj = 0;
		int maxj = 0;

		if (minGrid.first > maxGrid.first)
		{
			mini = maxGrid.first;
			maxi = minGrid.first;
		}
		else
		{
			mini = minGrid.first;
			maxi = maxGrid.first;
		}

		if (minGrid.second > maxGrid.second)
		{
			minj = maxGrid.second;
			maxj = minGrid.second;
		}
		else
		{
			minj = minGrid.second;
			maxj = maxGrid.second;
		}

		/*Ogre::Vector3 sceneNodePos = getSceneManipulator()->getTerrainData()->_getPosition(mini, minj);

		mIntersectGridsNode->setPosition(sceneNodePos);*/

        // 每次都是重新填充selection，这样才能让当前这个选择的块随用户的拖拉而变化
        selection->reset();

        bool exist = false;

        for ( int i=mini; i<maxi+1; ++i )
            for ( int j=minj; j<maxj+1; ++j )
            {
                // 如果当前要添加的grid之前没被添加过才添加，避免重复选择
                for ( GridSelections::const_iterator it = mGridSelectionList.begin();
                    it != mGridSelectionList.end(); ++it )
                {
                    if ( (*it)->exist(i,j) == true )
                        exist = true;
                }

                if (exist == false)
				{
					if ( i < mMinX )
						mMinX = i;
					else if ( i > mMaxX )
						mMaxX = i;

					if ( j < mMinZ )
						mMinZ = j;
					else if ( j > mMaxZ )
						mMaxZ = j;

                    selection->add(i, j);
				}
            }

        mIntersectGridsNode->setVisible(intersected);

		Ogre::Vector3 sceneNodePos = 
			getSceneManipulator()->getTerrainData()->_getPosition( (mMaxX-mMinX)/2+mMinX, (mMaxZ-mMinZ)/2+mMinZ);

		mIntersectGridsNode->setPosition(sceneNodePos);
        refresh();

    }

    void TerrainSelectionGrids::reshape(void)
    {
        _clearGridSelections();
        mOriginPos = Ogre::Vector2::ZERO;
        intersectGrids->setGridColour( Ogre::ColourValue(0.0, 0.0, 0.6, 0.3) );
    }

    void TerrainSelectionGrids::_clearGridSelections(void)
    {
        GridSelections::iterator it = mGridSelectionList.begin();

        while ( it != mGridSelectionList.end() )
        {
            delete *it;
            *it = 0;

            ++it;
        }

        mGridSelectionList.clear();

		mMinX = 65535;
		mMinZ = 65535;
		mMaxX = 0;
		mMaxZ = 0;
    }

    void TerrainSelectionGrids::setCurrentGridSelection( bool multiSelecting )
    {
        // 如果不是多选，在选择当前块的地形之前，可以把之前选择的地形块删掉
        if (!multiSelecting)
        {
            _clearGridSelections();
        }        

        mCurrentSelection = new GridSelection(getTerrain(),getSceneManipulator());
        mGridSelectionList.push_back(mCurrentSelection);
    }

    void TerrainSelectionGrids::addCurrentGridSelection( bool multiSelecting )
    {
        mOriginPos = Ogre::Vector2::ZERO;

        if ( !multiSelecting )
        {
            // 清空全局的GridSelection
            mTotalGridSelection->reset();
            mTotalJunctionSelection->reset();
        }

        // 把当前地形块数组中的grid都加入到一个总的GridSelection中
        GridSelections::const_iterator it = mGridSelectionList.begin();

        while ( it != mGridSelectionList.end() )
        {
            GridSelection* selection = *it;

            const GridSelection::GridMap &mGrids = selection->getGrids();

            if (!mGrids.empty())
            {
                GridSelection::GridMap::const_iterator it = mGrids.begin();

                while (it != mGrids.end())
                {        
                    mTotalGridSelection->add(it->second.x, it->second.z);

                    ++it;
                } 
            }

            ++it;
        }

        // 根据gridSelection计算出junctionSelection
        const GridSelection::GridMap &mGrids = mTotalGridSelection->getGrids();

        if (!mGrids.empty())
        {
            GridSelection::GridMap::const_iterator it = mGrids.begin();

            while (it != mGrids.end())
            {  
                if ( !mTotalJunctionSelection->exist(it->second.x, it->second.z) )
                    mTotalJunctionSelection->add(it->second.x, it->second.z, 1);

                if ( !mTotalJunctionSelection->exist(it->second.x+1, it->second.z) )
                    mTotalJunctionSelection->add(it->second.x+1, it->second.z, 1);

                if ( !mTotalJunctionSelection->exist(it->second.x, it->second.z+1) )
                    mTotalJunctionSelection->add(it->second.x, it->second.z+1, 1);

                if ( !mTotalJunctionSelection->exist(it->second.x+1, it->second.z+1) )
                    mTotalJunctionSelection->add(it->second.x+1, it->second.z+1, 1);

                ++it;
            } 
        }       
    }


    void TerrainSelectionGrids::refresh(void)
    {
        // 对当前所有的GridSelection进行重绘
        GridSelections::const_iterator listIterator = mGridSelectionList.begin();

        while ( listIterator != mGridSelectionList.end() )
        {
            GridSelection* selection = *listIterator;

            const GridSelection::GridMap &mGrids = selection->getGrids();

            TerrainData *terrainData = getTerrainData();

            if (!mGrids.empty())
            {
                GridSelection::GridMap::const_iterator it = mGrids.begin();

				Ogre::Vector3 sceneNodePos = mIntersectGridsNode->getPosition();

                while (it != mGrids.end())
                {        
                    // 取得这个grid的四个角的坐标
                    int minx = it->second.x;
                    int minz = it->second.z;
                    int maxx = minx+1;
                    int maxz = minz+1;

                    // 判断是否翻转了，因为翻转了会改变顶点的高度
                    if ( terrainData->getGridInfo(minx, minz).flags & TerrainData::GridInfo::FlipDiagonal )
                    {
                        // 第一个三角形
                        mPosArray.push_back(terrainData->_getPosition(minx,minz)-sceneNodePos);
                        mPosArray.push_back(terrainData->_getPosition(minx,maxz)-sceneNodePos);
                        mPosArray.push_back(terrainData->_getPosition(maxx,maxz)-sceneNodePos);

                        // 第二个三角形
                        mPosArray.push_back(terrainData->_getPosition(maxx,minz)-sceneNodePos);
                        mPosArray.push_back(terrainData->_getPosition(minx,minz)-sceneNodePos);
                        mPosArray.push_back(terrainData->_getPosition(maxx,maxz)-sceneNodePos);
                    }
                    else
                    {
                        // 第一个三角形
                        mPosArray.push_back(terrainData->_getPosition(minx,minz)-sceneNodePos);
                        mPosArray.push_back(terrainData->_getPosition(minx,maxz)-sceneNodePos);
                        mPosArray.push_back(terrainData->_getPosition(maxx,minz)-sceneNodePos);

                        // 第二个三角形
                        mPosArray.push_back(terrainData->_getPosition(maxx,minz)-sceneNodePos);
                        mPosArray.push_back(terrainData->_getPosition(minx,maxz)-sceneNodePos);
                        mPosArray.push_back(terrainData->_getPosition(maxx,maxz)-sceneNodePos);
                    }                

                    ++it;
                } 
            }

            ++listIterator;
        }        

        intersectGrids->fillPosition(mPosArray);

        mPosArray.clear();        
    }

    void TerrainSelectionGrids::show()
    {
        assert (mIntersectGridsNode);
        mIntersectGridsNode->setVisible(true);
    }
}