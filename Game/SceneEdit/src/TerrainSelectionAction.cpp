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
			// 如果这个标志位还没设置，说明是想获取拷贝信息，否则，是获取旋转后的信息
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

	   // 重新进行拖拉操作，说明要重新拷贝地形，就把这个标志位设成false，表示还没进行拷贝操作
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

        // 获取当前所选择的全部junctionselection
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
        // 保存当前选择的junction 的最低高度
        /** FIXME 这里保存所选择的地形的高度保存的是相对信息，相对于第一个junction的高度，如果
            所选的地形是一个完整的山，这种方法没问题，如果第一个junction的位置在山顶的话，拷贝的
            结果是不正确的，因为其他junction的位置都比第一个低，所以相对高度为负数
        */
   //     float minJunctionHeight = 0.0f;
        float worldHeight = 0.0f;

        for (it = junctions.begin(); it != junctions.end(); ++it)
        {
            mJunctionHeightInfoString += "junction\n{\n";

            const WX::JunctionSelection::Junction& junction = it->second;

            // 第一个junction的相对坐标为0 0
   //         if ( it == junctions.begin() )
   //         {
   //             minJunctionX = junction.x;
   //             minJunctionZ = junction.z;

   //             mJunctionHeightInfoString += Ogre::StringConverter::toString(0);
   //             mJunctionHeightInfoString += " ";
   //             mJunctionHeightInfoString += Ogre::StringConverter::toString(0);
   //             mJunctionHeightInfoString += " ";

   //             // 第一个junction的高度为相对最低高度，其他junction的高度都是相对于此高度的相对高度
   //             minJunctionHeight = manipulator->getTerrainData()->getHeight(junction.x, junction.z);
			//}
   //         // 其他的junction的坐标都是相对于第一个junction的相对坐标
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

			// 直接使用世界坐标，不使用相对坐标
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

            // 第一个grid的相对坐标为0 0
            //if ( it == grids.begin() )
            //{
            //    minGridX = grid.x;
            //    minGridZ = grid.z;

            //    mGridTextureInfoString += Ogre::StringConverter::toString(0);
            //    mGridTextureInfoString += " ";
            //    mGridTextureInfoString += Ogre::StringConverter::toString(0);
            //    mGridTextureInfoString += " ";         
            //}
            //// 其他的grid的坐标都是相对于第一个grid的相对坐标
            //else
            //{
                mGridTextureInfoString += Ogre::StringConverter::toString(grid.x - middleGridX);
                mGridTextureInfoString += " ";
                mGridTextureInfoString += Ogre::StringConverter::toString(grid.z - middleGridZ);
                mGridTextureInfoString += " ";
          //  }

            const WX::TerrainData::GridInfo &gridInfo = grid.info;

			/** 原来是只保存当前格子的纹理的id，现在要保存
			1 纹理名称
			2 纹理的上下左右坐标点
			3 纹理的方向
			*/
            // 获取第一层的信息
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

            // 获取第二层的信息
			id = gridInfo.layers[1].pixmapId;
			// 如果第二层有纹理才拷贝纹理信息
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

            // 第一个grid的相对坐标为0 0
            //if ( it == grids.begin() )
            //{
            //    minGridX = grid.x;
            //    minGridZ = grid.z;

            //    mGridDiagonalInfoString += Ogre::StringConverter::toString(0);
            //    mGridDiagonalInfoString += " ";
            //    mGridDiagonalInfoString += Ogre::StringConverter::toString(0);
            //    mGridDiagonalInfoString += " ";         
            //}
            //// 其他的grid的坐标都是相对于第一个grid的相对坐标
            //else
            //{
                mGridDiagonalInfoString += Ogre::StringConverter::toString(grid.x - middleGridX);
                mGridDiagonalInfoString += " ";
                mGridDiagonalInfoString += Ogre::StringConverter::toString(grid.z - middleGridZ);
                mGridDiagonalInfoString += " ";
         //   }

            const WX::TerrainData::GridInfo &gridInfo = grid.info;

            // 获取第一层的信息
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

        // 根据需要构建要解析的字符串
        if ( mPasteHeightInfo )
            infoString += mJunctionHeightInfoString;
        if ( mPasteDiagonalInfo )
            infoString += mGridDiagonalInfoString;
        if ( mPasteTextureInfo )
            infoString += mGridTextureInfoString;

        // 根据clipboard中的内容来构建一个临时的数据流，所有的解析工作是在这个数据流上做的
        Ogre::MemoryDataStream* stream = new Ogre::MemoryDataStream((void *)(infoString.c_str()),
            infoString.length(), false);

        // 分配了两组selection，加入当前改变的地形，作为notify用
        WX::JunctionSelection* junctionSelection = new WX::JunctionSelection( manipulator->getTerrain(),getSceneManipulator() );
        WX::GridSelection* gridSelection = new WX::GridSelection( manipulator->getTerrain(),getSceneManipulator() );

        // 新建一个operator（集成了height，paint和diagonal三个操作）
        std::auto_ptr<WX::TerrainSelectionActionOperator> op( new WX::TerrainSelectionActionOperator(manipulator) );

        String line;
        // 开始进行数据流的操作 
        std::vector<TerrainInfo> terrainInfo;
        while(!stream->eof())
        {
            line = stream->getLine();

            // junction height
            if ( line.substr(0,8) == "junction" && mPasteHeightInfo )
            {
				mCurrentMousePos = getSceneManipulator()->_alignPosition(mCurrentMousePos,1.0f);
                std::pair<int, int> mouseJunction = pTerrainData->getJunction(mCurrentMousePos.x, mCurrentMousePos.z);

                // 排除 { 这一行
                stream->skipLine();

                String valueLine;
                Ogre::StringVector vecparams;

                valueLine = stream->getLine();

                while ( valueLine != "}" )
                {
                    vecparams = Ogre::StringUtil::split(valueLine);

                    assert ( vecparams.size() == 3 );            

                    // 解析下一行
                    valueLine = stream->getLine();

                    // 获取当前要操作的junction的绝对坐标
                    int absJunctionX = Ogre::StringConverter::parseInt(vecparams[0]) + mouseJunction.first;
                    int absJunctionZ = Ogre::StringConverter::parseInt(vecparams[1]) + mouseJunction.second;

                    if ( false == pTerrainData->isValidJunction(absJunctionX, absJunctionZ) )
                        continue;

                    // 计算绝对高度（也就是新的高度）
					// 不使用相对高度
					float worldHeight = Ogre::StringConverter::parseReal(vecparams[2]);

                   // float absHeight = Ogre::StringConverter::parseReal(vecparams[2]) +
                     //   pTerrainData->getHeight(absJunctionX, absJunctionZ);

                    // 拷贝前先得到该位置的老的高度
                    float oldHeight = pTerrainData->getHeight(absJunctionX, absJunctionZ);
                    // 设置新的高度

                    TerrainInfo terrInfo;
                    terrInfo.x = absJunctionX;
                    terrInfo.z = absJunctionZ;
                    terrInfo.oldHeight = oldHeight;
                    terrInfo.nowHeight = worldHeight;
                    terrainInfo.push_back(terrInfo);                    

                    pTerrainData->setHeight(absJunctionX, absJunctionZ, worldHeight);

                    // 加入到operator中
                    if (worldHeight != oldHeight)
                    {
                        op->mModifyHeightOperator->add(absJunctionX, absJunctionZ, oldHeight, worldHeight);
                    }

                    // 把当前这一个junction加入到notify的selection中 TODO
                    junctionSelection->add(absJunctionX, absJunctionZ, 1.0);
                }
            } // end of junction
            // texture
            else if ( line.substr(0,12) == "grid_texture" && mPasteTextureInfo )
            {
                std::pair<int, int> mouseGrid = pTerrainData->getGrid(mCurrentMousePos.x, mCurrentMousePos.z);

                // 排除 { 这一行
                stream->skipLine();

                String valueLine;
                Ogre::StringVector vecparams;

                valueLine = stream->getLine();

                while ( valueLine != "}" )
                {
                    vecparams = Ogre::StringUtil::split(valueLine);

                    size_t paraSize = vecparams.size();
					// 一行最少的参数个数是8个，分别是格子的x，y坐标，纹理名称，left，top，right，bottom，orientation
					assert (paraSize>=8);

                    // 解析下一行
                    valueLine = stream->getLine();

                    // 获取当前要操作的grid的绝对坐标
                    int absGridX = Ogre::StringConverter::parseInt(vecparams[0]) + mouseGrid.first;
                    int absGridZ = Ogre::StringConverter::parseInt(vecparams[1]) + mouseGrid.second;

                    if ( false == pTerrainData->isValidGrid(absGridX, absGridZ) )
                        continue;

                    WX::TerrainData::GridInfo gridInfo = pTerrainData->getGridInfo(absGridX, absGridZ);

                    // 得到grid原有的信息
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

					// 如果有第二层纹理的信息，就进行处理
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
						// 把原地形的上层纹理删除
						gridInfo.layers[1].pixmapId = 0;
						gridInfo.layers[1].orientation = 0;
					}

                    pTerrainData->setGridInfo(absGridX, absGridZ, gridInfo);

                    // 获取该grid现在的信息
                    const WX::TerrainData::GridInfo &newGridInfo = pTerrainData->getGridInfo(absGridX, absGridZ);

                    if (newGridInfo != oldGridInfo)
                    {
                        op->mModifyPaintOperator->add(absGridX, absGridZ, oldGridInfo, newGridInfo);
                    }

                    // 如果这个grid已经被加入到notify的selection中了，就不再加入（因为diagonal和texture的grid
                    // 都加入到同一个notify的grid selection）
                    if ( !gridSelection->exist(absGridX,absGridZ) )
                        gridSelection->add(absGridX, absGridZ);
                }
            } // end of texture 
            // diagonal
            else if ( line.substr(0,13) == "grid_diagonal" && mPasteDiagonalInfo )
            {
                std::pair<int, int> mouseGrid = pTerrainData->getGrid(mCurrentMousePos.x, mCurrentMousePos.z);

                // 排除 { 这一行
                stream->skipLine();

                String valueLine;
                Ogre::StringVector vecparams;

                valueLine = stream->getLine();

                while ( valueLine != "}" )
                {
                    vecparams = Ogre::StringUtil::split(valueLine);

                    assert ( vecparams.size() == 3 );            

                    // 解析下一行
                    valueLine = stream->getLine();

                    // 获取当前要操作的grid的绝对坐标
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

        // 加入到operator
        if (!op->empty())
            manipulator->getOperatorManager()->addOperator(op.release());

        // 删除临时数据流
        delete stream;
    }

	void TerrainSelectionAction::beginRotateTerrain()
	{
		// 旋转节点（使地形块指示器进行旋转）
		mTerrainSelectionGrids->rotateSceneNode();

		// 用于处理数据的数据流
		Ogre::MemoryDataStream* stream = NULL;

		String line;
		String tempStr("");

		// 重新格式化数据
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

					//tempStr += vecparams[2];	// 名称
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
					//	tempStr += vecparams[8];	// 名称
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
					// 由于纹理信息不止三项，所以要用一个循环来处理
					for ( size_t i=2; i<vecparams.size()-1; ++i )
					{
						tempStr += vecparams[i];
						tempStr += " ";
					}

					// 旋转所拷贝的地形块时所进行的纹理旋转
					/** 由于现在只保存了l t r b四个数值，所以没办法做到真正的旋转
						下面这种方法是用mRotateCount这个成员来保存旋转地形块的次数，
						依次设置该块纹理的orientation数值，每次进行新的地形块copy时
						这个成员都会被清零。
						这种方法的缺点是，要拷贝的地形块的纹理的初始方向只能是0，也是其他的情况
						因为我们是直接改orientation这个数值的，所以粘贴的结果不正确
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