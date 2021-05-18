#ifndef __TerrainTileRenderable_H__
#define __TerrainTileRenderable_H__

#include "BasicRenderable.h"

namespace WX {

class TerrainTile;

//////////////////////////////////////////////////////////////////////////

class TerrainTileRenderable : public BasicRenderable
{
public:
    TerrainTileRenderable(TerrainTile *parent);
    ~TerrainTileRenderable();

    /** Overridden - see Renderable. */
    const Ogre::LightList& getLights(void) const;
};

}

#endif // 
