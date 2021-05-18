#include "WXSceneSerializer.h"
#include "WXScene.h"
#include "WXObjectFactoryManager.h"
#include "WXResources.h"
#include "WXXMLAttributes.h"
#include "WXXMLHandler.h"
#include "WXXMLParser.h"
#include "WXXMLNode.h"
#include "WXXMLWriter.h"

namespace WX
{
static const String CURRENT_FORMAT_VERSION = "0.1.1";

/*!
\brief
	Handler class used to parse the Scene XML files using SAX2
*/
class SceneXMLHandler : public XMLHandler
{
public:
	/*************************************************************************
		Construction & Destruction
	*************************************************************************/

	/** 
		Constructor for SceneXMLHandler objects
	*/
	explicit SceneXMLHandler(Scene* scene);

	/** Destructor for SceneXMLHandler objects */
	~SceneXMLHandler(void);

	/*************************************************************************
		SAX2 Handler overrides
	*************************************************************************/ 

    /** Overridden from XMLHandler */
	virtual void characters(const String& chars);

    /** Overridden from XMLHandler */
	virtual void startElement(const String& element, const XMLAttributes& attributes);

    /** Overridden from XMLHandler */
    virtual void endElement(const String& element);

protected:

    typedef std::map<String, String> NameMap;
    typedef std::map<String, NameMap> NameCollectionMap;

    Scene* mScene;
    NameCollectionMap mPropertyNameCollectionMap;
    const NameMap* mPropertyNameMap;
    ObjectPtr mObject;
    String mAttributeName;
    String mAttributeValue;
    String* mString;
};

//////////////////////////////////////////////////////////////////////////

SceneXMLHandler::SceneXMLHandler(Scene* scene)
    : mScene(scene)
    , mPropertyNameCollectionMap()
    , mPropertyNameMap()
    , mObject()
    , mAttributeName()
    , mAttributeValue()
    , mString()
{
}

SceneXMLHandler::~SceneXMLHandler(void)
{
}

//////////////////////////////////////////////////////////////////////////

void SceneXMLHandler::characters(const String& chars)
{
    if (mString)
    {
        mString->append(chars);
    }
}

void
SceneXMLHandler::startElement(const String& element, const XMLAttributes& attributes)
{
    if (element == "Property")
    {
        assert(mObject);

        const String* name = &attributes.getValue("name");
        const String& value = attributes.getValue("value");
        if (mPropertyNameMap)
        {
            NameMap::const_iterator it = mPropertyNameMap->find(*name);
            if (it != mPropertyNameMap->end())
                name = &it->second;
        }

        mObject->setPropertyAsString(*name, value);
    }
    else if (element == "Object")
    {
        assert(!mObject);

        const String& type = attributes.getValue("type");

        mObject = ObjectFactoryManager::getSingleton().createInstance(type);
        if (attributes.exists("name"))
            mObject->setName(attributes.getValue("name"));
        else
            mObject->setName(mScene->generateAutoName(mObject));
        mScene->addObject(mObject);

        NameCollectionMap::const_iterator it = mPropertyNameCollectionMap.find(type);
        if (it != mPropertyNameCollectionMap.end())
        {
            mPropertyNameMap = &it->second;
        }
    }
    else if (element == "Attribute")
    {
        assert(!mString);
        mAttributeName = attributes.getValue("name");
        mAttributeValue.clear();
        mString = &mAttributeValue;
    }
    else if (element == "Terrain")
    {
        mScene->mTerrainFilename = attributes.getValue("filename");
    }
    else if (element == "Scene")
    {
        String formatVersion = attributes.getValue("formatVersion");
        if (formatVersion != CURRENT_FORMAT_VERSION &&
            formatVersion != "0.1.0")
        {
            OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR,
                "Unexpected format version was found while parsing the scene file."
                "\nformat version require: " + CURRENT_FORMAT_VERSION +
                "\nformat version in scene file: " + formatVersion,
                "SceneXMLHandler::startElement");
        }

        mScene->mName = attributes.getValueAs<String>("name", Ogre::StringUtil::BLANK);

        if (formatVersion == "0.1.0")
        {
            mPropertyNameCollectionMap.clear();

            NameMap& enviroment = mPropertyNameCollectionMap["Enviroment"];
            enviroment["fog mode"] = "fog.mode";
            enviroment["fog colour"] = "fog.colour";
            enviroment["fog exp density"] = "fog.exp density";
            enviroment["fog linear start"] = "fog.linear start";
            enviroment["fog linear end"] = "fog.linear end";
        }
    }
    else if (element == "Author")
    {
        assert(!mString);
        mString = &mScene->mAuthor;
    }
    else if (element == "Copyright")
    {
        assert(!mString);
        mString = &mScene->mCopyright;
    }
    else if (element == "Description")
    {
        assert(!mString);
        mString = &mScene->mDescription;
    }
    // anything else is an error which *should* have already been caught by XML validation
    else
    {
        OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR,
            "Unexpected data was found while parsing the scene file: '" + element + "' is unknown.",
            "SceneXMLHandler::startElement");
    }
}

