#include "TerrainData.h"
#include "ieee.h"
#include "SerializerUtil.h"

#include <OgreException.h>
#include <OgreArchiveManager.h>
#include <OgreImage.h>
#include <OgreStringConverter.h>
#include <OgreRay.h>
#include <OgreFrustum.h>
#include <OgreLogManager.h>
#include <OgreGpuProgramManager.h>

namespace WX {

//////////////////////////////////////////////////////////////////////////

/* Each grid(X,Z) is default tessellate as:

    X+0,Z+0     X+1,Z+0

        +------+
        |     /|
        |    / |
        |   /  |
        |  /   |
        | /    |
        |/     |
        +------+

    X+0,Z+1     X+1,Z+1

  When flip diagonal, tessellate as:

    X+0,Z+0     X+1,Z+0

        +------+
        |\     |
        | \    |
        |  \   |
        |   \  |
        |    \ |
        |     \|
        +------+

    X+0,Z+1     X+1,Z+1

*/

// NOTE: This map is dependence with 'Corner', search [Corner Map] for detail
const TerrainData::Corner TerrainData::msConnerMap[2][4] =
{
    LEFT_TOP, RIGHT_TOP, LEFT_BOTTOM, RIGHT_BOTTOM,
    LEFT_BOTTOM, LEFT_TOP, RIGHT_BOTTOM, RIGHT_TOP,
};

const TerrainData::SurfaceParams TerrainData::msDefaultSurfaceParams =
{
    Ogre::ColourValue::White,   // ambient
    Ogre::ColourValue::White,   // diffuse
    Ogre::ColourValue::White,   // specular
    Ogre::ColourValue::Black,   // emissive
    64,                         // shininess
};

TerrainData::TerrainData()
    : mXSize(0)
    , mZSize(0)
    , mTileSize(0)
    , mScale(Ogre::Vector3::UNIT_SCALE)
    , mPosition(Ogre::Vector3::ZERO)

    , mNumTilePerX(0)
    , mNumTilePerZ(0)

    , mHeightmap()
    , mGridInfos()
    , mPixmaps()
    , mTextures()
    , mLightmapImage()

    , mName()

    , mResourceGroupName()
    , mHeightmapType()
    , mHeightmapFilename()
    , mGridInfoFilename()
    , mLightmapType()
    , mLightmapFilename()

    , mMaterialTemplates()
    , mSurfaceParams(msDefaultSurfaceParams)
{
}

TerrainData::~TerrainData()
{
    delete mLightmapImage;
    mLightmapImage = 0;
}

void
TerrainData::reset(void)
{
    mXSize = 0;
    mZSize = 0;
    mTileSize = 0;
    mScale = Ogre::Vector3::UNIT_SCALE;
    mPosition = Ogre::Vector3::ZERO;

    mNumTilePerX = 0;
    mNumTilePerZ = 0;

    mHeightmap.clear();
    mGridInfos.clear();
    mPixmaps.clear();
    mTextures.clear();

    delete mLightmapImage;
    mLightmapImage = 0;

    mName.clear();

    mHeightmapType.clear();
    mHeightmapFilename.clear();
    mLightmapType.clear();
    mLightmapFilename.clear();
    mGridInfoFilename.clear();

    mMaterialTemplates.clear();
    mFogReplacements.clear();
    mSupportedFogReplacements.clear();
    mSurfaceParams = msDefaultSurfaceParams;

    // trigger update of bounding box
    mBoundBox.setNull();
}

void
TerrainData::_computeTileBoundingBox(Ogre::AxisAlignedBox& box, int startx, int startz, int width, int depth) const
{
	assert(0 <= startx);
	assert(0 < width);
	assert(startx + width <= mXSize);
	assert(0 <= startz);
	assert(0 < depth);
	assert(startz + depth <= mZSize);

	Real minHeight = Ogre::Math::POS_INFINITY;
	Real maxHeight = Ogre::Math::NEG_INFINITY;
	size_t patch = mXSize + 1;
	const Real* pHeight = &mHeightmap[startz * patch + startx];
    for (int z = 0; z <= depth; ++z, pHeight += patch)
    {
        for (int x = 0; x <= width; ++x)
        {
            Real h = pHeight[x];
			if (minHeight > h) minHeight = h;
			if (maxHeight < h) maxHeight = h;
		}
	}

    box.setExtents(
		_convertPosition(startx, minHeight, startz),
		_convertPosition(startx+width, maxHeight, startz+depth));
}

Ogre::Vector3
TerrainData::_getNormal(int x, int z) const
{
	/*
		   x-1 x x+1
		z-1 +--+--+
			| /| /|
			|/ |/ |
		z   +--+--+
			| /| /|
			|/ |/ |
		z+1 +--+--+
	*/

    assert(isValidJunction(x, z));

	if (!isValidJunction(x, z))
		return Ogre::Vector3::UNIT_Y;

	Real h = _getRawHeight(x, z);
    Ogre::Vector3 corners[7];
	int count = 0;

    // FIXME: Support FlipDiagonal flag

#define V(i,j)	((void)(corners[count++] = Ogre::Vector3((i)*mScale.x, (_getRawHeight(x+(i),z+(j)) - h)*mScale.y, (j)*mScale.z)))
	if (x == 0)
	{
		if (z != mZSize)
		{
			V( 0,+1);
		}
			V(+1, 0);
		if (z != 0)
		{
			V(+1,-1);
			V( 0,-1);
		}
	}
	else if (x == mXSize)
	{
		if (z != 0)
		{
			V( 0,-1);
		}
			V(-1, 0);
		if (z != mZSize)
		{
			V(-1,+1);
			V( 0,+1);
		}
	}
	else
	{
		if (z != 0)
		{
			V(+1, 0);
			V(+1,-1);
			V(0 ,-1);
		}
			V(-1, 0);
		if (z != mZSize)
		{
			V(-1,+1);
			V( 0,+1);
			V(+1, 0);
		}
	}
#undef V

	assert(2 <= count && count <= sizeof(corners)/sizeof(*corners));

    Ogre::Vector3 sum(0, 0, 0);
    for (int i = 1; i < count; ++i)
    {
        Ogre::Vector3 n = corners[i-1].crossProduct(corners[i]);
		assert(n.y > 0);
        n.normalise();
        sum += n;
    }
    sum.normalise();

    return sum;
}

const uint32 TERRAIN_HEIGHTMAP_MAGIC = 'FEHM';
const uint32 TERRAIN_HEIGHTMAP_VERSION = 0x00100000;

const uint32 TERRAIN_LIGHTMAP_MAGIC = 'FELM';
const uint32 TERRAIN_LIGHTMAP_VERSION = 0x00100002;

const uint32 TERRAIN_GRIDINFO_MAGIC = 'FEGI';
const uint32 TERRAIN_GRIDINFO_VERSION = 0x00100002;

void
TerrainData::_loadHeightmap(const String& filename, const String& type, const String& groupName)
{
    // Allocate buffers
    mHeightmap.resize((mXSize + 1) * (mZSize + 1));

    if (filename.empty())
    {
        std::fill(mHeightmap.begin(), mHeightmap.end(), (Real)0);
    }
    else if (type == "image")
    {
        Ogre::Image image;
        image.load(filename, groupName);

        Ogre::PixelFormat pf = image.getFormat();
        if (pf != Ogre::PF_L8 && pf != Ogre::PF_L16)
        {
            OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS,
                "Error: Image is not a grayscale image.",
                "DemoApplication::createScene" );
        }
        //check to make sure it's the expected size
        if (image.getWidth() != mXSize+1 || image.getHeight() != mZSize+1 || image.getDepth() != 1)
        {
            String err = "Error: Invalid heightmap size : " +
                Ogre::StringConverter::toString(image.getWidth()) + "x" +
                Ogre::StringConverter::toString(image.getHeight()) + "x" +
                Ogre::StringConverter::toString(image.getDepth()) +
                ". Should be " +
                Ogre::StringConverter::toString(mXSize+1) + "x" +
                Ogre::StringConverter::toString(mZSize+1);
            OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS, err,
                "HeightmapTerrainPageSource::loadHeightmap");
        }

