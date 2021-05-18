#include "StdAfx.h"
#include "RenderSystem.h"

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

#include "TerrainGrid.h"

#include "Core/WXSystem.h"
#include "Core/TerrainData.h"

namespace WX
{

	static Ogre::ColourValue colourRed(1.0,0.0,0.0,0.5);
	static Ogre::ColourValue colourWhite(1.0,1.0,1.0,0.5);

    TerrainGridRenderable::TerrainGridRenderable()
    : mStride(10)
    { 
        mRenderOp.vertexData = NULL;
	}

    TerrainGridRenderable::~TerrainGridRenderable() 
    {
		freeBuffer();
    }

    VOID TerrainGridRenderable::lock()
    {
        if(!m_locked_data)
        {
            m_locked_buffer = mRenderOp.vertexData->vertexBufferBinding->getBuffer(0);
            m_locked_data = static_cast<Ogre::Real*>(m_locked_buffer->lock(Ogre::HardwareBuffer::HBL_NORMAL));
        }
    }

    VOID TerrainGridRenderable::unlock()
    {
        m_locked_buffer->unlock();
        m_locked_data = NULL;

        m_radius = Ogre::Vector3((mBox.getMinimum() - mBox.getMaximum())*0.5).length();
    }

    VOID TerrainGridRenderable::drawLine(const Ogre::Vector3 &v1, const Ogre::Vector3 &v2, const Ogre::ColourValue &c1, const Ogre::ColourValue &c2,unsigned int lineCount)
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

    VOID TerrainGridRenderable::clear()
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

    VOID
        TerrainGridRenderable::initTerrainGrid(int vertexCount)
    {
        m_locked_data = 0;

        mRenderOp.vertexData = new Ogre::VertexData();
        mRenderOp.indexData = 0;

        mRenderOp.vertexData->vertexCount = mVertexCount;
        mRenderOp.vertexData->vertexStart = 0;

        mRenderOp.operationType = Ogre::RenderOperation::OT_LINE_LIST; 
        mRenderOp.useIndexes = FALSE; 

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
        
		getMaterial()->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA);
        setQueryFlags(0);
    }

    VOID
    TerrainGridRenderable::drawGridLines()
    {
        int lineCount = 0;
        for(int z=mZStart;z<mZStart + terrData->mTileSize+1;z++)
        {
            for(int x=mXStart;x<mXStart + terrData->mTileSize;x++)
            {
                Ogre::Vector3 startPt = terrData->_getPosition(x,z);
                Ogre::Vector3 endPt = terrData->_getPosition(x+1,z);

                startPt.y +=1;
                endPt.y +=1;

                if(z%TerrainGrid::getZoneSize() == 0)
                    mColorValue = colourRed;
                else
                    mColorValue = colourWhite;

                drawLine(startPt,endPt,mColorValue,mColorValue,lineCount++);
            }
        }

        for(int x=mXStart;x<mXStart+terrData->mTileSize+1;x++)
        {
            for(int z=mZStart;z<mZStart+terrData->mTileSize;z++)
            {
                Ogre::Vector3 startPt = terrData->_getPosition(x,z);
                Ogre::Vector3 endPt = terrData->_getPosition(x,z+1);

                startPt.y +=1;
                endPt.y +=1;

				if(x%TerrainGrid::getZoneSize() == 0)
                    mColorValue = colourRed;
                else
                    mColorValue = colourWhite;

                drawLine(startPt,endPt,mColorValue,mColorValue,lineCount++);
            }
        }
    }


    VOID
    TerrainGridRenderable::_updateRenderQueue(Ogre::RenderQueue* queue)
    {   
        queue->addRenderable( this, mRenderQueueID, Ogre::RENDER_QUEUE_OVERLAY); 
        //SimpleRenderable::_updateRenderQueue(queue);
    }

    VOID
    TerrainGridRenderable::init(int x,int z)
    {
        mXStart = x;
		mZStart = z;

		terrData = CRenderSystem::GetMe()->GetRenderSystem()->getTerrainData();

		//mVertexCount = 2 * terrData->mXSize * (terrData->mZSize+1) + 2 * terrData->mZSize * (terrData->mXSize+1);
		mVertexCount = 2 * terrData->mTileSize* (terrData->mTileSize+1) + 2 * terrData->mTileSize* (terrData->mTileSize+1);

        initTerrainGrid(mVertexCount);          
    }

    VOID TerrainGridRenderable::freeBuffer()
    {
        if(mRenderOp.vertexData)
            delete mRenderOp.vertexData;
    }

	int TerrainGrid::sZoneSize = 20;
    TerrainGrid::TerrainGrid(Ogre::SceneManager* sceneMgr, int zonesize)
	: mActive(FALSE) 
	{
		mTerrGridRend.clear();

        mSceneMgr = sceneMgr;

		mBaseSceneNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
		mTerrData = CRenderSystem::GetMe()->GetRenderSystem()->getTerrainData();

		sZoneSize = zonesize;
    }

    TerrainGrid::~TerrainGrid()
    {
        destroyRenderable();

		mBaseSceneNode->getParentSceneNode()->removeChild(mBaseSceneNode->getName());
		mBaseSceneNode->getCreator()->destroySceneNode(mBaseSceneNode->getName());
    }

    VOID
    TerrainGrid::createRenderable()
    {
		destroyRenderable();

		//base on terrain tile,create the TerrainGridRenderable
		int zTile = Ogre::Math::Ceil((FLOAT)(mTerrData->mZSize)/(FLOAT)(mTerrData->mTileSize));
		int xTile = Ogre::Math::Ceil((FLOAT)(mTerrData->mXSize)/(FLOAT)(mTerrData->mTileSize));
		for(int z = 0;z<zTile ;z++)
		{
			for(int x = 0;x< xTile;x++)
			{
				TerrainGridRenderable* terrGridRend = new TerrainGridRenderable();
				terrGridRend->init(x*mTerrData->mTileSize,z*mTerrData->mTileSize);
				mBaseSceneNode->attachObject(terrGridRend);

				mTerrGridRend.push_back(terrGridRend);
			}
		}

    }

    VOID
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

	VOID 
	TerrainGrid::initTerrainRend()
	{
		//todo:
	}



}
