/*
-----------------------------------------------------------------------------
This source file is part of OGRE
(Object-oriented Graphics Rendering Engine)
For the latest info, see http://ogre.sourceforge.net/

Copyright (c) 2000-2005 The OGRE Team
Also see acknowledgements in Readme.html

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.
-----------------------------------------------------------------------------
*/
#include "WXBoxCenterMovable.h"
#include "SceneManipulator.h"

#include <OgreOverlayContainer.h>
#include <OgreOverlayElement.h>
#include <OgreOverlayManager.h>
#include <OgreSphere.h>
#include <OgreCamera.h>
#include <OgreViewport.h>
#include <OgreSceneQuery.h>

namespace WX {
	///////////////////////////////////////////////////////////////////////////////
	//class BoxCenterManager
	///////////////////////////////////////////////////////////////////////////////
	BoxCenterManager::BoxCenterManager(SceneManipulator* manip,Ogre::SceneManager* mSceneManager)
	: mRaySceneQuery(NULL)
	{
		mSceneMgr = mSceneManager;
		mSceneManipulator = manip;
		mSceneManipulator->addSceneListener(this);

		mRaySceneQuery = mSceneMgr->createRayQuery(Ogre::Ray());
		mRaySceneQuery->setSortByDistance(true);
		
		init();
	}

	BoxCenterManager::~BoxCenterManager()
	{
		if (mRaySceneQuery)
		{
			mSceneMgr->destroyQuery(mRaySceneQuery);
		}
	}

	void 
	BoxCenterManager::onSceneReset(void)
	{
		init();
	}

	void
	BoxCenterManager::onRemoveObject(const ObjectPtr& object)
	{
		ObjBoxCenterMap::iterator iter = mObjBoxCenterMap.find(object);
		if(iter != mObjBoxCenterMap.end())
		{
			BoxCenterObjMap::iterator it = mBoxCenterObjMap.find(iter->second);
			mBoxCenterObjMap.erase(it);

			delete iter->second;
			mObjBoxCenterMap.erase(iter);
		}
	}

	void
	BoxCenterManager::init()
	{
		mBoxCenterObjMap.clear();

		ObjBoxCenterMap::iterator iter;
		for(iter = mObjBoxCenterMap.begin();iter != mObjBoxCenterMap.end();iter++)
		{
			Ogre::MovableObject* mv = iter->second;
			if (mv->getParentSceneNode())
			{
				mv->getParentSceneNode()->destroy();
			}

			delete iter->second;
		}
		mObjBoxCenterMap.clear();
	}

	void
	BoxCenterManager::addObjBox(const ObjectPtr& object,BoxCenterMovable* mv)
	{
		mObjBoxCenterMap.insert(ObjBoxCenterMap::value_type(object,mv));
		mBoxCenterObjMap.insert(BoxCenterObjMap::value_type(mv,object));
	}