        const void *pOrigSrc = image.getData();
        bool is16bit = (pf == Ogre::PF_L16);
        Real* pDest = &mHeightmap[0];
        size_t width = mXSize;
        size_t depth = mZSize;
        bool flipTerrain = false;
        // Read the data
        if (is16bit)
        {
            const Real invScale = 1.0f / 65535.0f;
            const ushort *pSrc16 = reinterpret_cast<const ushort *>(pOrigSrc);
            if (flipTerrain)
            {
                // Work backwards
                pSrc16 += depth * (width + 1);
            }
            for (size_t j = 0; j <= depth; ++j)
            {
                for (size_t i = 0; i <= width; ++i)
                {
                    *pDest++ = Real(*pSrc16++) * invScale;
                }
                if (flipTerrain)
                {
                    // Work backwards
                    pSrc16 -= 2 * (width + 1);
                }
            }
        }
        else
        {
            const Real invScale = 1.0f / 255.0f;
            const uchar *pSrc8 = reinterpret_cast<const uchar *>(pOrigSrc);
            if (flipTerrain)
            {
                // Work backwards
                pSrc8 += depth * (width + 1);
            }
            for (size_t j = 0; j <= depth; ++j)
            {
                for (size_t i = 0; i <= width; ++i)
                {
                    *pDest++ = Real(*pSrc8++) * invScale;
                }
                if (flipTerrain)
                {
                    // Work backwards
                    pSrc8 -= 2 * (width + 1);
                }
            }
        }
    }
    else if (type == "standard")
    {
        Ogre::DataStreamPtr stream = Ogre::ResourceGroupManager::getSingleton().openResource(filename, groupName);
        DataStreamReader reader(stream);

        //////////////////////////////////////////////////////////////////////////

        uint32 header[4];
        reader.readInts(header, 4);
        if (header[0] != TERRAIN_HEIGHTMAP_MAGIC)
        {
            OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS,
                "Invalid magic of heightmap file '" + filename + "'while loading terrain '" + mName + "'.",
                "TerrainData::load");
        }
        if (header[1] != TERRAIN_HEIGHTMAP_VERSION)
        {
            OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS,
                "Invalid version of heightmap file '" + filename + "'while loading terrain '" + mName + "'.",
                "TerrainData::load");
        }
        if (header[2] != mXSize+1 || header[3] != mZSize+1)
        {
            OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS,
                "Dimensions of heightmap file '" + filename + "' doesn't match the dimensions of the terrain '" + mName + "'.",
                "TerrainData::load");
        }

        //////////////////////////////////////////////////////////////////////////

        reader.readFloats(&mHeightmap[0], mHeightmap.size());

        //////////////////////////////////////////////////////////////////////////

        if (stream->tell() != stream->size())
        {
            OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS,
                "Invalid heightmap file '" + filename + "'.",
                "TerrainData::_loadHeightmap");
        }
    }
    else
    {
        assert(type == "raw");

        Ogre::DataStreamPtr stream = Ogre::ResourceGroupManager::getSingleton().openResource(filename, groupName);
        DataStreamReader reader(stream);

        reader.readFloats(&mHeightmap[0], mHeightmap.size());

        if (stream->tell() != stream->size())
        {
            OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS,
                "Invalid heightmap file '" + filename + "'.",
                "TerrainData::_loadHeightmap");
        }
    }
}

void
TerrainData::_saveHeightmap(const String& filename) const
{
    FileWriter writer(filename);

    uint32 header[4];
    header[0] = TERRAIN_HEIGHTMAP_MAGIC;
    header[1] = TERRAIN_HEIGHTMAP_VERSION;
    header[2] = mXSize + 1;
    header[3] = mZSize + 1;
    writer.writeInts(header, 4);

    writer.writeFloats(&mHeightmap[0], mHeightmap.size());
}

bool TerrainData::reloadLightmap(void)
{
    if (!mLightmapImage && !mLightmapFilename.empty())
    {
        _loadLightmap(mLightmapFilename, mLightmapType, mResourceGroupName);
    }

    return mLightmapImage != 0;
}

void
TerrainData::_loadLightmap(const String& filename, const String& type, const String& groupName)
{
    assert(!mLightmapImage);

    if (filename.empty())
        return;

	String grp = groupName;
    if (!Ogre::ResourceGroupManager::getSingleton().resourceExists(grp, filename))
        return;

    if (type == "image")
    {
        mLightmapImage = new Ogre::Image;
        try
        {
            mLightmapImage->load(filename, grp);
        }
        catch (const Ogre::Exception&)
        {
            // In case fail to load image for some reason, just ignore here.
            delete mLightmapImage;
            mLightmapImage = 0;
            return;
        }

        if (mLightmapImage->getDepth() != 1 ||
            mLightmapImage->getNumFaces() != 1 ||
            mLightmapImage->getWidth() != mXSize * 8 ||
            mLightmapImage->getHeight() != mZSize * 8)
        {
            delete mLightmapImage;
            mLightmapImage = 0;
            Ogre::LogManager::getSingleton().logMessage("============================NOTICE===============================");
            Ogre::LogManager::getSingleton().logMessage("Terrain lightmap out of date, please update terrain lightmap.");
            Ogre::LogManager::getSingleton().logMessage("=================================================================");
            return;
        }
    }
    else if (type == "standard")
    {
        Ogre::DataStreamPtr stream = Ogre::ResourceGroupManager::getSingleton().
            openResource(filename, groupName);

        DataStreamReader reader(stream);

        uint32 header[4];
        reader.readInts(header, 4);

        if (header[0] != TERRAIN_LIGHTMAP_MAGIC)
        {
            OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS,
                "Invalid magic of lightmap file '" + filename + "'while loading terrain '" + mName + "'.",
                "TerrainData::_loadLightmap");
        }

        if (header[1] != TERRAIN_LIGHTMAP_VERSION)
        {
            Ogre::LogManager::getSingleton().logMessage("============================NOTICE===============================");
            Ogre::LogManager::getSingleton().logMessage("Terrain lightmap out of date, please update terrain lightmap.");
            Ogre::LogManager::getSingleton().logMessage("=================================================================");
            return;
        }

        if (header[2] != mXSize * 8 || header[3] != mZSize * 8)
        {
            OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS,
                "Dimensions of lightmap file '" + filename + "' doesn't match the dimensions of the terrain '" + mName + "'.",
                "TerrainData::_loadLightmap");
        }

        if (stream->tell() + header[2] * header[3] != stream->size())
        {
            OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS,
                "Invalid lightmap file '" + filename + "'.",
                "TerrainData::_loadLightmap");
        }

        mLightmapImage = new Ogre::Image;
        mLightmapImage->loadRawData(stream, header[2], header[3], Ogre::PF_L8);
    }
}

