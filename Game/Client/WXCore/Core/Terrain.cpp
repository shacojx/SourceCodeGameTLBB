#include "Terrain.h"
#include "TerrainTileEditable.h"
#include "TerrainTileOptimized.h"
#include "WXResources.h"
#include "ieee.h"

#include <OgreMaterialManager.h>
#include <OgreSceneManager.h>
#include <OgreStringConverter.h>
#include <OgreTextureManager.h>
#include <OgreHardwarePixelBuffer.h>
#include <OgreRoot.h>

// Mosaic Terrain ?

namespace WX {

struct Terrain::Atlas
{
    Ogre::SharedPtr<Ogre::Image> image;
    Ogre::TexturePtr texture;

    Atlas(void)
        : image()
        , texture()
    {
    }
};

Terrain::Terrain(void)
    : mData(NULL)

    , mTiles(NULL)
    , mMaterials()
    , mCurrentFogMode(Ogre::FOG_NONE)

    , mLightmapTextures()
    , mLightmapTextureSize(1024)
    , mLightmapQuality(LMQ_HIGH)

    , mIndexBuffer()
    , mIndexDatas()

    , mEditable(false)
    , mDisplayGridLine(false)
    , mDisplayGridType(false)
{
    mData = new TerrainData;

    mAtlasTextureSize = 512;
    mAtlasPixmapSize = 64;
    mAtlasNumMipMaps = 6;
    assert(mAtlasPixmapSize >= ((size_t)1 << mAtlasNumMipMaps));
    mAtlasBlockSize = mAtlasTextureSize / mAtlasPixmapSize;
    mMaxAtlasBlockId = mAtlasBlockSize * mAtlasBlockSize;
    mAtlasBlockTexCoordInc = 1.0f / mAtlasBlockSize;

    // Mark as invalidate
    mSolidAtlasAllocInfo.blockId = mMaxAtlasBlockId;
    mTransparentAtlasAllocInfo.blockId = mMaxAtlasBlockId;
}

Terrain::~Terrain()
{
    clearGeometry();
    delete mData;
}

void
Terrain::clearGeometry(void)
{
    _deleteTileList();
    _deleteMaterials();
    _deleteIndexDatas();

    mGridLineInfos.clear();
    mGridTypeInfos.clear();
}

void
Terrain::reset()
{
    clearGeometry();
    mData->reset();
}

void
Terrain::_deleteTileList(void)
{
    for (TileList::const_iterator it = mTiles.begin(); it != mTiles.end(); ++it)
    {
        TerrainTile* tile = *it;
        delete tile;
    }
    mTiles.clear();
}

void
Terrain::_initIndexBuffer(size_t maxQuads)
{
    assert(maxQuads <= 65536/4);

    mIndexBuffer =
        Ogre::HardwareBufferManager::getSingleton().createIndexBuffer(
            Ogre::HardwareIndexBuffer::IT_16BIT,
            maxQuads * 6,
            Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);

    ushort* pIndex = static_cast<unsigned short*>(
        mIndexBuffer->lock(0, mIndexBuffer->getSizeInBytes(), Ogre::HardwareBuffer::HBL_DISCARD));

    for (size_t i = 0; i < maxQuads; ++i)
    {
        // NB: We should take care with the clockwise ordered quad vertices

        *pIndex++ = i*4 + 1;
        *pIndex++ = i*4 + 0;
        *pIndex++ = i*4 + 2;

        *pIndex++ = i*4 + 1;
        *pIndex++ = i*4 + 2;
        *pIndex++ = i*4 + 3;
    }

    mIndexBuffer->unlock();
}

Ogre::IndexData*
Terrain::_getIndexData(size_t numQuads)
{
    assert(!mIndexBuffer.isNull());
    assert(numQuads * 6 <= mIndexBuffer->getNumIndexes());

    Ogre::IndexData* indexData;

    IndexDataMap::const_iterator it = mIndexDatas.find(numQuads);
    if (it != mIndexDatas.end())
    {
        indexData = it->second;
    }
    else
    {
        indexData = new Ogre::IndexData;
        indexData->indexBuffer = mIndexBuffer;
        indexData->indexStart = 0;
        indexData->indexCount = numQuads * 6;

        std::pair<IndexDataMap::iterator, bool> inserted =
            mIndexDatas.insert(IndexDataMap::value_type(numQuads, indexData));
        assert(inserted.second && "Internal fault while create index data");
    }

    return indexData;
}

void
Terrain::_deleteIndexDatas(void)
{
    for (IndexDataMap::const_iterator it = mIndexDatas.begin(); it != mIndexDatas.end(); ++it)
    {
        Ogre::IndexData* indexData = it->second;
        delete indexData;
    }
    mIndexDatas.clear();

    mIndexBuffer.setNull();
}

void
Terrain::loadResource(Ogre::Resource* resource)
{
	const String& name = resource->getName();
    if (name.find("<Lightmap>") != String::npos)
	{
        String::size_type left_parentheses = name.find_first_of('(');
        String::size_type right_parentheses = name.find_last_of(')');
        if (left_parentheses == String::npos ||
            right_parentheses == String::npos)
        {
            OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR,
                "Unorganised lightmap texture name '" + name + "'",
                "Terrain::loadResource");
        }

        Ogre::StringVector vec = Ogre::StringUtil::split(
            name.substr(left_parentheses + 1, right_parentheses - left_parentheses - 1), ",", 2);
        if (vec.size() != 2)
        {
            OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR,
                "Unorganised lightmap texture name '" + name + "'",
                "Terrain::loadResource");
        }

