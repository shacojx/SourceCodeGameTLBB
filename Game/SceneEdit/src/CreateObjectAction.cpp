#include "CreateObjectAction.h"
#include "SceneManipulator.h"

#include "Core/WXUtils.h"
#include "Core/WXSceneInfo.h"
#include "Core/WXObjectFactoryManager.h"
#include "Core/TerrainData.h"

#include <OgreString.h>
#include <OgreVector3.h>
#include <OgreStringConverter.h>

namespace WX {

    CreateObjectAction::CreateObjectAction(SceneManipulator* sceneManipulator)
        : Action(sceneManipulator)
        , mDirty(true)
        , mObject()
    {
    }

    CreateObjectAction::~CreateObjectAction()
    {
    }

    const String& CreateObjectAction::getName(void) const
    {
        static const String name = "CreateObjectAction";
        return name;
    }

    void CreateObjectAction::setParameter(const String& name, const String& value)
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

    String CreateObjectAction::getParameter(const String& name) const
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

    void CreateObjectAction::_onActive(bool active)
    {
        if (!active)
        {
            hide();
        }
    }

    void CreateObjectAction::_onMove(const Point& pt)
    {
        updateInstance(pt);
    }

    void CreateObjectAction::_onBegin(const Point& pt)
    {
        updateInstance(pt);
    }

    void CreateObjectAction::_onDrag(const Point& pt)
    {
        updateInstance(pt);
    }

    void CreateObjectAction::_onEnd(const Point& pt, bool canceled)
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

    void CreateObjectAction::hide(void)
    {
        mObject.reset();
    }

    bool CreateObjectAction::updateInstance(const Point& pt)
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
            mObject->createRenderInstance(getSceneManipulator());
        }
        else
        {
            mObject->setPropertyList(mParameters);
        }

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

        return true;
    }
}