void
TerrainData::_saveLightmap(const String& filename) const
{
    if (!mLightmapImage)
    {
        return;
    }

    if (mLightmapType == "image")
    {
        mLightmapImage->save(filename);
    }
    else
    {
        FileWriter writer(filename);

        uint32 header[4];
        header[0] = TERRAIN_LIGHTMAP_MAGIC;
        header[1] = TERRAIN_LIGHTMAP_VERSION;	
        header[2] = mLightmapImage->getWidth();
        header[3] = mLightmapImage->getHeight();
        writer.writeInts(header, 4);
        writer.writeData(mLightmapImage->getData(), mLightmapImage->getSize());
    }
}

void
TerrainData::_loadGridInfo(const String& filename, const String& groupName)
{
    size_t gridInfoCopysIndex = 0;

    bool gridInfoExist = _findGridInfoCopy(filename, gridInfoCopysIndex);

    if (gridInfoExist)
    {
        mGridInfos = mGridInfoArrayCopys[gridInfoCopysIndex].gridInfos;
        mGridInfoArrayCopys[gridInfoCopysIndex].touchTimes += 1;
    }
    else
    {
        // Allocate buffers
        mGridInfos.resize(mXSize * mZSize);

        if (filename.empty())
        {
            GridInfo gridInfo = { 0, 0, 0, 0, 0 };
            if (!mPixmaps.empty())
                gridInfo.layers[0].pixmapId = 1;
            std::fill(mGridInfos.begin(), mGridInfos.end(), gridInfo);
            return;
        }
        else
        {
            Ogre::DataStreamPtr stream = Ogre::ResourceGroupManager::getSingleton().openResource(filename, groupName);
            DataStreamReader reader(stream);

            //////////////////////////////////////////////////////////////////////////

            uint32 header[4];
            reader.readInts(header, 4);
            if (header[0] != TERRAIN_GRIDINFO_MAGIC)
            {
                OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS,
                    "Invalid magic of grid info file '" + filename + "'while loading terrain '" + mName + "'.",
                    "TerrainData::load");
            }
            if (header[1] > TERRAIN_GRIDINFO_VERSION)
            {
                Ogre::StringUtil::StrStreamType str;
                str << "A newest version (" << std::hex << header[1] << ")"
                    << " of grid info file '" << filename << "'"
                    << " found while loading terrain '" << mName << "'." << std::endl
                    << "Current support version is (" << std::hex << TERRAIN_GRIDINFO_VERSION << ")." << std::endl
                    << "You should run lastest version software to support this version grid info file.";
                OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS,
                    str.str(),
                    "TerrainData::load");
            }
            if (header[1] < 0x00100000)
            {
                OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS,
                    "Invalid version of grid info file '" + filename + "'while loading terrain '" + mName + "'.",
                    "TerrainData::load");
            }
            if (header[1] != TERRAIN_GRIDINFO_VERSION)
            {
                Ogre::StringUtil::StrStreamType str;
                str << "WARNING: " << filename
                    << " is an older format version (" << std::hex << header[1] << "),"
                    << " current version (" << std::hex << TERRAIN_GRIDINFO_VERSION << ");"
                    << " you should upgrade it as soon as possible when you re-save the file.";
                Ogre::LogManager::getSingleton().logMessage(str.str());
            }
            if (header[2] != mXSize || header[3] != mZSize)
            {
                OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS,
                    "Dimensions of grid info file '" + filename + "' doesn't match the dimensions of the terrain '" + mName + "'.",
                    "TerrainData::load");
            }

            bool pixmapId16Bits = false;
            if (header[1] >= 0x00100002)
            {
                reader.readBools(&pixmapId16Bits);
            }

            //////////////////////////////////////////////////////////////////////////

            size_t numPixmaps = mPixmaps.size();
            for (GridInfoArray::iterator it = mGridInfos.begin(); it != mGridInfos.end(); ++it)
            {
                GridInfo& gridInfo = *it;
                for (size_t i = 0; i < NumLayers; ++i) {
                    if (pixmapId16Bits)
                    {
                        uint16 pixmapId;
                        uchar orientation;
                        reader.readShorts(&pixmapId);
                        reader.readData(&orientation, sizeof orientation);
                        gridInfo.layers[i].pixmapId = pixmapId;
                        gridInfo.layers[i].orientation = orientation;
                    }
                    else
                    {
                        uchar v[2];
                        reader.readData(&v, sizeof v);
                        gridInfo.layers[i].pixmapId = v[0];
                        gridInfo.layers[i].orientation = v[1];
                    }
                    // pixmapId == 0 is means hole
                    if (gridInfo.layers[i].pixmapId > numPixmaps)
                    {
                        OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS,
                            "Invalid pixmapId in grid info '" + filename + "'.",
                            "TerrainData::_loadGridInfo");
                    }
                }
                gridInfo.flags = 0;
                if (header[1] >= 0x00100001)
                {
                    uchar flags;
                    reader.readData(&flags, sizeof flags);
                    gridInfo.flags = flags;
                }
            }

            //////////////////////////////////////////////////////////////////////////

            if (stream->tell() != stream->size())
            {
                OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS,
                    "Invalid grid info file '" + filename + "'while loading terrain '" + mName + "'.",
                    "TerrainData::load");
            }
        }

        _sortGridInfoCopys();

        GridInfoCopy gridInfoCopy;
        gridInfoCopy.fileName = filename;
        gridInfoCopy.gridInfos = mGridInfos;
        gridInfoCopy.gridCount = mGridInfos.size();
        gridInfoCopy.touchTimes = 1;

        mGridInfoArrayCopys.push_back(gridInfoCopy);
    }
}

void
TerrainData::_saveGridInfo(const String& filename) const
{
    FileWriter writer(filename);

    uint32 header[4];
    header[0] = TERRAIN_GRIDINFO_MAGIC;
    header[1] = TERRAIN_GRIDINFO_VERSION;
    header[2] = mXSize;
    header[3] = mZSize;
    writer.writeInts(header, 4);

    bool pixmapId16Bits = mPixmaps.size() >= (1 << 8);
    writer.writeBools(&pixmapId16Bits);

    for (GridInfoArray::const_iterator it = mGridInfos.begin(); it != mGridInfos.end(); ++it)
    {
        const GridInfo& gridInfo = *it;
        for (size_t i = 0; i < NumLayers; ++i) {
            if (pixmapId16Bits)
            {
                uint16 pixmapId = gridInfo.layers[i].pixmapId;
                uchar orientation = gridInfo.layers[i].orientation;
                assert(pixmapId == gridInfo.layers[i].pixmapId);
                assert(orientation == gridInfo.layers[i].orientation);
                writer.writeShorts(&pixmapId);
                writer.writeData(&orientation, sizeof orientation);
            }
            else
            {
                uchar v[2];
                v[0] = gridInfo.layers[i].pixmapId;
                v[1] = gridInfo.layers[i].orientation;
                assert(v[0] == gridInfo.layers[i].pixmapId);
                assert(v[1] == gridInfo.layers[i].orientation);
                writer.writeData(&v, sizeof v);
            }
        }
        uchar flags = gridInfo.flags;
        assert(flags == gridInfo.flags);
        writer.writeData(&flags, sizeof flags);
    }
}

