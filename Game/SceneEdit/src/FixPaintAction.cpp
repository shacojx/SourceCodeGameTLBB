#include "FixPaintAction.h"
#include "SceneManipulator.h"
#include "HitIndicator.h"
#include "TerrainSelections.h"
#include "BrushShape.h"
#include "TerrainSelectionActionOperator.h"
#include "WXModifyTerrainHeightOperator.h"
#include "WXModifyTerrainPaintOperator.h"
#include "WXSceneListener.h"
#include "WXOperatorManager.h"

#include <OgreDataStream.h>
#include <OgreStringConverter.h>
#include <OgreStringVector.h>

namespace WX {

	FixPaintSelection::FixPaintSelection()
	{
	}

	FixPaintSelection::~FixPaintSelection()
	{
	}

	const String&
	FixPaintSelection::getType(void) const
	{
		static const String type = "FixPaintSelection";
		return type;
	}

	bool
	FixPaintSelection::empty(void) const
	{
		return mGrids.empty() && mJunctions.empty();
	}

	void
	FixPaintSelection::reset(void)
	{
		mJunctions.clear();
		mGrids.clear();
	}

	FixPaintSelection::JunctionMap::iterator
	FixPaintSelection::addJunction(const Point& pt)
	{
		std::pair<JunctionMap::iterator, bool> inserted =
			mJunctions.insert(JunctionMap::value_type(pt, Junction()));
		return inserted.first;
	}

	FixPaintSelection::GridMap::iterator
	FixPaintSelection::addGrid(const Point& pt)
	{
		std::pair<GridMap::iterator, bool> inserted =
			mGrids.insert(GridMap::value_type(pt, Grid()));
		return inserted.first;
	}

	bool
	FixPaintSelection::existJunction(const Point& pt) const
	{
		return mJunctions.find(pt) != mJunctions.end();
	}

	bool
	FixPaintSelection::existGrid(const Point& pt) const
	{
		return mGrids.find(pt) != mGrids.end();
	}

	bool
	FixPaintSelection::setJunctionValue(const Point& pt, Real height,Real weight)
	{
		JunctionMap::iterator it;

		if(!existJunction(pt))
			it = addJunction(pt); 
		else
			it = mJunctions.find(pt);

		it->second.height = height;
		it->second.weight = weight;
		return true;
	}

	bool
	FixPaintSelection::setGridValue(const Point& pt, const TextureGridInfo& info)
	{
		GridMap::iterator it;

		if(!existGrid(pt))
			it = addGrid(pt);
		else
			it = mGrids.find(pt);

		it->second.info = info;
		return true;
	}

	bool
	FixPaintSelection::setFlagValue(const Point& pt,unsigned int flags)
	{
		GridMap::iterator it;

		if(!existGrid(pt))
			it = addGrid(pt);
		else
			it = mGrids.find(pt);

		it->second.flags = flags;
		return true;
	}

	FixPaintAction::FixPaintAction(SceneManipulator* sceneManipulator)
        : PaintAction(sceneManipulator)
		, mPasteHeightInfo(false)
		, mPasteDiagonalInfo(false)
		, mPasteTextureInfo(false)
    {
		mHintModifiedJunctions = new JunctionSelection(getTerrain(),sceneManipulator);
		mModifiedJunctions = new JunctionSelection(getTerrain(),sceneManipulator);
        mHintModifiedGrids = new GridSelection(getTerrain(),sceneManipulator);
        mModifiedGrids = new GridSelection(getTerrain(),sceneManipulator);
		
		mGridSelection.reset();
	}

	FixPaintAction::~FixPaintAction()
    {
		delete mHintModifiedJunctions;
		delete mModifiedJunctions;

        delete mHintModifiedGrids;
        delete mModifiedGrids;
    }

	const String&
	FixPaintAction::getName(void) const
    {
        static const String name = "FixPaintAction";
        return name;
    }

	void
	FixPaintAction::setParameter(const String& name, const String& value)
	{
		if (Ogre::StringUtil::startsWith(name, "%", false))
		{
			Ogre::String parameter = name.substr(1);

			if ( parameter == "InfoString" )
			{
				mGridSelection.reset();
				mInfoString = value;
				parseInfoString();
			}
		}
		else
		{
			Action::setParameter(name, value);
		}
	}
    
	String
	FixPaintAction::getParameter(const String& name) const
	{
        if (Ogre::StringUtil::startsWith(name, "%", false))
        {
            Ogre::String parameter = name.substr(1);

            if ( parameter == "InfoString" )
            {
                return mInfoString;
            }
			else
                return Action::getParameter(name);
        }
        else
            return Action::getParameter(name);
	}  

