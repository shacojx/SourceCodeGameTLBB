#include "WXScene.h"

#include <OgreException.h>
#include <OgreStringConverter.h>

#include <boost/lambda/lambda.hpp>

namespace WX {
    const String Scene::DEFAULT_AUTO_NAME_PREFIX = "Unnamed_";
    //-----------------------------------------------------------------------
    Scene::Scene()
        : mObjects()
        , mAttributes()

        , mAutoNamePrefix(DEFAULT_AUTO_NAME_PREFIX)
        , mAutoNameIndex(0)
    {
    }
    //-----------------------------------------------------------------------
    Scene::~Scene()
    {
    }
    //-----------------------------------------------------------------------
    void Scene::setAutoNamePrefix(const String& newAutoNamePrefix)
    {
        // Rename all auto named mObjects to new name
        Objects::index<by_sequenced>::type& indices = mObjects.get<by_sequenced>();
        Objects::index<by_sequenced>::type::iterator it;
        for (it = indices.begin(); it != indices.end(); ++it)
        {
            if (isAutoName((*it)->getName()))
            {
                String newName = (*it)->getName();
                newName.replace(0, mAutoNamePrefix.length(), newAutoNamePrefix);
                mObjects.get<by_name>().modify_key(mObjects.project<by_name>(it), boost::lambda::_1 = newName);
            }
        }

        // Update to new auto name prefix
        mAutoNamePrefix = newAutoNamePrefix;
    }
    //-----------------------------------------------------------------------
    String Scene::generateAutoName(const ObjectPtr &object)
    {
		assert (object);
		String objectType = object->getType();
		assert (!objectType.empty());
        return mAutoNamePrefix + objectType + "_" + Ogre::StringConverter::toString(mAutoNameIndex++, 5, '0');
    }
    //-----------------------------------------------------------------------
    bool Scene::isAutoName(const String &name) const
    {
        return Ogre::StringUtil::startsWith(name, mAutoNamePrefix, false);
    }
    //-----------------------------------------------------------------------
    void Scene::reset()
    {
        mName.clear();
        mAuthor.clear();
        mCopyright.clear();
        mDescription.clear();
        mTerrainFilename.clear();

        mObjects.clear();
        mAttributes.clear();

        mAutoNamePrefix = DEFAULT_AUTO_NAME_PREFIX;
        mAutoNameIndex = 0;
    }
    //-----------------------------------------------------------------------
    void Scene::addObject(const ObjectPtr& object)
    {
        if (!mObjects.get<by_identity>().insert(object).second)
        {
            OGRE_EXCEPT(Ogre::Exception::ERR_DUPLICATE_ITEM,
                "The given object with name '" + object->getName() + "' already contain by this scene",
                "Scene::addObject");
        }
    }
    //-----------------------------------------------------------------------
    void Scene::removeObject(const String& name)
    {
        if (mObjects.get<by_name>().erase(name) <= 0)
        {
            OGRE_EXCEPT(Ogre::Exception::ERR_ITEM_NOT_FOUND,
                "The given object with name '" + name + "' doesn't contain by this scene",
                "Scene::removeObject");
        }
    }
    //-----------------------------------------------------------------------
    void Scene::removeObject(const ObjectPtr& object)
    {
        if (mObjects.get<by_identity>().erase(object) <= 0)
        {
            OGRE_EXCEPT(Ogre::Exception::ERR_ITEM_NOT_FOUND,
                "The given object with name '" + object->getName() + "' doesn't contain by this scene",
                "Scene::removeObject");
        }
    }
    //-----------------------------------------------------------------------
    void Scene::renameObject(const String& oldName, const String& newName)
    {
        Objects::index<by_name>::type& name_index = mObjects.get<by_name>();
        Objects::index<by_name>::type::iterator it = name_index.find(oldName);
        if (it == name_index.end())
        {
            OGRE_EXCEPT(Ogre::Exception::ERR_ITEM_NOT_FOUND,
                "Can't found object with name '" + oldName + "'",
                "Scene::renameObject");
        }

        name_index.modify_key(it, boost::lambda::_1 = newName);
    }
    //-----------------------------------------------------------------------
    void Scene::renameObject(const ObjectPtr& object, const String& newName)
    {
        Objects::index<by_identity>::type& identity_index = mObjects.get<by_identity>();
        Objects::index<by_identity>::type::iterator it = identity_index.find(object);
        if (it == identity_index.end())
        {
            OGRE_EXCEPT(Ogre::Exception::ERR_ITEM_NOT_FOUND,
                "The given object with name '" + object->getName() + "' doesn't contain by this scene",
                "Scene::renameObject");
        }

        mObjects.get<by_name>().modify_key(mObjects.project<by_name>(it), boost::lambda::_1 = newName);
    }
    //-----------------------------------------------------------------------
    const ObjectPtr& Scene::findObjectByName(const String& name) const
    {
        const Objects::index<by_name>::type& name_index = mObjects.get<by_name>();
        Objects::index<by_name>::type::const_iterator it = name_index.find(name);
        if (it == name_index.end())
        {
            static const ObjectPtr nil;
            return nil;
        }

        return *it;
    }
    //-----------------------------------------------------------------------
    Scene::ObjectsByTypeRange Scene::findObjectsByType(const String& type) const
    {
        const Objects::index<by_type>::type& type_index = mObjects.get<by_type>();
        return ObjectsByTypeRange(type_index.lower_bound(type), type_index.upper_bound(type));
    }
    //-----------------------------------------------------------------------
    Scene::ObjectsByCategoryRange Scene::findObjectsByCategory(const String& category) const
    {
        const Objects::index<by_category>::type& category_index = mObjects.get<by_category>();
        return ObjectsByCategoryRange(category_index.lower_bound(category), category_index.upper_bound(category));
    }
    //-----------------------------------------------------------------------
    void Scene::broadcastMessage(const String& msg, const Variant& params)
    {
        for (Objects::const_iterator it = mObjects.begin(); it != mObjects.end(); ++it)
        {
            const ObjectPtr& object = *it;
            object->processMessage(msg, params);
        }
    }
    //-----------------------------------------------------------------------
    bool Scene::existAttribute(const String& name)
    {
        return mAttributes.find(name) != mAttributes.end();
    }
    //-----------------------------------------------------------------------
    const String& Scene::getAttribute(const String& name) const
    {
        Attributes::const_iterator it = mAttributes.find(name);
        if (it == mAttributes.end())
        {
            OGRE_EXCEPT(Ogre::Exception::ERR_ITEM_NOT_FOUND,
                "The attribute with name '" + name + "' doesn't exists",
                "Scene::getAttribute");
        }

        return it->second;
    }
    //-----------------------------------------------------------------------
    void Scene::setAttribute(const String& name, const String& value)
    {
        mAttributes[name] = value;
    }
    //-----------------------------------------------------------------------
    void Scene::removeAttribute(const String& name)
    {
        Attributes::iterator it = mAttributes.find(name);
        if (it == mAttributes.end())
        {
            OGRE_EXCEPT(Ogre::Exception::ERR_ITEM_NOT_FOUND,
                "The attribute with name '" + name + "' doesn't exists",
                "Scene::removeAttribute");
        }

        mAttributes.erase(it);
    }

}
