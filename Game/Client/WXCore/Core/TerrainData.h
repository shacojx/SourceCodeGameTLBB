#ifndef __TERRAINDATA_H__
#define __TERRAINDATA_H__

#include "WXPrerequisites.h"

#include <OgreVector3.h>
#include <OgreAxisAlignedBox.h>
#include <OgreColourValue.h>
#include <OgreCommon.h>

namespace WX {

class XMLParser;
class XMLWriter;

class TerrainData
{
public:

	// NOTE: Some code dependence with this enum, search [Corner Map] for detail
    enum OrientationFlags {
	    FlipHorizontal		= 1,	//!< Horizontal flip the pixmap
	    FlipVertical		= 2,	//!< Vertical flip the pixmap
	    RotateRightAngle	= 4,	//!< Rotate the pixmap anticlockwise 90 degree
        MirrorDiagonal      = 8,    //!< Mirror the pixmap by diagonal
    };

	// NOTE: Some code dependence with this enum, search [Corner Map] for detail
    enum Corner {
        LEFT_TOP            = 0,
        RIGHT_TOP           = 1,
        LEFT_BOTTOM         = 2,
        RIGHT_BOTTOM        = 3,
    };

    enum {
        NumLayers = 2,              //!< number texture layers
    };

	// NOTE: Some code dependence with fields order of this structure, search [Pixmap Fields Order] for detail
    struct Pixmap {
        size_t textureId;           //!< the texture this pixmap reference to, access by mTextures[textureId]
        Real left;                  //!< defining the left of area on the texture that makes up this pixmap
        Real top;                   //!< defining the top of area on the texture that makes up this pixmap
        Real right;                 //!< defining the right of area on the texture that makes up this pixmap
        Real bottom;                //!< defining the bottom of area on the texture that makes up this pixmap

        Pixmap()
        {
            // null constructor
        }

        explicit Pixmap(size_t textureId)
            : textureId(textureId)
            , left(0)
            , top(0)
            , right(1)
            , bottom(1)
        {
        }

        Pixmap(size_t textureId, Real left, Real top, Real right, Real bottom)
            : textureId(textureId)
            , left(left)
            , top(top)
            , right(right)
            , bottom(bottom)
        {
        }

        bool operator== (const Pixmap& rhs) const
        {
            return textureId == rhs.textureId &&
                   left == rhs.left && right == rhs.right &&
                   top == rhs.top && bottom == rhs.bottom;
        }

        bool operator!= (const Pixmap& rhs) const
        {
            return !(*this == rhs);
        }

        bool operator< (const Pixmap& rhs) const
        {
            if (textureId < rhs.textureId) return true;
            if (textureId > rhs.textureId) return false;
            if (left < rhs.left) return true;
            if (left > rhs.left) return false;
            if (top < rhs.top) return true;
            if (top > rhs.top) return false;
            if (right < rhs.right) return true;
            if (right > rhs.right) return false;
            if (bottom < rhs.bottom) return true;
            if (bottom > rhs.bottom) return false;
            return false;
        }
    };

    struct LayerInfo {
        ushort pixmapId;            //!< the pixmap this layer info reference to, 0 means hole, otherwise access by mPixmaps[pixmapId-1]
        ushort orientation;         //!< combine flags of OrientationFlags

        bool operator== (const LayerInfo& rhs) const
        {
            return pixmapId == rhs.pixmapId &&
                   orientation == rhs.orientation;
        }

        bool operator!= (const LayerInfo& rhs) const
        {
            return !(*this == rhs);
        }
    };

    struct GridInfo {
        LayerInfo layers[NumLayers];
        uint flags;

        enum Flag
        {
            FlipDiagonal    = 1 << 0,
            DisableDecal    = 1 << 1,
        };

        const Corner* getCorners(void) const
        {
            return msConnerMap[(flags & FlipDiagonal) ? 1 : 0];
        }

        bool operator== (const GridInfo& rhs) const
        {
            return layers[0] == rhs.layers[0] &&
                   layers[1] == rhs.layers[1] &&
                   flags == rhs.flags;
        }