	void 
	FixPaintAction::parseInfoString()
	{
		Ogre::MemoryDataStream* stream = new Ogre::MemoryDataStream((void *)(mInfoString.c_str()),
			mInfoString.length(), false);

		String line;
		Ogre::String textureName;

		while(!stream->eof())
		{
			line = stream->getLine();

			// junction height
			if ( line.substr(0,8) == "junction")
			{
				mPasteHeightInfo = true;

				stream->skipLine();

				String valueLine;
				Ogre::StringVector vecparams;

				valueLine = stream->getLine();

				while ( valueLine != "}" )
				{
					vecparams = Ogre::StringUtil::split(valueLine);
					assert ( vecparams.size() == 3 );            

					valueLine = stream->getLine();

					int junctionX = Ogre::StringConverter::parseInt(vecparams[0]);
					int junctionZ = Ogre::StringConverter::parseInt(vecparams[1]);
					float height = Ogre::StringConverter::parseReal(vecparams[2]);

					mGridSelection.setJunctionValue(Point(junctionX, junctionZ), height);
				}
			}
			// texture
			else if ( line.substr(0,12) == "grid_texture" )
			{
				mPasteTextureInfo = true;

				stream->skipLine();

				String valueLine;
				Ogre::StringVector vecparams;

				valueLine = stream->getLine();
				char strBuf[128];
				strcpy(strBuf,valueLine.c_str());
				while ( valueLine != "}" )
				{
					vecparams = Ogre::StringUtil::split(valueLine);
					assert ( vecparams.size() == 14 || vecparams.size() == 8 );            

					valueLine = stream->getLine();
					char strBuf[128];
					strcpy(strBuf,valueLine.c_str());


					// 获取当前要操作的grid的绝对坐标
					int gridX = Ogre::StringConverter::parseInt(vecparams[0]);
					int gridZ = Ogre::StringConverter::parseInt(vecparams[1]);

					FixPaintSelection::TextureGridInfo gridInfo;                

					textureName = Ogre::StringConverter::toString(Ogre::StringConverter::parseStringVector(vecparams[2]));
					gridInfo.layers[0].textureName = textureName;
					gridInfo.layers[0].left = Ogre::StringConverter::parseReal(vecparams[3]);
					gridInfo.layers[0].top = Ogre::StringConverter::parseReal(vecparams[4]);
					gridInfo.layers[0].right = Ogre::StringConverter::parseReal(vecparams[5]);
					gridInfo.layers[0].bottom = Ogre::StringConverter::parseReal(vecparams[6]);
					gridInfo.layers[0].orientation = Ogre::StringConverter::parseUnsignedInt(vecparams[7]);

					if(vecparams.size() == 14)
					{
						textureName = Ogre::StringConverter::toString(Ogre::StringConverter::parseStringVector(vecparams[8]));
						gridInfo.layers[1].textureName = textureName;
						gridInfo.layers[1].left = Ogre::StringConverter::parseReal(vecparams[9]);
						gridInfo.layers[1].top = Ogre::StringConverter::parseReal(vecparams[10]);
						gridInfo.layers[1].right = Ogre::StringConverter::parseReal(vecparams[11]);
						gridInfo.layers[1].bottom = Ogre::StringConverter::parseReal(vecparams[12]);
						gridInfo.layers[1].orientation = Ogre::StringConverter::parseUnsignedInt(vecparams[13]);
					}
					else
						gridInfo.layers[1].textureName = "UnTextured";
					mGridSelection.setGridValue(Point(gridX, gridZ),gridInfo);
				}
			}
			// diagonal
			else if ( line.substr(0,13) == "grid_diagonal" )
			{
				mPasteDiagonalInfo = true;

				stream->skipLine();

				String valueLine;
				Ogre::StringVector vecparams;

				valueLine = stream->getLine();

				while ( valueLine != "}" )
				{
					vecparams = Ogre::StringUtil::split(valueLine);
					assert ( vecparams.size() == 3 );            

					valueLine = stream->getLine();

					int gridX = Ogre::StringConverter::parseInt(vecparams[0]);
					int gridZ = Ogre::StringConverter::parseInt(vecparams[1]);
					uint flags = Ogre::StringConverter::parseUnsignedInt(vecparams[2]);

					mGridSelection.setFlagValue(Point(gridX, gridZ),flags);
				}
			}
		}

		delete stream;
	}

    void
	FixPaintAction::_buildHitIndicator(const Point& pt)
    {
        Ogre::Vector3 position;
        bool intersected = getSceneManipulator()->getTerrainIntersects(pt, position);
        if (!intersected)
        {
            return;
        }

		getSceneManipulator()->getHitIndicator("IntersectGrids")->setHitPoint(pt);

        getSceneManipulator()->getHitIndicator("IntersectPoint")->setHitPoint(pt);
    }

