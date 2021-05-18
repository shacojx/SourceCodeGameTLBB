#include "WXTerrainLiquid.h"
#include "Terrain.h"
#include "BasicRenderable.h"
#include "WXSystem.h"
#include "TerrainTypeInfos.h"

#include <OgreMaterialManager.h>
#include <OgreCamera.h>
#include <OgreNode.h>
#include <OgreLogManager.h>

#include <boost/scoped_array.hpp>

#define LIQUID_SPLIT_SIZE          64
#define LIQUID_SPLIT_THRESHOLD     4096

namespace WX {

    class LiquidRenderable : public BasicRenderable
    {
    public:
        Ogre::AxisAlignedBox mBoundingBox;

    public:
        LiquidRenderable(TerrainLiquid* liquid)
            : BasicRenderable(liquid)
            , mBoundingBox()
        {
        }

        ~LiquidRenderable()
        {
            delete mRenderOp.indexData;
        }
        
        Ogre::Real getSquaredViewDepth(const Ogre::Camera* camera) const
        {
            const Ogre::Vector3& cameraPos = camera->getDerivedPosition();
            const Ogre::AxisAlignedBox& aabb = mBoundingBox;
            Ogre::Vector3 diff(0, 0, 0);
            diff.makeFloor(cameraPos - aabb.getMinimum());
            diff.makeCeil(cameraPos - aabb.getMaximum());

            return diff.squaredLength();
        }
    };

    //////////////////////////////////////////////////////////////////////////

    const String TerrainLiquid::msMovableType = "TerrainLiquid";
    uint TerrainLiquid::ms_uGenNameCount = 0;

    //-----------------------------------------------------------------------
    TerrainLiquid::TerrainLiquid(System* system, const Ogre::Vector3& position, size_t subdivision, size_t vertexLimit)
        : mSystem(system)
        , mTerrain(NULL)
        , mPosition(position)
        , mSubdivision(subdivision)
        , mVertexLimit(vertexLimit)
        , mTexcoordScale(1.0)
		, mTexcoordRotate(0.0)
        , mDepthTexLayerEnabled(false)
        , mDepthTexLayerScale(1.0)
        , mDepthTexLayerAdjust(0.0)
        , mProjectionCamera(0)
        , mVertexFlags()
        , mAllocatedBuffer()
        , mNormalBuffer()
        , mUsedBaseX()
        , mUsedBaseZ()
        , mUsedWidth()
        , mUsedHeight()
        , mNumUsedVertices()

        , mLastFrameTime(0)
        , mRemainTimeStamp(0)
        , mVertexBuffersDirty(true)
        , mTexcoordsDirty(true)

