#ifndef __FAIRYSCENEINFO_H__
#define __FAIRYSCENEINFO_H__

#include "WXScene.h"

namespace WX
{
    class XMLParser;
    class XMLWriter;

    class System;
    class Terrain;
    class TerrainData;
    class IncrementalSceneBuilder;

    class SceneInfo : public Scene
    {
    protected:
        Terrain* mTerrain;
        TerrainData* mTerrainData;
        IncrementalSceneBuilder* mIncrementalSceneBuilder;

    public:
        SceneInfo(void);
        ~SceneInfo();

        Terrain* getTerrain(void) const
        {
            return mTerrain;
        }

        TerrainData* getTerrainData(void) const
        {
            return mTerrainData;
        }

        void load(XMLParser* parser, const String& filename, const String& groupName, bool completely);
        void save(XMLWriter* writer, const String& filename);

		/** Destroys all the created render instance state (reverse of initialise). 
		@remarks
			You can call initialise() again after this.
		*/
        void destroy(void);

		/** Clears any of the objects added to this scene and 
			destroys anything which has already been initialised.
		*/
        void reset(void);

		/** Initialise the scene. 
        */
        void initialise(System* system);
    };
}

#endif 