bool 
TerrainData::_findGridInfoCopy(const String& fileName, size_t& index)
{
    for (size_t i = 0; i < mGridInfoArrayCopys.size(); ++i)
    {
        if ( mGridInfoArrayCopys[i].fileName == fileName )
        {
            index = i;
            return true;
        }
    }

    return false;
}

struct gridInfoCopyMore 
{
    bool operator()(const TerrainData::GridInfoCopy& a, const TerrainData::GridInfoCopy& b) const
    {
        if ( a.touchTimes > b.touchTimes )
            return true;
        else if ( a.touchTimes < b.touchTimes )
            return false;
        else
        {
            return a.gridCount > b.gridCount;
        }
    }
};


void 
TerrainData::_sortGridInfoCopys(void)
{
    const uint8 MAX_GRIDINFOCOPY = 6;

    if (mGridInfoArrayCopys.size() > MAX_GRIDINFOCOPY)
    {
        std::sort( mGridInfoArrayCopys.begin(), mGridInfoArrayCopys.end(), gridInfoCopyMore() );

        mGridInfoArrayCopys.pop_back();
    }
}

void
TerrainData::_updateDerivedFields(void)
{
    mInvScale.x = 1.0 / mScale.x;
    mInvScale.y = 1.0 / mScale.y;
    mInvScale.z = 1.0 / mScale.z;

    mCentre.x = mPosition.x + mScale.x * mXSize / 2;
    mCentre.y = mPosition.y;
    mCentre.z = mPosition.z + mScale.z * mZSize / 2;

    mMinX = mPosition.x + mScale.x * 0;
    mMaxX = mPosition.x + mScale.x * mXSize;
    mMinZ = mPosition.z + mScale.z * 0;
    mMaxZ = mPosition.z + mScale.z * mZSize;

    mNumTilePerX = (mXSize - 1) / mTileSize + 1;
    mNumTilePerZ = (mZSize - 1) / mTileSize + 1;

    // Allocate buffers
    mHeightmap.resize((mXSize + 1) * (mZSize + 1));
    mGridInfos.resize(mXSize * mZSize);

    mBoundBox.setNull();
}

void
TerrainData::_fixupSupportedMaterials(void)
{
    mSupportedFogReplacements.clear();

    Ogre::GpuProgramManager& gpm = Ogre::GpuProgramManager::getSingleton();
    for (FogReplacements::const_iterator it = mFogReplacements.begin(); it != mFogReplacements.end(); ++it)
    {
        const FogReplacement& fogReplacement = *it;
        bool supported = true;
        for (size_t i = 0; i < sizeof(fogReplacement.programNames)/sizeof(*fogReplacement.programNames); ++i)
        {
            Ogre::GpuProgramPtr program = gpm.getByName(fogReplacement.programNames[i]);
            if (program.isNull() || !program->isSupported())
            {
                supported = false;
                break;
            }

            // Check it is loadable
            try
            {
                program->load();
            }
            catch (const Ogre::Exception& e)
            {
                Ogre::LogManager::getSingleton().logMessage(
                    "Can't load program: " + program->getName() + "\n" + e.getFullDescription());
                supported = false;
                break;
            }
        }

        if (supported)
        {
            mSupportedFogReplacements.push_back(fogReplacement);
        }
    }
}

void
TerrainData::load(XMLParser* parser, const String& filename, const String& groupName, bool loadLightmap)
{
    // Clear old data
    reset();

    mResourceGroupName = groupName;

    // Parse the terrain file
    _loadBaseInfo(parser, filename, groupName);

    _fixupMissingInfo();
    _fixupSupportedMaterials();

    // Setup derived fields
    _updateDerivedFields();

    // Validate the terrain file
    size_t numTextures = mTextures.size();
    for (PixmapArray::const_iterator it = mPixmaps.begin(); it != mPixmaps.end(); ++it)
    {
        const Pixmap& pixmap = *it;
        if (pixmap.textureId >= numTextures)
        {
            OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS,
                "Invalid pixmap textureId in terrain '" + filename + "'.",
                "TerrainData::load");
        }
    }

    // Load heightmap
    _loadHeightmap(mHeightmapFilename, mHeightmapType, groupName);

    // Load grid info
    _loadGridInfo(mGridInfoFilename, groupName);

    if (loadLightmap)
    {
        // Load lightmap
        _loadLightmap(mLightmapFilename, mLightmapType, groupName);
    }
}

void
TerrainData::save(XMLWriter* writer, const String& filename, bool preservingNames) const
{
    assert(writer);

    String baseName, path;
    Ogre::StringUtil::splitFilename(filename, baseName, path);
    if (Ogre::StringUtil::endsWith(baseName, ".xml"))
        baseName.erase(baseName.length() - (sizeof(".xml") - 1));
    if (Ogre::StringUtil::endsWith(baseName, ".terrain"))
        baseName.erase(baseName.length() - (sizeof(".terrain") - 1));
    if (baseName.empty())
    {
        OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS,
            "File name can't start with '.'",
            "TerrainData::save");
    }

    if (!preservingNames || mName.empty())
        const_cast<TerrainData*>(this)->mName = baseName;
    const_cast<TerrainData*>(this)->mHeightmapType = "standard";
	const_cast<TerrainData*>(this)->mLightmapType = "image";

    if (!preservingNames || mHeightmapFilename.empty())
        const_cast<TerrainData*>(this)->mHeightmapFilename = baseName + ".Heightmap";

	if (!mLightmapImage)
	{
		const_cast<TerrainData*>(this)->mLightmapFilename.clear();
	}
	else
	{
		if (!preservingNames || mLightmapFilename.empty())
		{
			if (mLightmapType != "image" && mLightmapImage->getFormat() != Ogre::PF_L8)
				const_cast<TerrainData*>(this)->mLightmapType != "image";
			if (mLightmapType == "image")
				const_cast<TerrainData*>(this)->mLightmapFilename = baseName + ".lightmap.png";
			else
				const_cast<TerrainData*>(this)->mLightmapFilename = baseName + ".Lightmap";
		}
	}

    if (!preservingNames || mGridInfoFilename.empty())
        const_cast<TerrainData*>(this)->mGridInfoFilename = baseName + ".GridInfo";    
	
    _saveHeightmap(path + mHeightmapFilename);
    _saveGridInfo(path + mGridInfoFilename);

	if (!mLightmapFilename.empty())
	{
		_saveLightmap(path + mLightmapFilename);
	}

	_saveBaseInfo(writer, filename);
}

const Ogre::AxisAlignedBox&
TerrainData::getBoundBox() const
{
    if (mBoundBox.isNull())
    {
        if (!mHeightmap.empty())
        {
            RealArray::const_iterator it = mHeightmap.begin();
            Real minHeight = *it;
            Real maxHeight = *it;
            while (++it != mHeightmap.end())
            {
                Real height = *it;
                if (minHeight > height)
                    minHeight = height;
                else if (maxHeight < height)
                    maxHeight = height;
            }
            mBoundBox.setExtents(mMinX, mPosition.y + mScale.y * minHeight, mMinZ,
                                 mMaxX, mPosition.y + mScale.y * maxHeight, mMaxZ);
        }
    }
    else
    {
        if (mHeightmap.empty())
            mBoundBox.setNull();
    }

    return mBoundBox;
}

