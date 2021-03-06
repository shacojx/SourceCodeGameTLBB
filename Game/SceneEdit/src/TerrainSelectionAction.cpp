#include "TerrainSelectionAction.h"
#include "SceneManipulator.h"
#include "HitIndicator.h"
#include "IntersectGrids.h"
#include "TerrainSelections.h"
#include "TerrainSelectionActionOperator.h"
#include "WXModifyTerrainHeightOperator.h"
#include "WXModifyTerrainPaintOperator.h"
#include "GridInfoFlagModifyOperator.h"
#include "WXOperatorManager.h"

#include <OgreStringConverter.h>
#include <OgreDataStream.h>

namespace WX {

    //////////////////////////////////////////////////////////////////////////  

    TerrainSelectionAction::TerrainSelectionAction(SceneManipulator* sceneManipulator)
        : Action(sceneManipulator), mPasteHeightInfo(true), mPasteTextureInfo(true), mPasteDiagonalInfo(true),
        mCurrentMousePos(Ogre::Vector3::ZERO), mCurrentMode("SingleSelecting")
    {
        mTerrainSelectionGrids = static_cast<TerrainSelectionGrids *>(sceneManipulator->getHitIndicator("TerrainSelectionGrids"));
		mAfterCopy = false;
		mRotateCount = 0;
	//	mMaxJunctionX = 0;
	//	mMaxJunctionZ = 0;
	}

    TerrainSelectionAction::~TerrainSelectionAction()
    {        
    }

    void TerrainSelectionAction::setParameter(const String& name, const String& value)
    {
        if (name == "%CurrentMode")
        {
            mCurrentMode = value;
        }   
        else if (name == "%PasteHeightInfo")
        {
            mPasteHeightInfo = Ogre::StringConverter::parseBool(value);
        }
        else if (name == "%PasteTextureInfo")
        {
            mPasteTextureInfo = Ogre::StringConverter::parseBool(value);
        }
        else if (name == "%PasteDiagonalInfo")
        {
            mPasteDiagonalInfo = Ogre::StringConverter::parseBool(value);
        }        
        else if (name == "%CurrentMousePosition")
        {
            mCurrentMousePos = Ogre::StringConverter::parseVector3(value);
            pasteTerrainInfo();
        }
		else if (name == "%AfterCopy")
		{
			mAfterCopy = true;
			mRotateCount = 0;
		}
		else if (name == "%BeginRotateTerrain")
		{
			++mRotateCount;
			beginRotateTerrain();
		}
		else
        {
            Action::setParameter(name, value);
        }
    }

    String TerrainSelectionAction::getParameter(const String& name) const
    {
        if (name == "%CurrentMode")
        {
            return mCurrentMode;
        }
        else if (name == "%JunctionHeightInfo")
        {
			// ??????????????????????????????????????????????????????????????????????
			if (!mAfterCopy)
				getJunctionInfoAsString();

            assert( !mJunctionHeightInfoString.empty() );
            return mJunctionHeightInfoString;
        }		
        else if (name == "%GridTextureInfo")
        {
			if (!mAfterCopy)
				getGridTextureInfoAsString();

            assert( !mGridTextureInfoString.empty() );
            return mGridTextureInfoString;
        }
        else if (name == "%GridDiagonalInfo")
        {
			if (!mAfterCopy)
				getGridDiagonalInfoAsString();

            assert( !mGridDiagonalInfoString.empty() );
            return mGridDiagonalInfoString;
        }
        else if (name == "%PasteHeightInfo")
        {
            return Ogre::StringConverter::toString(mPasteHeightInfo);
        }
        else if (name == "%PasteTextureInfo")
        {
            return Ogre::StringConverter::toString(mPasteTextureInfo);
        }
        else if (name == "%PasteDiagonalInfo")
        {
            return Ogre::StringConverter::toString(mPasteDiagonalInfo);
        }
		else if (name == "%AfterCopy")
		{
			return Ogre::StringConverter::toString(mAfterCopy);
		}

        return Action::getParameter(name);
    }

    void 
    TerrainSelectionAction::_onActive(bool active)
    {
        if (!active)
        {
            mTerrainSelectionGrids->hide();
        }
        else
        {
            mTerrainSelectionGrids->show();
        }
    }