        int nPosX = Ogre::StringConverter::parseInt(vec[0]);
        int nPosZ = Ogre::StringConverter::parseInt(vec[1]);
        if (nPosX < 0 || nPosX >= mData->mNumTilePerX ||
            nPosZ < 0 || nPosZ >= mData->mNumTilePerZ)
        {
            OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR,
                "Unorganised lightmap texture name '" + name + "'",
                "Terrain::loadResource");
        }

        uint uWidth, uHeight;
        if (getLightmapQuality() == LMQ_LOW)
        {
            uWidth = mData->mTileSize * 2;
            uHeight = mData->mTileSize * 2;
        }
        else
        {
            uWidth = mData->mTileSize * 8;
            uHeight = mData->mTileSize * 8;
        }

		Ogre::Texture* pTexture = static_cast<Ogre::Texture*>(resource);
        if (pTexture->getWidth() != uWidth || pTexture->getHeight() != uHeight)
        {
            pTexture->freeInternalResources();
            pTexture->setWidth(uWidth);
            pTexture->setHeight(uHeight);
        }
		pTexture->createInternalResources();

		Ogre::HardwarePixelBufferSharedPtr bufferPtr = pTexture->getBuffer();
        size_t l = nPosX * mData->mTileSize * 8;
        size_t t = nPosZ * mData->mTileSize * 8;
        size_t r = l + mData->mTileSize * 8;
        size_t b = t + mData->mTileSize * 8;
        if (r > mData->mLightmapImage->getWidth())
            r = mData->mLightmapImage->getWidth();
        if (b > mData->mLightmapImage->getHeight())
            b = mData->mLightmapImage->getHeight();
		bufferPtr->blitFromMemory(
			mData->mLightmapImage->getPixelBox().getSubVolume(Ogre::Box(l, t, r, b)));
	}
	else
	{
		for (AtlasArray::const_iterator it = mAtlases.begin(); it != mAtlases.end(); ++it)
		{
			if (it->texture.get() == resource)
			{
                assert(!it->image.isNull());
				it->texture->loadImage(*it->image);
				break;
			}
		}
	}
}

size_t
Terrain::_getPixmapAtlasId(size_t pixmapId)
{
    assert(pixmapId < mData->mPixmaps.size());
    if (pixmapId >= mAtlasPixmaps.size())
        mAtlasPixmaps.resize(pixmapId + 1);

    if (!mAtlasPixmaps[pixmapId].atlasId)
    {
        _createAtlasPixmap(pixmapId);
    }

    assert(mAtlasPixmaps[pixmapId].atlasId > 0 && mAtlasPixmaps[pixmapId].atlasId <= mAtlases.size());
    return mAtlasPixmaps[pixmapId].atlasId - 1;
}

