#include "ManipObjectAction.h"
#include "SceneManipulator.h"
#include "WXOperatorManager.h"
#include "WXModifyObjectPropertyOperator.h"
#include "Axis3d.h"

#include "Core/WXUtils.h"
#include "Core/TerrainData.h"
#include "Core/WXObjectProxy.h"
#include "Core/WXRayCollide.h"
#include "Core/WXActorObject.h"
#include "Core/WXActor.h"

#include "ModelSystem/WXLogicModel.h"

#include <OgreString.h>
#include <OgreSceneManager.h>
#include <OgreSceneNode.h>
#include <OgreCamera.h>
#include <OgreViewport.h>
#include <OgreOverlay.h>
#include <OgreOverlayContainer.h>
#include <OgreOverlayElement.h>
#include <OgreOverlayManager.h>
#include <OgreStringConverter.h>
#include <OgreWireBoundingBox.h>
#include <OgreMatrix3.h>

namespace WX
{

    static const Ogre::ColourValue gSelectedColour(0, 1, 0);
    static const Ogre::ColourValue gHittedColour(1, 0, 0);
    static const Ogre::ColourValue gSelectedAndHittedColour(1, 1, 0);

    ManipObjectAction::ManipObjectAction(SceneManipulator* sceneManipulator)
        : Action(sceneManipulator)

        , mScaleFactor(0.1f)
        , mMinScale(0.01f)
        , mMaxScale(100.0f)
        , mMouseScaleScale(0.01f)
        , mMouseRotateScale(0.01f)
        , mCurrentMode("Move")

        , mRaySceneQuery(NULL)
        , mPlaneBoundedVolumeListSceneQuery(NULL)
        , mSelectingOverlay(NULL)
        , mSelectingRect(NULL)

        , mBaseSceneNode(NULL)
        , mIndicators()
        , mFreeIndicators()

        , mHitInfo()
        , mSelecting(false)

        , mSelectedObjects()
        , mHitObject()
        , mHitIndicator(NULL)

        , lr(NULL)
        , mAxisNode(NULL)
		, mBoxCenterMgr(sceneManipulator,getSceneManager())
    {
        getSceneManipulator()->addSceneListener(this);

        // Create scene queries
        mRaySceneQuery = getSceneManager()->createRayQuery(Ogre::Ray());
        mRaySceneQuery->setSortByDistance(true);

        mPlaneBoundedVolumeListSceneQuery = getSceneManager()->createPlaneBoundedVolumeQuery(Ogre::PlaneBoundedVolumeList());

        createGuiElements();

        mBaseSceneNode = getIndicatorRootSceneNode()->createChildSceneNode();		
	}

    ManipObjectAction::~ManipObjectAction()
    {
        getSceneManipulator()->removeSceneListener(this);

        hide();

        if (mBaseSceneNode)
        {
            mBaseSceneNode->destroy();
        }

        for (IndicatorList::const_iterator it = mIndicators.begin(); it != mIndicators.end(); ++it)
        {
            Indicator* indicator = *it;
            delete indicator;
        }

        if (mSelectingRect)
        {
            Ogre::OverlayManager::getSingleton().destroyOverlayElement(mSelectingRect->getName());
        }
        if (mSelectingOverlay)
        {
            Ogre::OverlayManager::getSingleton().destroy(mSelectingOverlay->getName());
        }

        if (mRaySceneQuery)
        {
            getSceneManager()->destroyQuery(mRaySceneQuery);
        }
        if (mPlaneBoundedVolumeListSceneQuery)
        {
            getSceneManager()->destroyQuery(mPlaneBoundedVolumeListSceneQuery);
        }

        destroyAxis();
		
		clearBoxCenterManager();
    }

    const String& ManipObjectAction::getName(void) const
    {
        static const String name = "ManipObjectAction";
        return name;
    }

    void ManipObjectAction::setParameter(const String& name, const String& value)
    {
        if (name == "%CurrentMode")
        {
            mCurrentMode = value;
        }
        else
        {
            Action::setParameter(name, value);
        }
    }

    String ManipObjectAction::getParameter(const String& name) const
    {
        if (name == "%CurrentMode")
        {
            return mCurrentMode;
        }

        return Action::getParameter(name);
    }

    void ManipObjectAction::_onActive(bool active)
    {
        if (!active)
        {
            hide();
            
            if(getSceneManipulator()->getUseAxis() && lr)
				hideAxis();
				

            if(getSceneManipulator()->getUseBoxCenterSelection())
				clearBoxCenterManager();
        }
        else
        {
			updateAxis();
        }
    }

    void ManipObjectAction::_onMove(const Point& pt)
    {
        if(getSceneManipulator()->getUseBoxCenterSelection())
        {
            mBoxCenterMgr.init();
			addBoxCenterMovable(pt);     
        }
        else
        {
            ObjectPtr hitObject = findHitObject(pt);
            ObjectPtr prevHitObject = mHitObject.lock();

            if (prevHitObject != hitObject)
            {
                if (prevHitObject)
                {
                    SelectedObjectMap::const_iterator it = mSelectedObjects.find(mHitObject);
                    if (it != mSelectedObjects.end() && it->second.indicator)
                    {
                        it->second.indicator->setMaterial(createPureColourMaterial(gSelectedColour)->getName());
                    }
                }

                mHitObject = hitObject;

                if (hitObject && hitObject->hasProperty("bounding box"))
                {
                    if (!mHitIndicator)
                        mHitIndicator = allocateIndicator();

                    setupIndicator(mHitIndicator, hitObject);

                    if (getSceneManipulator()->isObjectSelected(hitObject))
                        mHitIndicator->setMaterial(createPureColourMaterial(gSelectedAndHittedColour)->getName());
                    else
                        mHitIndicator->setMaterial(createPureColourMaterial(gHittedColour)->getName());
                }
                else
                {
                    if (mHitIndicator)
                    {
                        freeIndicator(mHitIndicator);
                        mHitIndicator = NULL;
                    }
                }
            }
        }

        if(getSceneManipulator()->getUseAxis() && lr)
        {
            Ogre::Ray ray = getSceneManipulator()->getWindowToViewportRay(pt);
            lr->collideAxis(ray);
        }
       
    }