    void 
    TerrainSelectionAction::_onMove(const Point& pt)
    {
        getSceneManipulator()->getHitIndicator("IntersectPoint")->setHitPoint(pt);

		Ogre::Vector3 pos;
		bool intersected = getSceneManipulator()->getTerrainIntersects(pt, pos);

		pos = getSceneManipulator()->_alignPosition(pos,1.0f);
		if (intersected && mAfterCopy)
		{
			mTerrainSelectionGrids->setSceneNodePos(pos);
		}
    }

    void 
    TerrainSelectionAction::_onBegin(const Point& pt)
    {
        if ( mCurrentMode == "MultiSelecting" )
            mTerrainSelectionGrids->setCurrentGridSelection(true);
        else
            mTerrainSelectionGrids->setCurrentGridSelection(false);
    }

    void 
    TerrainSelectionAction::_onDrag(const Point& pt)
    {
       mTerrainSelectionGrids->setHitPoint(pt);

	   // ????????????????????????????????????????????????????????false??????????????????????
	   mAfterCopy = false;
	   mTerrainSelectionGrids->setOriginSceneNodeOrientation();
    }

    void 
    TerrainSelectionAction::_onEnd(const Point& pt, bool canceled)
    {
        if ( mCurrentMode == "MultiSelecting" )
            mTerrainSelectionGrids->addCurrentGridSelection(true);
        else
            mTerrainSelectionGrids->addCurrentGridSelection(false);
    }

    const String& TerrainSelectionAction::getName(void) const
    {
        static const String name = "TerrainSelectionAction";
        return name;
    }

    void TerrainSelectionAction::getJunctionInfoAsString(void) const
    {
        mJunctionHeightInfoString.clear();

        // ????????????????????junctionselection
        WX::JunctionSelection* selection = mTerrainSelectionGrids->getTotalJunctionSelection();

        assert ( selection->empty() == false );

        WX::SceneManipulator *manipulator = getSceneManipulator();

        assert (manipulator);

        const WX::JunctionSelection::JunctionMap& junctions = selection->getJunctions();
        WX::JunctionSelection::JunctionMap::const_iterator it;

        int minJunctionX = 0;
        int minJunctionZ = 0;

		int middleJunctionX = (mTerrainSelectionGrids->getMaxX() - mTerrainSelectionGrids->getMinX() + 1) / 2 + mTerrainSelectionGrids->getMinX();
		int middleJunctionZ = (mTerrainSelectionGrids->getMaxZ() - mTerrainSelectionGrids->getMinZ() + 1) / 2 + mTerrainSelectionGrids->getMinZ();
		float minJunctionHeight = manipulator->getTerrainData()->getHeight(middleJunctionX, middleJunctionZ);
        // ??????????????junction ??????????
        /** FIXME ????????????????????????????????????????????????????????junction????????????
            ????????????????????????????????????????????????????junction????????????????????????
            ????????????????????????junction??????????????????????????????????????
        */
   //     float minJunctionHeight = 0.0f;
        float worldHeight = 0.0f;

        for (it = junctions.begin(); it != junctions.end(); ++it)
        {
            mJunctionHeightInfoString += "junction\n{\n";

            const WX::JunctionSelection::Junction& junction = it->second;

            // ??????junction????????????0 0
   //         if ( it == junctions.begin() )
   //         {
   //             minJunctionX = junction.x;
   //             minJunctionZ = junction.z;

   //             mJunctionHeightInfoString += Ogre::StringConverter::toString(0);
   //             mJunctionHeightInfoString += " ";
   //             mJunctionHeightInfoString += Ogre::StringConverter::toString(0);
   //             mJunctionHeightInfoString += " ";

   //             // ??????junction??????????????????????????junction????????????????????????????????
   //             minJunctionHeight = manipulator->getTerrainData()->getHeight(junction.x, junction.z);
			//}
   //         // ??????junction??????????????????????junction??????????
   //         else
   //         {
				int x = junction.x - middleJunctionX;
				/*if (x>mMaxJunctionX)
					mMaxJunctionX = x;*/

                mJunctionHeightInfoString += Ogre::StringConverter::toString(x);
                mJunctionHeightInfoString += " ";

				int z = junction.z - middleJunctionZ;
			/*	if (z>mMaxJunctionZ)
					mMaxJunctionZ = z;*/

                mJunctionHeightInfoString += Ogre::StringConverter::toString(z);
                mJunctionHeightInfoString += " ";
        //    }

            worldHeight = manipulator->getTerrainData()->getHeight(junction.x, junction.z);

			// ????????????????????????????????
         //   float relativeHeight = worldHeight - minJunctionHeight;

		//	mHeightArray.push_back(relativeHeight);
            mJunctionHeightInfoString += Ogre::StringConverter::toString(worldHeight);

            mJunctionHeightInfoString += "\n}\n";
       }
    }