        bool operator!= (const GridInfo& rhs) const
        {
            return !(*this == rhs);
        }
    };

    struct SurfaceParams
    {
        Ogre::ColourValue ambient;
        Ogre::ColourValue diffuse;
        Ogre::ColourValue specular;
        Ogre::ColourValue emissive;
        Ogre::Real shininess;

        bool operator== (const SurfaceParams& rhs) const
        {
            return ambient == rhs.ambient &&
                   diffuse == rhs.diffuse &&
                   specular == rhs.specular &&
                   emissive == rhs.emissive &&
                   shininess == rhs.shininess;
        }

        bool operator!= (const SurfaceParams& rhs) const
        {
            return !(*this == rhs);
        }
    };

    struct FogReplacement
    {
        String programNames[4];      // access by fog mode
    };

    typedef std::vector<GridInfo> GridInfoArray;

    struct GridInfoCopy
    {
        String        fileName;
        GridInfoArray gridInfos;
        uint32        touchTimes;
        uint32        gridCount;
    };

    typedef std::vector<GridInfoCopy> GridInfoArrayCopys;

    typedef std::vector<Real> RealArray;
    typedef std::vector<Pixmap> PixmapArray;
    typedef std::vector<String> TextureArray;
    typedef std::map<String, String> MaterialTemplates;
    typedef std::vector<FogReplacement> FogReplacements;

    // Access by flip diagonal or not
    static const Corner msConnerMap[2][4];

    // Default surface params
    static const SurfaceParams msDefaultSurfaceParams;

    // options

    int mXSize;							//!< X dimension of the map (in grids)
    int mZSize;							//!< Z dimension of the map (in grids)
    int mTileSize;						//!< Dimensions of each tile (in grids)
    Ogre::Vector3 mScale;				//!< World space unit of each grid
    Ogre::Vector3 mPosition;			//!< World space position of the first grid	

    // derived fields

    Ogre::Vector3 mInvScale;        //!< 1 / mScale
    Ogre::Vector3 mCentre;          //!< The centre of the terrain
    Real mMinX, mMaxX;
    Real mMinZ, mMaxZ;
    int mNumTilePerX;               //!< (width - 1) / tileSize + 1
    int mNumTilePerZ;               //!< (depth - 1) / tileSize + 1
    mutable Ogre::AxisAlignedBox mBoundBox; //!< The bound box

    // artist's data

    RealArray mHeightmap;           //!< Height of each end point, ((width+1) * (depth+1)) elements
    GridInfoArray mGridInfos;       //!< Grid info array, (width * depth) elements
    PixmapArray mPixmaps;           //!< The pixmaps the each layer reference to
    TextureArray mTextures;         //!< Textures that pixmap is referenced to
    Ogre::Image* mLightmapImage;	//!< Lightmap image of terrain, NULL if not exists

    GridInfoArrayCopys mGridInfoArrayCopys;

    String mName;                   //!< The name of the grid scene

    String mResourceGroupName;      //!< The resource group name contains terrain resources
    String mHeightmapType;          //!< The height-map file type
    String mHeightmapFilename;      //!< The height-map file name
	String mLightmapType;
	String mLightmapFilename;		//!< The light-map file name
    String mGridInfoFilename;       //!< The grid info file name    

    /// The material templates
    MaterialTemplates mMaterialTemplates;
    /// The fog replacements
    FogReplacements mFogReplacements;
    /// The supported fog replacements
    FogReplacements mSupportedFogReplacements;

    SurfaceParams mSurfaceParams;

public:

	void _computeTileBoundingBox(Ogre::AxisAlignedBox& box, int startx, int startz, int width, int depth) const;

	Ogre::Vector3 _convertPosition(Real x, Real h, Real z) const
	{
		return Ogre::Vector3(mPosition.x + mScale.x * x,
					         mPosition.y + mScale.y * h,
                             mPosition.z + mScale.z * z);
	}

    size_t _getJunctionIndex(int x, int z) const
    {
		assert(isValidJunction(x, z));
        return z * (mXSize + 1) + x;
    }