	ObjectWeakPtr
	BoxCenterManager::findHitObject(const Point& pt)
	{
		Ogre::Ray ray = mSceneManipulator->getWindowToViewportRay(pt);
		mRaySceneQuery->setRay(ray);
		const Ogre::RaySceneQueryResult& queryResult = mRaySceneQuery->execute();
		for (Ogre::RaySceneQueryResult::const_iterator it = queryResult.begin(); it != queryResult.end(); ++it)
		{
			Ogre::MovableObject* movable = it->movable;
			BoxCenterObjMap::iterator iter = mBoxCenterObjMap.find(movable);
			if( iter != mBoxCenterObjMap.end())
                return iter->second;
		}

		return ObjectWeakPtr();;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////
	//class BoxCenterMovable
	///////////////////////////////////////////////////////////////////////////////////////////////////
    unsigned int BoxCenterMovable::ms_uGenNameCount = 0;
    static const Real PointPixelSize = 10;


    BoxCenterMovable::BoxCenterMovable()
    : mObjectCenterPointRect(NULL)
    , mObjectCenterPointOverlay(NULL)
    {
        // Disable cast shadows by default
        mCastShadows = false;

        m_pParentSceneManager = NULL;

        mParentNode = NULL;
        m_pCamera = NULL;

        // Generate name
		Ogre::StringUtil::StrStreamType str;
		str << "BoxCenterMovable" << ms_uGenNameCount++;
		mName = str.str();
    }
    
    BoxCenterMovable::~BoxCenterMovable()
    {
        if (mObjectCenterPointRect)
        {
            Ogre::OverlayManager::getSingleton().destroyOverlayElement(mObjectCenterPointRect->getName());
        }
        if(mObjectCenterPointOverlay)
        {
            Ogre::OverlayManager::getSingleton().destroy(mObjectCenterPointOverlay);
        }
    }

    void 
    BoxCenterMovable::init(const ObjectPtr& obj,Ogre::SceneManager* mgr,Ogre::Viewport* viewport,Ogre::Camera* camera)
    {
        object = obj;
        m_pParentSceneManager = mgr;
        mViewport = viewport;
        m_pCamera = camera;

        getBoundingBoxCenter();
        
        float baseExtent = 25.0f;
        float dist = (camera->getPosition() - mCenterPosWC).length();    
        float farClip = camera->getFarClipDistance();
        float nearClip = camera->getNearClipDistance();
        float extent = 2.0f * dist / (farClip + nearClip) * baseExtent;

        Ogre::AxisAlignedBox box((mCenterPosWC + Ogre::Vector3(-extent,-extent,-extent)),(mCenterPosWC + Ogre::Vector3(extent,extent,extent)));
       
        setBoundingBox(box);

        createGuiElements();
    }

   

    void BoxCenterMovable::_notifyCurrentCamera(Ogre::Camera* cam)
    {
        m_pCamera = cam;
        Ogre::MovableObject::_notifyCurrentCamera(cam);
    }

    void BoxCenterMovable::setBoundingBox( const Ogre::AxisAlignedBox& box )
    {
        mBox = box;
    }

    const Ogre::AxisAlignedBox& BoxCenterMovable::getBoundingBox(void) const
    {
        return mBox;
    }

    Real BoxCenterMovable::getBoundingRadius(void) const
    {
        return 0.0f;
    }

    void BoxCenterMovable::_updateRenderQueue(Ogre::RenderQueue* queue)
    { 
		if(!object.lock())
			return;

        getBoundingBoxCenter();

        // Project the sphere onto the camera
        Ogre::Real left, right, top, bottom;
        Ogre::Sphere sphere(mCenterPosWC,PointPixelSize);

        if (m_pCamera->projectSphere(sphere, &left, &top, &right, &bottom))
        {
            int iLeft, iTop, iWidth, iHeight;
            mViewport->getActualDimensions(iLeft, iTop, iWidth, iHeight);
            size_t szLeft, szRight, szTop, szBottom;

            szLeft = (size_t)(iLeft + ((left + 1) * 0.5 * iWidth));
            szRight = (size_t)(iLeft + ((right + 1) * 0.5 * iWidth));
            szTop = (size_t)(iTop + ((-top + 1) * 0.5 * iHeight));
            szBottom = (size_t)(iTop + ((-bottom + 1) * 0.5 * iHeight));

            mCenterPosSC.x = ( szLeft + szRight ) / 2;
            mCenterPosSC.y = ( szTop + szBottom ) / 2;
        }

        int nTop = mCenterPosSC.y  - PointPixelSize/2;
        int nLeft = mCenterPosSC.x  - PointPixelSize/2;

        mObjectCenterPointRect->setTop(nTop);
        mObjectCenterPointRect->setLeft(nLeft);
        mObjectCenterPointRect->setWidth(PointPixelSize);
        mObjectCenterPointRect->setHeight(PointPixelSize);
    }
    //-----------------------------------------------------------------------
    const Ogre::String& BoxCenterMovable::getMovableType(void) const
    {
        static Ogre::String movType = "BoxCenterMovable";
        return movType;
    }
    //-----------------------------------------------------------------------
    void BoxCenterMovable::createGuiElements(void)
    {
        mObjectCenterPointOverlay = Ogre::OverlayManager::getSingleton().create(getName());
        mObjectCenterPointOverlay->setZOrder(500);

        // this panel will be the main container for our selecting rectangle
        mObjectCenterPointRect = static_cast<Ogre::OverlayContainer*>(
            Ogre::OverlayManager::getSingleton().createOverlayElement("BorderPanel", getName()));
        mObjectCenterPointRect->setMetricsMode(Ogre::GMM_PIXELS);
        mObjectCenterPointRect->setMaterialName("Ogre/Eyes");
        mObjectCenterPointRect->setParameter("border_size", "1 1 1 1");
        mObjectCenterPointRect->setParameter("border_material", "Core/StatsBlockBorder");
        mObjectCenterPointRect->setParameter("border_topleft_uv", "0.0000 1.0000 0.0039 0.9961");
        mObjectCenterPointRect->setParameter("border_top_uv", "0.0039 1.0000 0.9961 0.9961");
        mObjectCenterPointRect->setParameter("border_topright_uv", "0.9961 1.0000 1.0000 0.9961");
        mObjectCenterPointRect->setParameter("border_left_uv","0.0000 0.9961 0.0039 0.0039");
        mObjectCenterPointRect->setParameter("border_right_uv","0.9961 0.9961 1.0000 0.0039");
        mObjectCenterPointRect->setParameter("border_bottomleft_uv","0.0000 0.0039 0.0039 0.0000");
        mObjectCenterPointRect->setParameter("border_bottom_uv","0.0039 0.0039 0.9961 0.0000");
        mObjectCenterPointRect->setParameter("border_bottomright_uv","0.9961 0.0039 1.0000 0.0000");

		mObjectCenterPointRect->getMaterial()->setSceneBlending(Ogre::SBT_ADD);
		mObjectCenterPointRect->getMaterial()->setDepthCheckEnabled(false);
		mObjectCenterPointRect->getMaterial()->setDepthWriteEnabled(false);
        mObjectCenterPointOverlay->add2D(mObjectCenterPointRect);
        mObjectCenterPointOverlay->show();
    }

    void
    BoxCenterMovable::getBoundingBoxCenter()
    {
        if(object.lock()->hasProperty("bounding box") && object.lock()->hasProperty("position"))
        {
            Ogre::AxisAlignedBox aabb = VariantCast<Ogre::AxisAlignedBox>(object.lock()->getProperty("bounding box"));
            /*Ogre::Vector3 position = VariantCast<Ogre::Vector3>(object.lock()->getProperty("position"));
            Ogre::Matrix4 matTrans;
            matTrans.makeTrans( position );
            aabb.transformAffine(matTrans);*/

			Ogre::Matrix4 transform = Ogre::Matrix4::IDENTITY;

			Ogre::Matrix3 rot3x3;
			if (object.lock()->hasProperty("orientation"))
			{
				Ogre::Quaternion orientation = VariantCast<Ogre::Quaternion>(object.lock()->getProperty("orientation"));
				orientation.ToRotationMatrix(rot3x3);
			}
			else
			{
				rot3x3 = Ogre::Matrix3::IDENTITY;
			}

			Ogre::Matrix3 scale3x3;
			if (object.lock()->hasProperty("scale"))
			{
				Ogre::Vector3 scale = VariantCast<Ogre::Vector3>(object.lock()->getProperty("scale"));
				scale3x3 = Ogre::Matrix3::ZERO;
				scale3x3[0][0] = scale.x;
				scale3x3[1][1] = scale.y;
				scale3x3[2][2] = scale.z;
			}
			else
			{
				scale3x3 = Ogre::Matrix3::IDENTITY;
			}

			transform = rot3x3 * scale3x3;

			if (object.lock()->hasProperty("position"))
			{
				Ogre::Vector3 position = VariantCast<Ogre::Vector3>(object.lock()->getProperty("position"));
				transform.setTrans(position);
			}
			aabb.transformAffine(transform);

            mCenterPosWC = aabb.getCenter();
        }
    }

}