    void TerrainSelectionAction::getGridTextureInfoAsString(void) const
    {
        mGridTextureInfoString.clear();

        WX::GridSelection* gridSelection = mTerrainSelectionGrids->getTotalGridSelection();

        assert ( gridSelection->empty() == false );

        WX::SceneManipulator *manipulator = getSceneManipulator();

        assert (manipulator);

        const WX::GridSelection::GridMap& grids = gridSelection->getGrids();
        WX::GridSelection::GridMap::const_iterator it;

        int minGridX = 0;
        int minGridZ = 0;

		int middleGridX = (mTerrainSelectionGrids->getMaxX() - mTerrainSelectionGrids->getMinX()) / 2 + mTerrainSelectionGrids->getMinX();
		int middleGridZ = (mTerrainSelectionGrids->getMaxZ() - mTerrainSelectionGrids->getMinZ()) / 2 + mTerrainSelectionGrids->getMinZ();

        for (it = grids.begin(); it != grids.end(); ++it)
        {
            mGridTextureInfoString += "grid_texture\n{\n";

            const WX::GridSelection::Grid& grid = it->second;

            // ??????grid????????????0 0
            //if ( it == grids.begin() )
            //{
            //    minGridX = grid.x;
            //    minGridZ = grid.z;

            //    mGridTextureInfoString += Ogre::StringConverter::toString(0);
            //    mGridTextureInfoString += " ";
            //    mGridTextureInfoString += Ogre::StringConverter::toString(0);
            //    mGridTextureInfoString += " ";         
            //}
            //// ??????grid??????????????????????grid??????????
            //else
            //{
                mGridTextureInfoString += Ogre::StringConverter::toString(grid.x - middleGridX);
                mGridTextureInfoString += " ";
                mGridTextureInfoString += Ogre::StringConverter::toString(grid.z - middleGridZ);
                mGridTextureInfoString += " ";
          //  }

            const WX::TerrainData::GridInfo &gridInfo = grid.info;

			/** ????????????????????????????id????????????
			1 ????????
			2 ????????????????????
			3 ??????????
			*/
            // ????????????????
			ushort id = gridInfo.layers[0].pixmapId;
			assert (id>=0);
			WX::TerrainData *pTerrainData = manipulator->getTerrainData();
			assert (pTerrainData);
			
			if (id>0)
			{
				mGridTextureInfoString += pTerrainData->mTextures[ pTerrainData->mPixmaps[id-1].textureId ];
				mGridTextureInfoString += " ";
				mGridTextureInfoString += Ogre::StringConverter::toString(pTerrainData->mPixmaps[id-1].left);
				mGridTextureInfoString += " ";
				mGridTextureInfoString += Ogre::StringConverter::toString(pTerrainData->mPixmaps[id-1].top);
				mGridTextureInfoString += " ";
				mGridTextureInfoString += Ogre::StringConverter::toString(pTerrainData->mPixmaps[id-1].right);
				mGridTextureInfoString += " ";
				mGridTextureInfoString += Ogre::StringConverter::toString(pTerrainData->mPixmaps[id-1].bottom);
				mGridTextureInfoString += " ";
				mGridTextureInfoString += Ogre::StringConverter::toString(gridInfo.layers[0].orientation);
				mGridTextureInfoString += " ";
			}
			else
			{
				mGridTextureInfoString += "UnTextured 0.0 0.0 1.0 1.0 0.0";				
			}

            // ????????????????
			id = gridInfo.layers[1].pixmapId;
			// ??????????????????????????????
			if (id>0)
			{
				mGridTextureInfoString += pTerrainData->mTextures[ pTerrainData->mPixmaps[id-1].textureId ];
				mGridTextureInfoString += " ";
				mGridTextureInfoString += Ogre::StringConverter::toString(pTerrainData->mPixmaps[id-1].left);
				mGridTextureInfoString += " ";
				mGridTextureInfoString += Ogre::StringConverter::toString(pTerrainData->mPixmaps[id-1].top);
				mGridTextureInfoString += " ";
				mGridTextureInfoString += Ogre::StringConverter::toString(pTerrainData->mPixmaps[id-1].right);
				mGridTextureInfoString += " ";
				mGridTextureInfoString += Ogre::StringConverter::toString(pTerrainData->mPixmaps[id-1].bottom);
				mGridTextureInfoString += " ";
				mGridTextureInfoString += Ogre::StringConverter::toString(gridInfo.layers[1].orientation);
				mGridTextureInfoString += " ";
			}

            mGridTextureInfoString += "\n}\n";
        }
    }