void
Terrain::_createAtlasPixmap(size_t pixmapId)
{
    const TerrainData::Pixmap& pixmap = mData->mPixmaps[pixmapId];
    size_t textureId = pixmap.textureId;
    assert(textureId < mData->mTextures.size());
    const String& textureName = mData->mTextures[textureId];

    // If the atlas texture already exist, use it.

    AtlasArray::const_iterator it;
    for (it = mAtlases.begin(); it != mAtlases.end(); ++it)
    {
        if (it->texture->getName() == textureName)
            break;
    }
    if (it != mAtlases.end())
    {
        // Fill up atlas pixmap info
        size_t atlasId = it - mAtlases.begin() + 1;
        mAtlasPixmaps[pixmapId].atlasId = atlasId;
        mAtlasPixmaps[pixmapId].left = pixmap.left;
        mAtlasPixmaps[pixmapId].top = pixmap.top;
        mAtlasPixmaps[pixmapId].right = pixmap.right;
        mAtlasPixmaps[pixmapId].bottom = pixmap.bottom;
        return;
    }

    // If texture already loaded and is composited, use it without any modify.
    Ogre::TexturePtr texture = Ogre::TextureManager::getSingleton().getByName(textureName);
    if (!texture.isNull() &&
        (texture->getWidth() > mAtlasPixmapSize || texture->getHeight() > mAtlasPixmapSize))
    {
        mAtlases.push_back(Atlas());
        Atlas& atlas = mAtlases.back();

        atlas.texture = texture;

        // Fill up atlas pixmap info
        size_t atlasId = mAtlases.size();
        mAtlasPixmaps[pixmapId].atlasId = atlasId;
        mAtlasPixmaps[pixmapId].left = pixmap.left;
        mAtlasPixmaps[pixmapId].top = pixmap.top;
        mAtlasPixmaps[pixmapId].right = pixmap.right;
        mAtlasPixmaps[pixmapId].bottom = pixmap.bottom;
        return;
    }

    // Load the image
    Ogre::Image image;
    image.load(textureName, BRUSH_RESOURCE_GROUP_NAME);

    // If the image is composited, use it without any modify.
    if (image.getWidth() > mAtlasPixmapSize || image.getHeight() > mAtlasPixmapSize)
    {
        mAtlases.push_back(Atlas());
        Atlas& atlas = mAtlases.back();

        // re-use the loaded image avoid load it again
        atlas.texture =
            Ogre::TextureManager::getSingleton()
            .loadImage(textureName, BRUSH_RESOURCE_GROUP_NAME, image);

        // Fill up atlas pixmap info
        size_t atlasId = mAtlases.size();
        mAtlasPixmaps[pixmapId].atlasId = atlasId;
        mAtlasPixmaps[pixmapId].left = pixmap.left;
        mAtlasPixmaps[pixmapId].top = pixmap.top;
        mAtlasPixmaps[pixmapId].right = pixmap.right;
        mAtlasPixmaps[pixmapId].bottom = pixmap.bottom;
        return;
    }

    // Composite into the atlas texture.

    bool isTransparent = image.getHasAlpha();
    AtlasAllocInfo& allocInfo = isTransparent ? mTransparentAtlasAllocInfo : mSolidAtlasAllocInfo;
    if (allocInfo.blockId >= mMaxAtlasBlockId)
    {
        // Use special name to avoid confuse with other reference with this texture
        String atlasName = "<Terrain/Atlas>:" + Ogre::StringConverter::toString(mAtlases.size());

        mAtlases.push_back(Atlas());
        Atlas& atlas = mAtlases.back();

        Ogre::PixelFormat pixelFormat = isTransparent ? Ogre::PF_A8R8G8B8 : Ogre::PF_X8R8G8B8;
        atlas.image.bind(new Ogre::Image);
        atlas.image->loadDynamicImage(0, mAtlasTextureSize, mAtlasTextureSize, 1, pixelFormat, true, 1, mAtlasNumMipMaps);
        memset(atlas.image->getData(), 0, atlas.image->getSize());

        atlas.texture =
            Ogre::TextureManager::getSingleton().createManual(atlasName,
                BRUSH_RESOURCE_GROUP_NAME, Ogre::TEX_TYPE_2D,
                atlas.image->getWidth(), atlas.image->getHeight(),
                mAtlasNumMipMaps, atlas.image->getFormat(),
                Ogre::TU_DYNAMIC_WRITE_ONLY_DISCARDABLE,
                this);

        allocInfo.atlasId = mAtlases.size();
        allocInfo.blockId = 0;
    }

    // Copy origin pixmap to atlas image
    Atlas& atlas = mAtlases[allocInfo.atlasId - 1];
    int blockX = allocInfo.blockId % mAtlasBlockSize;
    int blockY = allocInfo.blockId / mAtlasBlockSize;
    Ogre::PixelBox dst = atlas.image->getPixelBox().getSubVolume(Ogre::Box(
        blockX * mAtlasPixmapSize,
        blockY * mAtlasPixmapSize,
        blockX * mAtlasPixmapSize + mAtlasPixmapSize,
        blockY * mAtlasPixmapSize + mAtlasPixmapSize));
    Ogre::PixelBox src = image.getPixelBox().getSubVolume(Ogre::Box(
        fast_ifloor(pixmap.left * image.getWidth() + 0.5f),
        fast_ifloor(pixmap.top * image.getHeight() + 0.5f),
        fast_ifloor(pixmap.right * image.getWidth() + 0.5f),
        fast_ifloor(pixmap.bottom * image.getHeight() + 0.5f)));
    if (src.getWidth() == mAtlasPixmapSize && src.getHeight() == mAtlasPixmapSize)
        Ogre::PixelUtil::bulkPixelConversion(src, dst);
    else
        Ogre::Image::scale(src, dst);

    // Generate mipmaps manual
    for (size_t mipmap = 1; mipmap <= mAtlasNumMipMaps; ++mipmap)
    {
        src = dst;
        size_t pixmapSize = mAtlasPixmapSize >> mipmap;
        dst = atlas.image->getPixelBox(0, mipmap).getSubVolume(Ogre::Box(
            blockX * pixmapSize,
            blockY * pixmapSize,
            blockX * pixmapSize + pixmapSize,
            blockY * pixmapSize + pixmapSize));
        Ogre::Image::scale(src, dst);
    }

    // Notify that the atlas texture need to reload
    if (atlas.texture->isLoaded())
        atlas.texture->unload();

    ++allocInfo.blockId;

    // Fill up atlas pixmap info

    mAtlasPixmaps[pixmapId].atlasId = allocInfo.atlasId;
    mAtlasPixmaps[pixmapId].left = blockX * mAtlasBlockTexCoordInc + 0.5f / mAtlasTextureSize;
    mAtlasPixmaps[pixmapId].top = blockY * mAtlasBlockTexCoordInc + 0.5f / mAtlasTextureSize;
    mAtlasPixmaps[pixmapId].right = mAtlasPixmaps[pixmapId].left + mAtlasBlockTexCoordInc -  1.0f / mAtlasTextureSize;
    mAtlasPixmaps[pixmapId].bottom = mAtlasPixmaps[pixmapId].top + mAtlasBlockTexCoordInc -  1.0f / mAtlasTextureSize;
}

