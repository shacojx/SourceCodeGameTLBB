#include "TerrainGrid.h"

#include "SceneManipulator.h"

#include "Core/TerrainData.h"

#include <OgreQuaternion.h>
#include <OgreSimpleRenderable.h>
#include <OgreVector3.h>
#include <OgreHardwareBufferManager.h>
#include <OgreHardwareBuffer.h>
#include <OgreRenderQueue.h>
#include <OgreViewport.h>
#include <OgreCamera.h>
#include <OgreRoot.h>
#include <OgreMatrix4.h>
#include <OgreSceneNode.h>
#include <OgrePlane.h>
#include <OgreRay.h>
#include <OgreTextAreaOverlayElement.h>
#include <OgreOverlayContainer.h>
#include <OgreOverlayElement.h>
#include <OgreOverlayManager.h>
#include <OgreVector2.h>
#include <OgreStringConverter.h>
#include <OgreRenderSystem.h>


namespace WX
{
    /// 每个分块的大小
    #define STRIDE 16
    #define RED_LINE_STRIDE 10

    static Ogre::ColourValue NormalLineColour = Ogre::ColourValue(0.65f, 0.65f, 0.65f);
    static Ogre::ColourValue SeparatorLineColour = Ogre::ColourValue::Red;
    static Ogre::ColourValue ReshapeLineColour = Ogre::ColourValue::Green;

    TerrainGridRenderable::TerrainGridRenderable(SceneManipulator* manip) :
    mSceneManipulator(manip),
    mStride(10),
    mVertexCount(0),
    m_locked_data(NULL),
    terrData(NULL),
    mColorValue(Ogre::ColourValue::White),
    m_radius(0.0f),
    mXStart(0),
    mZStart(0),
    mXEnd(0),
    mZEnd(0)
    { 
        mRenderOp.vertexData = NULL;
	}

    TerrainGridRenderable::~TerrainGridRenderable() 
    {
		freeBuffer();
    }

    void TerrainGridRenderable::lock()
    {
        if(!m_locked_data)
        {
            m_locked_buffer = mRenderOp.vertexData->vertexBufferBinding->getBuffer(0);
            m_locked_data = static_cast<Ogre::Real*>(m_locked_buffer->lock(Ogre::HardwareBuffer::HBL_NORMAL));
        }
    }

    void TerrainGridRenderable::unlock()
    {
        m_locked_buffer->unlock();
        m_locked_data = NULL;

        m_radius = Ogre::Vector3((mBox.getMinimum() - mBox.getMaximum())*0.5).length();
    }

    void TerrainGridRenderable::drawLine(const Ogre::Vector3 &v1, const Ogre::Vector3 &v2, const Ogre::ColourValue &c1, const Ogre::ColourValue &c2,unsigned int lineCount)
    {
        Ogre::Real *pPos = m_locked_data;

        pPos = (Ogre::Real *)((unsigned int)(pPos) + 6*lineCount*sizeof(Ogre::Real) + 2*lineCount*sizeof(unsigned int));
        *pPos++ = v1.x;
        *pPos++ = v1.y;
        *pPos++ = v1.z;
        Ogre::Root::getSingleton().convertColourValue(c1,((Ogre::uint32 *)pPos));
        pPos = (Ogre::Real *)((unsigned int)(pPos) + sizeof(unsigned int));
        *pPos++ = v2.x;
        *pPos++ = v2.y;
        *pPos++ = v2.z;
        Ogre::Root::getSingleton().convertColourValue(c2,((Ogre::uint32 *)pPos));

        mBox.merge(v1);
        mBox.merge(v2);

        setBoundingBox(mBox);
    }

    void TerrainGridRenderable::clear()
    {
        mBox.setExtents(-100000,-100000,-100000,100000,100000,100000);
    }

    Ogre::Real TerrainGridRenderable::getSquaredViewDepth(const Ogre::Camera* cam) const
    {
        Ogre::Vector3 min, max, mid, dist;
        min = mBox.getMinimum();
        max = mBox.getMaximum();
        mid = ((min - max) * 0.5) + min;
        dist = cam->getDerivedPosition() - mid;
        return dist.squaredLength();
    }

    void
        TerrainGridRenderable::initTerrainGrid(int vertexCount)
    {
        m_locked_data = 0;

        mRenderOp.vertexData = new Ogre::VertexData();
        mRenderOp.indexData = 0;

        mRenderOp.vertexData->vertexCount = mVertexCount;
        mRenderOp.vertexData->vertexStart = 0;

        mRenderOp.operationType = Ogre::RenderOperation::OT_LINE_LIST; 
        mRenderOp.useIndexes = false; 

        Ogre::VertexDeclaration* decl = mRenderOp.vertexData->vertexDeclaration;
        Ogre::VertexBufferBinding* bind = mRenderOp.vertexData->vertexBufferBinding;

        decl->addElement(0, 0, Ogre::VET_FLOAT3, Ogre::VES_POSITION);
        decl->addElement(0, 3*sizeof(Ogre::Real), Ogre::VET_COLOUR, Ogre::VES_DIFFUSE);

        Ogre::HardwareVertexBufferSharedPtr vbuf = 
            Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
            decl->getVertexSize(0),
            vertexCount,
            Ogre::HardwareBuffer::HBU_WRITE_ONLY);