        , mVertexData()
        , mIndexBuffer()
        , mLiquidRenderables()
        , mCurrentCamera()
    {
        mTerrain = mSystem->getTerrain();
        assert(mTerrain);
        assert(0 < subdivision && subdivision <= 16);
        assert((subdivision & (subdivision-1)) == 0);
        assert(0 < vertexLimit && vertexLimit < 65500);

#ifdef _DEBUG
        mLevelBuffers[0] = NULL;
        mLevelBuffers[1] = NULL;
        mLevelBuffers[2] = NULL;
        mPositionBuffer = NULL;
#endif

        // Generate name
        Ogre::StringUtil::StrStreamType str;
        str << msMovableType << ms_uGenNameCount++;
        mName = str.str();

        // initialize algorithm parameters
        PARAM_C = 0.3f;     // ripple speed 
        PARAM_D = 0.4f;     // distance
        PARAM_U = 0.05f;    // viscosity
        PARAM_T = 0.13f;    // time
        mAnimationIteratorInterval = 1.0f / 100.0f;
        useFakeNormals = false;

        setMaterialName("BaseWhite");
        build();
    }
    //-----------------------------------------------------------------------
    TerrainLiquid::~TerrainLiquid()
    {
        cleanup();
    }
    //-----------------------------------------------------------------------
    const String& TerrainLiquid::getMovableType(void) const
    {
        return msMovableType;
    }
    //-----------------------------------------------------------------------
    const Ogre::AxisAlignedBox& TerrainLiquid::getBoundingBox(void) const
    {
        return mBoundingBox;
    }
    //-----------------------------------------------------------------------
    Real TerrainLiquid::getBoundingRadius(void) const
    {
        return (mBoundingBox.getMaximum() - mBoundingBox.getCenter()).length();
    }
    //-----------------------------------------------------------------------
    void TerrainLiquid::_notifyCurrentCamera(Ogre::Camera* camera)
    {
        mCurrentCamera = camera;
        if (mProjectionCamera)
        {
            const Ogre::Vector3& viewPos = camera->getDerivedPosition();
            mProjectionCamera->setPosition(viewPos.x, mPosition.y * 2 - viewPos.y, viewPos.z);
        }

        Ogre::MovableObject::_notifyCurrentCamera(camera);
    }
    //-----------------------------------------------------------------------
    void TerrainLiquid::_updateRenderQueue(Ogre::RenderQueue *queue)
    {
        if (!mLiquidRenderables.empty())
        {
            updateVertexBuffers();

            Ogre::uint8 renderQueueID;
            if (mRenderQueueIDSet)
                renderQueueID = mRenderQueueID;
            else
                renderQueueID = Ogre::RENDER_QUEUE_WORLD_GEOMETRY_2;

            for (LiquidRenderableList::const_iterator it = mLiquidRenderables.begin(); it != mLiquidRenderables.end(); ++it)
            {
                LiquidRenderable* liquidRenderable = *it;
                // Send to render queue only when it's visible
                if (mCurrentCamera->isVisible(liquidRenderable->mBoundingBox))
                {
                    queue->addRenderable(liquidRenderable, renderQueueID);
                }
            }
        }
    }
    //-----------------------------------------------------------------------
    void TerrainLiquid::setMaterialName(const String& materialName)
    {
        mMaterialName = materialName;
        mMaterial = Ogre::MaterialManager::getSingleton().getByName(materialName);
        if (mMaterial.isNull())
            OGRE_EXCEPT(Ogre::Exception::ERR_ITEM_NOT_FOUND,
                "Could not find material " + materialName,
                "TerrainLiquid::setMaterialName");

        updateRenderables();
    }
    //-----------------------------------------------------------------------
    const String& TerrainLiquid::getMaterialName(void) const
    {
        return mMaterialName;
    }
    //-----------------------------------------------------------------------
    void TerrainLiquid::setTexcoordScale(Real scale)
    {
        if (mTexcoordScale != scale)
        {
            mTexcoordScale = scale;
            mTexcoordsDirty = true;
        }
    }
    //-----------------------------------------------------------------------
    Real TerrainLiquid::getTexcoordScale(void) const
    {
        return mTexcoordScale;
    }
    //-----------------------------------------------------------------------
    void TerrainLiquid::setTexcoordRotate(Ogre::Radian rotate)
    {
        if (mTexcoordRotate != rotate)
        {
            mTexcoordRotate = rotate;
            mTexcoordsDirty = true;
        }
    }
    //-----------------------------------------------------------------------
    Ogre::Radian TerrainLiquid::getTexcoordRotate(void) const
    {
        return mTexcoordRotate;
    }
    //-----------------------------------------------------------------------
    void TerrainLiquid::setDepthTexLayerSettings(bool enable, Real scale, Real adjust)
    {
        if (mDepthTexLayerEnabled != enable ||
            mDepthTexLayerScale != scale ||
            mDepthTexLayerAdjust != adjust)
        {
            mDepthTexLayerEnabled = enable;
            mDepthTexLayerScale = scale;
            mDepthTexLayerAdjust = adjust;
            mTexcoordsDirty = true;
        }
    }
    //-----------------------------------------------------------------------
    bool TerrainLiquid::isDepthTexLayerEnabled(void) const
    {
        return mDepthTexLayerEnabled;
    }
    //-----------------------------------------------------------------------
    Real TerrainLiquid::getDepthTexLayerScale(void) const
    {
        return mDepthTexLayerScale;
    }
    //-----------------------------------------------------------------------
    Real TerrainLiquid::getDepthTexLayerAdjust(void) const
    {
        return mDepthTexLayerAdjust;
    }
    //-----------------------------------------------------------------------
    void TerrainLiquid::setProjectionCamera(Ogre::Camera* camera)
    {
        mProjectionCamera = camera;
    }
    //-----------------------------------------------------------------------
    void TerrainLiquid::cleanup(void)
    {
        mUsedBaseX = 0;
        mUsedBaseZ = 0;
        mUsedWidth = 0;
        mUsedHeight = 0;
        mNumUsedVertices = 0;
        delete [] mVertexFlags; mVertexFlags = NULL;
        delete [] mAllocatedBuffer; mAllocatedBuffer = NULL;
#ifdef _DEBUG
        mLevelBuffers[0] = NULL;
        mLevelBuffers[1] = NULL;
        mLevelBuffers[2] = NULL;
        mPositionBuffer = NULL;
#endif
        delete [] mNormalBuffer; mNormalBuffer = NULL;
        mBoundingBox.setNull();

        delete mVertexData; mVertexData = NULL;
        mIndexBuffer.setNull();
        for (LiquidRenderableList::const_iterator it = mLiquidRenderables.begin(); it != mLiquidRenderables.end(); ++it)
        {
            delete *it;
        }
        mLiquidRenderables.swap(LiquidRenderableList());
    }
    //-----------------------------------------------------------------------
    namespace {
        struct Triangle
        {
            size_t v0, v1, v2;

            Triangle(void) {}
            Triangle(size_t v0, size_t v1, size_t v2) : v0(v0), v1(v1), v2(v2) {}

            bool operator== (const Triangle& rhs) const
            {
                return this->v0 == rhs.v0 &&
                       this->v1 == rhs.v1 &&
                       this->v2 == rhs.v2;
            }

            bool operator< (const Triangle& rhs) const
            {
                if (this->v0 < rhs.v0) return true;
                if (this->v0 > rhs.v0) return false;
                if (this->v1 < rhs.v1) return true;
                if (this->v1 > rhs.v1) return false;
                return this->v2 < rhs.v2;
            }
        };

        struct MappedTriangle : public Triangle
        {
            size_t id;

            MappedTriangle(void) {}

            bool operator== (const MappedTriangle& rhs) const
            {
                return this->id == rhs.id && Triangle::operator== (rhs);
            }

            bool operator< (const MappedTriangle& rhs) const
            {
                if (this->id < rhs.id) return true;
                if (this->id > rhs.id) return false;
                return Triangle::operator< (rhs);
            }
        };