std::pair<Real, Real>
Terrain::_getPixmapCorner(const TerrainData::LayerInfo& layerInfo, TerrainData::Corner corner, uint flags) const
{
    assert(layerInfo.pixmapId && "Internal fault get pixmap corner");
	assert(0 <= corner && corner < 4 && "Invalid corner");
	assert(0 <= layerInfo.orientation && layerInfo.orientation < 16 && "Invalid orientation");

	// NOTE: This enum should be as same order as fields of 'Pixmap', search [Pixmap Fields Order] for detail
	enum {
		Left,
		Top,
		Right,
		Bottom,
	};

	// NOTE: This map is dependence 'OrientationFlags' and 'Corner', search [Corner Map] for detail
	static const int cornerIndexMap[2][16][4][2] =
	{
        Left,   Top,    Right,  Top,    Left,   Bottom, Right,  Bottom,
        Right,  Top,    Left,   Top,    Right,  Bottom, Left,   Bottom,
        Left,   Bottom, Right,  Bottom, Left,   Top,    Right,  Top,
        Right,  Bottom, Left,   Bottom, Right,  Top,    Left,   Top,

        //----------------------------------------

        Right,  Top,    Right,  Bottom, Left,   Top,    Left,   Bottom,
        Left,   Top,    Left,   Bottom, Right,  Top,    Right,  Bottom,
        Right,  Bottom, Right,  Top,    Left,   Bottom, Left,   Top,
        Left,   Bottom, Left,   Top,    Right,  Bottom, Right,  Top,

        //------------------------------------------------------------

        Left,   Top,    Right,  Top,    Left,   Bottom, Left,   Top,
        Right,  Top,    Left,   Top,    Right,  Bottom, Right,  Top,
        Left,   Bottom, Right,  Bottom, Left,   Top,    Left,   Bottom,
        Right,  Bottom, Left,   Bottom, Right,  Top,    Right,  Bottom,

        //----------------------------------------

        Right,  Top,    Right,  Bottom, Left,   Top,    Right,  Top,
        Left,   Top,    Left,   Bottom, Right,  Top,    Left,   Top,
        Right,  Bottom, Right,  Top,    Left,   Bottom, Right,  Bottom,
        Left,   Bottom, Left,   Top,    Right,  Bottom, Left,   Bottom,

        //----------------------------------------------------------------------

        Left,   Top,    Right,  Top,    Left,   Bottom, Right,  Bottom,
        Right,  Top,    Left,   Top,    Right,  Bottom, Left,   Bottom,
        Left,   Bottom, Right,  Bottom, Left,   Top,    Right,  Top,
        Right,  Bottom, Left,   Bottom, Right,  Top,    Left,   Top,

        //----------------------------------------

        Right,  Top,    Right,  Bottom, Left,   Top,    Left,   Bottom,
        Left,   Top,    Left,   Bottom, Right,  Top,    Right,  Bottom,
        Right,  Bottom, Right,  Top,    Left,   Bottom, Left,   Top,
        Left,   Bottom, Left,   Top,    Right,  Bottom, Right,  Top,

        //------------------------------------------------------------

        Left,   Top,    Right,  Top,    Right,  Top,    Right,  Bottom,
        Right,  Top,    Left,   Top,    Left,   Top,    Left,   Bottom,
        Left,   Bottom, Right,  Bottom, Right,  Bottom, Right,  Top,
        Right,  Bottom, Left,   Bottom, Left,   Bottom, Left,   Top,

        //----------------------------------------

        Right,  Top,    Right,  Bottom, Right,  Bottom, Left,   Bottom,
        Left,   Top,    Left,   Bottom, Left,   Bottom, Right,  Bottom,
        Right,  Bottom, Right,  Top,    Right,  Top,    Left,   Top,
        Left,   Bottom, Left,   Top,    Left,   Top,    Right,  Top,
    };

    assert(layerInfo.pixmapId <= mAtlasPixmaps.size());
    const AtlasPixmap& pixmap = mAtlasPixmaps[layerInfo.pixmapId-1];
    assert(pixmap.atlasId);
    const int *indices = cornerIndexMap[(flags & TerrainData::GridInfo::FlipDiagonal) != 0][layerInfo.orientation][corner];
	return std::pair<Real, Real>((&pixmap.left)[indices[0]], (&pixmap.left)[indices[1]]);
}