std::pair<int, int>
TerrainData::getJunction(Real x, Real z) const
{
    return std::pair<int, int>(
        fast_ifloor((x - mPosition.x) * mInvScale.x + (Real)0.5),
        fast_ifloor((z - mPosition.z) * mInvScale.z + (Real)0.5));
}

std::pair<Real, Real>
TerrainData::worldToJunction(Real x, Real z) const
{
    return std::pair<Real, Real>(
        ((x - mPosition.x) * mInvScale.x + (Real)0.5),
        ((z - mPosition.z) * mInvScale.z + (Real)0.5));
}

std::pair<Real, Real>
TerrainData::junctionToWorld(Real x, Real z) const
{
    return std::pair<Real, Real>(
        ((x - (Real)0.5) * mScale.x + mPosition.x),
        ((z - (Real)0.5) * mScale.z + mPosition.z));
}

std::pair<int, int>
TerrainData::getGrid(Real x, Real z) const
{
    return std::pair<int, int>(
        fast_ifloor((x - mPosition.x) * mInvScale.x),
        fast_ifloor((z - mPosition.z) * mInvScale.z));
}

std::pair<Real, Real>
TerrainData::worldToGrid(Real x, Real z) const
{
    return std::pair<Real, Real>(
        ((x - mPosition.x) * mInvScale.x),
        ((z - mPosition.z) * mInvScale.z));
}

std::pair<Real, Real>
TerrainData::gridToWorld(Real x, Real z) const
{
    return std::pair<Real, Real>(
        ((x * mScale.x) + mPosition.x),
        ((z * mScale.z) + mPosition.z));
}

Real
TerrainData::getHeightAt(Real x, Real z) const
{
    if (mXSize <= 0 || mZSize <= 0)
        return 0;

    Real x_pct, z_pct;
    int x_index, z_index;

    // Safety catch, if the point asked for is outside of
    // the map, it will clamp to edge
    if (x <= mMinX)
    {
        x_index = 0;
        x_pct = 0;
    }
    else if (x >= mMaxX)
    {
        x_index = mXSize-1;
        x_pct = 1;
    }
    else
    {
        x_pct = (x - mPosition.x) * mInvScale.x;
        x_index = fast_int(x_pct);
        x_pct -= x_index;
        if (x_index >= mXSize)
        {
            x_index = mXSize-1;
            x_pct = 1;
        }
    }
    if (z <= mMinZ)
    {
        z_index = 0;
        z_pct = 0;
    }
    else if (z >= mMaxZ)
    {
        z_index = mZSize-1;
        z_pct = 1;
    }
    else
    {
        z_pct = (z - mPosition.z) * mInvScale.z;
        z_index = fast_int(z_pct);
        z_pct -= z_index;
        if (z_index >= mZSize)
        {
            z_index = mZSize-1;
            z_pct = 1;
        }
    }

    // bilinear interpolate to find the height.
    Real t, h;
    if (getGridInfo(x_index, z_index).flags & GridInfo::FlipDiagonal)
    {
        if (z_pct <= x_pct) {
            t = _getRawHeight(x_index + 1, z_index + 0);
            h = t +
                (_getRawHeight(x_index + 0, z_index + 0) - t) * (1 - x_pct) +
                (_getRawHeight(x_index + 1, z_index + 1) - t) * z_pct;
        } else {
            t = _getRawHeight(x_index + 0, z_index + 1);
            h = t +
                (_getRawHeight(x_index + 0, z_index + 0) - t) * (1 - z_pct) +
                (_getRawHeight(x_index + 1, z_index + 1) - t) * x_pct;
        }
    }
    else
    {
        if (x_pct + z_pct <= 1) {
            t = _getRawHeight(x_index + 0, z_index + 0);
            h = t +
                (_getRawHeight(x_index + 1, z_index + 0) - t) * x_pct +
                (_getRawHeight(x_index + 0, z_index + 1) - t) * z_pct;
        } else {
            t = _getRawHeight(x_index + 1, z_index + 1);
            h = t +
                (_getRawHeight(x_index + 1, z_index + 0) - t) * (1 - z_pct) +
                (_getRawHeight(x_index + 0, z_index + 1) - t) * (1 - x_pct);
        }
    }

    return mPosition.y + mScale.y * h;
}

Real 
TerrainData::getHeightAtGrid(Real x, Real z) const
{
    std::pair<Real, Real> world = gridToWorld(x, z);
	return getHeightAt(world.first, world.second);
}

Ogre::Vector3
TerrainData::getPositionAt(Real x, Real z) const
{
    return Ogre::Vector3(x, getHeightAt(x, z), z);
}

Ogre::Vector3
TerrainData::getPositionAtGrid(Real x, Real z) const
{
    std::pair<Real, Real> world = gridToWorld(x, z);
    return Ogre::Vector3(
        world.first,
        getHeightAt(world.first, world.second),
        world.second);
}

Ogre::Vector3
TerrainData::getNormalAt(Real x, Real z) const
{
    Ogre::Vector3 position, normal;
    getPositionAndNormalAt(x, z, position, normal);
    return normal;
}

void
TerrainData::getPositionAndNormalAt(Real x, Real z, Ogre::Vector3& position, Ogre::Vector3& normal) const
{
    Ogre::Vector3& here = position;
    here = getPositionAt(x, z);

    // FIXME

    Ogre::Vector3 left;
    left.x = x - 1;
    left.z = z;
    left.y = getHeightAt(left.x, left.z);

    Ogre::Vector3 down;
    down.x = x;
    down.z = z + 1;
    down.y = getHeightAt(down.x, down.z);

    left = left - here;
    down = down - here;

    normal = left.crossProduct(down);
    normal.normalise();
}

Real
TerrainData::getGridHeight(int x, int z) const
{
    assert(isValidGrid(x, z));
    if (getGridInfo(x, z).flags & GridInfo::FlipDiagonal)
        return mPosition.y + (_getRawHeight(x+0,z+0) + _getRawHeight(x+1,z+1)) * mScale.y / 2;
    else
        return mPosition.y + (_getRawHeight(x+1,z+0) + _getRawHeight(x+0,z+1)) * mScale.y / 2;
}

Ogre::Vector3
TerrainData::getGridPosition(int x, int z) const
{
    return Ogre::Vector3(mPosition.x + (x + (Real)0.5) * mScale.x,
                         getGridHeight(x, z),
                         mPosition.z + (z + (Real)0.5) * mScale.z);
}

std::pair<bool, Real>
TerrainData::_intersectGrid(const Ogre::Ray& ray, int x, int z,
                            bool positiveSide, bool negativeSide) const
{
    if (isValidGrid(x, z))
    {
        const Ogre::Vector3 vertices[4] =
        {
            _getPosition(x+0, z+0),
            _getPosition(x+1, z+0),
            _getPosition(x+0, z+1),
            _getPosition(x+1, z+1),
        };
        const Corner* corners = getGridInfo(x, z).getCorners();
        std::pair<bool, Real> r;

        r = Ogre::Math::intersects(ray, vertices[corners[1]], vertices[corners[0]], vertices[corners[2]], positiveSide, negativeSide);
        if (r.first)
            return r;

        r = Ogre::Math::intersects(ray, vertices[corners[1]], vertices[corners[2]], vertices[corners[3]], positiveSide, negativeSide);
        if (r.first)
            return r;
    }

    return std::pair<bool, Real>(false, 0);
}

namespace {
    struct IsCaseInsensitiveEqual
    {
        const String& value;

        IsCaseInsensitiveEqual(const String& value) : value(value) {}