    void ManipObjectAction::_onBegin(const Point& pt)
    {
        getSceneManipulator()->makeTerrainHitInfo(mHitInfo, pt);

        ObjectPtr hitObject ;
        if(getSceneManipulator()->getUseBoxCenterSelection())
            hitObject = findFirstHitObjectByBoxCenter(pt).lock();
        else
            hitObject = findHitObject(pt);

        if (hitObject)
        {
            if (mCurrentMode == "MultiSelecting")
            {
                mSelecting = true;
            }
            else if (mCurrentMode == "Move")
            {
                if (!getSceneManipulator()->isObjectSelected(hitObject))
                    mSelecting = true;
            }
        }
        else
        {
            if (mCurrentMode == "MultiSelecting")
            {
                mSelecting = true;
            }
            else if (mCurrentMode == "Move")
            {
				if(getSceneManipulator()->getUseAxis() && lr)
				{
					Ogre::Ray ray = getSceneManipulator()->getWindowToViewportRay(pt);
					if(!(lr->collideAxis(ray)))
						mSelecting = true;
				}
				else
					mSelecting = true;
            }
            else
            {
                if (!getSceneManipulator()->hasSelectedObjects())
                    mSelecting = true;
            }
        }
            
        if (mCurrentMode == "MultiRotate" || mCurrentMode == "MultiScale")
        {
            mCenterPos = getCenterPos(true);
        }
    }

	void 
	ManipObjectAction::dragObjectByMax(const Point& pt)
	{
		int nSelAxis = 0;
		if(lr)
		{   
			nSelAxis = lr->getSelAxis();
			if(nSelAxis < 0)
				nSelAxis = 1;
		}

		if (mCurrentMode == "Move")
		{
			if (mHitInfo.hitted)
			{
				Ogre::Ray ray = getSceneManipulator()->getWindowToViewportRay(mDragOrigin.x, pt.y);
				Real s = ray.getDirection().x*ray.getDirection().x + ray.getDirection().z*ray.getDirection().z;
				if (s)
				{
					Ogre::Vector3 v = mHitInfo.hitPosition - mHitInfo.position;
					Real adjust = Ogre::Math::Sqrt((v.x*v.x + v.z*v.z) / s) * ray.getDirection().y - v.y;
					for (SelectedObjectMap::const_iterator it = mSelectedObjects.begin(); it != mSelectedObjects.end(); ++it)
					{
						ObjectPtr object = it->first.lock();
						if (object && object->hasProperty("position"))
						{
							const ObjectInfo& info = it->second;
							const Ogre::Vector3& oldPosition = info.position;
							Ogre::Vector3 newPosition = oldPosition;
							newPosition[nSelAxis] += adjust;

							if(getSceneManipulator()->getUseAxis() && lr)
								lr->getParentSceneNode()->setPosition(newPosition);

							object->setProperty("position", newPosition);
							getSceneManipulator()->_fireObjectPropertyChanged(object, "position", this);
							updateIndicator(object);
						}
					}  
					updateAxis();
				}
			}
		}
		else if (mCurrentMode == "Rotate")
		{
			if(getSceneManipulator()->getRotationAlignGranularity() > 0)
				mDragDelta = pt - mDragOrigin;

			Ogre::Degree degrees(mDragDelta.y * getSceneManipulator()->getRotateFactor() / getViewport()->getActualWidth());

			Ogre::Vector3 selAxis(0,0,0);
			
			selAxis[nSelAxis] = 1;

			switch(nSelAxis)
			{
			case 0:
				setParameter("%CurrentMode","RotateX");
				break;
			case 1:
				setParameter("%CurrentMode","RotateY");
				break;
			case 2:
				setParameter("%CurrentMode","RotateZ");
				break;
			}			

			Ogre::Quaternion delta(degrees, selAxis);
			for (SelectedObjectMap::const_iterator it = mSelectedObjects.begin(); it != mSelectedObjects.end(); ++it)
			{
				ObjectPtr object = it->first.lock();
				if (object)
				{
					if (object->hasProperty("orientation"))
					{
						Ogre::Quaternion oldOrientation ;
						Ogre::Quaternion newOrientation ;

						if(getSceneManipulator()->getRotationAlignGranularity() > 0)
						{
							const ObjectInfo& info = it->second;                    
							oldOrientation = info.orientation;
							newOrientation = delta * oldOrientation;
						}
						else
						{
							oldOrientation = VariantCast<Ogre::Quaternion>(object->getProperty("orientation"));
							newOrientation = delta * oldOrientation;
						}

						newOrientation = getSceneManipulator()->_alignRotation(newOrientation);
						object->setProperty("orientation", newOrientation);
						getSceneManipulator()->_fireObjectPropertyChanged(object, "orientation", this);
						updateIndicator(object);
					}

				}
			}
			setParameter("%CurrentMode","Rotate");
		}
		else if (mCurrentMode == "Scale")
		{
			Real diff = (pt.y - mDragOrigin.y);
			Real factor;
			if (diff >= 0)
				factor = 1 + mScaleFactor * diff;
			else
				factor = 1 / (1 - mScaleFactor * diff);

			for (SelectedObjectMap::const_iterator it = mSelectedObjects.begin(); it != mSelectedObjects.end(); ++it)
			{
				ObjectPtr object = it->first.lock();
				if (object && object->hasProperty("scale"))
				{
					const ObjectInfo& info = it->second;
					Ogre::Vector3 scale = info.scale;
					scale[nSelAxis] *= factor;
					object->setProperty("scale", scale);
					getSceneManipulator()->_fireObjectPropertyChanged(object, "scale", this);
					updateIndicator(object);
				}
			}
		}
	}