	size_t _getLightmapSamplingIndex(int x, int z) const
	{
		assert(isValidLightmapSampling(x, z));
		return z * (mXSize << 3) + x;
	}

    size_t _getJunctionIndex(const std::pair<int, int>& coord) const
    {
		return _getJunctionIndex(coord.first, coord.second);
    }

    size_t _getGridIndex(int x, int z) const
    {
        assert(isValidGrid(x, z));
        return z * mXSize + x;
    }

    size_t _getGridIndex(const std::pair<int, int>& coord) const
    {
        return _getGridIndex(coord.first, coord.second);
    }

    /** Returns the terrain RAW height at the given junction coordinates */
    Real _getRawHeight(int x, int z) const
	{
		return mHeightmap[_getJunctionIndex(x, z)];
	}

    /** Returns the terrain world space position at the given junction coordinates */
    Ogre::Vector3 _getPosition(int x, int z) const
	{
		return _convertPosition(x, _getRawHeight(x, z), z);
	}

    /** Returns the terrain normal at the given junction coordinates */
    Ogre::Vector3 _getNormal(int x, int z) const;

    std::pair<bool, Real> _intersectGrid(const Ogre::Ray& ray, int x, int z,
        bool positiveSide, bool negativeSide) const;

    int _registerPixmap(const String& textureName, Real left, Real top, Real right, Real bottom);

    /** Optimize the terrain data
    @remark
        Remove the useless pixmaps and textures, so we has more slot
        to store the pixmap and texture infomation. Re-sort pixmaps
        and textures for more effective.
    @return
        true if the terrain is compactable (so the terrain data is modified,
        you need to rebuild terrain renderable), false otherwise.
    */
    bool _optimize(void);

    bool _reshape(int left, int top, int right, int bottom, size_t pixmapId = 0);

public:

    void _updateDerivedFields(void);

protected:
    void _loadBaseInfo(XMLParser* parser, const String& filename, const String& groupName);
    void _saveBaseInfo(XMLWriter* writer, const String& filename) const;

    void _fixupMissingInfo(void);
    void _fixupSupportedMaterials(void);

    void _loadHeightmap(const String& filename, const String& type, const String& groupName);
    void _saveHeightmap(const String& filename) const;

	void _loadLightmap(const String& filename, const String& type, const String& groupName);
	void _saveLightmap(const String& filename) const;

    void _loadGridInfo(const String& filename, const String& groupName);
    void _saveGridInfo(const String& filename) const;

    bool _findGridInfoCopy(const String& fileName, size_t& index);
    void _sortGridInfoCopys(void);

public:
    TerrainData();
    ~TerrainData();

    void reset(void);
    void load(XMLParser* parser, const String& filename, const String& groupName, bool loadLightmap);
    void save(XMLWriter* writer, const String& filename, bool preservingNames = false) const;		
    bool reloadLightmap(void);

    const Ogre::AxisAlignedBox& getBoundBox() const;

    int getXSize(void) const
    {
        return mXSize;
    }

    int getZSize(void) const
    {
        return mZSize;
    }

    // Check the terrain is empty
    bool isEmpty(void) const
    {
        return mXSize <= 0 || mZSize <= 0;
    }

    // Check the given values is a valid junction coordinates
    bool isValidJunction(int x, int z) const
    {
        return !isEmpty() && 0 <= x && x <= mXSize && 0 <= z && z <= mZSize;
    }

	// Check the given values is a valid lightmap sampling coordinates
	bool isValidLightmapSampling(int x, int z) const
	{
		return !isEmpty() && 0 <=x && x < (mXSize << 3) && 0 <= z && z < (mZSize << 3);
	}

    // Check the given values is a valid junction coordinates
    bool isValidJunction(const std::pair<int, int>& coord) const
    {
        return isValidJunction(coord.first, coord.second);
    }

    // Check the given values is a valid grid coordinates
    bool isValidGrid(int x, int z) const
    {
        return 0 <= x && x < mXSize && 0 <= z && z < mZSize;
    }

