#ifndef AXIS_RENDERABLE_H
#define AXIS_RENDERABLE_H

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
}
namespace WX
{
	class TerrainGridRenderable : public Ogre::SimpleRenderable
	{
	protected:

		Ogre::Real* m_locked_data;
		Ogre::HardwareVertexBufferSharedPtr m_locked_buffer;

		int mStride;
		int mVertexCount;
		TerrainData* terrData;
		Ogre::ColourValue mColorValue;
		Ogre::Real m_radius;

	public:
		TerrainGridRenderable();
		~TerrainGridRenderable();
		Ogre::Real getSquaredViewDepth(const Ogre::Camera* cam) const;
		Ogre::Real getBoundingRadius(VOID) const { return m_radius; }

		// Delete all lines in the TerrainGridRenderable. Since lines can't be modified after added, use this function to
		// clear the TerrainGridRenderable so you can re-add the lines with new positions/colours.
		VOID clear();

		// Lock the TerrainGridRenderable, ready for drawLine() calls.
		VOID lock();

		// Unlock the TerrainGridRenderable.
		VOID unlock();

		// Draw a line between v1 and v2, with the colours c1 and c2 at the corresponding ends.
		// Lines are preserved (except after a clear()), if they don't change then you only need to add them once.
		// Lines can only be drawn while the TerrainGridRenderable is locked.
		VOID drawLine(const Ogre::Vector3 &v1, const Ogre::Vector3 &v2, const Ogre::ColourValue &c1 = Ogre::ColourValue::White, const Ogre::ColourValue &c2 = Ogre::ColourValue::White,unsigned int lineCount =0);

		VOID init(int x,int z);
		VOID freeBuffer();

		int mXStart;
		int mZStart;

	protected:
		VOID _updateRenderQueue(Ogre::RenderQueue* queue);
		VOID initTerrainGrid(int vertexCount);
		VOID drawGridLines();
	};

    class TerrainGrid  
    {
    private:
		std::list<TerrainGridRenderable*> mTerrGridRend;

        Ogre::SceneManager* mSceneMgr;
        Ogre::SceneNode* mBaseSceneNode;
		BOOL mActive;
		TerrainData* mTerrData;
		static int sZoneSize;

		VOID initTerrainRend();
    public:
        TerrainGrid(Ogre::SceneManager* sceneMgr, int zonesize=16);
        ~TerrainGrid();

    public:
        VOID createRenderable();
        VOID destroyRenderable();
		static int getZoneSize(VOID) { return sZoneSize; }
		
		BOOL isActive()
		{
			return mActive;
		}
		VOID setActive(BOOL active)
		{

			unsigned int size = (int)mTerrGridRend.size();
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