	void 
	ManipObjectAction::dragObjectByKey(const Point& pt)
	{
		//组合键模式
		if (mCurrentMode == "Scale")
		{
			Real diff = (pt.y - mDragOrigin.y);
			Real factor;
			if (diff >= 0)
				factor = 1 + mScaleFactor * diff;
			else
				factor = 1 / (1 - mScaleFactor * diff);

			for (SelectedObjectMap::const_iterator it = mSelectedObjects.begin(); it != mSelectedObjects.end(); ++it)
			{
				ObjectPtr object = it->first.lock();
				if (object && object->hasProperty("scale"))
				{
					const ObjectInfo& info = it->second;
					Ogre::Vector3 scale = info.scale * factor;
					object->setProperty("scale", scale);
					getSceneManipulator()->_fireObjectPropertyChanged(object, "scale", this);
					updateIndicator(object);
				}
			}
		}
		else if (mCurrentMode == "Move")
		{
			if (mHitInfo.hitted)
			{
				Ogre::Ray ray = getSceneManipulator()->getWindowToViewportRay(pt);
				if (ray.getDirection().y)
				{
					Real length = (mHitInfo.position.y - mHitInfo.hitPosition.y) / ray.getDirection().y;
					Ogre::Vector3 offset = mHitInfo.position - (mHitInfo.hitPosition + ray.getDirection() * length);
					offset = getSceneManipulator()->_alignPosition(offset);
					for (SelectedObjectMap::const_iterator it = mSelectedObjects.begin(); it != mSelectedObjects.end(); ++it)
					{
						ObjectPtr object = it->first.lock();
						if (object && object->hasProperty("position"))
						{
							const ObjectInfo& info = it->second;
							Ogre::Vector3 groundingPoint = Ogre::Vector3::ZERO;
							if (object->hasProperty("grounding point"))
								groundingPoint = VariantCast<Ogre::Vector3>(object->getProperty("grounding point"));
							const Ogre::Vector3& oldPosition = info.position;
							Ogre::Vector3 newPosition = oldPosition + offset;
							// Adjust height relative to terrain
							newPosition.y +=
								getTerrainData()->getHeightAt(newPosition.x + groundingPoint.x, newPosition.z + groundingPoint.z) -
								getTerrainData()->getHeightAt(oldPosition.x + groundingPoint.x, oldPosition.z + groundingPoint.z);
							object->setProperty("position", newPosition);
							getSceneManipulator()->_fireObjectPropertyChanged(object, "position", this);
							updateIndicator(object);
						}
					}
				}
			}
		}
		else if (mCurrentMode == "RotateX")
		{
			if(getSceneManipulator()->getRotationAlignGranularity() > 0)
				mDragDelta = pt - mDragOrigin;

			Ogre::Degree degrees(mDragDelta.y * getSceneManipulator()->getRotateFactor() / getViewport()->getActualWidth());
			Ogre::Quaternion delta(degrees, Ogre::Vector3::UNIT_X);
			for (SelectedObjectMap::const_iterator it = mSelectedObjects.begin(); it != mSelectedObjects.end(); ++it)
			{
				ObjectPtr object = it->first.lock();
				if (object)
				{
					if (object->hasProperty("orientation"))
					{
						Ogre::Quaternion oldOrientation ;
						Ogre::Quaternion newOrientation ;

						if(getSceneManipulator()->getRotationAlignGranularity() > 0)
						{
							const ObjectInfo& info = it->second;                    
							oldOrientation = info.orientation;
							newOrientation = delta * oldOrientation;
						}
						else
						{
							oldOrientation = VariantCast<Ogre::Quaternion>(object->getProperty("orientation"));
							newOrientation = delta * oldOrientation;
						}
						newOrientation = getSceneManipulator()->_alignRotation(newOrientation);
						object->setProperty("orientation", newOrientation);
						getSceneManipulator()->_fireObjectPropertyChanged(object, "orientation", this);
						updateIndicator(object);
					}

				}
			}
		}
		else if (mCurrentMode == "RotateY")
		{
			if(getSceneManipulator()->getRotationAlignGranularity() > 0)
				mDragDelta = pt - mDragOrigin;

			Ogre::Degree degrees(mDragDelta.y * getSceneManipulator()->getRotateFactor() / getViewport()->getActualWidth());
			Ogre::Quaternion delta(degrees, Ogre::Vector3::UNIT_Y);
			for (SelectedObjectMap::const_iterator it = mSelectedObjects.begin(); it != mSelectedObjects.end(); ++it)
			{
				ObjectPtr object = it->first.lock();
				if (object)
				{
					if (object->hasProperty("orientation"))
					{
						Ogre::Quaternion oldOrientation ;
						Ogre::Quaternion newOrientation ;

						if(getSceneManipulator()->getRotationAlignGranularity() > 0)
						{
							const ObjectInfo& info = it->second;                    
							oldOrientation = info.orientation;
							newOrientation = delta * oldOrientation;
						}
						else
						{
							oldOrientation = VariantCast<Ogre::Quaternion>(object->getProperty("orientation"));
							newOrientation = delta * oldOrientation;
						}
						newOrientation = getSceneManipulator()->_alignRotation(newOrientation);
						object->setProperty("orientation", newOrientation);
						getSceneManipulator()->_fireObjectPropertyChanged(object, "orientation", this);
						updateIndicator(object);
					}

					else if (object->hasProperty("direction"))
					{

						Ogre::Vector3 lightDir;

						// 获取视口的大小
						int screenWidth = getViewport()->getActualWidth();
						int screenHeight = getViewport()->getActualHeight();

						float halfScreenWidth = screenWidth / 2;
						float halfScreenHeight = screenHeight / 2;

						lightDir.x = mDragCurrent.x - halfScreenWidth;
						lightDir.z = mDragCurrent.y - halfScreenHeight;

						// 选取视口中较小的边作为我们要进行比较的圆的直径
						int screenSize = screenHeight<screenWidth ? screenHeight : screenWidth;

						// 用当前鼠标所在点与半径做比较，得出光源方向的y值（鼠标点越在中心，y值越大，所以光源方向与地面的角度就越大）
						lightDir.y = 1.0 - Ogre::Math::Sqrt(lightDir.x*lightDir.x + lightDir.z*lightDir.z) / (screenSize / 2);

						// 把y值限制在0到1的范围内
						if (lightDir.y <= 0.0f)
							lightDir.y = 0.0f;
						// 把x值和z值的范围限制在-1到1之间
						lightDir.x = lightDir.x / halfScreenWidth;
						lightDir.z = lightDir.z / halfScreenHeight;

						if (lightDir.x >= 1.0f)
							lightDir.x = 1.0f;
						if (lightDir.x <= -1.0f)
							lightDir.x = -1.0f;

						if (lightDir.z >= 1.0f)
							lightDir.z = 1.0f;
						if (lightDir.z <= -1.0f)
							lightDir.z = -1.0f;

						lightDir = -lightDir;
						object->setProperty("direction", lightDir);
						getSceneManipulator()->_fireObjectPropertyChanged(object, "direction", this);
					}
				}
			}
		}
		else if (mCurrentMode == "RotateZ")
		{
			if(getSceneManipulator()->getRotationAlignGranularity() > 0)
				mDragDelta = pt - mDragOrigin;

			Ogre::Degree degrees(mDragDelta.y * getSceneManipulator()->getRotateFactor() / getViewport()->getActualWidth());
			Ogre::Quaternion delta(degrees, Ogre::Vector3::UNIT_Z);
			for (SelectedObjectMap::const_iterator it = mSelectedObjects.begin(); it != mSelectedObjects.end(); ++it)
			{
				ObjectPtr object = it->first.lock();
				if (object)
				{
					if (object->hasProperty("orientation"))
					{
						Ogre::Quaternion oldOrientation ;
						Ogre::Quaternion newOrientation ;

						if(getSceneManipulator()->getRotationAlignGranularity() > 0)
						{
							const ObjectInfo& info = it->second;                    
							oldOrientation = info.orientation;
							newOrientation = delta * oldOrientation;
						}
						else
						{
							oldOrientation = VariantCast<Ogre::Quaternion>(object->getProperty("orientation"));
							newOrientation = delta * oldOrientation;
						}
						newOrientation = getSceneManipulator()->_alignRotation(newOrientation);
						object->setProperty("orientation", newOrientation);
						getSceneManipulator()->_fireObjectPropertyChanged(object, "orientation", this);
						updateIndicator(object);
					}
				}
			}
		}
	}

