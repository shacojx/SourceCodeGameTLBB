#include "WXModifyObjectPropertyOperator.h"
#include "SceneManipulator.h"

#include "Core/WXSceneInfo.h"

namespace WX
{

    ModifyObjectPropertyOperator::ModifyObjectPropertyOperator(SceneManipulator* sceneManipulator)
        : mSceneManipulator(sceneManipulator)
        , mObjectProperties()
    {
    }

    ModifyObjectPropertyOperator::~ModifyObjectPropertyOperator()
    {
    }

    void ModifyObjectPropertyOperator::add(const String& object, const String& property, const Variant& oldValue, const Variant& newValue)
    {
        assert(!object.empty());
        assert(!property.empty());
        assert(!oldValue.empty());
        assert(!newValue.empty());

        assert(mObjectProperties.find(object) == mObjectProperties.end() ||
               mObjectProperties[object].find(property) == mObjectProperties[object].end());

        mObjectProperties[object][property] = VariantPair(oldValue, newValue);
    }

    bool ModifyObjectPropertyOperator::empty(void) const
    {
        return mObjectProperties.empty();
    }

    const String& ModifyObjectPropertyOperator::getGroupName(void) const
    {
        static const String groupName = "Objects";
        return groupName;
    }

    String ModifyObjectPropertyOperator::getDescription(void) const
    {
        // TODO:
        return "";
    }

    String ModifyObjectPropertyOperator::getHelp(void) const
    {
        // TODO
        return "";
    }

    void ModifyObjectPropertyOperator::undo(void)
    {
        for (ObjectPropertiesMap::const_iterator i = mObjectProperties.begin(); i != mObjectProperties.end(); ++i)
        {
            ObjectPtr object = mSceneManipulator->getSceneInfo()->findObjectByName(i->first);
            assert(object);
            for (NameVariantPairMap::const_iterator j = i->second.begin(); j != i->second.end(); ++j)
            {
                object->setProperty(j->first, j->second.first);
                if (mSceneManipulator)
                    mSceneManipulator->_fireObjectPropertyChanged(object, j->first);
            }
        }
    }

    void ModifyObjectPropertyOperator::redo(void)
    {
        for (ObjectPropertiesMap::const_iterator i = mObjectProperties.begin(); i != mObjectProperties.end(); ++i)
        {
            ObjectPtr object = mSceneManipulator->getSceneInfo()->findObjectByName(i->first);
            assert(object);
            for (NameVariantPairMap::const_iterator j = i->second.begin(); j != i->second.end(); ++j)
            {
                object->setProperty(j->first, j->second.second);
                if (mSceneManipulator)
                    mSceneManipulator->_fireObjectPropertyChanged(object, j->first);
            }
        }
    }

}