void
SceneXMLHandler::endElement(const String& element)
{
    if (element == "Object")
    {
        assert(mObject);
        mObject.reset();
        mPropertyNameMap = NULL;
    }
    else if (element == "Attribute")
    {
        assert(mString);
        mString = NULL;
        mScene->setAttribute(mAttributeName, mAttributeValue);
    }
    else if (element == "Author")
    {
        assert(mString);
        mString = NULL;
    }
    else if (element == "Copyright")
    {
        assert(mString);
        mString = NULL;
    }
    else if (element == "Description")
    {
        assert(mString);
        mString = NULL;
    }
}

//////////////////////////////////////////////////////////////////////////

SceneSerializer::SceneSerializer(void)
{
}

SceneSerializer::~SceneSerializer()
{
}

void
SceneSerializer::load(Scene* scene, XMLParser* parser, const String& filename, const String& groupName)
{
    assert(scene);
    assert(parser);

    if (filename.empty())
    {
        OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS,
            "Filename supplied for scene loading must be valid",
            "SceneSerializer::load");
    }

    //--------------------------------------------------------------------

    // setup handler object
    SceneXMLHandler handler(scene);

    // parse the scene XML file
    parser->parseXMLFile(handler, filename, groupName,
        SCENE_SCHEMA_NAME, GRAMMAR_RESOURCE_GROUP_NAME);
}

void
SceneSerializer::save(Scene* scene, XMLWriter* writer, const String& filename)
{
    assert(scene);
    assert(writer);

    if (filename.empty())
    {
        OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS,
            "Filename supplied for scene saveing must be valid",
            "SceneSerializer::save");
    }

    //--------------------------------------------------------------------

    std::auto_ptr<XMLNode> root(new XMLNode("Scene"));

    root->setAttribute("formatVersion", CURRENT_FORMAT_VERSION);
    root->setAttribute("name", scene->mName, Ogre::StringUtil::BLANK);

    if (!scene->mAuthor.empty())
    {
        XMLNode* e = root->createChild("Author");
        e->setTextContent(scene->mAuthor);
    }

    if (!scene->mCopyright.empty())
    {
        XMLNode* e = root->createChild("Copyright");
        e->setTextContent(scene->mCopyright);
    }

    if (!scene->mDescription.empty())
    {
        XMLNode* e = root->createChild("Description");
        e->setTextContent(scene->mDescription);
    }

    if (!scene->mTerrainFilename.empty())
    {
        XMLNode* e = root->createChild("Terrain");
        e->setAttribute("filename", scene->mTerrainFilename);
    }

    const Scene::Attributes& attributes = scene->getAttributes();
    for (Scene::Attributes::const_iterator itAttribute = attributes.begin(); itAttribute != attributes.end(); ++itAttribute)
    {
        const String& name = itAttribute->first;
        const String& value = itAttribute->second;
        XMLNode* e = root->createChild("Attribute");
        e->setAttribute("name", name);
        e->setTextContent(value);
    }

    const Scene::Objects& objects = scene->getObjects();
    for (Scene::Objects::const_iterator itObject = objects.begin(); itObject != objects.end(); ++itObject)
    {
        const ObjectPtr& object = *itObject;
        XMLNode* n = root->createChild("Object");
        n->setAttribute("type", object->getType());
        if (!scene->isAutoName(object->getName()))
        {
            n->setAttribute("name", object->getName());
        }

        const PropertyList& properties = object->getProperties();
        for (PropertyList::const_iterator itProperty = properties.begin(); itProperty != properties.end(); ++itProperty)
        {
            const String& name = itProperty->name;
            if (!(object->getPropertyFlags(name) & PF_DERIVED))
            {
                String value = object->getPropertyAsString(name);
                if (value != itProperty->defaultValue)
                {
                    XMLNode* e = n->createChild("Property");
                    e->setAttribute("name", name);
                    e->setAttribute("value", value);
                }
            }
        }
    }

    writer->writeXMLFile(root.get(), filename);
}

}