	void
	ManipObjectAction::dragObject(const Point& pt)
	{
		if (mCurrentMode == "MultiScale")
		{
			Real diff = (pt.y - mDragOrigin.y);
			Real factor;
			if (diff >= 0)
				factor = 1 + mScaleFactor * diff;
			else
				factor = 1 / (1 - mScaleFactor * diff);

			for (SelectedObjectMap::const_iterator it = mSelectedObjects.begin(); it != mSelectedObjects.end(); ++it)
			{
				ObjectPtr object = it->first.lock();
				if (object && object->hasProperty("scale"))
				{
					const ObjectInfo& info = it->second;
					Ogre::Vector3 scaleVec = info.position - mCenterPos;
					scaleVec.normalise();

					Ogre::Vector3 scale;

					scale.x = info.scale.x * factor + Ogre::Math::Abs(scaleVec.x) ;
					scale.y = info.scale.y * factor + Ogre::Math::Abs(scaleVec.x)/2 + Ogre::Math::Abs(scaleVec.z)/2 ;
					scale.z = info.scale.z * factor + Ogre::Math::Abs(scaleVec.z) ;

					object->setProperty("scale", scale);
					getSceneManipulator()->_fireObjectPropertyChanged(object, "scale", this);
					updateIndicator(object);
				}
			}
		}
		else if (mCurrentMode == "MultiRotate")
		{
			if(getSceneManipulator()->getRotationAlignGranularity() > 0)
				mDragDelta = pt - mDragOrigin;

			Ogre::Degree degrees(mDragDelta.y * getSceneManipulator()->getRotateFactor() / getViewport()->getActualWidth());
			for (SelectedObjectMap::const_iterator it = mSelectedObjects.begin(); it != mSelectedObjects.end(); ++it)
			{
				ObjectPtr object = it->first.lock();
				if (object && object->hasProperty("position") && object->hasProperty("orientation"))
				{
					const ObjectInfo& info = it->second;                    
					Ogre::Vector3 offset = mCenterPos - info.position;

					Ogre::Matrix3 rot;
					info.orientation.ToRotationMatrix(rot);
					Ogre::Vector3 trans = offset * rot;

					Ogre::Quaternion orientation;
					if(getSceneManipulator()->getRotationAlignGranularity() > 0)
					{
						orientation = info.orientation;
					}
					else
					{
						orientation = VariantCast<Ogre::Quaternion>(object->getProperty("orientation"));  
					}

					Ogre::Quaternion q;
					q.FromAngleAxis(Ogre::Radian(degrees),Ogre::Vector3::UNIT_Y);
					orientation =  orientation * q ;
                    setParameter("%CurrentMode","RotateY");
					orientation = getSceneManipulator()->_alignRotation(orientation);
					Ogre::Vector3 position = mCenterPos + orientation * -trans;
					object->setProperty("position",position);
					object->setProperty("orientation", orientation);
					getSceneManipulator()->_fireObjectPropertyChanged(object, "position", this);
					getSceneManipulator()->_fireObjectPropertyChanged(object, "orientation", this);
					updateIndicator(object);
					setParameter("%CurrentMode","MultiRotate");
				}
			}   
		}

		//类Max模式
		if(getSceneManipulator()->getUseAxis() && lr)
		{
			dragObjectByMax(pt);
		}
		else
		{
			dragObjectByKey(pt);
		}

	}
	
