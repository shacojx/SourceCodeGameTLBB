#include "TerrainData.h"
#include "WXResources.h"

#include "WXXMLParser.h"
#include "WXXMLWriter.h"
#include "WXXMLHandler.h"
#include "WXXMLAttributes.h"
#include "WXXMLNode.h"

#include <OgreException.h>

//////////////////////////////////////////////////////////////////////////
// All XML relative stuff place here, so we can replace another
// XML implementation easy
//////////////////////////////////////////////////////////////////////////

namespace WX {

/*!
\brief
	Handler class used to parse the Configuration XML files using SAX2
*/
class TerrainData_xmlHandler : public XMLHandler
{
public:
	/*************************************************************************
		Construction & Destruction
	*************************************************************************/
	/*!
	\brief
		Constructor for TerrainData_xmlHandler objects
	*/
	TerrainData_xmlHandler(TerrainData* data);

	/*!
	\brief
		Destructor for TerrainData_xmlHandler objects
	*/
	~TerrainData_xmlHandler(void);

	/*************************************************************************
		SAX2 Handler overrides
	*************************************************************************/ 
	/*!
	\brief
		document processing (only care about elements, schema validates format)
	*/
	virtual void startElement(const String& element, const XMLAttributes& attrs);

protected:

    TerrainData* mData;
};

static void
readColourRGB(const XMLAttributes& attrs, Ogre::ColourValue& colour)
{
    colour.r = attrs.getValueAs<Real>("r");
    colour.g = attrs.getValueAs<Real>("g");
    colour.b = attrs.getValueAs<Real>("b");
}

static XMLNode*
writeColourRGB(XMLNode* node, const Ogre::ColourValue& colour)
{
    node->setAttribute("r", colour.r);
    node->setAttribute("g", colour.g);
    node->setAttribute("b", colour.b);
    return node;
}

//////////////////////////////////////////////////////////////////////////

TerrainData_xmlHandler::TerrainData_xmlHandler(TerrainData* data)
    : mData(data)
{
}

TerrainData_xmlHandler::~TerrainData_xmlHandler(void)
{
}

void
TerrainData_xmlHandler::startElement(const String& element, const XMLAttributes& attrs)
{
    if (element == "Terrain")
    {
        mData->mName = attrs.getValueAs<String>("name", Ogre::StringUtil::BLANK);
        mData->mXSize = attrs.getValueAs<uint>("xsize");
        mData->mZSize = attrs.getValueAs<uint>("zsize");
        mData->mTileSize = attrs.getValueAs<uint>("tileSize");
    }
    else if (element == "scale")
    {
        mData->mScale = Ogre::Vector3(attrs.getValueAs<Real>("x"),
                                      attrs.getValueAs<Real>("y"),
                                      attrs.getValueAs<Real>("z"));

        if (mData->mScale.x <= 0 || mData->mScale.y <= 0 || mData->mScale.z <= 0)
        {
            OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS,
                "Invalidate scale definition in terrain file",
                "TerrainData_xmlHandler::startElement");
        }

        mData->mPosition = Ogre::Vector3(- mData->mScale.x * mData->mXSize / 2,
                                           0,
                                         - mData->mScale.z * mData->mZSize / 2);
    }
    else if (element == "size")
    {
        mData->mScale = Ogre::Vector3(attrs.getValueAs<Real>("x") / mData->mXSize,
                                      attrs.getValueAs<Real>("y"),
                                      attrs.getValueAs<Real>("z") / mData->mZSize);

        if (mData->mScale.x <= 0 || mData->mScale.y <= 0 || mData->mScale.z <= 0)
        {
            OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS,
                "Invalidate size definition in terrain file",
                "TerrainData_xmlHandler::startElement");
        }