        typedef std::vector<Triangle> TriangleList;
        typedef std::vector<MappedTriangle> MappedTriangleList;
    }
    //-----------------------------------------------------------------------
    bool TerrainLiquid::build(void)
    {
        cleanup();

        Ogre::StringUtil::StrStreamType str;
        str << "Building liquid at position: " << mPosition;
        Ogre::LogManager::getSingleton().logMessage(str.str());

        // TODO: take mSubdivision into account

        const TerrainData* data = mTerrain->getData();
        size_t width = data->getXSize();
        size_t height = data->getZSize();
        size_t maxVertices = (width+1) * (height+1);
        boost::scoped_array<bool> enabledVerticesArray(new bool[maxVertices]);
        boost::scoped_array<bool> visitedVerticesArray(new bool[maxVertices]);
        boost::scoped_array<bool> touchedVerticesArray(new bool[maxVertices]);
        bool* enabledVertices = enabledVerticesArray.get();
        bool* visitedVertices = visitedVerticesArray.get();
        bool* touchedVertices = touchedVerticesArray.get();
        memset(enabledVertices, 0, maxVertices * sizeof(*enabledVertices));
        memset(visitedVertices, 0, maxVertices * sizeof(*visitedVertices));
        memset(touchedVertices, 0, maxVertices * sizeof(*touchedVertices));
        TriangleList triangles;
        const Real* heightmap = &data->mHeightmap[0];
        const Real level = (mPosition.y - data->mPosition.y) * data->mInvScale.y;
        size_t numEnabledVertices = 0;
        size_t numVisitedVertices = 0;
        int minX = width;
        int maxX = 0;
        int minZ = height;
        int maxZ = 0;

        typedef std::pair<int, int> Pos;
        typedef std::list<Pos> PosList;
        PosList pendingVertices;

#define _LOC(x,z)               ((z)*(width+1)+(x))
#define _LOCP(p)                _LOC((p).first, (p).second)
#define _IS_VALID(x,z)          ((size_t)(x) <= width && (size_t)(z) <= height)
#define _IS_OK(x,z)             (_IS_VALID((x),(z)) && level >= heightmap[_LOC((x),(z))])

#define _PUSH(x,z)                                                      \
        do {                                                            \
            if (!touchedVertices[_LOC((x),(z))])                        \
            {                                                           \
                touchedVertices[_LOC((x),(z))] = true;                  \
                pendingVertices.push_back(Pos((x),(z)));                \
            }                                                           \
        } while (0)

#define _PUSH_IF_VALID(x,z)                                             \
        do {                                                            \
            if (_IS_VALID((x),(z)))                                     \
                _PUSH((x),(z));                                         \
        } while(0)

#define _PUSH_IF_OK(x,z)                                                \
        do {                                                            \
            if (_IS_OK((x),(z)))                                        \
                _PUSH((x),(z));                                         \
        } while(0)

#define _ADD_TRIANGLE(x1,z1,x2,z2)                                      \
    do {                                                                \
        if (visitedVertices[_LOC(pos.first+(x1), pos.second+(z1))] &&   \
            visitedVertices[_LOC(pos.first+(x2), pos.second+(z2))])     \
        {                                                               \
            triangles.push_back(Triangle(                               \
                _LOC(pos.first,      pos.second),                       \
                _LOC(pos.first+(x1), pos.second+(z1)),                  \
                _LOC(pos.first+(x2), pos.second+(z2))));                \
        }                                                               \
    } while(0)

        // Flood fill initialise positions
        Pos initPos = data->getGrid(mPosition.x, mPosition.z);
        _PUSH_IF_OK(initPos.first+0, initPos.second+0);
        _PUSH_IF_OK(initPos.first+1, initPos.second+0);
        _PUSH_IF_OK(initPos.first+0, initPos.second+1);
        _PUSH_IF_OK(initPos.first+1, initPos.second+1);

        while (!pendingVertices.empty())
        {
            Pos pos = pendingVertices.front();
            pendingVertices.pop_front();

            if (visitedVertices[_LOCP(pos)])
                continue;
            visitedVertices[_LOCP(pos)] = true;
            ++numVisitedVertices;

            // Update bounds
            if (minX > pos.first) minX = pos.first;
            if (maxX < pos.first) maxX = pos.first;
            if (minZ > pos.second) minZ = pos.second;
            if (maxZ < pos.second) maxZ = pos.second;

            // Reserve space for store triangles
            if (triangles.capacity() < triangles.size() + 8)
            {
                if (triangles.capacity() < 1024)
                {
                    triangles.reserve(triangles.capacity() + 1024);
                }
                else
                {
                    triangles.reserve(triangles.capacity() * 2);
                }
            }

            // Add new triangles
            if (pos.first > 0)
            {
                if (pos.second > 0)
                {
                    if (data->getGridInfo(pos.first-1, pos.second-1).flags & TerrainData::GridInfo::FlipDiagonal)
                    {
                        // *--+
                        // |\ |
                        // | \|
                        // +--o
                        _ADD_TRIANGLE(-1,-1,  0,-1);
                        _ADD_TRIANGLE(-1, 0, -1,-1);
                    }
                    else
                    {
                        // +--+
                        // | /|
                        // |/ |
                        // +--o
                        _ADD_TRIANGLE(-1, 0,  0,-1);
                    }
                }

                if ((size_t)pos.second < height)
                {
                    if (!(data->getGridInfo(pos.first-1, pos.second).flags & TerrainData::GridInfo::FlipDiagonal))
                    {
                        // +--o
                        // | /|
                        // |/ |
                        // *--+
                        _ADD_TRIANGLE(-1,+1, -1, 0);
                        _ADD_TRIANGLE( 0,+1, -1,+1);
                    }
                    else
                    {
                        // +--o
                        // |\ |
                        // | \|
                        // +--+
                        _ADD_TRIANGLE( 0,+1,  -1, 0);
                    }
                }
            }

            if ((size_t)pos.first < width)
            {
                if (pos.second > 0)
                {
                    if (!(data->getGridInfo(pos.first, pos.second-1).flags & TerrainData::GridInfo::FlipDiagonal))
                    {
                        // +--*
                        // | /|
                        // |/ |
                        // o--+
                        _ADD_TRIANGLE(+1,-1, +1, 0);
                        _ADD_TRIANGLE( 0,-1, +1,-1);
                    }
                    else
                    {
                        // +--+
                        // |\ |
                        // | \|
                        // o--+
                        _ADD_TRIANGLE( 0,-1, +1, 0);
                    }
                }

                if ((size_t)pos.second < height)
                {
                    if (data->getGridInfo(pos.first, pos.second).flags & TerrainData::GridInfo::FlipDiagonal)
                    {
                        // o--+
                        // |\ |
                        // | \|
                        // +--*
                        _ADD_TRIANGLE(+1,+1,  0,+1);
                        _ADD_TRIANGLE(+1, 0, +1,+1);
                    }
                    else
                    {
                        // o--+
                        // | /|
                        // |/ |
                        // +--+
                        _ADD_TRIANGLE(+1, 0,  0,+1);
                    }
                }
            }

            // Check it isn't edge
            if (pos.first <= 0  || (size_t)pos.first >= width ||
                pos.second <= 0 || (size_t)pos.second >= height)
                continue;

            // Check is lower than level
            if (level < heightmap[_LOCP(pos)])
                continue;

            enabledVertices[_LOCP(pos)] = true;
            ++numEnabledVertices;

            // set the terrain type and the height of the liquid
            mSystem->getTerrainTypeInfos()->setTerrainTypeInfo(pos.first, pos.second,
                TerrainTypeInfos::TerrainType_LIQUID, mPosition.y);

            // Stop hand on if overrun, but the stacked vertices must be continue process
            if (numVisitedVertices + pendingVertices.size() + 8 >= mVertexLimit)
                continue;

            // Propagate along the edges

            _PUSH_IF_VALID(pos.first-1, pos.second);
            _PUSH_IF_VALID(pos.first+1, pos.second);
            _PUSH_IF_VALID(pos.first, pos.second-1);
            _PUSH_IF_VALID(pos.first, pos.second+1);

            if (pos.first > 0)
            {
                if (pos.second > 0)
                {
                    if (data->getGridInfo(pos.first-1, pos.second-1).flags & TerrainData::GridInfo::FlipDiagonal)
                    {
                        // *--+
                        // |\ |
                        // | \|
                        // +--o
                        _PUSH(pos.first-1, pos.second-1);
                    }
                    else
                    {
                        // +--+
                        // | /|
                        // |/ |
                        // +--o
                    }
                }

                if ((size_t)pos.second < height)
                {
                    if (!(data->getGridInfo(pos.first-1, pos.second).flags & TerrainData::GridInfo::FlipDiagonal))
                    {
                        // +--o
                        // | /|
                        // |/ |
                        // *--+
                        _PUSH(pos.first-1, pos.second+1);
                    }
                    else
                    {
                        // +--o
                        // |\ |
                        // | \|
                        // +--+
                    }
                }
            }

            if ((size_t)pos.first < width)
            {
                if (pos.second > 0)
                {
                    if (!(data->getGridInfo(pos.first, pos.second-1).flags & TerrainData::GridInfo::FlipDiagonal))
                    {
                        // +--*
                        // | /|
                        // |/ |
                        // o--+
                        _PUSH(pos.first+1, pos.second-1);
                    }
                    else
                    {
                        // +--+
                        // |\ |
                        // | \|
                        // o--+
                    }
                }

                if ((size_t)pos.second < height)
                {
                    if (data->getGridInfo(pos.first, pos.second).flags & TerrainData::GridInfo::FlipDiagonal)
                    {
                        // o--+
                        // |\ |
                        // | \|
                        // +--*
                        _PUSH(pos.first+1, pos.second+1);
                    }
                    else
                    {
                        // o--+
                        // | /|
                        // |/ |
                        // +--+
                    }
                }
            }
        }

        if (triangles.empty())
            return false;

        assert(numVisitedVertices <= 65536);
        if (numVisitedVertices > 65536)
            return false;   // Just in case

        // Setup basic parameters
        mUsedBaseX = minX;
        mUsedBaseZ = minZ;
        mUsedWidth = maxX - minX + 1;
        mUsedHeight = maxZ - minZ + 1;
        mNumUsedVertices = numVisitedVertices;
        assert(mUsedWidth >= 2 && mUsedHeight >= 2);
        assert(mNumUsedVertices <= mUsedWidth * mUsedHeight);

        // Setup bounding box
        mBoundingBox.setExtents(
            data->mScale.x * minX + data->mPosition.x, mPosition.y, data->mScale.z * minZ + data->mPosition.z,
            data->mScale.x * maxX + data->mPosition.x, mPosition.y, data->mScale.z * maxZ + data->mPosition.z);

        str.str(Ogre::StringUtil::BLANK);
        str << "Bounding box: " << mBoundingBox;
        Ogre::LogManager::getSingleton().logMessage(str.str());

        // Setup vertex flags (used to check a vertex is used in rendering and/or animable)
        // and vertex index map (used to mapping standard grid base index to vertex buffer index)
        mVertexFlags = new uint8[mUsedWidth * mUsedHeight];
        boost::scoped_array<ushort> vertexIndexMap(new ushort[mUsedWidth * mUsedHeight]);
        uint8* pVertexFlags = mVertexFlags;
        ushort* pVertexIndex = vertexIndexMap.get();
        size_t vertexIndex = 0;
        for (size_t z = 0; z < mUsedHeight; ++z)
        {
            for (size_t x = 0; x < mUsedWidth; ++x)
            {
                uint8 flags = 0;
                ushort vi = 0;
                if (visitedVertices[_LOC(x+mUsedBaseX,z+mUsedBaseZ)])
                {
                    vi = vertexIndex++;
                    flags |= VF_USED;
                    if (enabledVertices[_LOC(x+mUsedBaseX,z+mUsedBaseZ)])
                        flags |= VF_ANIMABLE;
                }
                *pVertexFlags++ = flags;
                *pVertexIndex++ = vi;
            }
        }
        assert(vertexIndex == mNumUsedVertices);

        // Allocate and initialise buffers
        initBuffers();

        // Check how to split the renderables, improve triangle through put
        size_t horizSplitCount = 1;
        size_t vertSplitCount = 1;
        if (triangles.size() > LIQUID_SPLIT_THRESHOLD)
        {
            horizSplitCount = (mUsedWidth + LIQUID_SPLIT_SIZE - 2) / LIQUID_SPLIT_SIZE;
            vertSplitCount = (mUsedHeight + LIQUID_SPLIT_SIZE - 2) / LIQUID_SPLIT_SIZE;
        }
        size_t numRenderables = horizSplitCount * vertSplitCount;

        // Remap triangle list index buffer
        MappedTriangleList mappedTriangles(triangles.size());
        MappedTriangleList::iterator itDst = mappedTriangles.begin();
        for (TriangleList::const_iterator it = triangles.begin(); it != triangles.end(); ++it, ++itDst)
        {
            size_t maxx = 0, maxz = 0;
#define _DO_INDEX(v)                                                    \
            size_t v = it->v;                                           \
            do {                                                        \
                size_t x = (v) % (width+1) - mUsedBaseX;                \
                size_t z = (v) / (width+1) - mUsedBaseZ;                \
                assert(x < mUsedWidth && z < mUsedHeight);              \
                if (maxx < x) maxx = x;                                 \
                if (maxz < z) maxz = z;                                 \
                size_t index = z * mUsedWidth + x;                      \
                assert(mVertexFlags[index] & VF_USED);                  \
                v = vertexIndexMap[index];                              \
            } while(0)

            _DO_INDEX(v0);
            _DO_INDEX(v1);
            _DO_INDEX(v2);

#undef _DO_INDEX

            // Find out the triangle split id
            size_t xid = maxx * horizSplitCount / mUsedWidth;
            size_t zid = maxz * vertSplitCount / mUsedHeight;
            size_t id = zid * horizSplitCount + xid;;
            assert(id < numRenderables);
            itDst->id = id;

            // Sort the vertex indices of the triangle
            if (v1 < v0)
            {
                if (v2 < v1)
                {
                    // v2 is minimise
                    itDst->v0 = v2;
                    itDst->v1 = v0;
                    itDst->v2 = v1;
                }
                else
                {
                    // v1 is minimise
                    itDst->v0 = v1;
                    itDst->v1 = v2;
                    itDst->v2 = v0;
                }
            }
            else
            {
                if (v2 < v0)
                {
                    // v2 is minimise
                    itDst->v0 = v2;
                    itDst->v1 = v0;
                    itDst->v2 = v1;
                }
                else
                {
                    // v0 is minimise
                    itDst->v0 = v0;
                    itDst->v1 = v1;
                    itDst->v2 = v2;
                }
            }
        }

        // Sort the triangles to improve cache hit and grouping by split id
        std::sort(mappedTriangles.begin(), mappedTriangles.end());

        // Create vertex data, vertex buffer, and index buffer
        createHardwareBuffers(numVisitedVertices, mappedTriangles.size());

        // Create renderables and fill up index buffer
        ushort* indices = static_cast<ushort*>(mIndexBuffer->lock(Ogre::HardwareBuffer::HBL_DISCARD));
        size_t lastId = ~0;
        Ogre::IndexData* lastIndexData = 0;
        size_t currentIndex = 0;
        for (MappedTriangleList::const_iterator it = mappedTriangles.begin(); it != mappedTriangles.end(); ++it)
        {
            size_t id = it->id;
            if (lastId != id)
            {
                lastId = id;

                // Create new liquid renderable
                LiquidRenderable* liquidRenderable = new LiquidRenderable(this);
                mLiquidRenderables.push_back(liquidRenderable);

                lastIndexData = new Ogre::IndexData;
                liquidRenderable->mRenderOp.indexData = lastIndexData;
                liquidRenderable->mRenderOp.useIndexes = true;
                liquidRenderable->mRenderOp.operationType = Ogre::RenderOperation::OT_TRIANGLE_LIST;
                liquidRenderable->mRenderOp.vertexData = mVertexData;
                liquidRenderable->mMaterial = mMaterial;

                lastIndexData->indexBuffer = mIndexBuffer;
                lastIndexData->indexStart = currentIndex;
                lastIndexData->indexCount = 0;

                // Setup renderable bounding box 
                size_t xid = id % horizSplitCount;
                size_t zid = id / horizSplitCount;
                liquidRenderable->mBoundingBox.setExtents(
                    data->mScale.x * ((xid+0) * mUsedWidth / horizSplitCount + mUsedBaseX) + data->mPosition.x,
                    mPosition.y,
                    data->mScale.z * ((zid+0) * mUsedHeight / vertSplitCount + mUsedBaseZ) + data->mPosition.z,
                    data->mScale.x * ((xid+1) * mUsedWidth / horizSplitCount + mUsedBaseX) + data->mPosition.x,
                    mPosition.y,
                    data->mScale.z * ((zid+1) * mUsedHeight / vertSplitCount + mUsedBaseZ) + data->mPosition.z);

                str.str(Ogre::StringUtil::BLANK);
                str << "New renderable - Bounding box: " << liquidRenderable->mBoundingBox;
                Ogre::LogManager::getSingleton().logMessage(str.str());
            }

            lastIndexData->indexCount += 3;
            currentIndex += 3;

            *indices++ = it->v0;
            *indices++ = it->v2;
            *indices++ = it->v1;
        }
        mIndexBuffer->unlock();

        mLastFrameTime = 0;
        mRemainTimeStamp = 0;
        mVertexBuffersDirty = true;
        mTexcoordsDirty = true;

        str.str(Ogre::StringUtil::BLANK);
        str << "Built liquid at position: " << mPosition
            << " - Renderables: " << mLiquidRenderables.size()
            << " - UsedVertices: " << mNumUsedVertices
            << " - Triangles: " << triangles.size()
            << " - Range: [" << minX << "," << minZ << "]-[" << maxX << "," << maxZ << "]";
        Ogre::LogManager::getSingleton().logMessage(str.str());

        // Notify update
        if (mParentNode)
        {
            mParentNode->needUpdate();
        }

        return true;
    }
    //-----------------------------------------------------------------------
    void TerrainLiquid::createHardwareBuffers(size_t vertexCount, size_t faceCount)
    {
        assert(!mVertexData);
        assert(mIndexBuffer.isNull());
        assert(vertexCount <= 65536);

        mVertexData = new Ogre::VertexData;
        Ogre::VertexData* vertexData = mVertexData; // cache to register for optimise

        vertexData->vertexStart = 0;
        vertexData->vertexCount = vertexCount;

	    Ogre::VertexDeclaration* vdecl = vertexData->vertexDeclaration; 
	    Ogre::VertexBufferBinding* vbind = vertexData->vertexBufferBinding;

	    vdecl->addElement(0, 0, Ogre::VET_FLOAT3, Ogre::VES_POSITION);
	    vdecl->addElement(1, 0, Ogre::VET_FLOAT3, Ogre::VES_NORMAL);

        Ogre::HardwareVertexBufferSharedPtr positionVertexBuffer =
            Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
                3*sizeof(float),
                vertexCount,
                Ogre::HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY);
        vbind->setBinding(0, positionVertexBuffer);