        bool operator() (const String& rhs) const
        {
            return stricmp(value.c_str(), rhs.c_str()) == 0;
        }
    };

    struct CaseInsensitiveLess
    {
        bool operator() (const String& lhs, const String& rhs) const
        {
            return stricmp(lhs.c_str(), rhs.c_str()) < 0;
        }
    };

    static void normalisePath(String& name)
    {
        std::replace(name.begin(), name.end(), '\\', '/');
        // FIXME: erase "./" and "../"
        if (Ogre::StringUtil::startsWith(name, "./", false))
            name.erase(0, 2);
    }
}

int
TerrainData::_registerPixmap(const String& textureName, Real left, Real top, Real right, Real bottom)
{
    size_t pixmapId;
    String texture = textureName;
    normalisePath(texture);
    if (texture.empty())
    {
        pixmapId = 0;
    }
    else
    {
        size_t textureId;
        TextureArray::const_iterator itexture =
            std::find_if(mTextures.begin(), mTextures.end(), IsCaseInsensitiveEqual(texture));
        // If the texture doesn't exist, we will add it later because exception may be throw
        textureId = itexture - mTextures.begin();

        Pixmap pixmap(textureId, left, top, right, bottom);
        PixmapArray::const_iterator ipixmap =
            std::find(mPixmaps.begin(), mPixmaps.end(), pixmap);
        if (ipixmap == mPixmaps.end())
        {
            if (mPixmaps.size() >= 65535)
            {
                OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS,
                    "The terrain brush pixmaps overrun",
                    "TerrainData::_registerPixmap");
            }
            mPixmaps.push_back(pixmap);
            pixmapId = mPixmaps.size();
        }
        else
        {
            pixmapId = ipixmap - mPixmaps.begin() + 1;
        }

        if (itexture == mTextures.end())
        {
            mTextures.push_back(texture);
        }
    }
    assert(pixmapId < 65536);

    return pixmapId;
}

bool
TerrainData::_optimize(void)
{
    bool modified = false;

    // Compute used pixmaps and textures

    std::vector<size_t> usedPixmaps(mPixmaps.size());
    std::vector<size_t> usedTextures(mTextures.size());
    size_t numUsedPixmaps = 0;
    size_t numUsedTextures = 0;
    for (GridInfoArray::const_iterator it = mGridInfos.begin(); it != mGridInfos.end(); ++it)
    {
        const GridInfo& gridInfo = *it;
        for (size_t i = 0; i < NumLayers; ++i)
        {
            size_t pixmapId = gridInfo.layers[i].pixmapId;
            if (!pixmapId) break;
            --pixmapId;

            assert(pixmapId < mPixmaps.size());
            if (!usedPixmaps[pixmapId]++)
            {
                size_t textureId = mPixmaps[pixmapId].textureId;
                assert(textureId < mTextures.size());
                if (!usedTextures[textureId]++)
                {
                    ++numUsedTextures;
                }

                ++numUsedPixmaps;
                if (numUsedPixmaps >= mPixmaps.size())
                    break;
            }
        }
        if (numUsedPixmaps >= mPixmaps.size())
            break;
    }

    {
        // Build textures id mapping

        typedef std::multimap<String, size_t, CaseInsensitiveLess> TextureIdMap;
        TextureIdMap textureIdMap;
        for (size_t oldTextureId = 0; oldTextureId < mTextures.size(); ++oldTextureId)
        {
            if (usedTextures[oldTextureId])
            {
                String texture = mTextures[oldTextureId];
                // Normalise texture name
                normalisePath(texture);
                textureIdMap.insert(TextureIdMap::value_type(texture, oldTextureId));
            }
        }

        // Create mapping of textures id from old to new and the new textures

        TextureArray newTextures;
        newTextures.reserve(numUsedTextures);
        TextureIdMap::const_iterator it = textureIdMap.begin();
        while (it != textureIdMap.end())
        {
            const String& texture = it->first;
            size_t newTextureId = newTextures.size();
            newTextures.push_back(texture);
            do {
                assert(usedTextures[it->second]);
                usedTextures[it->second] = newTextureId;
            } while (++it != textureIdMap.end() && it->first == texture);
        }
        std::swap(mTextures, newTextures);
    }

    {
        // Build pixmaps id mapping

        typedef std::multimap<Pixmap, size_t> PixmapIdMap;
        PixmapIdMap pixmapIdMap;
        for (size_t oldPixmapId = 0; oldPixmapId < mPixmaps.size(); ++oldPixmapId)
        {
            if (usedPixmaps[oldPixmapId])
            {
                Pixmap pixmap = mPixmaps[oldPixmapId];
                // Mapping pixmap reference texture id from old to new
                pixmap.textureId = usedTextures[pixmap.textureId];
                pixmapIdMap.insert(PixmapIdMap::value_type(pixmap, oldPixmapId));
            }
        }

        // Create mapping of pixmaps id from old to new and the new pixmaps

        PixmapArray newPixmaps;
        newPixmaps.reserve(numUsedPixmaps);
        PixmapIdMap::const_iterator it = pixmapIdMap.begin();
        while (it != pixmapIdMap.end())
        {
            const Pixmap& pixmap = it->first;
            size_t newPixmapId = newPixmaps.size();
            newPixmaps.push_back(pixmap);
            do {
                assert(usedPixmaps[it->second]);
                usedPixmaps[it->second] = newPixmapId;
            } while (++it != pixmapIdMap.end() && it->first == pixmap);
        }
        std::swap(mPixmaps, newPixmaps);
    }

    // Mappping grid info reference pixmap id from old to new

    for (GridInfoArray::iterator it = mGridInfos.begin(); it != mGridInfos.end(); ++it)
    {
        GridInfo& gridInfo = *it;
        for (size_t i = 0; i < NumLayers; ++i)
        {
            size_t pixmapId = gridInfo.layers[i].pixmapId;
            if (!pixmapId) break;
            gridInfo.layers[i].pixmapId = usedPixmaps[pixmapId-1]+1;
        }
    }

    // TODO: Check modified exactly
    modified = true;

    return modified;
}

bool
TerrainData::_reshape(int left, int top, int right, int bottom, size_t pixmapId)
{
    assert(pixmapId <= mPixmaps.size());
    if (pixmapId > mPixmaps.size())
        return false;

    assert(left < right && top < bottom);
    int xsize = right - left;
    int zsize = bottom - top;
    if (xsize <= 0 || zsize <= 0)
        return false;

    // Create new heightmap and grid infos
    RealArray heightmap((xsize + 1) * (zsize+1));
    GridInfoArray gridInfos(xsize * zsize);
    int x, z;

    RealArray::iterator itHeightmap = heightmap.begin();
    for (z = top; z <= bottom; ++z)
    {
        for (x = left; x <= right; ++x)
        {
            *itHeightmap++ = _getRawHeight(std::min(mXSize, std::max(0, x)), std::min(mZSize, std::max(0, z)));
        }
    }
    assert(itHeightmap == heightmap.end());	

    GridInfo fillGridInfo = { pixmapId, 0, 0, 0, 0, };
    GridInfoArray::iterator itGridInfo = gridInfos.begin();
    for (z = top; z < bottom; ++z)
    {
        for (x = left; x < right; ++x)
        {
            *itGridInfo++ = isValidGrid(x, z) ? getGridInfo(x, z) : fillGridInfo;
        }
    }
    assert(itGridInfo == gridInfos.end());

    // Setup affected fields
    mXSize = xsize;
    mZSize = zsize;
    mPosition.x += mScale.x * left;
    mPosition.z += mScale.z * top;

    // Store new heightmap and grid infos
    std::swap(mHeightmap, heightmap);
    std::swap(mGridInfos, gridInfos);

	// Just destroy lightmap, TODO: keep lightmap as soon as possible
	if (mLightmapImage)
	{
		delete mLightmapImage;
		mLightmapImage = 0;
	}

    // Fill up derived fields
    _updateDerivedFields();

    return true;
}