    void TerrainSelectionAction::getGridDiagonalInfoAsString(void) const
    {
        mGridDiagonalInfoString.clear();

        WX::GridSelection* gridSelection = mTerrainSelectionGrids->getTotalGridSelection();

        assert ( gridSelection->empty() == false );

        WX::SceneManipulator *manipulator = getSceneManipulator();

        assert (manipulator);

        const WX::GridSelection::GridMap& grids = gridSelection->getGrids();
        WX::GridSelection::GridMap::const_iterator it;
       
		int middleGridX = (mTerrainSelectionGrids->getMaxX() - mTerrainSelectionGrids->getMinX()) / 2 + mTerrainSelectionGrids->getMinX();
		int middleGridZ = (mTerrainSelectionGrids->getMaxZ() - mTerrainSelectionGrids->getMinZ()) / 2 + mTerrainSelectionGrids->getMinZ();

        for (it = grids.begin(); it != grids.end(); ++it)
        {
            mGridDiagonalInfoString += "grid_diagonal\n{\n";

            const WX::GridSelection::Grid& grid = it->second;

            // ??????grid????????????0 0
            //if ( it == grids.begin() )
            //{
            //    minGridX = grid.x;
            //    minGridZ = grid.z;

            //    mGridDiagonalInfoString += Ogre::StringConverter::toString(0);
            //    mGridDiagonalInfoString += " ";
            //    mGridDiagonalInfoString += Ogre::StringConverter::toString(0);
            //    mGridDiagonalInfoString += " ";         
            //}
            //// ??????grid??????????????????????grid??????????
            //else
            //{
                mGridDiagonalInfoString += Ogre::StringConverter::toString(grid.x - middleGridX);
                mGridDiagonalInfoString += " ";
                mGridDiagonalInfoString += Ogre::StringConverter::toString(grid.z - middleGridZ);
                mGridDiagonalInfoString += " ";
         //   }

            const WX::TerrainData::GridInfo &gridInfo = grid.info;

            // ????????????????
            mGridDiagonalInfoString += Ogre::StringConverter::toString(gridInfo.flags);

            mGridDiagonalInfoString += "\n}\n";
        }
    }  