        mData->mPosition = Ogre::Vector3(- mData->mScale.x * mData->mXSize / 2,
                                           0,
                                         - mData->mScale.z * mData->mZSize / 2);
    }
    else if (element == "position")
    {
        mData->mPosition = Ogre::Vector3(attrs.getValueAs<Real>("x"),
                                         attrs.getValueAs<Real>("y"),
                                         attrs.getValueAs<Real>("z"));
    }
    else if (element == "centre" || element == "center")
    {
        mData->mPosition = Ogre::Vector3(attrs.getValueAs<Real>("x") - mData->mScale.x * mData->mXSize / 2,
                                         attrs.getValueAs<Real>("y"),
                                         attrs.getValueAs<Real>("z") - mData->mScale.z * mData->mZSize / 2);
    }
    else if (element == "heightmap")
    {
        const String& type = attrs.getValueAs<String>("type", "standard");
        const String& filename = attrs.getValue("filename");

        if (type != "image" && type != "standard" && type != "raw")
        {
            OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS,
                "Invalidate height data type",
                "TerrainData_xmlHandler::startElement");
        }

        mData->mHeightmapType = type;
        mData->mHeightmapFilename = filename;
        if (mData->mHeightmapFilename.empty())
        {
            OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS,
                "Invalidate height data filename",
                "TerrainData_xmlHandler::startElement");
        }
    }
    else if (element == "gridInfo")
    {
        const String& type = attrs.getValueAs<String>("type", "standard");
        const String& filename = attrs.getValue("filename");

        if (type != "standard")
        {
            OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS,
                "Invalidate height info type",
                "TerrainData_xmlHandler::startElement");
        }

        mData->mGridInfoFilename = filename;
        if (mData->mGridInfoFilename.empty())
        {
            OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS,
                "Invalidate grid info filename",
                "TerrainData_xmlHandler::startElement");
        }
    }    
    else if (element == "lightmap")
    {   
        const String& type = attrs.getValueAs<String>("type", "standard");
        const String& filename = attrs.getValue("filename");

        if (type != "standard" &&
			type != "image")
        {
            OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS,
                "Invalidate lightmap type",
                "TerrainData_xmlHandler::startElement");
        }

        mData->mLightmapType = type;
        mData->mLightmapFilename = filename;
        if (mData->mLightmapFilename.empty())
        {
            OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS,
                "Invalidate lightmap filename",
                "TerrainData_xmlHandler::startElement");
        }
    }
    else if (element == "pixmap")
    {
        TerrainData::Pixmap pixmap;
        pixmap.textureId = attrs.getValueAs<uint>("textureId");
        pixmap.left = attrs.getValueAs<Real>("left", 0);
        pixmap.top = attrs.getValueAs<Real>("top", 0);
        pixmap.right = attrs.getValueAs<Real>("right", 1);
        pixmap.bottom = attrs.getValueAs<Real>("bottom", 1);

        if (!(0 <= pixmap.left && pixmap.left <= 1 &&
              0 <= pixmap.top && pixmap.top <= 1 &&
              0 <= pixmap.right && pixmap.right <= 1 &&
              0 <= pixmap.bottom && pixmap.bottom <= 1))
        {
            OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS,
                "Invalidate pixmap definition in terrain file",
                "TerrainData_xmlHandler::startElement");
        }

        mData->mPixmaps.push_back(pixmap);
    }
    else if (element == "texture")
    {
        const String& type = attrs.getValueAs<String>("type", "image");
        const String& filename = attrs.getValue("filename");

        if (type != "image")
        {
            OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS,
                "Invalidate texture type",
                "TerrainData_xmlHandler::startElement");
        }

        mData->mTextures.push_back(filename);
    }
    else if (element == "template")
    {
        const String& name = attrs.getValue("name");
        const String& material = attrs.getValue("material");

        std::pair<TerrainData::MaterialTemplates::iterator, bool> inserted =
            mData->mMaterialTemplates.insert(TerrainData::MaterialTemplates::value_type(
                name, material));
        if (!inserted.second)
        {
            OGRE_EXCEPT(Ogre::Exception::ERR_DUPLICATE_ITEM,
                "Duplicate material template '" + inserted.first->first + "' in terrain file",
                "TerrainData_xmlHandler::startElement");
        }
    }
    else if (element == "fog_replacement")
    {
        TerrainData::FogReplacement fogReplacement;
        fogReplacement.programNames[Ogre::FOG_NONE] = attrs.getValue("none");
        fogReplacement.programNames[Ogre::FOG_EXP] = attrs.getValue("exp");
        fogReplacement.programNames[Ogre::FOG_EXP2] = attrs.getValue("exp2");
        fogReplacement.programNames[Ogre::FOG_LINEAR] = attrs.getValue("linear");
        mData->mFogReplacements.push_back(fogReplacement);
    }
    else if (element == "ambient")
    {
        readColourRGB(attrs, mData->mSurfaceParams.ambient);
    }
    else if (element == "diffuse")
    {
        readColourRGB(attrs, mData->mSurfaceParams.diffuse);
    }
    else if (element == "specular")
    {
        readColourRGB(attrs, mData->mSurfaceParams.specular);
        mData->mSurfaceParams.shininess = attrs.getValueAs<Real>("shininess", mData->msDefaultSurfaceParams.shininess);
    }
    else if (element == "emissive")
    {
        readColourRGB(attrs, mData->mSurfaceParams.emissive);
    }
    else if (element == "pixmaps")
    {
        // no attributes
    }
    else if (element == "textures")
    {
        // no attributes
    }
    else if (element == "materials")
    {
        // no attributes
    }
    else if (element == "surface")
    {
        // no attributes
    }
	// anything else is an error which *should* have already been caught by XML validation
	else
	{
        OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR,
            "Unexpected data was found while parsing the terrain file: '" + element + "' is unknown.",
            "TerrainData_xmlHandler::startElement");
	}
}

