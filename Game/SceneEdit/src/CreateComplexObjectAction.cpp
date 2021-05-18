#include "CreateComplexObjectAction.h"
#include "SceneManipulator.h"
#include "HitIndicator.h"

#include "Core/WXUtils.h"
#include "Core/WXSceneInfo.h"
#include "Core/WXObjectFactoryManager.h"
#include "Core/TerrainData.h"

#include <OgreString.h>
#include <OgreVector3.h>
#include <OgreStringConverter.h>

namespace WX {

	CreateComplexObjectAction::CreateComplexObjectAction(SceneManipulator* sceneManipulator)
		: Action(sceneManipulator)
		, mDirty(true)
		, mObject()
	{
	}

	CreateComplexObjectAction::~CreateComplexObjectAction()
	{
	}

	const String& CreateComplexObjectAction::getName(void) const
	{
		static const String name = "CreateComplexObjectAction";
		return name;
	}

	void CreateComplexObjectAction::setParameter(const String& name, const String& value)
	{
		if (name == "type")
		{
			mObject.reset();
			mObjectType = value;
			mParameters.clear();
		}
		else if (Ogre::StringUtil::startsWith(name, "%", false))
		{
			mParameters[name.substr(1)] = value;
		}		
		else
		{
			Action::setParameter(name, value);
		}

		mDirty = true;
	}

	String CreateComplexObjectAction::getParameter(const String& name) const
	{
		if (name == "type")
		{
			return mObjectType;
		}
		else if (Ogre::StringUtil::startsWith(name, "%", false))
		{
			NameValuePairList::const_iterator it = mParameters.find(name.substr(1));
			if (it != mParameters.end())
				return it->second;
		}

		return Action::getParameter(name);
	}

	void CreateComplexObjectAction::_onActive(bool active)
	{
		if (!active)
		{
			hide();
		}
	}

	void CreateComplexObjectAction::_onMove(const Point& pt)
	{
		getSceneManipulator()->getHitIndicator("IntersectPoint")->setHitPoint(pt);
	}

	void CreateComplexObjectAction::_onBegin(const Point& pt)
	{
		updateInstance(pt);
	}

	void CreateComplexObjectAction::_onDrag(const Point& pt)
	{
		getSceneManipulator()->getHitIndicator("IntersectPoint")->setHitPoint(pt);
	}

	void CreateComplexObjectAction::_onEnd(const Point& pt, bool canceled)
	{
		if (!canceled && updateInstance(pt))
		{
			assert(mObject);

			// Permanent it to final scene
			mObject->setName(getSceneManipulator()->getSceneInfo()->generateAutoName(mObject));
			getSceneManipulator()->addObject(mObject);
		}

		mObject.reset();
	}

	void CreateComplexObjectAction::hide(void)
	{
		mObject.reset();
		getSceneManipulator()->getHitIndicator("IntersectPoint")->hide();
	}

	bool CreateComplexObjectAction::updateInstance(const Point& pt)
	{
		if (mObjectType.empty())
		{
			hide();
			return false;
		}

		Ogre::Vector3 position, normal;
		if (!getSceneManipulator()->getTerrainIntersects(pt, position, &normal, true))
		{
			hide();
			return false;
		}

		if (!mObject)
		{
			mObject = ObjectFactoryManager::getSingleton().createInstance(mObjectType);
			mObject->setPropertyList(mParameters);

			// 设置物体的位置
			Ogre::Vector3 groundingPoint(Ogre::Vector3::ZERO);
			if (mObject->hasProperty("grounding point"))
			{
				// Adjust position by grounding point
				groundingPoint = VariantCast<Ogre::Vector3>(mObject->getProperty("grounding point"));
				position -= groundingPoint;
			}
			position = getSceneManipulator()->_alignPosition(position);
			position.y = getTerrainData()->getHeightAt(position.x, position.z) - groundingPoint.y;
			position = getSceneManipulator()->_alignPosition(position);
			mObject->setProperty("position", position);

			mObject->createRenderInstance(getSceneManipulator());
		}
		else
		{
			mObject->setPropertyList(mParameters);
		}
		
		return true;
	}
}