	void
	ManipObjectAction::pickObject(const Point& pt)
	{
		Real left = std::min(pt.x, mDragOrigin.x);
		Real top = std::min(pt.y, mDragOrigin.y);
		Real right = std::max(pt.x, mDragOrigin.x);
		Real bottom = std::max(pt.y, mDragOrigin.y);

		mSelectingOverlay->show();
		mSelectingRect->setTop(top);
		mSelectingRect->setLeft(left);
		mSelectingRect->setWidth(right - left);
		mSelectingRect->setHeight(bottom - top);
	}

    void ManipObjectAction::_onDrag(const Point& pt)
    {
        if (!mSelecting)
		{
			dragObject(pt);
		}
        else
        {
			pickObject(pt);       
        }
		
		updateAxis();
	}

    void ManipObjectAction::_onEnd(const Point& pt, bool canceled)
    {
        if (canceled)
        {
            if (mSelecting)
            {
                // Nothing todo
            }
            else
            {
                // Cancel current modifition
                for (SelectedObjectMap::const_iterator it = mSelectedObjects.begin(); it != mSelectedObjects.end(); ++it)
                {
                    ObjectPtr object = it->first.lock();
                    if (object)
                    {
                        bool modified = false;
                        const ObjectInfo& info = it->second;
                        if (object->hasProperty("position"))
                        {
                            if (info.position != VariantCast<Ogre::Vector3>(object->getProperty("position")))
                            {
                                object->setProperty("position", info.position);
                                getSceneManipulator()->_fireObjectPropertyChanged(object, "position", this);
                                modified = true;
                            }
                        }
                        if (object->hasProperty("orientation"))
                        {
                            if (info.orientation != VariantCast<Ogre::Quaternion>(object->getProperty("orientation")))
                            {
                                object->setProperty("orientation", info.orientation);
                                getSceneManipulator()->_fireObjectPropertyChanged(object, "orientation", this);
                                modified = true;
                            }
                        }
                        if (object->hasProperty("scale"))
                        {
                            if (info.scale != VariantCast<Ogre::Vector3>(object->getProperty("scale")))
                            {
                                object->setProperty("scale", info.scale);
                                getSceneManipulator()->_fireObjectPropertyChanged(object, "scale", this);
                                modified = true;
                            }
                        }

                        if (modified)
                            updateIndicator(object);
                    }
                }
            }
        }
        else
        {
            if (mSelecting)
            {
                bool multiSelecing = mCurrentMode == "MultiSelecting";
                if (!multiSelecing)
                    getSceneManipulator()->clearSelectedObjects();

                if (pt.x == mDragOrigin.x || pt.y == mDragOrigin.y)
                {
                    ObjectPtr object;
                    if(getSceneManipulator()->getUseBoxCenterSelection())
                        object = findFirstHitObjectByBoxCenter(pt).lock();
                    else    
                         object = findHitObject(pt);
                    if (object)
                    {
                        if (multiSelecing && getSceneManipulator()->isObjectSelected(object))
                            getSceneManipulator()->deselectObject(object);
                        else
                            getSceneManipulator()->selectObject(object);
                    }
                }
                else
                {
                    Real left = std::min(pt.x, mDragOrigin.x);
                    Real top = std::min(pt.y, mDragOrigin.y);
                    Real right = std::max(pt.x, mDragOrigin.x);
                    Real bottom = std::max(pt.y, mDragOrigin.y);

                    Ogre::PlaneBoundedVolume volume = buildSelectingRectVolume(getCamera(), left, top, right, bottom);
                    Ogre::PlaneBoundedVolumeList selectClipVolumes;
                    selectClipVolumes.push_back(volume);
                    mPlaneBoundedVolumeListSceneQuery->setVolumes(selectClipVolumes);

                    //得到选中场景节点集
                    const Ogre::SceneQueryResult& result = mPlaneBoundedVolumeListSceneQuery->execute();
                    const Ogre::SceneQueryResultMovableList& mvList = result.movables;

                    for(Ogre::SceneQueryResultMovableList::const_iterator it = mvList.begin(); it != mvList.end(); ++it)
                    {
                        Ogre::MovableObject* movable = *it;
                        ObjectPtr object = getObjectFromMovable(movable);
                        if (object)
                        {
							// 如果当前用户只要选择非中心选择的物体，而且当前处理的物体的类型属于中心
							// 选择物体，就跳过
							if ( false == getSceneManipulator()->getSelectAllObjects() &&
								getSceneManipulator()->IsCenterSelectObject( object->getType() ) )
								continue;

                            if (multiSelecing && getSceneManipulator()->isObjectSelected(object))
                                getSceneManipulator()->deselectObject(object);
                            else
                                getSceneManipulator()->selectObject(object);
                        }
                    }
                }
            }
            else
            {
                ModifyObjectPropertyOperator* op = new ModifyObjectPropertyOperator(getSceneManipulator());

                // Receive the modified values
                for (SelectedObjectMap::iterator it = mSelectedObjects.begin(); it != mSelectedObjects.end(); ++it)
                {
                    ObjectPtr object = it->first.lock();
                    if (object)
                    {
                        ObjectInfo& info = it->second;
                        if (object->hasProperty("position"))
                        {
                            Ogre::Vector3 newPosition = VariantCast<Ogre::Vector3>(object->getProperty("position"));
                            if (info.position != newPosition)
                            {
                                op->add(object->getName(), "position", info.position, newPosition);
                                info.position = newPosition;
                            }
                        }
                        if (object->hasProperty("orientation"))
                        {
                            Ogre::Quaternion newOrientation = VariantCast<Ogre::Quaternion>(object->getProperty("orientation"));
                            if (info.orientation != newOrientation)
                            {
                                op->add(object->getName(), "orientation", info.orientation, newOrientation);
                                info.orientation = newOrientation;
                            }
                        }
                        if (object->hasProperty("scale"))
                        {
                            Ogre::Vector3 newScale = VariantCast<Ogre::Vector3>(object->getProperty("scale"));
                            if (info.scale != newScale)
                            {
                                op->add(object->getName(), "scale", info.scale, newScale);
                                info.scale = newScale;
                            }
                        }                     
                    }
                }

                if (op->empty())
                    delete op;
                else
                    getOperatorManager()->addOperator(op);
            }

		}

		updateAxis();

        mSelecting = false;
        mSelectingOverlay->hide();
    }

