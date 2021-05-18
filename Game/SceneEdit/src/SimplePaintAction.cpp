#include "SimplePaintAction.h"
#include "SceneManipulator.h"
#include "HitIndicator.h"
#include "TerrainSelections.h"
#include "BrushShape.h"

#include <OgreStringConverter.h>

namespace WX {
    //-----------------------------------------------------------------------
    SimplePaintAction::SimplePaintAction(SceneManipulator* sceneManipulator)
        : PaintAction(sceneManipulator),
        mbTexNormalSize(true)
    {
        mCurrentGrids = new GridSelection(getTerrain(),sceneManipulator);
        mHintModified = new GridSelection(getTerrain(),sceneManipulator);
        mModifiedGrids = new GridSelection(getTerrain(),sceneManipulator);

        mbUseRandomOrientation = false;
    }
    //-----------------------------------------------------------------------
    SimplePaintAction::~SimplePaintAction()
    {
        delete mCurrentGrids;
        delete mHintModified;
        delete mModifiedGrids;
    }
    //-----------------------------------------------------------------------
    const String& SimplePaintAction::getName(void) const
    {
        static const String name = "SimplePaintAction";
        return name;
    }
    //-----------------------------------------------------------------------
	void SimplePaintAction::setParameter(const String& name, const String& value)
	{
        /** 增强了SimplePaintAction的功能，不仅可以用单张纹理进行非自动拼接的paint，也可以
            用一组纹理（同个文件夹下）来做为画刷
            如果只选择一张纹理，就可以调整纹理的方向，如果选择一组纹理，纹理的方向随机产生
            现在的设置action的顺序为
            GetSceneManipulator()->_getAction("SimplePaintAction");

            action->setParameter( "%TextureName", "InputBegin" );
            action->setParameter( "%TextureName", 纹理名称 );

            如有需要，设置方向
            action->setParameter( "%OrientationType"，。。。)

        */
		if (Ogre::StringUtil::startsWith(name, "%", false))
		{
			Ogre::String parameter = name.substr(1);

			if ( parameter == "TextureName" )
            {
                if ( value == "InputBegin" )
                {
                    mPixmapArray.clear();
                }
                else
                {
                    /* 解析纹理信息字符串
                        0 -- 纹理名称
                        1 -- height
                        2 -- width 
                        3 -- left 
                        4 -- top
                        5 -- 源纹理的高度
                        6 -- 源纹理的宽度
						7 -- 纹理的类型
						8 -- 纹理的方向

                        中间用"|"隔开
                    */

                    Ogre::StringVector infoVector = Ogre::StringUtil::split(value, "|");

                    assert (infoVector.size() == 8);

                    PaintPixmap pixmap(infoVector[0]);
                    
                    // 计算出纹理坐标
                    uint height = Ogre::StringConverter::parseUnsignedInt(infoVector[1]);
                    uint width = Ogre::StringConverter::parseUnsignedInt(infoVector[2]);

                    mXBrushSize = width;
                    mZBrushSize = height;
                   
                    uint leftcorner = Ogre::StringConverter::parseUnsignedInt(infoVector[3]);
                    uint topcorner = Ogre::StringConverter::parseUnsignedInt(infoVector[4]);

                    uint ownerTexHeight = Ogre::StringConverter::parseUnsignedInt(infoVector[5]);
                    uint ownerTexWidht = Ogre::StringConverter::parseUnsignedInt(infoVector[6]);

                    pixmap.left = (float)(TexTileSize*leftcorner) / (float)ownerTexWidht;
                    pixmap.top = (float)(TexTileSize*topcorner) / (float)ownerTexHeight;
                    pixmap.bottom = (float)(TexTileSize*(topcorner+height)) / (float)ownerTexHeight;
                    pixmap.right = (float)(TexTileSize*(leftcorner+width)) / (float)ownerTexWidht;		

					uint rotateType = Ogre::StringConverter::parseUnsignedInt(infoVector[7]);

					enum 
					{
						Left,
						Top,
						Right,
						Bottom,
					};

					enum
					{
						LeftTop,
						RightBottom = 3,
					};

					// 一组索引表，可以用于方便地找出旋转方向与纹理坐标的对应关系
					static const int cornerIndexMap[4][4][2] =
					{
						Left,	Top,
						Right,	Top,
						Left,	Bottom,
						Right,	Bottom,

						Right,	Top,
						Left,	Top,
						Right,	Bottom,
						Left,	Bottom,

						Left,	Bottom,
						Right,	Bottom,
						Left,	Top,
						Right,	Top,

						Right,	Bottom,
						Left,	Bottom,
						Right,	Top,
						Left,	Top,		
					};

					float texCoord[4] = {	pixmap.left, pixmap.top, pixmap.right, pixmap.bottom	};

					// 最后一个索引项，0表示纹理u坐标，1表示纹理v坐标
					pixmap.left = texCoord[ cornerIndexMap[ rotateType ][ LeftTop ][0] ];	
					pixmap.top = texCoord[ cornerIndexMap[ rotateType ][ LeftTop ][1] ];
					pixmap.right = texCoord[ cornerIndexMap[ rotateType ][ RightBottom ][0] ];
					pixmap.bottom = texCoord[ cornerIndexMap[ rotateType ][ RightBottom ][1] ];

                    mPixmapArray.push_back(pixmap);
                }
            }
            else if ( parameter == "TexNormalSize" )
            {
                if ( value == "true" )
                    mbTexNormalSize = true;
                else if ( value == "false" )
                    mbTexNormalSize = false;
            }          
			else if ( parameter == "OrientationType" )
			{
				setPixmapOrientationType( atoi(value.c_str()) );
			}
            else if ( parameter == "UseRandomOrientation" )
            {
                if ( value == "true" )
                    mbUseRandomOrientation = true;
                else if ( value == "false" )
                    mbUseRandomOrientation = false;
            }
		}
		else
		{
			Action::setParameter(name, value);
		}
	}
    //-----------------------------------------------------------------------
	String SimplePaintAction::getParameter(const String& name) const
	{
        if (Ogre::StringUtil::startsWith(name, "%", false))
        {
            Ogre::String parameter = name.substr(1);

            if ( parameter == "IsInit" )
            {
                if ( mPixmapArray.size() > 0 )
                {
                    return "Yes";
                }                     
                else
                    return "No";
            }
             else
                return Action::getParameter(name);
        }
        else
            return Action::getParameter(name);
	}  
    //-----------------------------------------------------------------------
    void SimplePaintAction::_buildHitIndicator(const Point& pt)
    {
        Ogre::Vector3 position;
        bool intersected = getSceneManipulator()->getTerrainIntersects(pt, position);
        if (!intersected)
        {
            mCurrentGrids->reset();
            return;
        }

        // 如果当前所选用的纹理的大小超过了64，那么要根据纹理的大小和画刷的大小来构建选择区域
        if (!mbTexNormalSize)
        {
            int oldBrushXSize = getSceneManipulator()->getBrushXSize();
            int oldBrushZSize = getSceneManipulator()->getBrushZSize();

            // 临时设置画刷大小
            getSceneManipulator()->setBrushXSize(oldBrushXSize*mXBrushSize);
            getSceneManipulator()->setBrushZSize(oldBrushZSize*mZBrushSize);
            getSceneManipulator()->getHitIndicator("IntersectGrids")->setHitPoint(pt);
            getSceneManipulator()->_buildSelection(mCurrentGrids, position.x, position.z);

            // 构建完区域后又设置回去
            getSceneManipulator()->setBrushXSize(oldBrushXSize);
            getSceneManipulator()->setBrushZSize(oldBrushZSize);
        }
        else
        {
            getSceneManipulator()->getHitIndicator("IntersectGrids")->setHitPoint(pt);
            getSceneManipulator()->_buildSelection(mCurrentGrids, position.x, position.z);
        }

        getSceneManipulator()->getHitIndicator("IntersectPoint")->setHitPoint(pt);
    }
    //-----------------------------------------------------------------------
    void SimplePaintAction::_doPaint(GridSelection* modified)
    {
        // 如果当前所保存的pixmap为0，直接返回
        if (mPixmapArray.empty())
        {
            return;
        }

        int texCount = mPixmapArray.size();

        int pixmapId = 0;
        int orientataionType = getPixmapOrientationType();
        int orientation = 0;
        String mCurrentTexName;

        const GridSelection::GridMap& grids = mCurrentGrids->getGrids();

        if (!grids.empty())
        {
            // 如果只选择一张纹理，或一个文件夹下只有一张纹理
            if ( texCount == 1 )
            {
                // 如果当前的纹理为普通大小，就马上设置纹理
                if ( mbTexNormalSize == true )
                {
                    setPixmap(mPixmapArray[0].textureName, mPixmapArray[0].left, mPixmapArray[0].top,
                        mPixmapArray[0].right, mPixmapArray[0].bottom);
                    pixmapId = getPixmapId();

                }
                // 否则，只得到纹理名称，在循环中设置纹理
                else
                {
                    mCurrentTexName = mPixmapArray[0].textureName;

                    orientation = 0;
                }
            }
          
            // 如果多于一张纹理，在这里也是随机取得一张纹理的名称
            if ( texCount > 1 && mbTexNormalSize == false )
            {
				int index = static_cast<int>(Ogre::Math::RangeRandom(0,texCount));
				if (index == texCount)
				{
					index = 0;
				}
				assert (index < texCount);
                mCurrentTexName = mPixmapArray[ index ].textureName;
            }

			bool anyModified = false;

            // 获取左上角的坐标
            const GridSelection::Grid &firstGrid = grids.begin()->second;

            int minx = firstGrid.x;
            int minz = firstGrid.z;

            // 计算格之间的纹理坐标增量
            float xInc = (mPixmapArray[0].right- mPixmapArray[0].left) / mXBrushSize;
            float zInc = (mPixmapArray[0].bottom - mPixmapArray[0].top) / mZBrushSize;

            for (GridSelection::GridMap::const_iterator it = grids.begin(); it != grids.end(); ++it)
            {
                // 如果纹理个数大于1，就使用随机纹理，不使用随机方向
                if ( texCount > 1 )
                {
                    if ( mbTexNormalSize )
                    {
                  //      String texName;

                        int index = 0;
                        if ( modified != mHintModified )
                        {
                            index = static_cast<int>(Ogre::Math::RangeRandom(0,texCount));
							if (index == texCount)
							{
								index = 0;
							}
							assert (index < texCount);
                        //    texName = mPixmapArray[index].textureName;
                           // orientation = static_cast<int>(Ogre::Math::RangeRandom(0,7));
                            orientation = 0;
                        }
                        /*else
                        {
                            texName = mPixmapArray[0].textureName;
                        }*/

                        // 用这个id来索引pixmap
                        setPixmap(mPixmapArray[index].textureName, mPixmapArray[index].left, mPixmapArray[index].top,
                            mPixmapArray[index].right, mPixmapArray[index].bottom);
                        pixmapId = getPixmapId(); 


                    }
                }
                else
                {
                    if ( mbTexNormalSize )
                    {
                        if ( mbUseRandomOrientation )
                        {
                            if (modified != mHintModified)
							{
								const int oriTypeCount = 8;
                                orientation = static_cast<int>(Ogre::Math::RangeRandom(0,oriTypeCount));
								if (orientation == oriTypeCount)
								{
									orientation = 0;
								}
								assert ( orientation < oriTypeCount);
							}
                            else
                                orientation = 0; 

                        }
                        else
                            orientation = orientataionType;
                    }
                }
                

                const GridSelection::Grid& grid = it->second;

                if ( mbTexNormalSize == false )
                {
                    int x = grid.x;
                    int z = grid.z;

                    float left = (x-minx)%mXBrushSize*xInc;
                    float top = (z-minz)%mZBrushSize*zInc;
                   
                    setPixmap( mPixmapArray[0].textureName, mPixmapArray[0].left + left, mPixmapArray[0].top + top,
                        mPixmapArray[0].left + left + xInc, mPixmapArray[0].top + top + zInc );
                    pixmapId = getPixmapId();     

                }

                TerrainData::GridInfo info = grid.info;

                const bool isTransparent = getPixmapTransparent();

                if (isTransparent)
                {
                    if (!info.layers[0].pixmapId)
                        continue;

					/** 这里有个隐藏的错误
						没判断当前格的纹理与所用的透明纹理是否属于同一组的
						如果同一组的，就不应该涂上该透明纹理，因为从画面上可能看不到效果，
						而且会造成效率上的一点点问题，因为会用双层纹理渲染的方法来渲染该格纹理
						这里暂时先用纹理名称来进行判断，判断是否为同组纹理
					*/
					TerrainData *terrainData = getTerrainData();
					assert (terrainData);

					// 比较上下层的画刷名称，如果一样，说明是同一组画刷
					String layerOneTexName = terrainData->mTextures[ terrainData->mPixmaps[info.layers[0].pixmapId-1].textureId ];
					assert ( !layerOneTexName.empty() );

					String layerOneBrushName = 
						mSceneManipulator->getTerrainPaintInfoContainer()->findBrushNameByTexName(layerOneTexName);				

					String layerTwoTexName = terrainData->mTextures[ terrainData->mPixmaps[pixmapId-1].textureId ];
					assert ( !layerTwoTexName.empty() );
						
					String layerTwoBrushName = 
						mSceneManipulator->getTerrainPaintInfoContainer()->findBrushNameByTexName(layerTwoTexName);

					if ( layerOneBrushName == layerTwoBrushName )
						continue;

                    info.layers[1].pixmapId = pixmapId;

                    info.layers[1].orientation = orientation;
                }
                else
                {
                    info.layers[0].pixmapId = pixmapId;
                    info.layers[0].orientation = orientation;
                    /*if (info.layers[1].pixmapId > 0)
                    {*/
                        info.layers[1].pixmapId = 0;
                        info.layers[1].orientation = 0;
                 //  }
                }

                if (info == grid.info)
                    continue;

                anyModified = true;
                modified->add(grid.x, grid.z);
                getTerrainData()->setGridInfo(grid.x, grid.z, info);
            }

            if (anyModified)
            {
                mCurrentGrids->notifyModified();
            }
        }

    }
    //-----------------------------------------------------------------------
    void SimplePaintAction::_onActive(bool active)
    {
        if (!active)
        {
            mHintModified->apply();
            mHintModified->reset();
        }
    }
    //-----------------------------------------------------------------------
    void SimplePaintAction::_onMove(const Point& pt)
    {
        mHintModified->apply();
        mHintModified->reset();
        _buildHitIndicator(pt);
        _doPaint(mHintModified);
    }
    //-----------------------------------------------------------------------
    void SimplePaintAction::_onBegin(const Point& pt)
    {
        mHintModified->apply();
        mHintModified->reset();
        mModifiedGrids->reset();
        _buildHitIndicator(pt);
        _doPaint(mModifiedGrids);
    }
    //-----------------------------------------------------------------------
    void SimplePaintAction::_onDrag(const Point& pt)
    {
        _buildHitIndicator(pt);
        _doPaint(mModifiedGrids);
    }
    //-----------------------------------------------------------------------
    void SimplePaintAction::_onEnd(const Point& pt, bool canceled)
    {
        /*if (canceled)
        {
            mModifiedGrids->apply();
        }*/
        doFinish(mModifiedGrids,canceled);
        mModifiedGrids->reset();
        _buildHitIndicator(pt);
    }
}