bool
TerrainData::getIntersects(const Ogre::Ray& ray, Ogre::Vector3& position, Ogre::Vector3* normal, bool allowOutside,
                           bool positiveSide, bool negativeSide) const
{
    float x, z;

    if (ray.getDirection().x == 0 && ray.getDirection().z == 0)
    {
        // special case ...

        if (!negativeSide && ray.getDirection().y >= 0)
            return false;
        if (!positiveSide && ray.getDirection().y <= 0)
            return false;

        x = ray.getOrigin().x;
        z = ray.getOrigin().z;
        if (!allowOutside && !isValidCoord(x, z))
            return false;

        Real h = getHeightAt(x, z);

        if (!negativeSide && ray.getOrigin().y < h)
            return false;

        if (!positiveSide && ray.getOrigin().y > h)
            return false;
    }
    else
    {
        // TODO: allowOutside

        // check bounding box intersection, find the intersection
        // start and end of the bounding box
        Real d1, d2;
        if (!Ogre::Math::intersects(ray, getBoundBox(), &d1, &d2))
            return false;

        Ogre::Vector3 start = ray.getPoint(d1);
        Ogre::Vector3 end = ray.getPoint(d2);

        // intersect test through each grid that 'touch' the line segment
        std::pair<bool ,Real> r(false, 0);

        Real xstep = ray.getDirection().x * mScale.z;
        Real zstep = ray.getDirection().z * mScale.x;
        if (Ogre::Math::Abs(xstep) < Ogre::Math::Abs(zstep))
        {
            // step base z axis
            int zinc = ray.getDirection().z > 0 ? +1 : -1;
            Real zstart = (start.z - mPosition.z) * mInvScale.z;
            int zbegin = fast_ifloor(zstart) - zinc;
            int zend = fast_ifloor((end.z - mPosition.z) * mInvScale.z) + zinc;
            Real xfactor = xstep / zstep;
            Real xinc = zinc * xfactor;
            Real xiter = ((start.x - mPosition.x) * mInvScale.x) - (zstart - zbegin) * xfactor;
            assert((zend - zbegin) * zinc > 0);
            assert(Ogre::Math::Abs(xinc) <= 1);
            for (int zi = zbegin; zi != zend; zi += zinc, xiter += xinc)
            {
                int xi = fast_ifloor(xiter);
                r = _intersectGrid(ray, xi+0, zi, positiveSide, negativeSide);
                if (r.first)
                    break;
                r = _intersectGrid(ray, xi-1, zi, positiveSide, negativeSide);
                if (r.first)
                    break;
                r = _intersectGrid(ray, xi+1, zi, positiveSide, negativeSide);
                if (r.first)
                    break;
            }
        }
        else
        {
            // step base x azis
            int xinc = ray.getDirection().x > 0 ? +1 : -1;
            Real xstart = (start.x - mPosition.x) * mInvScale.x;
            int xbegin = fast_ifloor(xstart) - xinc;
            int xend = fast_ifloor((end.x - mPosition.x) * mInvScale.x) + xinc;
            Real zfactor = zstep / xstep;
            Real zinc = xinc * zfactor;
            Real ziter = ((start.z - mPosition.z) * mInvScale.z) - (xstart - xbegin) * zfactor;
            assert((xend - xbegin) * xinc > 0);
            assert(Ogre::Math::Abs(zinc) <= 1);
            for (int xi = xbegin; xi != xend; xi += xinc, ziter += zinc)
            {
                int zi = fast_ifloor(ziter);
                r = _intersectGrid(ray, xi, zi+0, positiveSide, negativeSide);
                if (r.first)
                    break;
                r = _intersectGrid(ray, xi, zi-1, positiveSide, negativeSide);
                if (r.first)
                    break;
                r = _intersectGrid(ray, xi, zi+1, positiveSide, negativeSide);
                if (r.first)
                    break;
            }
        }

        if (!r.first)
            return false;

        x = ray.getOrigin().x + ray.getDirection().x * r.second;
        z = ray.getOrigin().z + ray.getDirection().z * r.second;
    }

	if (normal)
		getPositionAndNormalAt(x, z, position, *normal);
	else
		position = getPositionAt(x, z);
    return true;
}

static uint
getClippingMask(const Ogre::Matrix4& m, const Ogre::Vector3& v)
{
    Ogre::Vector4 p;
    p = v;
    p = m * p;

    uint mask = 0;
    if (p.x <= -p.w) mask |= 1 << Ogre::FRUSTUM_PLANE_LEFT;
    if (p.x >= +p.w) mask |= 1 << Ogre::FRUSTUM_PLANE_RIGHT;
    if (p.y <= -p.w) mask |= 1 << Ogre::FRUSTUM_PLANE_BOTTOM;
    if (p.y >= +p.w) mask |= 1 << Ogre::FRUSTUM_PLANE_TOP;
    if (p.z <= -p.w) mask |= 1 << Ogre::FRUSTUM_PLANE_NEAR;
    if (p.z >= +p.w) mask |= 1 << Ogre::FRUSTUM_PLANE_FAR;
    return mask;
}