    void ManipObjectAction::hide(void)
    {
        // TODO: hide hit object and selected objects ?
    }

    std::set<ObjectWeakPtr>& ManipObjectAction::getObjSet()
    {
        return objSet;
    }

    ObjectPtr ManipObjectAction::findHitObject(const Point& pt) const
    {
        Ogre::Ray ray = getSceneManipulator()->getWindowToViewportRay(pt);
        mRaySceneQuery->setRay(ray);
        const Ogre::RaySceneQueryResult& queryResult = mRaySceneQuery->execute();
        for (Ogre::RaySceneQueryResult::const_iterator it = queryResult.begin(); it != queryResult.end(); ++it)
        {
            Ogre::MovableObject* movable = it->movable;
            ObjectPtr object = getObjectFromMovable(movable);

			if (object)
			{
                if (object->getType() == "Actor")
                {
                    WX::ActorObject* actorObject = static_cast<WX::ActorObject*>(object.get());

                    assert (actorObject);

                    WX::Actor* actor = actorObject->getActor();

                    assert (actor);

                    if (actor->getImpl() && actor->getImpl()->isUseExternalBoundingBox())
                    {
                        Ogre::SceneNode* modelNode = actorObject->getSceneNode();

                        const Ogre::AxisAlignedBox& modelBox = actor->getImpl()->getBoundingBox();
                        Ogre::AxisAlignedBox intersectedBox(modelBox.getMinimum(), modelBox.getMaximum());

                        intersectedBox.transformAffine(modelNode->_getFullTransform());

                        if ( false == Ogre::Math::intersects(ray, intersectedBox).first )
                            continue;
                    }
                }

                std::pair<bool, Real> hit = rayCollide(ray, movable, true);
                if (!hit.first)
                    continue;

				if ( false == getSceneManipulator()->getSelectAllObjects() &&
					getSceneManipulator()->IsCenterSelectObject( object->getType() ) )
					continue;
			}

            if (object)
                return object;
        }

        return ObjectPtr();
    }

    void ManipObjectAction::findHitObjectByBoxCenter(const Point& pt)
    {
        objSet.clear();

        Ogre::Ray ray = getSceneManipulator()->getWindowToViewportRay(pt);
        mRaySceneQuery->setRay(ray);
        const Ogre::RaySceneQueryResult& queryResult = mRaySceneQuery->execute();
        for (Ogre::RaySceneQueryResult::const_iterator it = queryResult.begin(); it != queryResult.end(); ++it)
        {
            Ogre::MovableObject* movable = it->movable;
            ObjectWeakPtr object = getObjectFromMovable(movable);
            if (object.lock())
            {
                objSet.insert(object);
            }
        }
    }    

    ObjectWeakPtr ManipObjectAction::findFirstHitObjectByBoxCenter(const Point& pt)
    {
		return mBoxCenterMgr.findHitObject(pt);
    }

    Ogre::PlaneBoundedVolume ManipObjectAction::buildSelectingRectVolume(const Ogre::Camera* camera,
        Real left, Real top, Real right, Real bottom) const
    {
        //摄像机的位置
        Ogre::Vector3 cameraPos= camera->getPosition();

        Ogre::Ray leftTopRay = getSceneManipulator()->getWindowToViewportRay(left, top);
        Ogre::Ray leftBottomRay = getSceneManipulator()->getWindowToViewportRay(left, bottom);
        Ogre::Ray rightTopRay = getSceneManipulator()->getWindowToViewportRay(right, top);
        Ogre::Ray rightBottomRay = getSceneManipulator()->getWindowToViewportRay(right, bottom);

        Ogre::Vector3 leftTopVector3 = leftTopRay.getPoint(camera->getNearClipDistance());
        Ogre::Vector3 leftBottomVector3 = leftBottomRay.getPoint(camera->getNearClipDistance());
        Ogre::Vector3 rightTopVector3 = rightTopRay.getPoint(camera->getNearClipDistance());
        Ogre::Vector3 rightBottomVector3 = rightBottomRay.getPoint(camera->getNearClipDistance());

        //建立左右上下近5个面
        Ogre::Plane planeLeft   (leftBottomVector3, leftTopVector3,cameraPos);
        Ogre::Plane planeRight  (rightTopVector3, rightBottomVector3, cameraPos);
        Ogre::Plane planeTop    (leftTopVector3, rightTopVector3, cameraPos);
        Ogre::Plane planeBottom (rightBottomVector3, leftBottomVector3, cameraPos);
        Ogre::Plane planeNear   (leftTopVector3, rightTopVector3, rightBottomVector3);

        Ogre::PlaneBoundedVolume volumes;
        // in this case we have to build a degenerate clip volume
        volumes.outside = Ogre::Plane::NEGATIVE_SIDE;
        volumes.planes.push_back(planeNear);
        volumes.planes.push_back(planeLeft);
        volumes.planes.push_back(planeRight);
        volumes.planes.push_back(planeTop);
        volumes.planes.push_back(planeBottom);
        return volumes;
    }

