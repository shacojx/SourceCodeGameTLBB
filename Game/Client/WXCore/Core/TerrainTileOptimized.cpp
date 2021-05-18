#include "TerrainTileOptimized.h"
#include "Terrain.h"
#include "TerrainTileRenderable.h"

#include <OgreSceneManager.h>
#include <OgreSceneNode.h>

namespace WX {

class TerrainTileOptimizedRenderable : public TerrainTileRenderable
{
public:
    TerrainTileOptimizedRenderable(TerrainTile *parent)
        : TerrainTileRenderable(parent)
    {
    }

    ~TerrainTileOptimizedRenderable()
    {
        // Only vertex data need to delete
        delete mRenderOp.vertexData;
    }
};

//-----------------------------------------------------------------------

TerrainTileOptimized::TerrainTileOptimized(Ogre::SceneNode* parent, Terrain *owner,
									 int xbase, int zbase, int xsize, int zsize)
    : TerrainTile(parent, owner, xbase, zbase, xsize, zsize)
    , mRenderables()
    , mGeometryOutOfDate(true)
{
}

TerrainTileOptimized::~TerrainTileOptimized()
{
	destoryGeometry();
}

//-----------------------------------------------------------------------

const String&
TerrainTileOptimized::getMovableType(void) const
{
	static const String type = "TerrainTileOptimized";
    return type;
}

void
TerrainTileOptimized::_updateRenderQueue(Ogre::RenderQueue* queue)
{
    if (mGeometryOutOfDate)
    {
        createGeometry(mOwner->getData(), mXBase, mZBase, mXSize, mZSize);
    }

    queueRenderables(queue, mRenderables);
}

//-----------------------------------------------------------------------

void
TerrainTileOptimized::destoryGeometry(void)
{
    destroyRenderables(mRenderables);

    mGeometryOutOfDate = true;
}

void
TerrainTileOptimized::createGeometry(TerrainData* data, int xbase, int zbase, int xsize, int zsize)
{
    destoryGeometry();

    // build the material backet map
    MaterialBucketMap materialBucketMap;
    buildMaterialBucketMap(materialBucketMap);

    // statistic number grids for each layer
    size_t numGridsOfLayer[2] = { 0 };
    for (MaterialBucketMap::const_iterator im = materialBucketMap.begin(); im != materialBucketMap.end(); ++im)
    {
        numGridsOfLayer[im->second.layerIndex] += im->second.grids.size();
    }

    bool includeLightmap = mOwner->_isLightmapUsed();

    // create vertex buffer and lock it
    Ogre::VertexData vertexDatas[2];
    Ogre::HardwareVertexBufferSharedPtr buffers[2];
    float* pBuffers[2] = { NULL };
    for (size_t layerIndex = 0; layerIndex < 2; ++layerIndex)
	{
        if (!numGridsOfLayer[layerIndex])
            continue;

		enum
		{
			MAIN_BINDING,
		};

        Ogre::VertexDeclaration* decl = vertexDatas[layerIndex].vertexDeclaration;
        Ogre::VertexBufferBinding* bind = vertexDatas[layerIndex].vertexBufferBinding;
        vertexDatas[layerIndex].vertexStart = 0;
        vertexDatas[layerIndex].vertexCount = numGridsOfLayer[layerIndex] * 4;

        size_t offset = 0;
        size_t texCoordSet = 0;
        // positions
        decl->addElement(MAIN_BINDING, offset, Ogre::VET_FLOAT3, Ogre::VES_POSITION);
        offset += 3 * sizeof(float);
        // normals
        decl->addElement(MAIN_BINDING, offset, Ogre::VET_FLOAT3, Ogre::VES_NORMAL);
        offset += 3 * sizeof(float);
        // texture layer 0
        decl->addElement(MAIN_BINDING, offset, Ogre::VET_FLOAT2, Ogre::VES_TEXTURE_COORDINATES, texCoordSet++);
        offset += 2 * sizeof(float);
        // texture layer 1
        if (layerIndex == 1)
        {
            decl->addElement(MAIN_BINDING, offset, Ogre::VET_FLOAT2, Ogre::VES_TEXTURE_COORDINATES, texCoordSet++);
            offset += 2 * sizeof(float);
        }
        // light-map layer
		if (includeLightmap)
        {
            decl->addElement(MAIN_BINDING, offset, Ogre::VET_FLOAT2, Ogre::VES_TEXTURE_COORDINATES, texCoordSet++);
            offset += 2 * sizeof(float);
        }

        buffers[layerIndex] =
            Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
                decl->getVertexSize(MAIN_BINDING),
                vertexDatas[layerIndex].vertexCount, 
                Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);
        bind->setBinding(MAIN_BINDING, buffers[layerIndex]);

        pBuffers[layerIndex] = static_cast<float*>(buffers[layerIndex]->lock(Ogre::HardwareBuffer::HBL_DISCARD));
    }