//////////////////////////////////////////////////////////////////////////

void
TerrainData::_loadBaseInfo(XMLParser* parser, const String& filename, const String& groupName)
{
    assert(parser);

	if (filename.empty())
    {
        OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS,
            "Filename supplied for terrain loading must be valid",
            "TerrainData::_loadBaseInfo");
    }

    //--------------------------------------------------------------------

	// setup handler object
	TerrainData_xmlHandler handler(this);

    // parse the terrain XML file
    parser->parseXMLFile(handler, filename, groupName,
        TERRAIN_SCHEMA_NAME, GRAMMAR_RESOURCE_GROUP_NAME);
}

void
TerrainData::_saveBaseInfo(XMLWriter* writer, const String& filename) const
{
    assert(writer);

	if (filename.empty())
    {
        OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS,
            "Filename supplied for terrain saving must be valid",
            "TerrainData::_saveBaseInfo");
    }

    //--------------------------------------------------------------------

    std::auto_ptr<XMLNode> root(new XMLNode("Terrain"));

    root->setAttribute("name", mName, Ogre::StringUtil::BLANK);
    root->setAttribute("xsize", mXSize);
    root->setAttribute("zsize", mZSize);
    root->setAttribute("tileSize", mTileSize);	

    if (mScale != Ogre::Vector3::UNIT_SCALE)
    {
        XMLNode* n = root->createChild("scale");
        n->setAttribute("x", mScale.x);
        n->setAttribute("y", mScale.y);
        n->setAttribute("z", mScale.z);
    }

    if (mCentre != Ogre::Vector3::ZERO)
    {
        XMLNode* n = root->createChild("centre");
        n->setAttribute("x", mCentre.x);
        n->setAttribute("y", mCentre.y);
        n->setAttribute("z", mCentre.z);
    }

    if (!mHeightmapFilename.empty())
    {
        XMLNode* n = root->createChild("heightmap");
        n->setAttribute("type", mHeightmapType, Ogre::StringUtil::BLANK);
        n->setAttribute("filename", mHeightmapFilename);
    }

    if (!mGridInfoFilename.empty())
    {
        XMLNode* n = root->createChild("gridInfo");
        n->setAttribute("type", "standard");
        n->setAttribute("filename", mGridInfoFilename);
    }    

    if (!mLightmapFilename.empty())
    {
        XMLNode* n = root->createChild("lightmap");
        n->setAttribute("type", mLightmapType, Ogre::StringUtil::BLANK);
        n->setAttribute("filename", mLightmapFilename);
    }

    if (!mTextures.empty())
    {
        XMLNode* t = root->createChild("textures");
        for (TextureArray::const_iterator it = mTextures.begin(); it != mTextures.end(); ++it)
        {
            const String& texture = *it;
            XMLNode* n = t->createChild("texture");
            n->setAttribute("type", "image");
            n->setAttribute("filename", texture);
        }
    }

    if (!mPixmaps.empty())
    {
        XMLNode* t = root->createChild("pixmaps");
        for (PixmapArray::const_iterator it = mPixmaps.begin(); it != mPixmaps.end(); ++it)
        {
            const Pixmap& pixmap = *it;
            XMLNode* n = t->createChild("pixmap");
            n->setAttribute("textureId", pixmap.textureId);
            if (pixmap.left != 0 || pixmap.top != 0 || pixmap.right != 1 || pixmap.bottom != 1)
            {
                n->setAttribute("left", pixmap.left);
                n->setAttribute("top", pixmap.top);
                n->setAttribute("right", pixmap.right);
                n->setAttribute("bottom", pixmap.bottom);
            }
        }
    }

    if (!mMaterialTemplates.empty() ||
        !mFogReplacements.empty() ||
        mSurfaceParams != msDefaultSurfaceParams)
    {
        XMLNode* t = root->createChild("materials");

        for (MaterialTemplates::const_iterator it = mMaterialTemplates.begin(); it != mMaterialTemplates.end(); ++it)
        {
            const String& name = it->first;
            const String& material = it->second;
            XMLNode* n = t->createChild("template");
            n->setAttribute("name", name);
            n->setAttribute("material", material);
        }

        for (FogReplacements::const_iterator it = mFogReplacements.begin(); it != mFogReplacements.end(); ++it)
        {
            const FogReplacement& fogReplacement = *it;
            XMLNode* n = t->createChild("fog_replacement");
            n->setAttribute("none", fogReplacement.programNames[Ogre::FOG_NONE]);
            n->setAttribute("exp", fogReplacement.programNames[Ogre::FOG_EXP]);
            n->setAttribute("exp2", fogReplacement.programNames[Ogre::FOG_EXP2]);
            n->setAttribute("linear", fogReplacement.programNames[Ogre::FOG_LINEAR]);
        }

        if (mSurfaceParams != msDefaultSurfaceParams)
        {
            XMLNode* s = t->createChild("surface");

            if (mSurfaceParams.ambient != msDefaultSurfaceParams.ambient)
            {
                writeColourRGB(s->createChild("ambient"), mSurfaceParams.ambient);
            }

            if (mSurfaceParams.diffuse != msDefaultSurfaceParams.diffuse)
            {
                writeColourRGB(s->createChild("diffuse"), mSurfaceParams.diffuse);
            }

            if (mSurfaceParams.specular != msDefaultSurfaceParams.specular ||
                mSurfaceParams.shininess != msDefaultSurfaceParams.shininess)
            {
                writeColourRGB(s->createChild("specular"), mSurfaceParams.specular)
                    ->setAttribute("shininess", mSurfaceParams.shininess, msDefaultSurfaceParams.shininess);
            }

            if (mSurfaceParams.emissive != msDefaultSurfaceParams.emissive)
            {
                writeColourRGB(s->createChild("emissive"), mSurfaceParams.emissive);
            }
        }
    }

    writer->writeXMLFile(root.get(), filename);
}