void
Terrain::resetSurfaceParams(void)
{
    for (MaterialMap::const_iterator it = mMaterials.begin(); it != mMaterials.end(); ++it)
    {
        _applySurfaceParams(it->second);
    }
}

void
Terrain::_applySurfaceParams(const Ogre::MaterialPtr& material) const
{
    material->setAmbient(mData->mSurfaceParams.ambient);
    material->setDiffuse(mData->mSurfaceParams.diffuse);
    material->setSpecular(mData->mSurfaceParams.specular);
    material->setSelfIllumination(mData->mSurfaceParams.emissive);
    material->setShininess(mData->mSurfaceParams.shininess);
}

void
Terrain::_updateMaterials(void) const
{
    if (!mData->mSupportedFogReplacements.empty())
    {
        Ogre::FogMode currentFogMode = Ogre::Root::getSingleton()._getCurrentSceneManager()->getFogMode();
        if (mCurrentFogMode != currentFogMode)
        {
            for (MaterialMap::const_iterator it = mMaterials.begin(); it != mMaterials.end(); ++it)
            {
                _applyFogMode(it->second, mCurrentFogMode, currentFogMode);
            }

            mCurrentFogMode = currentFogMode;
        }
    }
}

void
Terrain::_applyFogMode(const Ogre::MaterialPtr& material, Ogre::FogMode oldFogMode, Ogre::FogMode newFogMode) const
{
    if (oldFogMode == newFogMode)
        return;

    if (mData->mSupportedFogReplacements.empty())
        return;

    String newProgramName;

    Ogre::Material::TechniqueIterator ti = material->getTechniqueIterator();
    while (ti.hasMoreElements())
    {
        Ogre::Technique* technique = ti.getNext();
        Ogre::Technique::PassIterator pi = technique->getPassIterator();
        while (pi.hasMoreElements())
        {
            Ogre::Pass* pass = pi.getNext();
            if (pass->hasVertexProgram() &&
                _checkFogProgramName(pass->getVertexProgram(), oldFogMode, newFogMode, newProgramName))
            {
                pass->setVertexProgram(newProgramName);
            }
            if (pass->hasFragmentProgram() &&
                _checkFogProgramName(pass->getFragmentProgram(), oldFogMode, newFogMode, newProgramName))
            {
                pass->setFragmentProgram(newProgramName);
            }
            if (pass->hasShadowCasterVertexProgram() &&
                _checkFogProgramName(pass->getShadowCasterVertexProgram(), oldFogMode, newFogMode, newProgramName))
            {
                pass->setShadowCasterVertexProgram(newProgramName);
            }
            if (pass->hasShadowReceiverVertexProgram() &&
                _checkFogProgramName(pass->getShadowReceiverVertexProgram(), oldFogMode, newFogMode, newProgramName))
            {
                pass->setShadowReceiverVertexProgram(newProgramName);
            }
            if (pass->hasShadowReceiverFragmentProgram() &&
                _checkFogProgramName(pass->getShadowReceiverFragmentProgram(), oldFogMode, newFogMode, newProgramName))
            {
                pass->setShadowReceiverFragmentProgram(newProgramName);
            }
        }
    }
}