size_t
TerrainData::getIntersectsTriangles(const Ogre::Frustum* frustum, int areaLimit,
                                    bool receivedDecalOnly,
                                    std::vector<Ogre::Vector3>& positions, std::vector<Ogre::Vector3>* normals)
{
    assert(areaLimit >= 0);

    const Ogre::AxisAlignedBox& bound = getBoundBox();
    if (!frustum->isVisible(bound))
        return false;

    // Clip by Y plane to found terrain region that affect by the frustum

    const Ogre::Vector3* corners = frustum->getWorldSpaceCorners();
    static const size_t edges[][2] =
    {
        0, 1,       // top-near
        1, 2,       // left-near
        2, 3,       // bottom-near
        3, 0,       // right-near

        4, 5,       // top-far
        5, 6,       // left-far
        6, 7,       // bottom-far
        7, 4,       // right-far

        0, 4,       // top-right
        1, 5,       // top-left
        2, 6,       // bottom-left
        3, 7,       // bottom-right
    };

    const Real minY = bound.getMinimum().y;
    const Real maxY = bound.getMaximum().y;
    Real minX = Ogre::Math::POS_INFINITY;
    Real minZ = Ogre::Math::POS_INFINITY;
    Real maxX = Ogre::Math::NEG_INFINITY;
    Real maxZ = Ogre::Math::NEG_INFINITY;
    const Real epsilon = 1e-3f;
    for (size_t e = 0; e < sizeof(edges)/sizeof(*edges); ++e)
    {
        const Ogre::Vector3& v0 = corners[edges[e][0]];
        const Ogre::Vector3& v1 = corners[edges[e][1]];
        Real diff = v1.y - v0.y;
        if (Ogre::Math::Abs(diff) <= epsilon)
        {
            if (Ogre::Math::RealEqual(minY, v0.y, epsilon) || Ogre::Math::RealEqual(maxY, v0.y, epsilon))
            {
                Real x = v0.x;
                Real z = v0.z;
                if (minX > x) minX = x;
                if (maxX < x) maxX = x;
                if (minZ > z) minZ = z;
                if (maxZ < z) maxZ = z;
            }
        }
        else
        {
            Real denom = 1 / diff;
            Real t;

            t = (minY - v0.y) * denom;
            if (0 <= t && t <= 1)
            {
                Real x = v0.x + (v1.x - v0.x) * t;
                Real z = v0.z + (v1.z - v0.z) * t;
                if (minX > x) minX = x;
                if (maxX < x) maxX = x;
                if (minZ > z) minZ = z;
                if (maxZ < z) maxZ = z;
            }

            t = (maxY - v0.y) * denom;
            if (0 <= t && t <= 1)
            {
                Real x = v0.x + (v1.x - v0.x) * t;
                Real z = v0.z + (v1.z - v0.z) * t;
                if (minX > x) minX = x;
                if (maxX < x) maxX = x;
                if (minZ > z) minZ = z;
                if (maxZ < z) maxZ = z;
            }
        }
    }

    if (!(minX < maxX && minZ < maxZ))
    {
        // The projector may be full inside the terrain bounding box, so use the
        // projector world bounding box to determinate the intersection triangles
        const Ogre::AxisAlignedBox& aabb = frustum->getWorldBoundingBox();
        minX = aabb.getMinimum().x; minZ = aabb.getMinimum().z;
        maxX = aabb.getMaximum().x; maxZ = aabb.getMaximum().z;
    }

    if (!(minX < maxX && minZ < maxZ))
        return 0;

    int startX = std::max(fast_ifloor((minX - this->mPosition.x) * this->mInvScale.x), 0);
    int startZ = std::max(fast_ifloor((minZ - this->mPosition.z) * this->mInvScale.z), 0);
    int endX = std::min(fast_iceil((maxX - this->mPosition.x) * this->mInvScale.x), this->mXSize);
    int endZ = std::min(fast_iceil((maxZ - this->mPosition.z) * this->mInvScale.z), this->mZSize);
    if (!(startX < endX && startZ < endZ))
        return 0;

    Ogre::Matrix4 eyeToWorld = frustum->getViewMatrix().inverse();

    // Adjust max vertices
    int sizeX = endX - startX;
    int sizeZ = endZ - startZ;
    if (sizeX*sizeZ > areaLimit)
    {
        Ogre::Ray ray(
            Ogre::Vector3(+eyeToWorld[0][3], +eyeToWorld[1][3], +eyeToWorld[2][3]),
            Ogre::Vector3(-eyeToWorld[0][2], -eyeToWorld[1][2], -eyeToWorld[2][2]));
        Ogre::Vector3 position;
        if (!this->getIntersects(ray, position))
            return 0; // just in case

        std::pair<int, int> hit = this->getJunction(position.x, position.z);
        if (sizeX < sizeZ)
        {
            sizeX = std::max(fast_ifloor(Ogre::Math::Sqrt(
                (Real)areaLimit * sizeX / sizeZ)), 1);
            sizeZ = areaLimit / sizeX;
        }
        else
        {
            sizeZ = std::max(fast_ifloor(Ogre::Math::Sqrt(
                (Real)areaLimit * sizeZ / sizeX)), 1);
            sizeX = areaLimit / sizeZ;
        }
        startX = std::max(hit.first - sizeX / 2, 0);
        startZ = std::max(hit.second - sizeZ / 2, 0);
        endX = std::min(startX + sizeX, this->mXSize);
        endZ = std::min(startZ + sizeZ, this->mZSize);
        assert(startX < endX && startZ < endZ);
        if (!(startX < endX && startZ < endZ))
            return 0; // just in case
    }
    size_t maxVertices = sizeX * sizeZ * 2 * 3;

    // Pre-allocate buffer
    positions.reserve(positions.size() + maxVertices);
    if (normals)
        normals->reserve(normals->size() + maxVertices);

    // Compute view and projection matrix for fast clipping 
    Ogre::Matrix4 viewProjectionMatrix =
        frustum->getProjectionMatrix() * frustum->getViewMatrix();

    // Get the view vector base on projection type
    Ogre::Vector4 vv;
    if (frustum->getProjectionType() == Ogre::PT_PERSPECTIVE)
    {
        // Use view position as view vector
        vv[0] = eyeToWorld[0][3];
        vv[1] = eyeToWorld[1][3];
        vv[2] = eyeToWorld[2][3];
        vv[3] = eyeToWorld[3][3];
    }
    else
    {
        // Use view direction as view vector
        vv[0] = eyeToWorld[0][2];
        vv[1] = eyeToWorld[1][2];
        vv[2] = eyeToWorld[2][2];
        vv[3] = eyeToWorld[3][2];
    }

    // Gather triangles
    size_t numTriangles = 0;
    for (int z = startZ; z < endZ; ++z)
    {
        for (int x = startX; x < endX; ++x)
        {
            const GridInfo& gridInfo = this->getGridInfo(x, z);

            // Skip decal disabled grids
            if (receivedDecalOnly && (gridInfo.flags & gridInfo.DisableDecal))
                continue;

            const Ogre::Vector3 p[4] =
            {
                this->_getPosition(x+0, z+0),
                this->_getPosition(x+1, z+0),
                this->_getPosition(x+0, z+1),
                this->_getPosition(x+1, z+1),
            };

            const uint m[4] =
            {
                getClippingMask(viewProjectionMatrix, p[0]),
                getClippingMask(viewProjectionMatrix, p[1]),
                getClippingMask(viewProjectionMatrix, p[2]),
                getClippingMask(viewProjectionMatrix, p[3]),
            };
            if (m[0] & m[1] & m[2] & m[3])
                continue;

            const Ogre::Vector3 n[4] =
            {
                this->_getNormal(x+0, z+0),
                this->_getNormal(x+1, z+0),
                this->_getNormal(x+0, z+1),
                this->_getNormal(x+1, z+1),
            };

            const TerrainData::Corner *corners = gridInfo.getCorners();
            Ogre::Vector4 faceNormal;

            faceNormal = Ogre::Math::calculateFaceNormalWithoutNormalize(p[corners[1]], p[corners[0]], p[corners[2]]);
            if (faceNormal.dotProduct(vv) > 0)
            {
                positions.push_back(p[corners[1]]);
                positions.push_back(p[corners[0]]);
                positions.push_back(p[corners[2]]);
                if (normals)
                {
                    normals->push_back(n[corners[1]]);
                    normals->push_back(n[corners[0]]);
                    normals->push_back(n[corners[2]]);
                }
                ++numTriangles;
            }

            faceNormal = Ogre::Math::calculateFaceNormalWithoutNormalize(p[corners[1]], p[corners[2]], p[corners[3]]);
            if (faceNormal.dotProduct(vv) > 0)
            {
                positions.push_back(p[corners[1]]);
                positions.push_back(p[corners[2]]);
                positions.push_back(p[corners[3]]);
                if (normals)
                {
                    normals->push_back(n[corners[1]]);
                    normals->push_back(n[corners[2]]);
                    normals->push_back(n[corners[3]]);
                }
                ++numTriangles;
            }
        }
    }

    return numTriangles * 3;
}

}