void
TerrainData::_fixupMissingInfo(void)
{
    if (mFogReplacements.empty())
    {
        FogReplacement fogReplacement;

        fogReplacement.programNames[Ogre::FOG_NONE] = "Terrain/OneLayer_ps";
        fogReplacement.programNames[Ogre::FOG_EXP] = "Terrain/OneLayer_ps%fog_exp";
        fogReplacement.programNames[Ogre::FOG_EXP2] = "Terrain/OneLayer_ps%fog_exp2";
        fogReplacement.programNames[Ogre::FOG_LINEAR] = "Terrain/OneLayer_ps%fog_linear";
        mFogReplacements.push_back(fogReplacement);

        fogReplacement.programNames[Ogre::FOG_NONE] = "Terrain/TwoLayer_ps";
        fogReplacement.programNames[Ogre::FOG_EXP] = "Terrain/TwoLayer_ps%fog_exp";
        fogReplacement.programNames[Ogre::FOG_EXP2] = "Terrain/TwoLayer_ps%fog_exp2";
        fogReplacement.programNames[Ogre::FOG_LINEAR] = "Terrain/TwoLayer_ps%fog_linear";
        mFogReplacements.push_back(fogReplacement);

        fogReplacement.programNames[Ogre::FOG_NONE] = "Terrain/OneLayerLightmap_ps";
        fogReplacement.programNames[Ogre::FOG_EXP] = "Terrain/OneLayerLightmap_ps%fog_exp";
        fogReplacement.programNames[Ogre::FOG_EXP2] = "Terrain/OneLayerLightmap_ps%fog_exp2";
        fogReplacement.programNames[Ogre::FOG_LINEAR] = "Terrain/OneLayerLightmap_ps%fog_linear";
        mFogReplacements.push_back(fogReplacement);

        fogReplacement.programNames[Ogre::FOG_NONE] = "Terrain/TwoLayerLightmap_ps";
        fogReplacement.programNames[Ogre::FOG_EXP] = "Terrain/TwoLayerLightmap_ps%fog_exp";
        fogReplacement.programNames[Ogre::FOG_EXP2] = "Terrain/TwoLayerLightmap_ps%fog_exp2";
        fogReplacement.programNames[Ogre::FOG_LINEAR] = "Terrain/TwoLayerLightmap_ps%fog_linear";
        mFogReplacements.push_back(fogReplacement);
    }
}

}