bool
Terrain::_checkFogProgramName(const Ogre::GpuProgramPtr& program, Ogre::FogMode oldFogMode, Ogre::FogMode newFogMode, String& newProgramName) const
{
    const String& programName = program->getName();

    TerrainData::FogReplacements::const_iterator itfr, itfrend;
    itfrend = mData->mSupportedFogReplacements.end();
    for (itfr = mData->mSupportedFogReplacements.begin(); itfr != itfrend; ++itfr)
    {
        if (programName == itfr->programNames[oldFogMode])
        {
            newProgramName = itfr->programNames[newFogMode];
            return true;
        }
    }

    return false;
}

const Ogre::MaterialPtr&
Terrain::_getGridMaterial(const TerrainData::GridInfo &gridInfo, ushort depthBias, int nTileX, int nTileZ)
{
    assert(mData != NULL);

    size_t textureIds[TerrainData::NumLayers] = { 0 };
    for (size_t i = 0; i < TerrainData::NumLayers; ++i)
    {
        if (gridInfo.layers[i].pixmapId)
        {
            textureIds[i] = _getPixmapAtlasId(gridInfo.layers[i].pixmapId - 1) + 1;
        }
    }
    assert(textureIds[0] && "Internal fault while create grid material");

    bool lightmapped = mData->mLightmapImage && getLightmapQuality() != LMQ_NONE;
    ulong lightmapId = lightmapped ? (nTileZ << 16) | nTileX : ~0;

    MaterialId id(textureIds[0], textureIds[1], lightmapId, depthBias);

    // find the material that already created
    MaterialMap::const_iterator it = mMaterials.find(id);
    if (it != mMaterials.end())
        return it->second;

    String name = "Terrain/";
    if (!mData->mName.empty())
        name += mData->mName + "/";
    name += Ogre::StringConverter::toString(textureIds[0]) + "." + Ogre::StringConverter::toString(textureIds[1]);
    if (depthBias)
        name += "_" + Ogre::StringConverter::toString(depthBias);

    String lightmapName = "<Lightmap>(" +
        Ogre::StringConverter::toString(nTileX / mData->mTileSize) + "," +
        Ogre::StringConverter::toString(nTileZ / mData->mTileSize) + ")";
    if (lightmapped)
        name += "_" + lightmapName;

	Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().getByName(name);
    if (material.isNull())
    {
        // get user supplied material
        String templateName = textureIds[1] ? "TwoLayer" : "OneLayer";
		if (lightmapped)
            templateName += "Lightmap";

        TerrainData::MaterialTemplates::const_iterator it =
            mData->mMaterialTemplates.find(templateName);
        if (it == mData->mMaterialTemplates.end())
            OGRE_EXCEPT(Ogre::Exception::ERR_ITEM_NOT_FOUND,
                "Can't found grid material template for '" + templateName + "'!", 
                "Terrain::_getGridMaterial");

        const String& baseName = it->second;
        Ogre::MaterialPtr templateMaterial = Ogre::MaterialManager::getSingleton().getByName(baseName);
        if (templateMaterial.isNull())
            OGRE_EXCEPT(Ogre::Exception::ERR_ITEM_NOT_FOUND, 
                "Can't load grid material template '" + baseName + "'!", 
                "Terrain::_getGridMaterial");

        // clone the material
        material = templateMaterial->clone(name, true, BRUSH_RESOURCE_GROUP_NAME);

        // Setup texture alias list
        Ogre::AliasTextureNamePairList aliasList;
        aliasList["<layer0>"] = mAtlases[textureIds[0]-1].texture->getName();
        if (textureIds[1])
            aliasList["<layer1>"] = mAtlases[textureIds[1]-1].texture->getName();
        if (lightmapped)
            aliasList["<lightmap>"] = lightmapName;

        // Applies texture names
        material->applyTextureAliases(aliasList);

        // Applies surface params
        _applySurfaceParams(material);

        // Applies fog
        _applyFogMode(material, Ogre::FOG_NONE, mCurrentFogMode);

        // Adjust other material attributes
        Ogre::Material::TechniqueIterator ti = material->getTechniqueIterator();
        while (ti.hasMoreElements())
        {
            Ogre::Technique* technique = ti.getNext();
            Ogre::Technique::PassIterator pi = technique->getPassIterator();
            while (pi.hasMoreElements())
            {
                Ogre::Pass* pass = pi.getNext();
                if (depthBias)
                {
#if OGRE_VERSION >= 0x010300
                    pass->setDepthBias(depthBias + pass->getDepthBiasConstant(), pass->getDepthBiasSlopeScale());
#else
                    pass->setDepthBias(depthBias + pass->getDepthBias());
#endif
                }
            }
        }
    }

    // The material will load on demand

    std::pair<MaterialMap::iterator, bool> inserted =
        mMaterials.insert(MaterialMap::value_type(id, material));
    assert(inserted.second && "Internal fault while create grid material");
    return inserted.first->second;
}