    void
	FixPaintAction::_doPaint(JunctionSelection* modifiedJunctions,GridSelection* modifiedGrids,const Point& mCurrentMousePos)
    {
		Ogre::Vector3 position;
		bool hit = getSceneManipulator()->getTerrainIntersects(mCurrentMousePos.x,mCurrentMousePos.y, position);
		if (!hit)
		{
			return;
		}
		
		std::vector<TerrainInfo> terrainInfo;

		bool junctionChanged =false;
		bool gridChanged = false;
		Ogre::String textureName;
		int pixmapId;
		float left,top,right,bottom;//bugfix,此处原来为int

		if(mPasteHeightInfo)
		{
			std::pair<int, int> mouseJunction = mSceneManipulator->getTerrainData()->getJunction(position.x, position.z);
			FixPaintSelection::JunctionMap::const_iterator iter;
			terrainInfo.clear();

			//TerrainInfo高度数据是绝对值
			for( iter = mGridSelection.getJunctions().begin(); iter!=mGridSelection.getJunctions().end();iter++)
			{
				int absJunctionX = iter->first.x + mouseJunction.first;
				int absJunctionZ = iter->first.y + mouseJunction.second;

				if ( false == mSceneManipulator->getTerrainData()->isValidJunction(absJunctionX, absJunctionZ) )
					continue;

				float oldHeight = mSceneManipulator->getTerrainData()->getHeight(absJunctionX, absJunctionZ);
				float absHeight;
				if(!modifiedJunctions->exist(absJunctionX,absJunctionZ))
				{
					modifiedJunctions->add(absJunctionX, absJunctionZ, 1.0);
					absHeight = iter->second.height;
				}
				else
				{
					JunctionSelection::JunctionMap::const_iterator it = modifiedJunctions->getJunctions().find(mSceneManipulator->getTerrainData()->_getJunctionIndex(absJunctionX, absJunctionZ));
					assert( it != modifiedJunctions->getJunctions().end());
					
					absHeight = iter->second.height;
				}

				mSceneManipulator->getTerrainData()->setHeight(absJunctionX, absJunctionZ, absHeight);
				
				if(oldHeight != absHeight)
				{
					TerrainInfo terrInfo;
					terrInfo.x = absJunctionX;
					terrInfo.z = absJunctionZ;
					terrInfo.oldHeight = oldHeight;
					terrInfo.nowHeight = absHeight;
					terrainInfo.push_back(terrInfo);
				}				
				junctionChanged = true;
			}
		}			
		if(mPasteTextureInfo)
		{
			std::pair<int, int> mouseGrid = mSceneManipulator->getTerrainData()->getGrid(position.x, position.z);

			FixPaintSelection::GridMap::const_iterator iter;
			for( iter = mGridSelection.getGrids().begin(); iter!=mGridSelection.getGrids().end();iter++)
			{
				int absGridX = iter->first.x + mouseGrid.first;
				int absGridZ = iter->first.y + mouseGrid.second;

				if ( false == mSceneManipulator->getTerrainData()->isValidGrid(absGridX, absGridZ) )
					continue;

				if ( !modifiedGrids->exist(absGridX,absGridZ) )
				{
					modifiedGrids->add(absGridX, absGridZ);
				}
				
				WX::TerrainData::GridInfo gridInfo = mSceneManipulator->getTerrainData()->getGridInfo(absGridX, absGridZ);

				textureName = iter->second.info.layers[0].textureName;
				left = iter->second.info.layers[0].left;
				top = iter->second.info.layers[0].top;
				right = iter->second.info.layers[0].right;
				bottom = iter->second.info.layers[0].bottom;
				if(textureName == "UnTextured")
					pixmapId = 0;
				else
				{
					pixmapId = mSceneManipulator->getTerrainData()->_registerPixmap(textureName,left,top,right,bottom);
				}
				gridInfo.layers[0].pixmapId = pixmapId;
				gridInfo.layers[0].orientation = iter->second.info.layers[0].orientation;

				
				textureName = iter->second.info.layers[1].textureName;
				if(textureName != "UnTextured")
				{
					left = iter->second.info.layers[1].left;
					top = iter->second.info.layers[1].top;
					right = iter->second.info.layers[1].right;
					bottom = iter->second.info.layers[1].bottom;
					
					pixmapId = mSceneManipulator->getTerrainData()->_registerPixmap(textureName,left,top,right,bottom);
				}
				else
				{
					pixmapId = 0;
					left = top = right = bottom = 0;
				}
				gridInfo.layers[1].pixmapId = pixmapId;
				gridInfo.layers[1].orientation = iter->second.info.layers[1].orientation;

				mSceneManipulator->getTerrainData()->setGridInfo(absGridX, absGridZ, gridInfo);
				
				gridChanged = true;
			}
		}
		if(mPasteDiagonalInfo)
		{
			std::pair<int, int> mouseGrid = mSceneManipulator->getTerrainData()->getGrid(position.x, position.z);
			FixPaintSelection::GridMap::const_iterator iter;
			for( iter = mGridSelection.getGrids().begin(); iter!=mGridSelection.getGrids().end();iter++)
			{
				int absGridX = iter->first.x + mouseGrid.first;
				int absGridZ = iter->first.y + mouseGrid.second;

				if ( false == mSceneManipulator->getTerrainData()->isValidGrid(absGridX, absGridZ) )
					continue;

				if ( !modifiedGrids->exist(absGridX,absGridZ) )
					modifiedGrids->add(absGridX, absGridZ);

				WX::TerrainData::GridInfo gridInfo = mSceneManipulator->getTerrainData()->getGridInfo(absGridX, absGridZ);
				gridInfo.flags = iter->second.info.flags;
				mSceneManipulator->getTerrainData()->setGridInfo(absGridX, absGridZ, gridInfo);
				
				gridChanged = true;
			}
		}

		if(junctionChanged)
		{
			getSceneManipulator()->_fireTerrainHeightChanged(terrainInfo);
			modifiedJunctions->notifyModified();
		}

		if(gridChanged)
		{
			modifiedGrids->notifyModified();
		}
    }
    