    Real xscale = 1.0 / xsize;
    Real zscale = 1.0 / zsize;

    // build renderables, group by material
    size_t vertexStarts[2] = { 0 };
    for (MaterialBucketMap::const_iterator im = materialBucketMap.begin(); im != materialBucketMap.end(); ++im)
	{
        TerrainTileOptimizedRenderable* renderable = new TerrainTileOptimizedRenderable(this);
        mRenderables.push_back(renderable);

		const MaterialBucket* mb = &im->second;
        size_t layerIndex = mb->layerIndex;
        size_t numQuads = mb->grids.size();
        size_t vertexCount = numQuads * 4;

		renderable->mMaterial = mb->material;

        // Clone vertex data but shared vertex buffers
		Ogre::VertexData* vertexData = vertexDatas[layerIndex].clone(false);
        vertexData->vertexStart = vertexStarts[layerIndex];
        vertexData->vertexCount = vertexCount;

        renderable->mRenderOp.vertexData = vertexData;
        renderable->mRenderOp.operationType = Ogre::RenderOperation::OT_TRIANGLE_LIST;
        renderable->mRenderOp.useIndexes = true;
        renderable->mRenderOp.indexData = mOwner->_getIndexData(numQuads);

        float* pFloat = pBuffers[layerIndex];
        for (GridIdList::const_iterator igrid = mb->grids.begin(); igrid != mb->grids.end(); ++igrid)
		{
            size_t grid = *igrid;
            const TerrainData::GridInfo& gridInfo = data->mGridInfos[grid];
            const TerrainData::Corner* corners = gridInfo.getCorners();
            int x = grid % data->mXSize;
            int z = grid / data->mXSize;

            // NB: Store the quad vertices in clockwise order, index data will
            // take care with this.
            for (size_t i = 0; i < 4; ++i)
			{
                Ogre::Vector3 v;
                std::pair<Real, Real> t;
                TerrainData::Corner corner = corners[i];
                // position
                v = data->_getPosition((x+(corner&1)), (z+(corner>>1)));
                *pFloat++ = v.x; *pFloat++ = v.y; *pFloat++ = v.z;
                // normal
                v = data->_getNormal((x+(corner&1)), (z+(corner>>1)));
                *pFloat++ = v.x; *pFloat++ = v.y; *pFloat++ = v.z;
                // layer 0
                t = mOwner->_getPixmapCorner(gridInfo.layers[0], corner, gridInfo.flags);
                *pFloat++ = t.first; *pFloat++ = t.second;
                // layer 1
                if (gridInfo.layers[1].pixmapId)
                {
                    t = mOwner->_getPixmapCorner(gridInfo.layers[1], corner, gridInfo.flags);
                    *pFloat++ = t.first; *pFloat++ = t.second;
                }
                // light-map
                if (includeLightmap)
                {
                    *pFloat++ = xscale * (x - xbase + (corner&1));
                    *pFloat++ = zscale * (z - zbase + (corner>>1));
                }
            }
        }
        pBuffers[layerIndex] = pFloat;
        vertexStarts[layerIndex] += vertexCount;
    }

    // unlock vertex buffer
    for (size_t layerIndex = 0; layerIndex < 2; ++layerIndex)
	{
        if (!buffers[layerIndex].isNull())
            buffers[layerIndex]->unlock();
    }

    mGeometryOutOfDate = false;
}

}