void
Terrain::_deleteMaterials(void)
{
    Ogre::MaterialManager* materialManager = Ogre::MaterialManager::getSingletonPtr();
    // So we can delete this terrain after material manager is delete
    if (materialManager)
    {
        for (MaterialMap::const_iterator it = mMaterials.begin(); it != mMaterials.end(); ++it)
        {
            Ogre::ResourceHandle handle = it->second->getHandle();

            materialManager->remove(handle);
        }
    }
    mMaterials.clear();

    Ogre::TextureManager* textureManager = Ogre::TextureManager::getSingletonPtr();
    if (textureManager)
    {
        for (AtlasArray::const_iterator it = mAtlases.begin(); it != mAtlases.end(); ++it)
        {
            // Only remove composited textures.
            if (!it->texture.isNull() && !it->image.isNull())
            {
                textureManager->remove(it->texture->getHandle());
            }
        }

        for (TextureList::const_iterator it = mLightmapTextures.begin(); it != mLightmapTextures.end(); ++it)
        {
            textureManager->remove((*it)->getHandle());
        }
    }
    mAtlases.clear();
    mLightmapTextures.clear();

    mAtlasPixmaps.clear();

    // Mark as invalidate
    mSolidAtlasAllocInfo.blockId = mMaxAtlasBlockId;
    mTransparentAtlasAllocInfo.blockId = mMaxAtlasBlockId;
}

void
Terrain::prepareLightmapTexture(void)
{
    assert(mLightmapTextures.empty());

	if (getLightmapQuality() == LMQ_NONE)
    {
        if (!mEditable && mData->mLightmapImage)
        {
            delete mData->mLightmapImage;
            mData->mLightmapImage = 0;
        }
		return;
    }
    else if (!mData->mLightmapImage)
    {
        if (mEditable || !mData->reloadLightmap())
            return;
    }

    int numTilePerX = mData->mNumTilePerX;
    int numTilePerZ = mData->mNumTilePerZ;

	uint uWidth, uHeight;
	if (getLightmapQuality() == LMQ_LOW)
	{
		uWidth = mData->mTileSize * 2;
		uHeight = mData->mTileSize * 2;
	}
	else
	{
		uWidth = mData->mTileSize * 8;
		uHeight = mData->mTileSize * 8;
	}

    for (int z = 0; z < numTilePerZ; ++z)
    {
        for (int x = 0; x < numTilePerX; ++x)
		{
            String lightmapName = "<Lightmap>(" +
                Ogre::StringConverter::toString(x) + "," +
                Ogre::StringConverter::toString(z) + ")";
            Ogre::TexturePtr texture = Ogre::TextureManager::getSingleton().createManual(lightmapName,
                BRUSH_RESOURCE_GROUP_NAME, Ogre::TEX_TYPE_2D,
                uWidth, uHeight,
				-1, Ogre::PF_L8,
                Ogre::TU_AUTOMIPMAP | Ogre::TU_STATIC_WRITE_ONLY,
                this);
            mLightmapTextures.push_back(texture);
		}
	}
}

void 
Terrain::setLightmapQuality(LightmapQuality lightmapQuality)
{	
	mLightmapQuality = lightmapQuality;

    for (TextureList::const_iterator it = mLightmapTextures.begin(); it != mLightmapTextures.end(); ++it)
    {
        (*it)->unload();
    }

	// TODO: Rebuild lightmap textures and geometries
}

void
Terrain::buildGeometry(Ogre::SceneNode* parent, bool editable)
{
    clearGeometry();	

    assert(parent);

    // NB: We must adjust world geometry bounding box, especially for OctreeSceneManager
    Ogre::AxisAlignedBox aabb = mData->getBoundBox();
    Ogre::Vector3 centre = aabb.getCenter();
    const Ogre::Vector3 adjust(2000, 10000, 2000);
    const Real scale = 1.5f;
    aabb.setExtents(
        (aabb.getMinimum() - centre) * scale + centre - adjust,
        (aabb.getMaximum() - centre) * scale + centre + adjust);
    parent->getCreator()->setOption("Size", &aabb);

    mEditable = editable;

    if (!mData->mXSize || !mData->mZSize)
    {
        // Do nothing if it's empty terrain.
        return;
    }

    // Prepare atlas texture
    for (size_t pixmapId = 0, numPixmap = mData->mPixmaps.size(); pixmapId < numPixmap; ++pixmapId)
    {
        _getPixmapAtlasId(pixmapId);
    }

	prepareLightmapTexture();

    int depth = mData->mZSize;
    int width = mData->mXSize;
    int tileSize = mData->mTileSize;
    int numTilePerX = mData->mNumTilePerX;
    int numTilePerZ = mData->mNumTilePerZ;

    if (mEditable)
    {
        mGridTypeInfos.resize(2);
        mGridTypeInfos[0].material.setNull();
        mGridTypeInfos[1].material = Ogre::MaterialManager::getSingleton().getByName(
            "FairyEditor/GridType");
    }
    else
    {
        _initIndexBuffer(tileSize * tileSize);
    }

    mTiles.reserve(numTilePerX * numTilePerZ);
    for (int z = 0; z < numTilePerZ; ++z)
    {
        for (int x = 0; x < numTilePerX; ++x)
        {
            // Create the tile
            int tileX = x * tileSize;
            int tileZ = z * tileSize;
            int tileWidth = std::min(width - tileX, tileSize);
            int tileDepth = std::min(depth - tileZ, tileSize);
            TerrainTile* tile;
			if (mEditable)
				tile = new TerrainTileEditable(parent, this, tileX, tileZ, tileWidth, tileDepth);
			else
				tile = new TerrainTileOptimized(parent, this, tileX, tileZ, tileWidth, tileDepth);

            // Use the render queue that the world geometry associated with.
            tile->setRenderQueueGroup(parent->getCreator()->getWorldGeometryRenderQueue());

            // Attach it to the aux data
            mTiles.push_back(tile);
        }
    }
}