    void
	FixPaintAction::_onActive(bool active)
    {
        if (!active)
        {
			mHintModifiedJunctions->apply();
			mHintModifiedJunctions->reset();
            mHintModifiedGrids->apply();
            mHintModifiedGrids->reset();
        }
    }
    
    void
	FixPaintAction::_onMove(const Point& pt)
    {
		mHintModifiedJunctions->apply();
		mHintModifiedJunctions->reset();
        mHintModifiedGrids->apply();
        mHintModifiedGrids->reset();
        _buildHitIndicator(pt);
        _doPaint(mHintModifiedJunctions,mHintModifiedGrids,pt);
    }
    
    void
	FixPaintAction::_onBegin(const Point& pt)
    {
        mHintModifiedJunctions->apply();
        mHintModifiedJunctions->reset();
		mHintModifiedGrids->apply();
		mHintModifiedGrids->reset();
		mModifiedJunctions->reset();
        mModifiedGrids->reset();
        _buildHitIndicator(pt);
		_doPaint(mModifiedJunctions,mModifiedGrids,pt);
    }
    
    void
	FixPaintAction::_onDrag(const Point& pt)
    {
        _buildHitIndicator(pt);
        _doPaint(mModifiedJunctions,mModifiedGrids,pt);
    }
    
    void
	FixPaintAction::_onEnd(const Point& pt, bool canceled)
    {
        doFinish(mModifiedJunctions,mModifiedGrids,canceled);
		mModifiedJunctions->reset();
        mModifiedGrids->reset();
        _buildHitIndicator(pt);
    }

	void 
	FixPaintAction::doFinish(JunctionSelection* originJunctions,GridSelection* originGrids, bool canceled)
	{
		assert(originJunctions || originGrids);

		if (canceled)
		{
			originJunctions->apply();
			originGrids->apply();
		}
		else
		{
			std::auto_ptr<WX::TerrainSelectionActionOperator> op( new WX::TerrainSelectionActionOperator(getSceneManipulator()) );

			if(!originJunctions->empty())
			{
				const JunctionSelection::JunctionMap& junctions = originJunctions->getJunctions();
				JunctionSelection::JunctionMap::const_iterator it;
				for (it = junctions.begin(); it != junctions.end(); ++it)
				{
					const JunctionSelection::Junction& junction = it->second;
					float oldHeight = junction.height;
					float newHeight = mSceneManipulator->getTerrainData()->getHeight(junction.x,junction.z);
			
					if (newHeight != oldHeight)
					{
						op->mModifyHeightOperator->add(junction.x, junction.z, oldHeight, newHeight);
					}
				}
			}
			//因为Grid信息里包含了纹理和对角线信息,所以放在一起
			if (!originGrids->empty())
			{
				const GridSelection::GridMap& grids = originGrids->getGrids();
				GridSelection::GridMap::const_iterator it;
				for (it = grids.begin(); it != grids.end(); ++it)
				{
					const GridSelection::Grid& grid = it->second;
					const TerrainData::GridInfo &oldGridInfo = grid.info;
					const TerrainData::GridInfo &newGridInfo = mSceneManipulator->getTerrainData()->getGridInfo(grid.x, grid.z);
					if (newGridInfo != oldGridInfo)
					{
						op->mModifyPaintOperator->add(grid.x, grid.z, oldGridInfo, newGridInfo);
					}
				}

			}

			if (!op->empty())
				getOperatorManager()->addOperator(op.release());

		}
	}
}
