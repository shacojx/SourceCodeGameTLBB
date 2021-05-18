#ifndef __TERRAIN_H__
#define __TERRAIN_H__

#include "WXPrerequisites.h"
#include "TerrainData.h"

#include <OgreMaterial.h>
#include <OgreHardwareBufferManager.h>

namespace WX {

class TerrainTile;

class Terrain : Ogre::ManualResourceLoader
{
public:
	enum LightmapQuality
	{
		LMQ_NONE,
		LMQ_LOW,
		LMQ_HIGH,
	};

protected:
    struct MaterialId
    {
        uchar textureId0;
        uchar textureId1;
        ushort depthBias;
		ulong  lightmapId;

        MaterialId()
            : textureId0()
            , textureId1()
            , depthBias()
			, lightmapId()
        {
        }

        MaterialId(uchar textureId0, uchar textureId1, ulong lightmapId, ushort depthBias)
            : textureId0(textureId0)
            , textureId1(textureId1)
			, lightmapId(lightmapId)
            , depthBias(depthBias)			
        {
        }

        bool operator== (const MaterialId& rhs) const
        {
            return textureId0 == rhs.textureId0 &&
                   textureId1 == rhs.textureId1 &&
				   lightmapId == rhs.lightmapId &&
                   depthBias == rhs.depthBias;
        }

        bool operator!= (const MaterialId& rhs) const
        {
            return !(*this == rhs);
        }

        bool operator< (const MaterialId& rhs) const
        {
            if (textureId0 < rhs.textureId0) return true;
            if (rhs.textureId0 < textureId0) return false;
            if (textureId1 < rhs.textureId1) return true;
            if (rhs.textureId1 < textureId1) return false;
			if (lightmapId < rhs.lightmapId) return true;
			if (rhs.lightmapId < lightmapId) return false;
            if (depthBias < rhs.depthBias) return true;
            if (rhs.depthBias < depthBias) return false;
            return false;
        }
    };

    struct AtlasPixmap
    {
        size_t atlasId;
        Real left;
        Real top;
        Real right;
        Real bottom;
    };

    struct Atlas;

    struct AtlasAllocInfo
    {
        size_t atlasId;
        size_t blockId;
    };

    typedef std::map<size_t, Ogre::IndexData*> IndexDataMap;
    typedef std::map<MaterialId, Ogre::MaterialPtr> MaterialMap;
    typedef std::vector<TerrainTile*> TileList;
    typedef std::vector<AtlasPixmap> AtlasPixmapArray;
    typedef std::vector<Atlas> AtlasArray;
    typedef std::vector<Ogre::TexturePtr> TextureList;

    //-----------------------------------------------------------------------

    // options

    TerrainData* mData;         //!< The terrain data

    // work data

    TileList mTiles;            //!< The tiles belong to the terrain
    MaterialMap mMaterials;     //!< materials that used for the scene
    mutable Ogre::FogMode mCurrentFogMode;  //!< Current materials fog mode

    AtlasPixmapArray mAtlasPixmaps;
    AtlasArray mAtlases;
    AtlasAllocInfo mSolidAtlasAllocInfo;
    AtlasAllocInfo mTransparentAtlasAllocInfo;
    size_t mAtlasTextureSize;
    size_t mAtlasPixmapSize;
    size_t mAtlasNumMipMaps;        //!< mAtlasPixmapSize >= (1 << mAtlasNumMipMaps)
    size_t mAtlasBlockSize;         //!< = mAtlasTextureSize / mAtlasPixmapSize
    size_t mMaxAtlasBlockId;        //!< = mAtlasBlockSize * mAtlasBlockSize
    Real mAtlasBlockTexCoordInc;    //!< = 1.0f / mAtlasBlockSize

    TextureList mLightmapTextures;  //!< Lightmap textures created by ourself
    size_t mLightmapTextureSize;    //!< The lightmap texture size
    LightmapQuality mLightmapQuality;

    // non-editable data

    Ogre::HardwareIndexBufferSharedPtr mIndexBuffer;    //!< index buffer for share between each map tile
    IndexDataMap mIndexDatas;   //!< index data for share between map tile

    // editable data

    bool mEditable;             //!< Indicate we intend to edit the terrain

    friend class TerrainTileEditable;

    struct GridLineInfo {
        size_t stride;
        float width;
        Ogre::MaterialPtr material;
    };

    struct GridTypeInfo {
        Ogre::MaterialPtr material;
    };

    bool mDisplayGridLine;
    bool mDisplayGridType;
    std::vector<GridLineInfo> mGridLineInfos;
    std::vector<GridTypeInfo> mGridTypeInfos;

	void prepareLightmapTexture(void);

    //-----------------------------------------------------------------------

public:

    void _deleteTileList(void);

    void _initIndexBuffer(size_t maxQuads);
    Ogre::IndexData* _getIndexData(size_t numQuads);
    void _deleteIndexDatas(void);

    void _createAtlasPixmap(size_t pixmapId);
    size_t _getPixmapAtlasId(size_t pixmapId);
    void _applySurfaceParams(const Ogre::MaterialPtr& material) const;
    void _applyFogMode(const Ogre::MaterialPtr& material, Ogre::FogMode oldFogMode, Ogre::FogMode newFogMode) const;
    bool _checkFogProgramName(const Ogre::GpuProgramPtr& program, Ogre::FogMode oldFogMode, Ogre::FogMode newFogMode, String& newProgramName) const;
    const Ogre::MaterialPtr& _getGridMaterial(const TerrainData::GridInfo &gridInfo, ushort depthBias = 0, int nTileX = 0, int nTileZ = 0);
    /** Returns the pixmap coordinates for the layer at the given corner */
    std::pair<Real, Real> _getPixmapCorner(const TerrainData::LayerInfo& layerInfo, TerrainData::Corner corner, uint flags) const;
    void _deleteMaterials(void);

    void _updateMaterials(void) const;

    bool _isLightmapUsed(void) const
    {
        return mData->mLightmapImage && getLightmapQuality() != LMQ_NONE;
    }

    /** Override, @copydoc ManualResourceLoader::loadResource
    */
    void loadResource(Ogre::Resource* resource);

public:
    Terrain(void);
    ~Terrain();

    TerrainData* getData() const
    {
        return mData;
    }

    bool getEditable(void) const
    {
        return mEditable;
    }

    TerrainTile* getTile(int x, int z) const;

    /** Empties the terrain and grid data */
    void reset(void);

    // This function should be call after modified terrain surface params and the geometry has been created
    void resetSurfaceParams(void);

	void setLightmapQuality(LightmapQuality lightmapQuality);

	LightmapQuality getLightmapQuality(void) const
	{
		return mLightmapQuality;
	}

    /** Create geometry use exists grid data
    @param
        sceneManager the scene manager that we want this terrain display on
        editable Indicate we intend to edit the terrain
    */
    void buildGeometry(Ogre::SceneNode* parent, bool editable = false);

    /** Empties the terrain only */
    void clearGeometry(void);

    /** Returns the terrain height at the given world coordinates */
    Real getHeightAt(float x, float z) const;

    /** Returns the terrain position at the given world coordinates */
    Ogre::Vector3 getPositionAt(float x, float z) const;

    /** Returns the terrain normal at the given world coordinates */
    Ogre::Vector3 getNormalAt(float x, float z) const;

    void notifyHeightModified(int xstart, int zstart, int xend, int zend);
    void notifyGridInfoModified(int xstart, int zstart, int xend, int zend);
};

}

#endif // 