    //场景一个选取区域的overlay
    void ManipObjectAction::createGuiElements(void)
    {
        // create a new overlay to hold our selecting rectangle display
        mSelectingOverlay = Ogre::OverlayManager::getSingleton().create("SelectingOverlay");
        mSelectingOverlay->setZOrder(500);

        // this panel will be the main container for our selecting rectangle
        mSelectingRect = static_cast<Ogre::OverlayContainer*>(
            Ogre::OverlayManager::getSingleton().createOverlayElement("BorderPanel", "SelectingRect"));
        mSelectingRect->setMetricsMode(Ogre::GMM_PIXELS);
        mSelectingRect->setMaterialName("Core/StatsBlockCenter");
        mSelectingRect->setParameter("border_size", "1 1 1 1");
        mSelectingRect->setParameter("border_material", "Core/StatsBlockBorder");
        mSelectingRect->setParameter("border_topleft_uv", "0.0000 1.0000 0.0039 0.9961");
        mSelectingRect->setParameter("border_top_uv", "0.0039 1.0000 0.9961 0.9961");
        mSelectingRect->setParameter("border_topright_uv", "0.9961 1.0000 1.0000 0.9961");
        mSelectingRect->setParameter("border_left_uv","0.0000 0.9961 0.0039 0.0039");
        mSelectingRect->setParameter("border_right_uv","0.9961 0.9961 1.0000 0.0039");
        mSelectingRect->setParameter("border_bottomleft_uv","0.0000 0.0039 0.0039 0.0000");
        mSelectingRect->setParameter("border_bottom_uv","0.0039 0.0039 0.9961 0.0000");
        mSelectingRect->setParameter("border_bottomright_uv","0.9961 0.0039 1.0000 0.0000");

        // throw everything all the GUI stuff into the overlay and display it
        mSelectingOverlay->add2D(mSelectingRect);
        mSelectingOverlay->hide();
    }

    ManipObjectAction::Indicator* ManipObjectAction::allocateIndicator(void)
    {
        Indicator* indicator;

        if (mFreeIndicators.empty())
        {
            indicator = new Indicator;
            // Disable cast shadows
            indicator->setCastShadows(false);
            indicator->setQueryFlags(0);
            mIndicators.push_back(indicator);
        }
        else
        {
            indicator = mFreeIndicators.back();
            mFreeIndicators.pop_back();
        }

        assert(!indicator->getParentSceneNode());
        mBaseSceneNode->attachObject(indicator);

        return indicator;
    }

    void ManipObjectAction::freeIndicator(Indicator* indicator)
    {
        assert(indicator);
        if (indicator->getParentSceneNode())
            indicator->getParentSceneNode()->detachObject(indicator->getName());
        mFreeIndicators.push_back(indicator);
    }

    void ManipObjectAction::onSceneReset(void)
    {
        ManipObjectAction::onDeselectAllObjects();
    }

    void ManipObjectAction::onRemoveObject(const ObjectPtr& object)
    {
        if (mHitObject.lock() == object)
        {
            mHitObject.reset();
            if (mHitIndicator)
            {
                freeIndicator(mHitIndicator);
                mHitIndicator = NULL;
            }
        }
    }

    void ManipObjectAction::onSelectObject(const ObjectPtr& object)
    {
        assert(mSelectedObjects.find(object) == mSelectedObjects.end());

        ObjectInfo info;
        getObjectTransform(object, info);

        if (object->hasProperty("bounding box"))
        {
            info.indicator = allocateIndicator();
            setupIndicator(info.indicator, object);
            if (object == mHitObject.lock())
                info.indicator->setMaterial(createPureColourMaterial(gSelectedAndHittedColour)->getName());
            else
                info.indicator->setMaterial(createPureColourMaterial(gSelectedColour)->getName());
        }

        std::pair<SelectedObjectMap::iterator, bool> inserted =
            mSelectedObjects.insert(SelectedObjectMap::value_type(object, info));
        assert(inserted.second);
    }

    void ManipObjectAction::onDeselectObject(const ObjectPtr& object)
    {
        SelectedObjectMap::iterator it = mSelectedObjects.find(object);
        assert(it != mSelectedObjects.end());
        if (it != mSelectedObjects.end())
        {
            if (it->second.indicator)
                freeIndicator(it->second.indicator);
            mSelectedObjects.erase(it);
        }
    }

    void ManipObjectAction::onDeselectAllObjects(void)
    {
        for (SelectedObjectMap::const_iterator it = mSelectedObjects.begin(); it != mSelectedObjects.end(); ++it)
        {
            if (it->second.indicator)
                freeIndicator(it->second.indicator);
        }
        mSelectedObjects.clear();

        mHitObject.reset();
        if (mHitIndicator)
        {
            freeIndicator(mHitIndicator);
            mHitIndicator = NULL;
        }

		hideAxis();
    }

    void ManipObjectAction::onObjectPropertyChanged(const ObjectPtr& object, const String& name)
    {
        SelectedObjectMap::iterator it = mSelectedObjects.find(object);
        if (it != mSelectedObjects.end() && it->second.indicator)
        {
            getObjectTransform(object, it->second);
        }

        updateIndicator(object);
    }

    void ManipObjectAction::updateIndicator(const ObjectPtr& object)
    {
        SelectedObjectMap::iterator it = mSelectedObjects.find(object);
        if (it != mSelectedObjects.end() && it->second.indicator)
        {
            setupIndicator(it->second.indicator, object);
        }

        if (mHitIndicator && mHitObject.lock() == object)
        {
            setupIndicator(mHitIndicator, object);
        }
    }