        Ogre::HardwareVertexBufferSharedPtr normalVertexBuffer =
            Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
                3*sizeof(float),
                vertexCount,
                Ogre::HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY);
        vbind->setBinding(1, normalVertexBuffer);

        // Texcoord buffer will create on demand

        mIndexBuffer =
            Ogre::HardwareBufferManager::getSingleton().createIndexBuffer(
                Ogre::HardwareIndexBuffer::IT_16BIT,
                faceCount * 3,
                Ogre::HardwareBuffer::HBU_STATIC, true);
    }
    //-----------------------------------------------------------------------
    void TerrainLiquid::calculatePositions(void)
    {
        assert(mVertexData);

        const TerrainData* data = mTerrain->getData();
        const Real xscale = data->mScale.x;
        const Real zscale = data->mScale.z;
        const Real xbais = data->mPosition.x + mUsedBaseX * xscale;
        const Real zbais = data->mPosition.z + mUsedBaseZ * zscale;
        const Real ybais = mPosition.y;
        const float* pLevel = mLevelBuffers[0];
        const uint8* pVertexFlags = mVertexFlags;
        float* pPosition = mPositionBuffer;
        size_t width = mUsedWidth;
        size_t height = mUsedHeight;
        for (size_t z = 0; z < height; ++z)
        {
            for (size_t x = 0; x < width; ++x)
            {
                uint8 flags = *pVertexFlags++;
                if (flags & VF_USED)
                {
                    *pPosition++ = x * xscale + xbais;
                    *pPosition++ = *pLevel + ybais;
                    *pPosition++ = z * zscale + zbais;
                }
                pLevel += 1;
            }
        }

        Ogre::HardwareVertexBufferSharedPtr positionVertexBuffer =
            mVertexData->vertexBufferBinding->getBuffer(0);
        positionVertexBuffer->writeData(0, mNumUsedVertices * 3 * sizeof(float), mPositionBuffer);
    }
    //-----------------------------------------------------------------------
    void TerrainLiquid::calculateNormals(void)
    {
        assert(mVertexData);
        assert(!mIndexBuffer.isNull());

        size_t numFaces = mIndexBuffer->getNumIndexes() / 3;
        size_t numVertices = mNumUsedVertices;
        Ogre::Vector3* pNormals = mNormalBuffer;
        size_t i;

        // zero normals
        for (i = 0; i < numVertices; ++i)
        {
            pNormals[i] = Ogre::Vector3::ZERO;
        }

        // first, calculate normals for faces, add them to proper vertices
        const ushort* indices = static_cast<const ushort*>(mIndexBuffer->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
        const float* positions = mPositionBuffer;
        for (i = 0; i < numFaces; ++i)
        {
            size_t p0 = indices[3*i+0] ;
            size_t p1 = indices[3*i+1] ;
            size_t p2 = indices[3*i+2] ;
            Ogre::Vector3 v0(positions[3*p0+0], positions[3*p0+1], positions[3*p0+2]);
            Ogre::Vector3 v1(positions[3*p1+0], positions[3*p1+1], positions[3*p1+2]);
            Ogre::Vector3 v2(positions[3*p2+0], positions[3*p2+1], positions[3*p2+2]);
            Ogre::Vector3 fn = Ogre::Math::calculateBasicFaceNormalWithoutNormalize(v0, v1, v2);
            pNormals[p0] += fn;
            pNormals[p1] += fn;
            pNormals[p2] += fn;
        }
        mIndexBuffer->unlock();

        // now normalize vertex normals
        Ogre::HardwareVertexBufferSharedPtr normalVertexBuffer =
            mVertexData->vertexBufferBinding->getBuffer(1);
        float* pFloat = static_cast<float*>(normalVertexBuffer->lock(Ogre::HardwareBuffer::HBL_DISCARD));
        for (i = 0; i < numVertices; ++i)
        {
            Ogre::Vector3 n = pNormals[i];
            n.normalise();
            *pFloat++ = n.x;
            *pFloat++ = n.y;
            *pFloat++ = n.z;
        }
        normalVertexBuffer->unlock();
    }
    //-----------------------------------------------------------------------
    void TerrainLiquid::manageTexcoords(void)
    {
        assert(mVertexData);

        Ogre::VertexData* vertexData = mVertexData; // cache to register for optimise
        size_t vertexCount = vertexData->vertexCount;
        Ogre::VertexDeclaration* vdecl = vertexData->vertexDeclaration; 
        Ogre::VertexBufferBinding* vbind = vertexData->vertexBufferBinding;

        bool requireHorizontalLayer = true;
        bool requireVerticalLayer = mDepthTexLayerEnabled;
        ushort bufferIndex = 2;
        size_t vertexSize = 0;

        // Create texture coordinate elements
        vdecl->removeElement(Ogre::VES_TEXTURE_COORDINATES, 0);
        vdecl->removeElement(Ogre::VES_TEXTURE_COORDINATES, 1);
        if (requireHorizontalLayer)
        {
            vdecl->addElement(bufferIndex, vertexSize, Ogre::VET_FLOAT2, Ogre::VES_TEXTURE_COORDINATES, 0);
            vertexSize += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT2);
        }
        if (requireVerticalLayer)
        {
            vdecl->addElement(bufferIndex, vertexSize, Ogre::VET_FLOAT1, Ogre::VES_TEXTURE_COORDINATES, 1);
            vertexSize += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT1);
        }

        // create texture coordinate vertex buffer
        Ogre::HardwareVertexBufferSharedPtr texcoordVertexBuffer;
        if (vbind->isBufferBound(bufferIndex))
        {
            // Re-use exists if possible
            texcoordVertexBuffer = vbind->getBuffer(bufferIndex);
            if (texcoordVertexBuffer->getVertexSize() != vertexSize ||
                texcoordVertexBuffer->getNumVertices() != vertexCount)
            {
                texcoordVertexBuffer.setNull();
            }
        }
        if (texcoordVertexBuffer.isNull())
        {
            texcoordVertexBuffer =
                Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
                    vertexSize, vertexCount,
                    Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);
            vbind->setBinding(bufferIndex, texcoordVertexBuffer);
        }

        const TerrainData* data = mTerrain->getData();
        float* pFloat = static_cast<float*>(texcoordVertexBuffer->lock(Ogre::HardwareBuffer::HBL_DISCARD));
        const uint8* pVertexFlags = mVertexFlags;
        int width = mUsedWidth;
        int height = mUsedHeight;

        Real cosValue = Ogre::Math::Cos(mTexcoordRotate);
        Real sinValue = Ogre::Math::Sin(mTexcoordRotate);
        Real basicHeight = mPosition.y + mDepthTexLayerAdjust;

        for (int z = 0; z < height; ++z)
        {
            for (int x = 0; x < width; ++x)
            {
                uint8 flags = *pVertexFlags++;
                if (flags & VF_USED)
                {
                    if (requireHorizontalLayer)
                    {
                        *pFloat++ = mTexcoordScale * (x*cosValue - z*sinValue);
                        *pFloat++ = mTexcoordScale * (x*sinValue + z*cosValue);
                    }
                    if (requireVerticalLayer)
                    {
                        Real height = data->getHeightAtGrid(x + mUsedBaseX, z + mUsedBaseZ);
                        *pFloat++ = (basicHeight - height) * mDepthTexLayerScale;
                    }
                }
            }
        }
        texcoordVertexBuffer->unlock();
    }
    //-----------------------------------------------------------------------
    void TerrainLiquid::initBuffers(void)
    {
        assert(!mAllocatedBuffer);
        assert(!mLevelBuffers[0]);
        assert(!mLevelBuffers[1]);
        assert(!mLevelBuffers[2]);
        assert(!mPositionBuffer);
        assert(!mNormalBuffer);
        size_t size = mUsedWidth * mUsedHeight;
        mAllocatedBuffer = new float[3 * size + 3 * mNumUsedVertices];
        memset(mAllocatedBuffer, 0, (3 * size + 3 * mNumUsedVertices) * sizeof(*mAllocatedBuffer));
        mLevelBuffers[0] = mAllocatedBuffer + 0 * size;
        mLevelBuffers[1] = mAllocatedBuffer + 1 * size;
        mLevelBuffers[2] = mAllocatedBuffer + 2 * size;
        mPositionBuffer = mAllocatedBuffer + 3 * size;
        mNormalBuffer = new Ogre::Vector3[mNumUsedVertices];
    }
    //-----------------------------------------------------------------------
    void TerrainLiquid::updateLevelBuffers(void)
    {
        if (mRemainTimeStamp < mAnimationIteratorInterval)
            return;

        mVertexBuffersDirty = true;

        /* we use an algorithm from
         * http://collective.valve-erc.com/index.php?go=water_simulation
         * The params could be dynamically changed every frame of course
         */
        const Real C = PARAM_C; // ripple speed 
        const Real D = PARAM_D; // distance
        const Real U = PARAM_U; // viscosity
        const Real T = PARAM_T; // time
        const Real TERM1 = ( 4 - 8*C*C*T*T/(D*D) ) / (U*T + 2);
        const Real TERM2 = ( U*T - 2 ) / (U*T + 2);
        const Real TERM3 = ( 2 * C*C*T*T/(D*D) ) / (U*T + 2);
        size_t width = mUsedWidth;
        size_t height = mUsedHeight;
        assert(width >= 2 && height >= 2);

        // do rendering to get mAnimationIteratorInterval
        do
        {
            // switch buffers
            float *buf = mLevelBuffers[2];
            float *buf1 = mLevelBuffers[0];
            float *buf2 = mLevelBuffers[1];
            mLevelBuffers[0] = buf;
            mLevelBuffers[1] = buf1;
            mLevelBuffers[2] = buf2;

            const uint8* vertexFlags = mVertexFlags;
            for (size_t z = 1; z < height-1; ++z)    // don't do anything with border values
            {
                float *row = buf + z*width;
                const float *row1 = buf1 + z*width;
                const float *row1up = buf1 + (z-1)*width;
                const float *row1down = buf1 + (z+1)*width;
                const float *row2 = buf2 + z*width;
                const uint8* flags = vertexFlags + z*width;
                for (size_t x = 1; x < width-1; ++x)
                {
                    if (flags[x] & VF_ANIMABLE)     // don't do anything with doesn't animable values
                    {
                        row[x] = TERM1 * row1[x] +
                                 TERM2 * row2[x] +
                                 TERM3 * ( row1[x-1] + row1[x+1] + row1up[x]+row1down[x] );
                    }
                }
            }

            mRemainTimeStamp -= mAnimationIteratorInterval;
        } while (mRemainTimeStamp >= mAnimationIteratorInterval);
    }
    //-----------------------------------------------------------------------
    void TerrainLiquid::updateVertexBuffers(void)
    {
        updateLevelBuffers();

        if (mTexcoordsDirty)
        {
            mTexcoordsDirty = false;
            manageTexcoords();
        }

        if (mVertexBuffersDirty)
        {
            mVertexBuffersDirty = false;
            calculatePositions();
            calculateNormals();
        }
    }
    //-----------------------------------------------------------------------
    void TerrainLiquid::updateRenderables(void)
    {
        for (LiquidRenderableList::const_iterator it = mLiquidRenderables.begin(); it != mLiquidRenderables.end(); ++it)
        {
            LiquidRenderable* liquidRenderable = *it;
            liquidRenderable->mMaterial = mMaterial;
        }
    }
    //-----------------------------------------------------------------------
    void TerrainLiquid::push(Real x, Real z, Real depth, bool absolute)
    {
        updateLevelBuffers();

        // TODO:
    }
    //-----------------------------------------------------------------------
    Real TerrainLiquid::getHeight(Real x, Real z)
    {
        updateLevelBuffers();

        // TODO:
        return mPosition.y;
    }
    //-----------------------------------------------------------------------
    void TerrainLiquid::update(Real timeSinceLastFrame)
    {
        mLastFrameTime = timeSinceLastFrame;
        mRemainTimeStamp += timeSinceLastFrame;
        // NB: We will update on demand
    }
}
