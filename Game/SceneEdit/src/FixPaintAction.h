#ifndef __FixPaintAction_H__
#define __FixPaintAction_H__

#include "PaintAction.h"
#include "Selection.h"

#include "Core/TerrainData.h"

namespace WX {

	class JunctionSelection;
    class GridSelection;
	class TerrainData;

	typedef Ogre::Vector2 Point;
	enum
	{
		NumLayers = 2,
	};

	class FixPaintSelection
	{
	public:
		Point pt;

		struct PointLess
		{
			bool operator()(const Point& v1, const Point& v2) const
			{
				if (v1.y < v2.y) return true;
				if (v1.y == v2.y && v1.x < v2.x) return true;

				return false;
			}
		};
		
		struct TextureLayerInfo
		{
			Ogre::String textureName;
			Real left,top,right,bottom;
			ushort orientation;  

			bool operator== (const TextureLayerInfo& rhs) const
			{
				return textureName == rhs.textureName &&
					orientation == rhs.orientation;
			}
			bool operator!= (const TextureLayerInfo& rhs) const
			{
				return !(*this == rhs);
			}

		};
		struct TextureGridInfo
		{
			TextureLayerInfo layers[NumLayers];
			uint flags;
		};

		struct Junction
		{
			Real height;
			Real weight;

			Junction()
			{
			}
		};
		struct Grid 
		{	
			TextureGridInfo info;
			unsigned int flags;

			Grid()
			{
			}
		};
		
		typedef std::map<Point,Junction,PointLess>JunctionMap;
		typedef std::map<Point, Grid,PointLess> GridMap;

	protected:
		JunctionMap mJunctions;
		GridMap mGrids;

	public:
		FixPaintSelection();
		~FixPaintSelection();

		const String& getType(void) const;
		bool empty(void) const;
		void reset(void);
		void apply(void);

	public:
		const JunctionMap& getJunctions(void) const
		{
			return mJunctions;
		}
		const GridMap& getGrids(void) const
		{
			return mGrids;
		}
		
		FixPaintSelection::JunctionMap::iterator addJunction(const Point& pt);
		FixPaintSelection::GridMap::iterator addGrid(const Point& pt);
		bool existJunction(const Point& pt) const;
		bool existGrid(const Point& pt) const;

		bool setJunctionValue(const Point& pt,Real height,Real weight = 1.0);

		bool setGridValue(const Point& pt,const TextureGridInfo& info);
		bool setFlagValue(const Point& pt,unsigned int flags);
	};

    class FixPaintAction : public PaintAction
    {
    public:
        FixPaintAction(SceneManipulator* sceneManipulator);
        ~FixPaintAction();

        const String& getName(void) const;

		void setParameter(const String& name, const String& value);
		String getParameter(const String& name) const;

    protected:
		JunctionSelection* mHintModifiedJunctions;
		JunctionSelection* mModifiedJunctions;
        GridSelection* mHintModifiedGrids;
        GridSelection* mModifiedGrids;

        void _buildHitIndicator(const Point& pt);
        void _doPaint(JunctionSelection* modifiedJunctions,GridSelection* modified,const Point& mCurrentMousePos);

        void _onActive(bool active);

        void _onMove(const Point& pt);
        void _onBegin(const Point& pt);
        void _onDrag(const Point& pt);
        void _onEnd(const Point& pt, bool canceled);
		void doFinish(JunctionSelection* originJunctions,GridSelection* originGrids, bool canceled);

		void parseInfoString();
    protected:
		bool mPasteHeightInfo;
		bool mPasteDiagonalInfo;
		bool mPasteTextureInfo;

		Ogre::String mInfoString;
		FixPaintSelection mGridSelection;
    };

}

#endif // __FixPaintAction_H__