    void ManipObjectAction::setupIndicator(Indicator* indicator, const ObjectPtr& object)
    {
        Ogre::Matrix4 transform = Ogre::Matrix4::IDENTITY;

        Ogre::Matrix3 rot3x3;
        if (object->hasProperty("orientation"))
        {
            Ogre::Quaternion orientation = VariantCast<Ogre::Quaternion>(object->getProperty("orientation"));
            orientation.ToRotationMatrix(rot3x3);
        }
        else
        {
            rot3x3 = Ogre::Matrix3::IDENTITY;
        }

        Ogre::Matrix3 scale3x3;
        if (object->hasProperty("scale"))
        {
            Ogre::Vector3 scale = VariantCast<Ogre::Vector3>(object->getProperty("scale"));
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

        if (object->hasProperty("position"))
        {
            Ogre::Vector3 position = VariantCast<Ogre::Vector3>(object->getProperty("position"));
            transform.setTrans(position);
        }

        Ogre::AxisAlignedBox aabb = VariantCast<Ogre::AxisAlignedBox>(object->getProperty("bounding box"));
        aabb.transformAffine(transform);
        indicator->setupBoundingBox(aabb);
    }

    void ManipObjectAction::getObjectTransform(const ObjectPtr& object, Transform& transform)
    {
        if (object->hasProperty("position"))
            transform.position = VariantCast<Ogre::Vector3>(object->getProperty("position"));
        if (object->hasProperty("orientation"))
            transform.orientation = VariantCast<Ogre::Quaternion>(object->getProperty("orientation"));
        if (object->hasProperty("scale"))
            transform.scale = VariantCast<Ogre::Vector3>(object->getProperty("scale"));
    }

    BoxCenterMovable*
    ManipObjectAction::createBoxCenterMovable(const ObjectPtr& object)
    {
        BoxCenterMovable* mv = new BoxCenterMovable();
        mv->init(object,getSceneManager(),getSceneManipulator()->getViewport(),getSceneManipulator()->getCamera());

        Ogre::SceneNode* mSceneNode = getIndicatorRootSceneNode()->createChildSceneNode();
        mSceneNode->attachObject(mv);
		
		return mv;
    }

	void 
	ManipObjectAction::hideAxis()
	{
		if(mAxisNode)
		{
			mAxisNode->setVisible(false);
			assert(lr);
			lr->showElement(false);
		}
	}

    void 
    ManipObjectAction::destroyAxis()
    {
        if(lr)
        {
            delete lr;
            lr = NULL;
        }

		if (mAxisNode)
		{
			mAxisNode->destroy();
			mAxisNode = NULL;
		}
    }

    AxisRenderable* ManipObjectAction::createAxis(Ogre::SceneManager* sceneMgr,Ogre::Camera* camera,Ogre::Viewport* viewport)
    { 
        if(mSelectedObjects.size() <= 0)
            return NULL;

		Ogre::Vector3 pos = getCenterPos();

		if(mAxisNode)
		{
			mAxisNode->setPosition(pos);
			mAxisNode->setVisible(true);
			assert(lr);
			lr->showElement(true);
			return (AxisRenderable*)(mAxisNode->getAttachedObject(0));
		}

		mAxisNode = getIndicatorRootSceneNode()->createChildSceneNode(pos);
        int length = 200;

        lr = new AxisRenderable(3,camera,viewport);
        lr->calcAxisInfo(pos,length);

        lr->setLocal(false);

        lr->lock();
        lr->drawLine(Ogre::Vector3(0,0,0), Ogre::Vector3(length,0,0), Ogre::ColourValue::Red,   Ogre::ColourValue::Red,0);
        lr->drawLine(Ogre::Vector3(0,0,0), Ogre::Vector3(0,length,0), Ogre::ColourValue::Green, Ogre::ColourValue::Green,1);
        lr->drawLine(Ogre::Vector3(0,0,0), Ogre::Vector3(0,0,length), Ogre::ColourValue::Blue,  Ogre::ColourValue::Blue,2);
        lr->unlock();

        mAxisNode->attachObject(lr);

        return lr;

    }
    
    void
    ManipObjectAction::updateAxis()
    {
        if(getSceneManipulator()->getUseAxis())
			createAxis(getSceneManager(),getSceneManipulator()->getCamera(),getSceneManipulator()->getViewport());
    }

    Ogre::Vector3 ManipObjectAction::getCenterPos(bool old)
    {
         Ogre::Vector3 pos(0.0f,0.0f,0.0f);
        int sum = 0;
        for (SelectedObjectMap::const_iterator it = mSelectedObjects.begin(); it != mSelectedObjects.end(); ++it)
		{

			ObjectPtr object = it->first.lock();
			if (object && object->hasProperty("position") && object->hasProperty("bounding box"))
			{
				if(old)
				{
					const ObjectInfo& info = it->second;
					Ogre::Vector3 position = info.position;
					pos += info.position;
				}
				else
				{
					Ogre::AxisAlignedBox aabb = VariantCast<Ogre::AxisAlignedBox>(object->getProperty("bounding box"));
					Ogre::Vector3 position = VariantCast<Ogre::Vector3>(object->getProperty("position"));
					Ogre::Matrix4 matTrans;
					matTrans.makeTrans( position );
					aabb.transformAffine(matTrans);

					pos += aabb.getCenter();
				}
				sum++;
			}

		}

        if(sum>0)
            return pos/(float)(sum);

		return pos;
    }

	void
	ManipObjectAction::clearBoxCenterManager()
	{
		mBoxCenterMgr.init();
	}

	void
	ManipObjectAction::addBoxCenterMovable(const Point& pt)
	{
		findHitObjectByBoxCenter(pt);

		for(ObjectPtrSet::const_iterator iter = objSet.begin();iter != objSet.end(); iter++)
		{
			mBoxCenterMgr.addObjBox((*iter).lock(),createBoxCenterMovable((*iter).lock()));	
		}
	}
}
