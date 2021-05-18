#ifndef AXIS_RENDERABLE_H
#define AXIS_RENDERABLE_H

#include "WXSceneListener.h"

#include <OgrePrerequisites.h>
#include <OgreSimpleRenderable.h>
#include <OgreHardwareVertexBuffer.h>

namespace Ogre
{
    class TextAreaOverlayElement;
}

namespace WX
{
    class TerrainData;
    class SceneManipulator;
}
namespace WX
{
	class TerrainGridRenderable : public Ogre::SimpleRenderable
	{
	protected:

		Ogre::Real* m_locked_data;
		Ogre::HardwareVertexBufferSharedPtr m_locked_buffer;
		SceneManipulator* mSceneManipulator;

		int mStride;
		int mVertexCount;
		TerrainData* terrData;
		Ogre::ColourValue mColorValue;
		Ogre::Real m_radius;

	public:
		TerrainGridRenderable(SceneManipulator* manip);
		~TerrainGridRenderable();
		Ogre::Real getSquaredViewDepth(const Ogre::Camera* cam) const;
		Ogre::Real getBoundingRadius(void) const { return m_radius; }

		// Delete all lines in the TerrainGridRenderable. Since lines can't be modified after added, use this function to
		// clear the TerrainGridRenderable so you can re-add the lines with new positions/colours.
		void clear();

		// Lock the TerrainGridRenderable, ready for drawLine() calls.
		void lock();

		// Unlock the TerrainGridRenderable.
		void unlock();

		// Draw a line between v1 and v2, with the colours c1 and c2 at the corresponding ends.
		// Lines are preserved (except after a clear()), if they don't change then you only need to add them once.
		// Lines can only be drawn while the TerrainGridRenderable is locked.
		void drawLine(const Ogre::Vector3 &v1, const Ogre::Vector3 &v2, const Ogre::ColourValue &c1 = Ogre::ColourValue::White, const Ogre::ColourValue &c2 = Ogre::ColourValue::White,unsigned int lineCount =0);

		void init(int xStart,int zStart, int xEnd, int zEnd);
		void freeBuffer();
		void onTerrainHeightChanged(const std::vector<TerrainInfo>& terrainInfo,int minx,int minz,int maxx,int maxz);

		int mXStart;
		int mZStart;
        int mXEnd;
        int mZEnd;

	protected:
		void _updateRenderQueue(Ogre::RenderQueue* queue);
		void initTerrainGrid(int vertexCount);
		void drawGridLines();
	};

    class TerrainGrid  : public SceneListener
    {
    private:
		std::list<TerrainGridRenderable*> mTerrGridRend;

        Ogre::SceneManager* mSceneMgr;
        SceneManipulator* mSceneManipulator;
        Ogre::SceneNode* mBaseSceneNode;
		bool mActive;
		TerrainData* mTerrData;

		void initTerrainRend();
    public:
        TerrainGrid(Ogre::SceneManager* sceneMgr,SceneManipulator* manip);
        ~TerrainGrid();

    public:
        void createRenderable();
        void destroyRenderable();

        void onTerrainHeightChanged(const std::vector<TerrainInfo>& terrainInfo);
        void onSceneReset(void);

        /*TerrainGridRenderable* getTerrainGridRenderable()
        {
            return mTerrGridRend;
        }*/
		
		bool isActive()
		{
			return mActive;
		}
		void setActive(bool active)
		{

			unsigned int size = mTerrGridRend.size();
			if(size>0)
			{
				std::list<TerrainGridRenderable*>::const_iterator iter;
				for(iter = mTerrGridRend.begin(); iter != mTerrGridRend.end(); iter++)
				{
					(*iter)->setVisible(!((*iter)->isVisible()));
				}
			}
			else if(size == 0)
			{
				createRenderable();
			}

		}
    };
}

#endif