    void TerrainSelectionAction::pasteTerrainInfo(void)
    {
        WX::SceneManipulator *manipulator = getSceneManipulator();
		WX::TerrainData *pTerrainData = manipulator->getTerrainData();
		assert (pTerrainData);

        String infoString;

        // ??????????????????????????
        if ( mPasteHeightInfo )
            infoString += mJunctionHeightInfoString;
        if ( mPasteDiagonalInfo )
            infoString += mGridDiagonalInfoString;
        if ( mPasteTextureInfo )
            infoString += mGridTextureInfoString;

        // ????clipboard??????????????????????????????????????????????????????????????????
        Ogre::MemoryDataStream* stream = new Ogre::MemoryDataStream((void *)(infoString.c_str()),
            infoString.length(), false);

        // ??????????selection??????????????????????????notify??
        WX::JunctionSelection* junctionSelection = new WX::JunctionSelection( manipulator->getTerrain(),getSceneManipulator() );
        WX::GridSelection* gridSelection = new WX::GridSelection( manipulator->getTerrain(),getSceneManipulator() );

        // ????????operator????????height??paint??diagonal??????????
        std::auto_ptr<WX::TerrainSelectionActionOperator> op( new WX::TerrainSelectionActionOperator(manipulator) );

        String line;
        // ???????????????????? 
        std::vector<TerrainInfo> terrainInfo;
        while(!stream->eof())
        {
            line = stream->getLine();

            // junction height
            if ( line.substr(0,8) == "junction" && mPasteHeightInfo )
            {
				mCurrentMousePos = getSceneManipulator()->_alignPosition(mCurrentMousePos,1.0f);
                std::pair<int, int> mouseJunction = pTerrainData->getJunction(mCurrentMousePos.x, mCurrentMousePos.z);

                // ???? { ??????
                stream->skipLine();

                String valueLine;
                Ogre::StringVector vecparams;

                valueLine = stream->getLine();

                while ( valueLine != "}" )
                {
                    vecparams = Ogre::StringUtil::split(valueLine);

                    assert ( vecparams.size() == 3 );            

                    // ??????????
                    valueLine = stream->getLine();

                    // ????????????????junction??????????
                    int absJunctionX = Ogre::StringConverter::parseInt(vecparams[0]) + mouseJunction.first;
                    int absJunctionZ = Ogre::StringConverter::parseInt(vecparams[1]) + mouseJunction.second;

                    if ( false == pTerrainData->isValidJunction(absJunctionX, absJunctionZ) )
                        continue;

                    // ??????????????????????????????
					// ??????????????
					float worldHeight = Ogre::StringConverter::parseReal(vecparams[2]);

                   // float absHeight = Ogre::StringConverter::parseReal(vecparams[2]) +
                     //   pTerrainData->getHeight(absJunctionX, absJunctionZ);

                    // ????????????????????????????
                    float oldHeight = pTerrainData->getHeight(absJunctionX, absJunctionZ);
                    // ????????????

                    TerrainInfo terrInfo;
                    terrInfo.x = absJunctionX;
                    terrInfo.z = absJunctionZ;
                    terrInfo.oldHeight = oldHeight;
                    terrInfo.nowHeight = worldHeight;
                    terrainInfo.push_back(terrInfo);                    

                    pTerrainData->setHeight(absJunctionX, absJunctionZ, worldHeight);

                    // ??????operator??
                    if (worldHeight != oldHeight)
                    {
                        op->mModifyHeightOperator->add(absJunctionX, absJunctionZ, oldHeight, worldHeight);
                    }

                    // ????????????junction??????notify??selection?? TODO
                    junctionSelection->add(absJunctionX, absJunctionZ, 1.0);
                }
            } // end of junction
            // texture
            else if ( line.substr(0,12) == "grid_texture" && mPasteTextureInfo )
            {
                std::pair<int, int> mouseGrid = pTerrainData->getGrid(mCurrentMousePos.x, mCurrentMousePos.z);

                // ???? { ??????
                stream->skipLine();

                String valueLine;
                Ogre::StringVector vecparams;

                valueLine = stream->getLine();

                while ( valueLine != "}" )
                {
                    vecparams = Ogre::StringUtil::split(valueLine);

                    size_t paraSize = vecparams.size();
					// ????????????????????8????????????????x??y????????????????left??top??right??bottom??orientation
					assert (paraSize>=8);

                    // ??????????
                    valueLine = stream->getLine();

                    // ????????????????grid??????????
                    int absGridX = Ogre::StringConverter::parseInt(vecparams[0]) + mouseGrid.first;
                    int absGridZ = Ogre::StringConverter::parseInt(vecparams[1]) + mouseGrid.second;

                    if ( false == pTerrainData->isValidGrid(absGridX, absGridZ) )
                        continue;

                    WX::TerrainData::GridInfo gridInfo = pTerrainData->getGridInfo(absGridX, absGridZ);

                    // ????grid??????????
                    const WX::TerrainData::GridInfo oldGridInfo = gridInfo;                    

					int layerPixmapId = 0;

					if (vecparams[2] != "UnTextured")
					{
						layerPixmapId = pTerrainData->_registerPixmap(
						vecparams[2], // texture name
						Ogre::StringConverter::parseReal(vecparams[3]),  // left
						Ogre::StringConverter::parseReal(vecparams[4]),  // top
						Ogre::StringConverter::parseReal(vecparams[5]),  // right
						Ogre::StringConverter::parseReal(vecparams[6])  // bottom
						);
					}					 

					assert (layerPixmapId>=0);
					gridInfo.layers[0].pixmapId = layerPixmapId;
					ushort orientation = Ogre::StringConverter::parseUnsignedInt(vecparams[7]);
					assert (0 <= orientation && orientation < 16 && "Invalid orientation");
					gridInfo.layers[0].orientation = orientation;

					// ??????????????????????????????????
					if (paraSize>8)
					{
						assert (paraSize == 14);

						layerPixmapId = pTerrainData->_registerPixmap(
							vecparams[8], // texture name
							Ogre::StringConverter::parseReal(vecparams[9]),  // left
							Ogre::StringConverter::parseReal(vecparams[10]),  // top
							Ogre::StringConverter::parseReal(vecparams[11]),  // right
							Ogre::StringConverter::parseReal(vecparams[12])  // bottom
							);

						assert (layerPixmapId>0);
						gridInfo.layers[1].pixmapId = layerPixmapId;
						ushort orientation = Ogre::StringConverter::parseUnsignedInt(vecparams[13]);
						assert (0 <= orientation && orientation < 16 && "Invalid orientation");
						gridInfo.layers[1].orientation = orientation;
					}
					else
					{
						// ??????????????????????
						gridInfo.layers[1].pixmapId = 0;
						gridInfo.layers[1].orientation = 0;
					}

                    pTerrainData->setGridInfo(absGridX, absGridZ, gridInfo);

                    // ??????grid??????????
                    const WX::TerrainData::GridInfo &newGridInfo = pTerrainData->getGridInfo(absGridX, absGridZ);

                    if (newGridInfo != oldGridInfo)
                    {
                        op->mModifyPaintOperator->add(absGridX, absGridZ, oldGridInfo, newGridInfo);
                    }

                    // ????????grid????????????notify??selection??????????????????????diagonal??texture??grid
                    // ??????????????notify??grid selection??
                    if ( !gridSelection->exist(absGridX,absGridZ) )
                        gridSelection->add(absGridX, absGridZ);
                }
            } // end of texture 
            // diagonal
            else if ( line.substr(0,13) == "grid_diagonal" && mPasteDiagonalInfo )
            {
                std::pair<int, int> mouseGrid = pTerrainData->getGrid(mCurrentMousePos.x, mCurrentMousePos.z);

                // ???? { ??????
                stream->skipLine();

                String valueLine;
                Ogre::StringVector vecparams;

                valueLine = stream->getLine();

                while ( valueLine != "}" )
                {
                    vecparams = Ogre::StringUtil::split(valueLine);

                    assert ( vecparams.size() == 3 );            

                    // ??????????
                    valueLine = stream->getLine();

                    // ????????????????grid??????????
                    int absGridX = Ogre::StringConverter::parseInt(vecparams[0]) + mouseGrid.first;
                    int absGridZ = Ogre::StringConverter::parseInt(vecparams[1]) + mouseGrid.second;

                    if ( false == pTerrainData->isValidGrid(absGridX, absGridZ) )
                        continue;

                    WX::TerrainData::GridInfo gridInfo = pTerrainData->getGridInfo(absGridX, absGridZ);

                    unsigned int oldFlipFlag = gridInfo.flags;

                    gridInfo.flags = Ogre::StringConverter::parseUnsignedInt(vecparams[2]);

                    pTerrainData->setGridInfo(absGridX, absGridZ, gridInfo);

                    unsigned int newFlipFlag = pTerrainData->getGridInfo(absGridX, absGridZ).flags;

                    if (newFlipFlag != oldFlipFlag)
                    {
                        op->mFlipDiagonalOperator->add(absGridX, absGridZ, oldFlipFlag, newFlipFlag);
                    }

                    if ( !gridSelection->exist(absGridX,absGridZ) )
                        gridSelection->add(absGridX, absGridZ);
                }
            } // end of diagonal
        }

        getSceneManipulator()->_fireTerrainHeightChanged(terrainInfo);

        // notify
        junctionSelection->notifyModified();
        delete junctionSelection;

        gridSelection->notifyModified();
        delete gridSelection;    

        // ??????operator
        if (!op->empty())
            manipulator->getOperatorManager()->addOperator(op.release());

        // ??????????????
        delete stream;
    }

