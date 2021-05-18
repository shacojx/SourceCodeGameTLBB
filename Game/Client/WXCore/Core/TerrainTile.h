#ifndef __TERRAINTILE_H__
#define __TERRAINTILE_H__

#include "WXPrerequisites.h"

#include <OgreMovableObject.h>

namespace WX {

class Terrain;

//////////////////////////////////////////////////////////////////////////

class TerrainTile : public Ogre::MovableObject
{
protected:
    Terrain *mOwner;
	int mXBase;
	int mZBase;
	int mXSize;
	int mZSize;
    Ogre::SceneNode* mNode;
    Ogre::AxisAlignedBox mBounds;

    typedef std::vector<size_t> GridIdList;

    struct MaterialBucket
    {
    	size_t layerIndex;
        const Ogre::MaterialPtr& material;
	    GridIdList grids;

        MaterialBucket(size_t layerIndex, const Ogre::MaterialPtr& material)
            : layerIndex(layerIndex)
            , material(material)
        {
        }
    };

    typedef std::map<Ogre::ResourceHandle, MaterialBucket> MaterialBucketMap;

    /// compute which material is used, and the grids that used that material
    void buildMaterialBucketMap(MaterialBucketMap& materialBucketMap);

    typedef std::list<Ogre::Renderable*> RenderableList;

    void queueRenderables(Ogre::RenderQueue* queue, const RenderableList& renderables);

    void destroyRenderables(RenderableList& renderables);

public:
    TerrainTile(Ogre::SceneNode* parent, Terrain *creator,
			 int xbase, int zbase, int xsize, int zsize);
    ~TerrainTile();

    virtual void _notifyHeightModified(int xstart, int zstart, int xend, int zend);
    virtual void _notifyGridInfoModified(int xstart, int zstart, int xend, int zend);

public:
    /** Returns the bounding box of this TerrainTile */
    const Ogre::AxisAlignedBox& getBoundingBox(void) const;

    /** Overridden from MovableObject */
	Real getBoundingRadius(void) const;

    const Ogre::LightList& getLights(void) const;
};

}

#endif // 
