#include "TerrainTile.h"
#include "Terrain.h"

#include <OgreSceneManager.h>
#include <OgreSceneNode.h>
#include <OgreRoot.h>

namespace WX {

TerrainTile::TerrainTile(Ogre::SceneNode* parent, Terrain *creator,
				   int xbase, int zbase, int xsize, int zsize)
    : MovableObject()
    , mOwner(creator)
	, mXBase(xbase)
	, mZBase(zbase)
	, mXSize(xsize)
	, mZSize(zsize)
    , mNode(NULL)
    , mBounds()
{
    mCastShadows = false;
    mQueryFlags = 0;

	// create base node
    mNode = parent->createChildSceneNode();
    mNode->attachObject(this);

    // Setup the movable object name
    mName = mNode->getName();

	// compue bounding box
	mOwner->getData()->_computeTileBoundingBox(mBounds, xbase, zbase, xsize, zsize);
}

TerrainTile::~TerrainTile()
{
    if (mNode)
    {
		mNode->destroy();
		mNode = NULL;
    }
}

//-----------------------------------------------------------------------

const Ogre::AxisAlignedBox&
TerrainTile::getBoundingBox(void) const
{
    return mBounds;
}

Real
TerrainTile::getBoundingRadius(void) const
{
    return 0;   /* not needed */
}

const Ogre::LightList& TerrainTile::getLights(void) const
{
    // Make sure we only update this once per frame no matter how many
    // times we're asked
    ulong frame = mNode->getCreator()->_getLightsDirtyCounter();
    if (mLightListUpdated != frame)
    {
        mLightListUpdated = frame;

        const Ogre::AxisAlignedBox& aabb = getWorldBoundingBox();
        Ogre::Vector3 position = aabb.getCenter();
        Ogre::Real radius = (aabb.getMaximum() - aabb.getMinimum()).length() / 2;
        mNode->getCreator()->_populateLightList(position, radius, mLightList);
    }

    return mLightList;
}

//-----------------------------------------------------------------------

void
TerrainTile::_notifyHeightModified(int xstart, int zbase, int xend, int zend)
{
	assert(0 <= xstart && xstart < xend && xend <= mXSize+1);
	assert(0 <= zbase && zbase < zend && zend <= mZSize+1);

	OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR,
		"The terrain is not editable",
		"TerrainTile::_notifyHeightModified");
}

void
TerrainTile::_notifyGridInfoModified(int xstart, int zstart, int xend, int zend)
{
	assert(0 <= xstart && xstart < xend && xend <= mXSize);
	assert(0 <= zstart && zstart < zend && zend <= mZSize);

	OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR,
		"The terrain is not editable",
		"TerrainTile::_notifyGridInfoModified");
}

//-----------------------------------------------------------------------

void
TerrainTile::buildMaterialBucketMap(MaterialBucketMap& materialBucketMap)
{
    // Cache member variable into register
    TerrainData* data = mOwner->getData();
    int patch = data->mXSize;
    int xbase = mXBase;
    int zbase = mZBase;
    int xsize = mXSize;
    int zsize = mZSize;

    for (int z = 0; z < zsize; ++z)
    {
        for (int x = 0; x < xsize; ++x)
        {
            size_t grid = (z+zbase) * patch + (x+xbase);
            const TerrainData::GridInfo& gridInfo = data->mGridInfos[grid];

            // If doesn't exists base layer, nothing to do
            if (!gridInfo.layers[0].pixmapId)
            {
                assert(!gridInfo.layers[1].pixmapId);
                continue;
            }

            size_t layerIndex = gridInfo.layers[1].pixmapId ? 1 : 0;

			const Ogre::MaterialPtr& material = mOwner->_getGridMaterial(gridInfo, 0, xbase, zbase);

            MaterialBucketMap::iterator it = materialBucketMap.find(material->getHandle());
            if (it == materialBucketMap.end())
            {
				std::pair<MaterialBucketMap::iterator, bool> inserted =
					materialBucketMap.insert(
					MaterialBucketMap::value_type(material->getHandle(),
												  MaterialBucket(layerIndex, material)));
				assert(inserted.second);
				it = inserted.first;

				// Pre-reserve memory for less thrashing
				it->second.grids.reserve(zsize * xsize / data->mTextures.size());
            }

            it->second.grids.push_back(grid);
        }
    }
}

void
TerrainTile::queueRenderables(Ogre::RenderQueue* queue, const RenderableList& renderables)
{
    mOwner->_updateMaterials();

    RenderableList::const_iterator itend = renderables.end();
    RenderableList::const_iterator it = renderables.begin();
    if (mRenderQueueIDSet)
    {
        for (; it != itend; ++it)
        {
            Ogre::Renderable* renderable = *it;
            queue->addRenderable(renderable, mRenderQueueID);
        }
    }
    else
    {
        for (; it != itend; ++it)
        {
            Ogre::Renderable* renderable = *it;
            queue->addRenderable(renderable);
        }
    }
}

void
TerrainTile::destroyRenderables(RenderableList& renderables)
{
    RenderableList::const_iterator itend = renderables.end();
    RenderableList::const_iterator it = renderables.begin();
    for (; it != itend; ++it)
    {
        Ogre::Renderable* renderable = *it;
        delete renderable;
    }
    renderables.clear();
}

}
