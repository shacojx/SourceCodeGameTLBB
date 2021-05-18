#include "WXCreateObjectOperator.h"
#include "SceneManipulator.h"

#include "Core/WXSceneInfo.h"
#include "Core/WXObjectFactoryManager.h"

namespace WX
{

    CreateObjectOperator::CreateObjectOperator(SceneManipulator* sceneManipulator)
        : mSceneManipulator(sceneManipulator)
        , mObjectInfos()
    {
    }
    //-----------------------------------------------------------------------
    CreateObjectOperator::~CreateObjectOperator()
    {
    }
    //-----------------------------------------------------------------------
    void CreateObjectOperator::add(const ObjectPtr& object, bool create)
    {
        assert(object);

        mObjectInfos.push_back(ObjectInfo());
        ObjectInfo& info = mObjectInfos.back();

        info.create = create;
        info.type = object->getType();
        info.name = object->getName();
        const PropertyList& properties = object->getProperties();
        for (PropertyList::const_iterator it = properties.begin(); it != properties.end(); ++it)
        {
            const PropertyDef& propertyDef = *it;
            if (object->getPropertyAsString(propertyDef.name) != propertyDef.defaultValue)
            {
                info.properties.insert(
                    PropertyNameVariantPairList::value_type(propertyDef.name, object->getProperty(propertyDef.name)));
            }
        }
    }
    //-----------------------------------------------------------------------
    const String& CreateObjectOperator::getGroupName(void) const
    {
        static const String groupName = "Objects";
        return groupName;
    }
    //-----------------------------------------------------------------------
    String CreateObjectOperator::getDescription(void) const
    {
        // TODO:
        return "";
    }
    //-----------------------------------------------------------------------
    String CreateObjectOperator::getHelp(void) const
    {
        // TODO
        return "";
    }
    //-----------------------------------------------------------------------
    void CreateObjectOperator::undo(void)
    {
        apply(true);
    }
    //-----------------------------------------------------------------------
    void CreateObjectOperator::redo(void)
    {
        apply(false);
    }
    //-----------------------------------------------------------------------
    void CreateObjectOperator::apply(bool reserve)
    {
        for (ObjectInfoList::const_iterator i = mObjectInfos.begin(); i != mObjectInfos.end(); ++i)
        {
            const ObjectInfo& info = *i;
            if (reserve == info.create)
            {
                const ObjectPtr& object = mSceneManipulator->getSceneInfo()->findObjectByName(i->name);
                assert(object);
                mSceneManipulator->removeObject(object, NULL, true);
            }
            else
            {
                ObjectPtr object = ObjectFactoryManager::getSingleton().createInstance(info.type);
                object->setName(info.name);
                object->setPropertyList(info.properties);
                object->createRenderInstance(mSceneManipulator);
                mSceneManipulator->addObject(object, NULL, true);
            }
        }
    }

}
