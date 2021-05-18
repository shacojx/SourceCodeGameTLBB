#include "WXSceneSolver.h"
#include "WXResourceSolverHelper.h"
#include "WXResources.h"
#include "WXSceneInfo.h"

#include <OgreResourceGroupManager.h>
#include <OgreMeshManager.h>

namespace WX {

    namespace {

    //////////////////////////////////////////////////////////////////////////

    class SceneSolver : public ResourceSolverBase
    {
    public:
        bool modified;

        SceneSolver(void)
            : ResourceSolverBase()
            , modified()
        {
        }

        const String& getName(void) const
        {
            static const String name = "Scene";
            return name;
        }

        bool commitPermanent(void)
        {
            bool ret = modified;
            modified = false;
            return ret;
        }
    };

    //////////////////////////////////////////////////////////////////////////

    class UnsolvedSceneItem : public UnsolvedResourceBase
    {
    public:
        SceneSolver* solver;
        typedef std::pair<ObjectPtr, String> Reference;
        typedef std::list<Reference> ReferenceList;
        ReferenceList references;

        UnsolvedSceneItem(SceneSolver* solver)
            : UnsolvedResourceBase()
            , solver(solver)
            , references()
        {
            assert(solver);
        }

        bool setResolvedValue(const String& value)
        {
            if (references.empty())
                return false;

            for (ReferenceList::const_iterator it = references.begin(); it != references.end(); ++it)
            {
                it->first->setPropertyAsString(it->second, value);
            }

            solver->modified = true;
            return true;
        }
    };

    //////////////////////////////////////////////////////////////////////////

    class Locator
    {
    public:
        virtual ~Locator() {}

        virtual bool exists(const String& value) = 0;

        virtual String getDescription(const String& value, const String& objectName, const String& propertyName)
        {
            Ogre::StringUtil::StrStreamType str;
            str << "Reference by object '" << objectName
                << "' in property '" << propertyName << "'";
            return str.str();
        }

        virtual StringVectorPtr getRecommendValues(const String& value) = 0;
        virtual StringVectorPtr getPossibleValues(const String& value) = 0;
    };

    class MeshLocator : public Locator
    {
    public:
        Ogre::MeshManager& mm;
        Ogre::ResourceGroupManager& rgm;
        String groupName;
        StringVectorPtr possibles;

        MeshLocator(const String& groupName)
            : mm(Ogre::MeshManager::getSingleton())
            , rgm(Ogre::ResourceGroupManager::getSingleton())
            , groupName(groupName)
            , possibles()
        {
        }

        bool exists(const String& value)
        {
            return !mm.getByName(value).isNull() || rgm.resourceExists(groupName, value);
        }

        StringVectorPtr getRecommendValues(const String& value)
        {
            String basename, path;
            Ogre::StringUtil::splitFilename(value, basename, path);
            return rgm.findResourceNames(groupName, basename);
        }

        StringVectorPtr getPossibleValues(const String& value)
        {
            if (possibles.isNull())
                possibles = rgm.findResourceNames(groupName, "*.mesh");
            return possibles;
        }
    };

    }   // end of unnamed namespace

    ResourceSolverPtr findUnsolvedSceneItems(SceneInfo* sceneInfo)
    {
        SceneSolver* solver = new SceneSolver;
        ResourceSolverPtr holder(solver);

        MeshLocator meshLocator(DEFAULT_RESOURCE_GROUP_NAME);

        typedef std::map<String, Locator*> LocatorMap;
        LocatorMap locators;
        locators["MeshName"] = &meshLocator;
        // TODO: other locators

        typedef std::pair<String, String> TypeValuePair;
        typedef std::map<TypeValuePair, UnsolvedSceneItem*> UnsolvedMap;
        UnsolvedMap unsolvedMap;

        const Scene::Objects& objects = sceneInfo->getObjects();
        for (Scene::Objects::const_iterator itObject = objects.begin(); itObject != objects.end(); ++itObject)
        {
            const ObjectPtr& object = *itObject;
            const PropertyList& properties = object->getProperties();
            for (PropertyList::const_iterator itProperty = properties.begin(); itProperty != properties.end(); ++itProperty)
            {
                const PropertyDef& propertyDef = *itProperty;
                LocatorMap::const_iterator itLocator = locators.find(propertyDef.type);
                if (itLocator == locators.end())
                    continue;

                Locator* locator = itLocator->second;
                assert(locator);

                String value = object->getPropertyAsString(propertyDef.name);
                TypeValuePair key(propertyDef.type, value);
                std::pair<UnsolvedMap::iterator, bool> inserted =
                    unsolvedMap.insert(UnsolvedMap::value_type(key, nullptr));
                if (!inserted.second)
                {
                    // The item already checked
                    if (!inserted.first->second)
                    {
                        // The item exists
                        continue;
                    }
                }
                else
                {
                    // First time to check the item
                    bool found = locator->exists(value);
                    if (found)
                    {
                        continue;
                    }

                    // Create new unsolved resource
                    UnsolvedSceneItem* item = new UnsolvedSceneItem(solver);
                    solver->unsolvedResources.push_back(UnsolvedResourcePtr(item));

                    // Setup origin name
                    item->origin = value;

                    // Setup description
                    item->description = locator->getDescription(value, object->getName(), propertyDef.name);

                    // Setup recommend values
                    item->recommends = locator->getRecommendValues(value);

                    // Setup possible values
                    item->possibles = locator->getPossibleValues(value);

                    inserted.first->second = item;
                }

                inserted.first->second->references.push_back(UnsolvedSceneItem::Reference(object, propertyDef.name));
            }
        }

        return holder;
    }

}