        bind->setBinding(0, vbuf);

        // set basic white material
        this->setMaterial("BaseWhiteNoLighting");
        mBox.setExtents(-10,-10,-10,10,10,10);

        lock();
        drawGridLines();
        unlock();
        
	//	getMaterial()->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA);
        setQueryFlags(0);
    }

    void
    TerrainGridRenderable::drawGridLines()
    {
        const TerrainReshapeSize& reshapeSize = mSceneManipulator->getSceneEditingData().getTerrainReshapeSize();

        // 计算reshape网格线
        int reshapeLeft = reshapeSize.left;
        int reshapeTop = reshapeSize.top;
        int reshapeRight = terrData->getXSize() - reshapeSize.right;
        int reshapeBottom = terrData->getZSize() - reshapeSize.bottom;

        int lineCount = 0;
        for(int z=mZStart;z<mZEnd+1;z++)
        {
            for(int x=mXStart;x<mXEnd;x++)
            {
                Ogre::Vector3 startPt = terrData->_getPosition(x,z);
                Ogre::Vector3 endPt = terrData->_getPosition(x+1,z);

                startPt.y +=1;
                endPt.y +=1;

                if(z % RED_LINE_STRIDE == 0)
                    mColorValue = SeparatorLineColour;
                else
                    mColorValue = NormalLineColour;

                // 显示地形reshape的线
                if ( (z == reshapeTop || z == reshapeBottom ) &&
                    x >= reshapeLeft && x < reshapeRight )
                    mColorValue = ReshapeLineColour;

                drawLine(startPt,endPt,mColorValue,mColorValue,lineCount++);
            }
        }

        for(int x=mXStart;x<mXEnd+1;x++)
        {
            for(int z=mZStart;z<mZEnd;z++)
            {
                Ogre::Vector3 startPt = terrData->_getPosition(x,z);
                Ogre::Vector3 endPt = terrData->_getPosition(x,z+1);

                startPt.y +=1;
                endPt.y +=1;

                if(x % RED_LINE_STRIDE == 0)
                    mColorValue = SeparatorLineColour;
                else
                    mColorValue = NormalLineColour;

                // 显示地形reshape的线
                if ( (x == reshapeLeft || x == reshapeRight) &&
                    z >= reshapeTop && z < reshapeBottom )
                    mColorValue = ReshapeLineColour;

                drawLine(startPt,endPt,mColorValue,mColorValue,lineCount++);
            }
        }
    }

    void TerrainGridRenderable::onTerrainHeightChanged(const std::vector<TerrainInfo>& terrainInfo,int minx,int minz,int maxx,int maxz)
    {
		/*minx  = std::max(mXStart,minx);
		minz  = std::max(mZStart,minz);

		maxx  = std::min(maxx,mXEnd);
		maxz  = std::min(maxz,mZEnd);


        lock();

        int lineCount = ((minz-mZStart) * (mZEnd - mZStart) + (minx-mXStart)) ;
        for(int z=minz;z<maxz+1;z++)
        {
            for(int x=minx;x<maxx;x++)
            {
                Ogre::Vector3 startPt = terrData->_getPosition(x,z);
                Ogre::Vector3 endPt = terrData->_getPosition(x+1,z);

                startPt.y +=1;
                endPt.y +=1;

                if(z%10 == 0)
                    mColorValue = colourRed;
                else
                    mColorValue = colourWhite;

                if (z == 2 || z == 62)
                    mColorValue = colourGreen;

                drawLine(startPt,endPt,mColorValue,mColorValue,lineCount++);
            }

            lineCount += (mXEnd - mXStart) - (maxx-minx);
        }

        lineCount = ((minx-mXStart) * (mZEnd - mZStart) + (minz-mZStart)) + (mXEnd - mXStart) * (mZEnd - mZStart+1);
        for(int x=minx;x<maxx+1;x++)
        {
            for(int z=minz;z<maxz;z++)
            {
                Ogre::Vector3 startPt = terrData->_getPosition(x,z);
                Ogre::Vector3 endPt = terrData->_getPosition(x,z+1);

                startPt.y +=1;
                endPt.y +=1;

                if(x%10 == 0)
                    mColorValue = colourRed;
                else
                    mColorValue = colourWhite;

                if (x == 2 || x == 62)
                    mColorValue = colourGreen;

                drawLine(startPt,endPt,mColorValue,mColorValue,lineCount++);
            }

            lineCount += (mZEnd - mZStart) - (maxz-minz);
        }

        unlock();*/
        /**
        hack的代码，临时修改了reshape后地形网格线的bug
        现在是每次改变地形高度都会重新填充整块地形网格线，效率有点低
        */
        lock();
        drawGridLines();
        unlock();
    }

    void
    TerrainGridRenderable::_updateRenderQueue(Ogre::RenderQueue* queue)
    {   
        queue->addRenderable( this, mRenderQueueID, Ogre::RENDER_QUEUE_OVERLAY); 
        //SimpleRenderable::_updateRenderQueue(queue);
    }

    void
    TerrainGridRenderable::init(int xStart,int zStart, int xEnd, int zEnd)
    {
        mXStart = xStart;
		mZStart = zStart;
        mXEnd = xEnd;
        mZEnd = zEnd;

        int xStride = mXEnd - mXStart;
        int zStride = mZEnd - mZStart;

        terrData = mSceneManipulator->getTerrainData();

		//mVertexCount = 2 * terrData->mXSize * (terrData->mZSize+1) + 2 * terrData->mZSize * (terrData->mXSize+1);
		mVertexCount = 2 * xStride* (xStride+1) + 2 * zStride* (zStride+1);

        initTerrainGrid(mVertexCount);          
    }

    void TerrainGridRenderable::freeBuffer()
    {
        if(mRenderOp.vertexData)
            delete mRenderOp.vertexData;
    }

    TerrainGrid::TerrainGrid(Ogre::SceneManager* sceneMgr,SceneManipulator* manip)
	: mActive(false) 
	{
		mTerrGridRend.clear();

        mSceneMgr = sceneMgr;
        mSceneManipulator = manip;
        mSceneManipulator->addSceneListener(this);

		mBaseSceneNode = mSceneManipulator->getIndicatorRootSceneNode()->createChildSceneNode();
		mTerrData = mSceneManipulator->getTerrainData();
    }

    TerrainGrid::~TerrainGrid()
    {
        destroyRenderable();

		mBaseSceneNode->destroy();
    }

    void
    TerrainGrid::createRenderable()
    {
		destroyRenderable();

		//base on terrain tile,create the TerrainGridRenderable
		int zTile = Ogre::Math::Ceil((float)(mTerrData->mZSize)/(float)(STRIDE));
		int xTile = Ogre::Math::Ceil((float)(mTerrData->mXSize)/(float)(STRIDE));
		
        for (int z=0; z<zTile; ++z)
            for (int x=0; x<xTile; ++x)
            {
                TerrainGridRenderable* terrGridRend = new TerrainGridRenderable(mSceneManipulator);

                int xStart, xEnd, zStart, zEnd;
                xStart = xEnd = zStart = zEnd = 0;

                xStart = x*STRIDE;
                zStart = z*STRIDE;
                xEnd = xStart + STRIDE;
                zEnd = zStart + STRIDE;

                // 如果当前块的原定大小超出了地形的tile size（reshape后有可能出现这种情况），就得计算块的大小
                if (xEnd > mTerrData->mXSize)
                    xEnd = xStart + STRIDE - ( xEnd - mTerrData->mXSize );

                if (zEnd > mTerrData->mZSize)
                    zEnd = zStart + STRIDE - ( zEnd - mTerrData->mZSize );

                terrGridRend->init(xStart, zStart, xEnd, zEnd);

                mBaseSceneNode->attachObject(terrGridRend);

                mTerrGridRend.push_back(terrGridRend);
            }
    }

    void
    TerrainGrid::destroyRenderable()
	{
		std::list<TerrainGridRenderable*>::iterator iter = mTerrGridRend.begin();
		for(;iter != mTerrGridRend.end();iter++)
		{
			mBaseSceneNode->detachObject((*iter)->getName()); 
			delete (*iter);
		}

		mTerrGridRend.clear();
	}

    void
    TerrainGrid::onTerrainHeightChanged(const std::vector<TerrainInfo>& terrainInfo)
    {
		int minx,minz;
		int maxx,maxz;

		minx = mTerrData->mXSize;
		minz = mTerrData->mZSize;
		maxx = 0;
		maxz = 0;

		for(unsigned int i=0; i<terrainInfo.size();i++)
		{
			minx  = std::min(terrainInfo[i].x,minx);
			minz  = std::min(terrainInfo[i].z,minz);

			maxx  = std::max(terrainInfo[i].x,maxx);
			maxz  = std::max(terrainInfo[i].z,maxz);
		}

		if(minx>0)
			minx--;
		if(minz>0)
			minz--;
		if(maxx<mTerrData->mXSize)
			maxx++;
		if(maxz<mTerrData->mZSize)
			maxz++;

		std::list<TerrainGridRenderable*>::iterator iter;
		for(iter = mTerrGridRend.begin(); iter != mTerrGridRend.end(); iter++)
		{
			if( minx > (*iter)->mXEnd || maxx < (*iter)->mXStart || minz > (*iter)->mZEnd || maxz < (*iter)->mZStart )
				continue;
			else
				(*iter)->onTerrainHeightChanged(terrainInfo,minx,minz,maxx,maxz);
		}
         
    }

    void
        TerrainGrid::onSceneReset(void)
    {
        if (mBaseSceneNode)
        {
            //initTerrGridRend();
			createRenderable();
        }
    }

	void 
	TerrainGrid::initTerrainRend()
	{
		//todo:
	}



}