TerrainTile*
Terrain::getTile(int x, int z) const
{
    assert(0 <= x && x < mData->mNumTilePerX);
    assert(0 <= z && z < mData->mNumTilePerZ);
    assert((size_t)(z * mData->mNumTilePerX + x) < mTiles.size());

    return mTiles[z * mData->mNumTilePerX + x];
}

void
Terrain::notifyHeightModified(int xstart, int zstart, int xend, int zend)
{
    assert(0 <= xstart && xstart < xend && xend <= mData->mXSize+1);
    assert(0 <= zstart && zstart < zend && zend <= mData->mZSize+1);

	// TODO: (optimization) some grid may be need to recompute normal only

    int tileSize = mData->mTileSize;
    int xtile0 = std::max(0, (xstart-2) / tileSize);
    int xtile1 = std::min(xend / tileSize + 1, mData->mNumTilePerX);
    int ztile0 = std::max(0, (zstart-2) / tileSize);
    int ztile1 = std::min(zend / tileSize + 1, mData->mNumTilePerZ);

    // Adjust boundary, make sure include changed normals
    if (xstart > 0) --xstart;
    if (xend <= mData->mXSize) ++xend;
    if (zstart > 0) --zstart;
    if (zend <= mData->mZSize) ++zend;

    for (int z = ztile0; z < ztile1; ++z)
    {
        int zbais = z * tileSize;
        for (int x = xtile0; x < xtile1; ++x)
        {
            int xbais = x * tileSize;
            getTile(x, z)->_notifyHeightModified(
                std::max(xstart - xbais, 0),
                std::max(zstart - zbais, 0),
                std::min(xend - xbais, tileSize + 1),
                std::min(zend - zbais, tileSize + 1));
        }
    }

    // Update of bounding box

    if (!mData->mBoundBox.isNull())
    {
        // Note: This update is not exactly, but can avoid compute entire terrain
        // height which should lower the performance

        Ogre::Vector3 maximum = mData->mBoundBox.getMaximum();
        Ogre::Vector3 minimum = mData->mBoundBox.getMinimum();
        for (int z = zstart; z < zend; ++z)
        {
            for (int x = xstart; x < xend; ++x)
            {
                Real h = mData->getHeight(x, z);
                if (maximum.y < h)
                    maximum.y = h;
                else if (minimum.y > h)
                    minimum.y = h;
            }
        }

        mData->mBoundBox.setExtents(minimum, maximum);
    }
}

void
Terrain::notifyGridInfoModified(int xstart, int zstart, int xend, int zend)
{
    assert(0 <= xstart && xstart < xend && xend <= mData->mXSize);
    assert(0 <= zstart && zstart < zend && zend <= mData->mZSize);

    int tileSize = mData->mTileSize;
    int xtile0 = xstart / tileSize;
    int xtile1 = (xend+tileSize-1) / tileSize;
    int ztile0 = zstart / tileSize;
    int ztile1 = (zend+tileSize-1) / tileSize;

    for (int z = ztile0; z < ztile1; ++z)
    {
        int zbais = z * tileSize;
        for (int x = xtile0; x < xtile1; ++x)
        {
            int xbais = x * tileSize;
            getTile(x, z)->_notifyGridInfoModified(
                std::max(xstart - xbais, 0),
                std::max(zstart - zbais, 0),
                std::min(xend - xbais, tileSize),
                std::min(zend - zbais, tileSize));
        }
    }
}

}