	void TerrainSelectionAction::beginRotateTerrain()
	{
		// ??????????????????????????????????
		mTerrainSelectionGrids->rotateSceneNode();

		// ????????????????????
		Ogre::MemoryDataStream* stream = NULL;

		String line;
		String tempStr("");

		// ??????????????
		if (mPasteHeightInfo)
		{
			stream = new Ogre::MemoryDataStream((void *)(mJunctionHeightInfoString.c_str()),
            mJunctionHeightInfoString.length(), false);

			tempStr.clear();

			while(!stream->eof())
			{
				line = stream->getLine();

				if (line == "junction" || line == "{" || line == "}")
				{
					tempStr += line;
					tempStr += "\n";
				}
				else
				{
					Ogre::StringVector vecparams = Ogre::StringUtil::split(line);

					int x = Ogre::StringConverter::parseInt(vecparams[1]);
					int z = -Ogre::StringConverter::parseInt(vecparams[0]);

					tempStr += Ogre::StringConverter::toString(x);
					tempStr += " ";
					tempStr += Ogre::StringConverter::toString(z);
					tempStr += " ";
					tempStr += vecparams[2];
					tempStr += "\n";				 
				}
			}

			mJunctionHeightInfoString = tempStr;

			delete stream; 
			stream = NULL;
		}

		if (mPasteTextureInfo)
		{
			stream = new Ogre::MemoryDataStream((void *)(mGridTextureInfoString.c_str()),
            mGridTextureInfoString.length(), false);

			tempStr.clear();

			while(!stream->eof())
			{
				line = stream->getLine();

				if (line == "{" || line == "}" || line == "grid_texture")
				{
					tempStr += line;
					tempStr += "\n";
				}
				else
				{
					Ogre::StringVector vecparams = Ogre::StringUtil::split(line);

					int x = Ogre::StringConverter::parseInt(vecparams[1]);
					int z = -Ogre::StringConverter::parseInt(vecparams[0]);

					tempStr += Ogre::StringConverter::toString(x);
					tempStr += " ";
					tempStr += Ogre::StringConverter::toString(z);
					tempStr += " ";

					//tempStr += vecparams[2];	// ????
					//tempStr += " ";

					//Ogre::String l = vecparams[3];
					//Ogre::String t = vecparams[4];
					//Ogre::String r = vecparams[5];
					//Ogre::String b = vecparams[6];

					//tempStr += r;
					//tempStr += " ";
					//tempStr += b;
					//tempStr += " ";
					//tempStr += l;
					//tempStr += " ";
					//tempStr += t;
					//tempStr += " ";

					//tempStr += vecparams[7];
					//tempStr += " ";

					//if (vecparams.size()>8)
					//{
					//	tempStr += vecparams[8];	// ????
					//	tempStr += " ";

					//	l = vecparams[9];
					//	t = vecparams[10];
					//	r = vecparams[11];
					//	b = vecparams[12];

					//	tempStr += r;
					//	tempStr += " ";
					//	tempStr += b;
					//	tempStr += " ";
					//	tempStr += l;
					//	tempStr += " ";
					//	tempStr += t;
					//	tempStr += " ";

					//	tempStr += vecparams[13];
					//}
					// ????????????????????????????????????????????
					for ( size_t i=2; i<vecparams.size()-1; ++i )
					{
						tempStr += vecparams[i];
						tempStr += " ";
					}

					// ????????????????????????????????????
					/** ????????????????l t r b??????????????????????????????????
						????????????????mRotateCount????????????????????????????????
						??????????????????orientation????????????????????????copy??
						????????????????????
						??????????????????????????????????????????????????????0????????????????
						????????????????orientation????????????????????????????????
					*/
					int nowRotateCount = mRotateCount % 4;

					switch (nowRotateCount)
					{
					case 1:
						tempStr += "6";
						break;
					case 2:
						tempStr += "3";
						break;
					case 3:
						tempStr += "5";
						break;
					default:
						tempStr += vecparams[vecparams.size()-1];
						break;
					}
					tempStr += " ";

					tempStr += "\n";
				}
			}

			mGridTextureInfoString = tempStr;

			delete stream;
			stream = NULL;
		}

		if (mPasteDiagonalInfo)
		{
			stream = new Ogre::MemoryDataStream((void *)(mGridDiagonalInfoString.c_str()),
            mGridDiagonalInfoString.length(), false);

			tempStr.clear();

			while(!stream->eof())
			{
				line = stream->getLine();

				if (line == "{" || line == "}" || line == "grid_diagonal")
				{
					tempStr += line;
					tempStr += "\n";
				}
				else
				{
					Ogre::StringVector vecparams = Ogre::StringUtil::split(line);

					int x = Ogre::StringConverter::parseInt(vecparams[1]);
					int z = -Ogre::StringConverter::parseInt(vecparams[0]);

					tempStr += Ogre::StringConverter::toString(x);
					tempStr += " ";
					tempStr += Ogre::StringConverter::toString(z);
					tempStr += " ";
					tempStr += vecparams[2];
					tempStr += "\n";				 
				}
			}

			mGridDiagonalInfoString = tempStr;

			delete stream; 
			stream = NULL;
		}		 
	}
}