    // Check the given values is a valid grid coordinates
    bool isValidGrid(const std::pair<int, int>& coord) const
    {
        return isValidGrid(coord.first, coord.second);
    }

    // Check the given values is a valid world coordinates
    bool isValidCoord(Real x, Real z) const
    {
        return mMinX <= x && x <= mMaxX && mMinZ <= z && z <= mMaxZ;
    }

    /** Returns the terrain world space height at the given junction coordinates */
    Real getHeight(int x, int z) const
	{
		return mPosition.y + _getRawHeight(x, z) * mScale.y;
	}

    /** Returns the terrain world space height at the given junction coordinates */
    Real getHeight(const std::pair<int, int>& coord) const
	{
		return getHeight(coord.first, coord.second);
	}

    /** Set the terrain world space height at the given junction coordinates */
    void setHeight(int x, int z, Real height)
    {
        mHeightmap[_getJunctionIndex(x, z)] = (height - mPosition.y) * mInvScale.y;
    }

    /** Set the terrain world space height at the given junction coordinates */
    void setHeight(const std::pair<int, int>& coord, Real height)
    {
        setHeight(coord.first, coord.second, height);
    }

    void setGridInfo(int x, int z, const GridInfo& info)
    {
        mGridInfos[_getGridIndex(x, z)] = info;
    }

    void setGridInfo(int index, const GridInfo& info)
    {
        mGridInfos[index] = info;
    }

    void setGridInfo(const std::pair<int, int>& coord, const GridInfo& info)
    {
        setGridInfo(coord.first, coord.second, info);
    }

    /** Returns the nearest terrain junction at the given world space coordinates */
    std::pair<int, int> getJunction(Real x, Real z) const;

    std::pair<Real, Real> worldToJunction(Real x, Real z) const;
    std::pair<Real, Real> junctionToWorld(Real x, Real z) const;

    /** Returns the terrain grid at the given world space coordinates */
    std::pair<int, int> getGrid(Real x, Real z) const;

    std::pair<Real, Real> worldToGrid(Real x, Real z) const;
    std::pair<Real, Real> gridToWorld(Real x, Real z) const;

    /** Returns the terrain height at the given world space coordinates */
    Real getHeightAt(Real x, Real z) const;

	/** Returns the terrain height at the given grid space coordinates */
    Real getHeightAtGrid(Real x, Real z) const;

    /** Returns the terrain position at the given world space coordinates */
    Ogre::Vector3 getPositionAt(Real x, Real z) const;

    /** Returns the terrain position at the given grid space coordinates */
    Ogre::Vector3 getPositionAtGrid(Real x, Real z) const;

    /** Returns the terrain normal at the given world space coordinates */
    Ogre::Vector3 getNormalAt(Real x, Real z) const;

    /** Returns the terrain position and normal at the given world space coordinates */
    void getPositionAndNormalAt(Real x, Real z, Ogre::Vector3& position, Ogre::Vector3& normal) const;

    /** Returns the terrain height at the given grid centre */
    Real getGridHeight(int x, int z) const;

    /** Returns the terrain position at the given grid centre */
    Ogre::Vector3 getGridPosition(int x, int z) const;

    /** Return the info of the grid at the given grid */
    const GridInfo & getGridInfo(int x, int z) const
    {
        return mGridInfos[ _getGridIndex(x,z) ];
    }

    /** Ray / terrain intersection, returns boolean result and
        the intersect point postion/normal at terrain. */
    bool getIntersects(const Ogre::Ray& ray, Ogre::Vector3& position, Ogre::Vector3* normal = NULL, bool allowOutside = false,
        bool positiveSide = true, bool negativeSide = false) const;

    /** Frustum / terrain intersection, returns an array of positions and normals,
        the positions/normals groupped by triangle of the intersection */
    size_t getIntersectsTriangles(const Ogre::Frustum* frustum, int areaLimit,
        bool receivedDecalOnly,
        std::vector<Ogre::Vector3>& positions, std::vector<Ogre::Vector3>* normals = NULL);
};

}

#endif // 
