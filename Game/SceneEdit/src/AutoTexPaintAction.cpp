#include "AutoTexPaintAction.h"
#include "SceneManipulator.h"
#include "HitIndicator.h"
#include "TerrainSelections.h"

#include <OgreStringConverter.h>
#include <OgreLogManager.h>
#include <OgreImage.h>

#include "TerrainPaintInfoContainer.h"

namespace WX {

    //-----------------------------------------------------------------------
    AutoTexPaintAction::AutoTexPaintAction(SceneManipulator* sceneManipulator)
        : PaintAction(sceneManipulator)
        , mCurrentBrush(NULL),mInitCurrentBrush(false),
        mCurrentInnerArray(NULL), 

		mCurrentInnerCornerArray(NULL), 
		mCurrentLTInnerCornerArray(NULL), 
		mCurrentRTInnerCornerArray(NULL), 
		mCurrentLBInnerCornerArray(NULL), 
		mCurrentRBInnerCornerArray(NULL), 

		mCurrentOuterCornerArray(NULL),
		mCurrentLTOuterCornerArray(NULL),
		mCurrentRTOuterCornerArray(NULL),
		mCurrentLBOuterCornerArray(NULL),
		mCurrentRBOuterCornerArray(NULL),

        mCurrentEdgeArray(NULL), 
		mCurrentLEdgeArray(NULL), 
		mCurrentREdgeArray(NULL), 
		mCurrentTEdgeArray(NULL), 
		mCurrentBEdgeArray(NULL), 

		mCurrentDuijiaoArray(NULL)
    {
        mCurrentInnerGrids = new GridSelection(getTerrain(),sceneManipulator);
        mCurrentOuterGrids = new GridSelection(getTerrain(),sceneManipulator);

        mHintModified = new GridSelection(getTerrain(),sceneManipulator);
        mModifiedGrids = new GridSelection(getTerrain(),sceneManipulator);
    }
    //-----------------------------------------------------------------------
    AutoTexPaintAction::~AutoTexPaintAction()
    {
        if ( mCurrentInnerGrids)
        {
            delete mCurrentInnerGrids;
            mCurrentInnerGrids = NULL;
        }

        if ( mCurrentOuterGrids)
        {
            delete mCurrentOuterGrids;
            mCurrentOuterGrids = NULL;
        }

        if ( mHintModified)
        {
            delete mHintModified;
            mHintModified = NULL;
        }

        if ( mModifiedGrids)
        {
            delete mModifiedGrids;
            mModifiedGrids = NULL;
        }

        for ( BrushMap::iterator i=mBrushMap.begin(); i!=mBrushMap.end(); ++i )
        {
            if ( i->second )
            {
                delete i->second;
                i->second = NULL;
            }
        }
        mBrushMap.clear();
    }
    //-----------------------------------------------------------------------
    const String& AutoTexPaintAction::getName(void) const
    {
        static const String name = "AutoTexPaintAction";
        return name;
    }

    void AutoTexPaintAction::setParameter(const String& name, const String& value)
    {
        if (Ogre::StringUtil::startsWith(name, "%", false))
        {
            Ogre::String parameter = name.substr(1);

            if ( parameter == "GroupName" )
            {
                mCurrentBrushName = value;

                // 如果BrushMap中没有这个group（可能是因为之前还没被用户选过），就新建一个，插入到map中
                if ( mBrushMap.find(value) == mBrushMap.end() )
                {
                    mInitCurrentBrush = false;

                    mCurrentBrush = new Brush;
                    mBrushMap.insert( BrushMap::value_type( value, mCurrentBrush ) );    
                }
                else 
                {
                    // 如果有的话，直接设置当前group为指定group
                    mCurrentBrush = mBrushMap.find(value)->second;
                }
         
                // 获取当前组的五个纹理数组的指针
                mCurrentInnerCornerArray = &(mCurrentBrush->innerCornerTexArray);
				mCurrentLTInnerCornerArray = &(mCurrentBrush->ltInnerCornerTexArray);
				mCurrentRTInnerCornerArray = &(mCurrentBrush->rtInnerCornerTexArray);
				mCurrentLBInnerCornerArray = &(mCurrentBrush->lbInnerCornerTexArray);
				mCurrentRBInnerCornerArray = &(mCurrentBrush->rbInnerCornerTexArray);

                mCurrentOuterCornerArray = &(mCurrentBrush->outerCornerTexArray);
				mCurrentLTOuterCornerArray = &(mCurrentBrush->ltOuterCornerTexArray);
				mCurrentRTOuterCornerArray = &(mCurrentBrush->rtOuterCornerTexArray);
				mCurrentLBOuterCornerArray = &(mCurrentBrush->lbOuterCornerTexArray);
				mCurrentRBOuterCornerArray = &(mCurrentBrush->rbOuterCornerTexArray);

                mCurrentInnerArray       = &(mCurrentBrush->innerTexArray);

                mCurrentEdgeArray        = &(mCurrentBrush->edgeTexArray);
				mCurrentLEdgeArray        = &(mCurrentBrush->lEdgeTexArray);
				mCurrentREdgeArray        = &(mCurrentBrush->rEdgeTexArray);
				mCurrentTEdgeArray        = &(mCurrentBrush->tEdgeTexArray);
				mCurrentBEdgeArray        = &(mCurrentBrush->bEdgeTexArray);

                mCurrentDuijiaoArray     = &(mCurrentBrush->duijiaoTexArray);

            }
            else if ( parameter == "TextureName" )
            {
                // 如果这一个group中还没初始化完成，就根据文件名来判断纹理所属的组
                if ( mInitCurrentBrush == false )
                {

                    if ( value == "InputFinish" )
                    {
                        // 如果有一个纹理类型数组中的大小为0，说明缺少某种纹理类型的纹理
                        if ( ( mCurrentBrush->innerCornerTexArray.size() == 0 &&
							mCurrentBrush->ltInnerCornerTexArray.size() == 0 &&
							mCurrentBrush->rtInnerCornerTexArray.size() == 0 &&
							mCurrentBrush->lbInnerCornerTexArray.size() == 0 &&
							mCurrentBrush->rbInnerCornerTexArray.size() == 0 ) ||

                          ( mCurrentBrush->outerCornerTexArray.size() == 0 &&
							mCurrentBrush->ltOuterCornerTexArray.size() == 0 &&
							mCurrentBrush->rtOuterCornerTexArray.size() == 0 &&
							mCurrentBrush->lbOuterCornerTexArray.size() == 0 &&
							mCurrentBrush->rbOuterCornerTexArray.size() == 0 ) ||

                            mCurrentBrush->innerTexArray.size() == 0 ||

                            ( mCurrentBrush->edgeTexArray.size() == 0 &&
							mCurrentBrush->lEdgeTexArray.size() == 0 &&
							mCurrentBrush->rEdgeTexArray.size() == 0 &&
							mCurrentBrush->tEdgeTexArray.size() == 0 &&
							mCurrentBrush->bEdgeTexArray.size() == 0 ) ||

                            mCurrentBrush->duijiaoTexArray.size() == 0 )
                        {
                            // 把刚建好的brush组删除了
                            if ( mCurrentBrush )
                            {
                                delete mCurrentBrush;
                                mCurrentBrush = NULL;
                            }

                            BrushMap::iterator i = mBrushMap.find(mCurrentBrushName);

                            if ( i != mBrushMap.end() )
                                mBrushMap.erase(i);

                            mInitCurrentBrush = false;

							mCurrentInnerCornerArray = NULL;
							mCurrentLTInnerCornerArray = NULL;
							mCurrentRTInnerCornerArray = NULL;
							mCurrentLBInnerCornerArray = NULL;
							mCurrentRBInnerCornerArray = NULL;

							mCurrentOuterCornerArray = NULL;
							mCurrentLTOuterCornerArray = NULL;
							mCurrentRTOuterCornerArray = NULL;
							mCurrentLBOuterCornerArray = NULL;
							mCurrentRBOuterCornerArray = NULL;

							mCurrentInnerArray       = NULL;

							mCurrentEdgeArray        = NULL;
							mCurrentLEdgeArray		 = NULL; 
							mCurrentREdgeArray		 = NULL; 
							mCurrentTEdgeArray		 = NULL; 
							mCurrentBEdgeArray		 = NULL; 

							mCurrentDuijiaoArray     = NULL;
                        }
                        else
                            mInitCurrentBrush = true;
                    }
                    else
                    {
						assert ( mCurrentBrush == mBrushMap.find(mCurrentBrushName)->second );
                        const WX::TextureInfo &currentPaintInfo = getSceneManipulator()->getTextureInfo(mCurrentBrushName, value);

                        PaintPixmap pixmap(currentPaintInfo.ownerTextureName);

                        Ogre::Image *ownerImage = getSceneManipulator()->getPreviewImage(currentPaintInfo.ownerTextureName);

                        assert (ownerImage);

                        // 计算出纹理坐标               
                        uint ownerTexHeight = ownerImage->getHeight();
                        uint ownerTexWidht = ownerImage->getWidth();

						float texCoordIncX = 0.5f / (float)ownerTexWidht;
						float texCoordIncZ = 0.5f / (float)ownerTexHeight;

						pixmap.left = (float)(TexTileSize*currentPaintInfo.leftCorner) / (float)ownerTexWidht + texCoordIncX;
						pixmap.top = (float)(TexTileSize*currentPaintInfo.topCorner) / (float)ownerTexHeight + texCoordIncZ;
						pixmap.bottom = (float)(TexTileSize*(currentPaintInfo.topCorner + currentPaintInfo.height)) / (float)ownerTexHeight
							- texCoordIncZ;
						pixmap.right = (float)(TexTileSize*(currentPaintInfo.leftCorner + currentPaintInfo.width)) / (float)ownerTexWidht
							- texCoordIncX;

						assert ( (pixmap.left < pixmap.right) && (pixmap.top < pixmap.bottom) );

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

						// 一组索引表，可以用于方便地找出旋转方向与纹理坐标的对应关系（）
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
						pixmap.left = texCoord[ cornerIndexMap[ currentPaintInfo.rotateType ][ LeftTop ][0] ];	
						pixmap.top = texCoord[ cornerIndexMap[ currentPaintInfo.rotateType ][ LeftTop ][1] ];
						pixmap.right = texCoord[ cornerIndexMap[ currentPaintInfo.rotateType ][ RightBottom ][0] ];
						pixmap.bottom = texCoord[ cornerIndexMap[ currentPaintInfo.rotateType ][ RightBottom ][1] ];	

                        assert ( pixmap.left >= 0.0f && pixmap.left <= 1.0f );
                        assert ( pixmap.top >= 0.0f && pixmap.top <= 1.0f );
                        assert ( pixmap.right >= 0.0f && pixmap.right <= 1.0f );
                        assert ( pixmap.bottom >= 0.0f && pixmap.bottom <= 1.0f );

                        if ( currentPaintInfo.textureType == EdgeTex )
                        {
							if ( currentPaintInfo.extraTexType == EOther )
								mCurrentBrush->edgeTexArray.push_back(pixmap);
							else if ( currentPaintInfo.extraTexType == LE )
								mCurrentBrush->lEdgeTexArray.push_back(pixmap);
							else if ( currentPaintInfo.extraTexType == RE )
								mCurrentBrush->rEdgeTexArray.push_back(pixmap);
							else if ( currentPaintInfo.extraTexType == TE )
								mCurrentBrush->tEdgeTexArray.push_back(pixmap);
							else if ( currentPaintInfo.extraTexType == BE )
								mCurrentBrush->bEdgeTexArray.push_back(pixmap);

                            mInitCurrentBrush = false;
                        }						
                        else if ( currentPaintInfo.textureType == InnerCornerTex )
                        {
							if ( currentPaintInfo.extraTexType == EOther )
								mCurrentBrush->innerCornerTexArray.push_back(pixmap);
							else if ( currentPaintInfo.extraTexType == LT )
								mCurrentBrush->ltInnerCornerTexArray.push_back(pixmap);
							else if ( currentPaintInfo.extraTexType == RT )
								mCurrentBrush->rtInnerCornerTexArray.push_back(pixmap);
							else if ( currentPaintInfo.extraTexType == LB )
								mCurrentBrush->lbInnerCornerTexArray.push_back(pixmap);
							else if ( currentPaintInfo.extraTexType == RB )
								mCurrentBrush->rbInnerCornerTexArray.push_back(pixmap);

                            mInitCurrentBrush = false;
                        }						
                        else if ( currentPaintInfo.textureType == OuterCornerTex )
                        {
							if ( currentPaintInfo.extraTexType == EOther )
								mCurrentBrush->outerCornerTexArray.push_back(pixmap);
							else if ( currentPaintInfo.extraTexType == LT )
								mCurrentBrush->ltOuterCornerTexArray.push_back(pixmap);
							else if ( currentPaintInfo.extraTexType == RT )
								mCurrentBrush->rtOuterCornerTexArray.push_back(pixmap);
							else if ( currentPaintInfo.extraTexType == LB )
								mCurrentBrush->lbOuterCornerTexArray.push_back(pixmap);
							else if ( currentPaintInfo.extraTexType == RB )
								mCurrentBrush->rbOuterCornerTexArray.push_back(pixmap);

                            mInitCurrentBrush = false;
                        }						
                        else if ( currentPaintInfo.textureType == DuijiaoTex )
                        {
                            mCurrentBrush->duijiaoTexArray.push_back(pixmap);
                            mInitCurrentBrush = false;
                        }                    
                        else if ( currentPaintInfo.textureType == InnerTex )
                        {
                            // 填充内部纹理数组
                            mCurrentBrush->innerTexArray.push_back(pixmap);
                            mInitCurrentBrush = false;
                        }
						else
						{
							OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS,
								"The texture type is wrong " + currentPaintInfo.brushName + " " + currentPaintInfo.ownerTextureName,
								"AutoTexPaintAction::setParameter");
						}
                    }                    
                }
            }
            
        }
        else
        {
            Action::setParameter(name, value);
        }
    }
    //-----------------------------------------------------------------------
    String AutoTexPaintAction::getParameter(const String& name) const
    {

        if (Ogre::StringUtil::startsWith(name, "%", false))
        {
            Ogre::String parameter = name.substr(1);

            if ( parameter == "IsInit" )
            {
                if ( mInitCurrentBrush )
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
    void AutoTexPaintAction::buildBoxEdgeSelection(GridSelection& selection, Real xcentre, Real zcentre, int xsize, int zsize)
    {
        assert(0 < xsize && 0 < zsize);

        selection.reset();
        Real cx = (xsize-1) * selection.getTerrainData()->mScale.x / 2;
        Real cz = (zsize-1) * selection.getTerrainData()->mScale.z / 2;
        Real xbais = xcentre - cx;
        Real zbais = zcentre - cz;

        // 得到实心区域的左上角的坐标（不管有没有超过边缘，如果左上角超过了边缘，那么这个值就会是负的）
        std::pair<int, int> bais = selection.getTerrainData()->getGrid(xbais, zbais);
        for (int z = -1; z < zsize+1; ++z)
        {
            for (int x = -1; x < xsize+1; ++x)
            {
                if ( z==-1 || z==zsize || x==-1 || x==xsize )
                    selection.add(x+bais.first, z+bais.second);
            }
        }  

        // 得到当前所选的box区域的左上角（不管有没有超过边缘，如果左上角超过了边缘，那么这个值就会是负的）
        mCurrentBoxSelectionMinX = bais.first-1;
        mCurrentBoxSelectionMinZ = bais.second-1;
    }
    //-----------------------------------------------------------------------
    void AutoTexPaintAction::_buildHitIndicator(const Point& pt)
    {
        getSceneManipulator()->getHitIndicator("IntersectPoint")->setHitPoint(pt);
        getSceneManipulator()->getHitIndicator("IntersectGrids")->setHitPoint(pt);

        Ogre::Vector3 position;
        bool intersected = getSceneManipulator()->getTerrainIntersects(pt, position);
        if (!intersected)
        {
            mCurrentInnerGrids->reset();
            mCurrentOuterGrids->reset();
            //hide();
            return;
        }

        // 选取不同的画刷，构建不同的选择区域
        getSceneManipulator()->_buildSelection(mCurrentInnerGrids, position.x, position.z);

        buildBoxEdgeSelection(*mCurrentOuterGrids,position.x,position.z,getSceneManipulator()->getBrushXSize(), getSceneManipulator()->getBrushZSize());
        //refresh();
    }
    //-----------------------------------------------------------------------
    void AutoTexPaintAction::_doPaint(GridSelection* modified)
    {
        // 如果所选的纹理组初始化未完成，不填充纹理
        if ( mInitCurrentBrush == false )
            return;

        bool anyModified = false;
   
        int pixmapId = 0;
        int orientationType = 0;

        int innercornerSize = mCurrentInnerCornerArray->size();
		int ltInnerCornerSize = mCurrentLTInnerCornerArray->size();
		int rtInnerCornerSize = mCurrentRTInnerCornerArray->size();
		int lbInnerCornerSize = mCurrentLBInnerCornerArray->size();
		int rbInnerCornerSize = mCurrentRBInnerCornerArray->size();

        int outercornerSize = mCurrentOuterCornerArray->size();
		int ltOutercornerSize = mCurrentLTOuterCornerArray->size();
		int rtOutercornerSize = mCurrentRTOuterCornerArray->size();
		int lbOutercornerSize = mCurrentLBOuterCornerArray->size();
		int rbOutercornerSize = mCurrentRBOuterCornerArray->size();

        int innerSize = mCurrentInnerArray->size();

        int edgeSize = mCurrentEdgeArray->size();
		int lEdgeSize = mCurrentLEdgeArray->size();
		int rEdgeSize = mCurrentREdgeArray->size();
		int tEdgeSize = mCurrentTEdgeArray->size();
		int bEdgeSize = mCurrentBEdgeArray->size();

        int duijiaoSize = mCurrentDuijiaoArray->size();

        // FIX IT 以下的纹理自动拼接的代码都是硬生生用if判断来写的，以上可能会改成用
        // 一个拼接关系索引表的处理

		// added by JiChong( 本来不想多嘴了, 实在看着难受, sorry:) )
		// 无须使用拼接关系索引表，请注意魔兽三里的地形贴图，其摆放方式为
		// 11 00 00 00 11 11 11 11     3  0  0  0
		// 11 01 10 11 11 11 11 11     3  1  2  3

		// 01 01 01 01 11 11 11 11     1  1  1  1
		// 00 01 10 11 11 11 11 11     0  1  2  3

		// 10 10 10 10 11 11 11 11     2  2  2  2
		// 00 01 10 11 11 11 11 11     0  1  2  3

		// 11 11 11 11 11 11 11 11     3  3  3  3
		// 00 01 10 11 11 11 11 11     0  1  2  3

		// 注意边角处贴图小块的编码正好与该小块在大图中的位置一致(上行为y坐标，下行为x坐标)
		// 接下来只要扫描每个tile中用到的贴图，根据某种地形在四个角的占用情况(占用为1, 不占用为0),即可得到正确
		// 的编码，然后以该编码找到相应贴图中的小块,当然按我们的引擎的情况，也可以以编码来命名贴图文件
		// ps: 看到太多地方用这种生写if的方式来做自动拼接，bug多且没有办法处理复杂的情况，其实自动拼接的代码是非常简单
		// 最后一点，因为四角都为0的贴图是没有用的，所以魔兽III里的第一张小贴图，也就是本来是0000的贴图，用的是1111
		// 估计是用来做预览图，或者是参与到1111图的随机上的(后者的可能性相对要小些)

        // 处理内部纹理
        const GridSelection::GridMap& innerGrids = mCurrentInnerGrids->getGrids();

        if (!innerGrids.empty())
        {
                    
            for (GridSelection::GridMap::const_iterator it = innerGrids.begin(); it != innerGrids.end(); ++it)
            {               
                // 当鼠标按下时才随机选择纹理
                PaintPixmap pixmap;  

				assert ( false == mCurrentInnerArray->empty() );
                if ( modified != mHintModified )
                {
					int index = static_cast<int>(Ogre::Math::RangeRandom(0,innerSize));
					if (index == innerSize)
					{
						index = 0;
					}
					assert (index < innerSize);
                    pixmap = (*mCurrentInnerArray)[ index ];
                }
                else
                {
                    pixmap = (*mCurrentInnerArray)[0];
                }

               // 用这个id来索引pixmap
                setPixmap(pixmap);
                pixmapId = getPixmapId();

                // 获取到当前要设置纹理的这一个小格的信息
                const GridSelection::Grid &grid = it->second;
                TerrainData::GridInfo info = grid.info;
               
                info.layers[0].pixmapId = pixmapId;           

                // 实心地形美术要求不用旋转，用原方向
                info.layers[0].orientation = 0;            

                info.layers[1].pixmapId = 0;
                info.layers[1].orientation = 0;
           
                if (info == grid.info)
                    continue;

                anyModified = true;
                modified->add(grid.x, grid.z);
                getTerrainData()->setGridInfo(grid.x, grid.z, info);
            }
        }

        const GridSelection::GridMap &outerGrids = mCurrentOuterGrids->getGrids();

        if ( !outerGrids.empty() )
        { 
            TexType type = Other;

            // 计算出当前selection的右下角的坐标
            int maxx = mCurrentBoxSelectionMinX + getSceneManipulator()->getBrushXSize()+1;
            int maxz = mCurrentBoxSelectionMinZ + getSceneManipulator()->getBrushZSize()+1;

            for (GridSelection::GridMap::const_iterator it = outerGrids.begin(); it != outerGrids.end(); ++it)
            {     
                const GridSelection::Grid &grid = it->second;
                TerrainData::GridInfo info = grid.info;

                int x = grid.x;
                int z = grid.z;

                // 如果第0层的纹理为本组的（肯定为内部纹理），就跳过
                if ( isTexInCurrentGroup( info.layers[0].pixmapId ) )
                    continue;
     
                // 获取当前格第1层的纹理类型
                TexType currentTexType = findTexTypeByPixmapId(info.layers[1].pixmapId);

                // 处理左上角
                if ( x == mCurrentBoxSelectionMinX && z == mCurrentBoxSelectionMinZ )
                {
                    if ( currentTexType == EdgeTex )
                    {
                        // 如果遇上右边缘
                        if ( info.layers[1].orientation == 1 || info.layers[1].orientation == 3 )
                        {
                            type = InnerCornerTex;
                            orientationType = 1;
                        }
                        // 如果遇上下边缘
                        else if ( info.layers[1].orientation == 4 || info.layers[1].orientation == 6 )
                        {
                            type = InnerCornerTex;
                            orientationType = 2;
                        }
                        // 如果遇上左边缘
                        else if ( info.layers[1].orientation == 0 || info.layers[1].orientation == 2 )
                        {
                            continue;
                        }
                        // 如果遇上上边缘
                        else if ( info.layers[1].orientation == 5 || info.layers[1].orientation == 7 )
                        {
                            continue;
                        }
                    }
                    else if ( currentTexType == OuterCornerTex )
                    {
                        // 如果碰上右上角，就变成上边缘
                        if ( info.layers[1].orientation == 1 || info.layers[1].orientation == 7 )
                        {
                            type = EdgeTex;
                            orientationType = 5;
                        }
                        // 如果碰上左下角，变成左边缘
                        else if ( info.layers[1].orientation == 2 || info.layers[1].orientation == 4 )
                        {
                            type = EdgeTex;
                            orientationType = 0;
                        }  
                        // 如果碰到右下角，变成对角
                        else if ( info.layers[1].orientation == 3 || info.layers[1].orientation == 6 )
                        {
                            type = DuijiaoTex;
                            orientationType = 1;
                        }
                        // 碰到左上角，也就是自身
                        else if ( info.layers[1].orientation == 0 || info.layers[1].orientation == 5 )
                        {
                            continue;
                        }
                    }                    
                    // 如果碰上非相对的（如左上外角与右下内角为相对的）内角，直接跳过，否则，就变成底面纹理
                    else if ( currentTexType == InnerCornerTex )
                    {
                        if ( info.layers[1].orientation == 3 || info.layers[1].orientation == 6 )
                        {
                            type = InnerTex;
							orientationType = 0;
                        }
                        else
                            continue;
                    }
                    // 如果碰上位置相同的对角（对角之前的位置是外角），就变成内角
                    else if ( currentTexType == DuijiaoTex )
                    {
                        if ( info.layers[1].orientation == 0 || info.layers[1].orientation == 3 ||
							 info.layers[1].orientation == 5 || info.layers[1].orientation == 6 )
                        {
                            type = InnerCornerTex;
                            orientationType = 0;
                        }
                        else
                            continue;
                    }
                    // 以上情况都不是，就进行正常情况下外角的绘制
                    else
                    {
                        type = OuterCornerTex;
                        orientationType = 0;
                    }
                }
                // 处理右上角
                else if ( x == maxx && z == mCurrentBoxSelectionMinZ )
                {
                    if ( currentTexType == EdgeTex )
                    {
                        // 如果遇上左边缘
                        if ( info.layers[1].orientation == 0 || info.layers[1].orientation == 2 )
                        {
                            type = InnerCornerTex;
                            orientationType = 0;
                        }
                        // 如果遇上下边缘
                        else if ( info.layers[1].orientation == 4 || info.layers[1].orientation == 6 )
                        {
                            type = InnerCornerTex;
                            orientationType = 3;
                        }
                        // 如果遇上右边缘
                        else if ( info.layers[1].orientation == 1 || info.layers[1].orientation == 3 )
                        {
                            continue;
                        }
                        // 如果遇上上边缘
                        else if ( info.layers[1].orientation == 5 || info.layers[1].orientation == 7 )
                        {
                            continue;
                        }
                    }
                    else if ( currentTexType == OuterCornerTex )
                    {
                        // 遇上左上角，变成上边缘
                        if ( info.layers[1].orientation == 0 || info.layers[1].orientation == 5 )
                        {
                            type = EdgeTex;
                            orientationType = 5;
                        }
                        // 遇上右下角，变成右边缘
                        else if ( info.layers[1].orientation == 3 || info.layers[1].orientation == 6 )
                        {
                            type = EdgeTex;
                            orientationType = 1;
                        }
                        // 如果碰上左下角，变成对角
                        else if ( info.layers[1].orientation == 2 || info.layers[1].orientation == 4 )
                        {
                            type = DuijiaoTex;
                            orientationType = 0;
                        }
                        // 碰到右上角，也就是自身
                        else if ( info.layers[1].orientation == 1 || info.layers[1].orientation == 7 )
                        {
                            continue;
                        }
                    }                    
                    // 如果碰上非相对的（如左上外角与右下内角为相对的）内角，直接跳过，否则，就变成底面纹理
                    else if ( currentTexType == InnerCornerTex )
                    {
                        if ( info.layers[1].orientation == 2 || info.layers[1].orientation == 4 )
                        {
							type = InnerTex;
							orientationType = 0;
                        }
                        else
                            continue;
                    }
                    // 如果碰上位置相同的对角（对角之前的位置是外角），就变成内角
                    else if ( currentTexType == DuijiaoTex )
                    {
                        if ( info.layers[1].orientation == 1 || info.layers[1].orientation == 2 
							 || info.layers[1].orientation == 4 || info.layers[1].orientation == 7 )
                        {
                            type = InnerCornerTex;
                            orientationType = 1;
                        }
                        else
                            continue;
                    }
                    // 以上情况都不是，就进行正常情况下外角的绘制
                    else
                    {
                        type = OuterCornerTex;
                        orientationType = 1;
                    }
                }
                // 处理左下角
                else if ( x == mCurrentBoxSelectionMinX && z == maxz )
                {
                    if ( currentTexType == EdgeTex )
                    {
                        // 如果遇上右边缘
                        if ( info.layers[1].orientation == 1 || info.layers[1].orientation == 3 )
                        {
                            type = InnerCornerTex;
                            orientationType = 3;
                        }
                        // 如果遇上上边缘
                        else if ( info.layers[1].orientation == 5 || info.layers[1].orientation == 7 )
                        {
                            type = InnerCornerTex;
                            orientationType = 0;
                        }
                        // 如果遇上左边缘
                        else if ( info.layers[1].orientation == 0 || info.layers[1].orientation == 2 )
                        {
                            continue;
                        }
                        // 如果遇上下边缘
                        else if ( info.layers[1].orientation == 4 || info.layers[1].orientation == 6 )
                        {
                            continue;
                        }
                    }
                    else if ( currentTexType == OuterCornerTex )
                    {
                        // 遇上右下角，变成下边缘
                        if ( info.layers[1].orientation == 3 || info.layers[1].orientation == 6 )
                        {
                            type = EdgeTex;
                            orientationType = 4;
                        }
                        // 遇上左上角，变成左边缘
                        else if ( info.layers[1].orientation == 0 || info.layers[1].orientation == 5 )
                        {
                            type = EdgeTex;
                            orientationType = 0;
                        }
                        // 如果碰上右上角，就变成对角
                        else if ( info.layers[1].orientation == 1 || info.layers[1].orientation == 7 )
                        {
                            type = DuijiaoTex;
                            orientationType = 0;
                        }
                        // 碰到左下角，也就是自身
                        else if ( info.layers[1].orientation == 2 || info.layers[1].orientation == 4 )
                        {
                            continue;
                        }
                    }                    
                    // 如果碰上非相对的（如左上外角与右下内角为相对的）内角，直接跳过，否则，就变成底面纹理
                    else if ( currentTexType == InnerCornerTex )
                    {
                        if ( info.layers[1].orientation == 1 || info.layers[1].orientation == 7 )
                        {
                            type = InnerTex;
							orientationType = 0;
                        }
                        else
                            continue;
                    }
                    // 如果碰上位置相同的对角（对角之前的位置是外角），就变成内角
                    else if ( currentTexType == DuijiaoTex )
                    {
                        if ( info.layers[1].orientation == 1 || info.layers[1].orientation == 2 
							 || info.layers[1].orientation == 4  || info.layers[1].orientation == 7 )
                        {
                            type = InnerCornerTex;
                            orientationType = 2;
                        }
                        else
                            continue;
                    }
                    // 以上情况都不是，就进行正常情况下外角的绘制
                    else
                    {
                        type = OuterCornerTex;
                        orientationType = 2;
                    }
                }
                // 处理右下角
                else if ( x == maxx && z == maxz )
                {
                    if ( currentTexType == EdgeTex )
                    {
                        // 如果遇上左边缘
                        if ( info.layers[1].orientation == 0 || info.layers[1].orientation == 2 )
                        {
                            type = InnerCornerTex;
                            orientationType = 2;
                        }
                        // 如果遇上上边缘
                        else if ( info.layers[1].orientation == 5 || info.layers[1].orientation == 7 )
                        {
                            type = InnerCornerTex;
                            orientationType = 1;
                        }
                        // 如果遇上右边缘
                        else if ( info.layers[1].orientation == 1 || info.layers[1].orientation == 3 )
                        {
                            continue;
                        }
                        // 如果遇上下边缘
                        else if ( info.layers[1].orientation == 4 || info.layers[1].orientation == 6 )
                        {
                            continue;
                        }
                    }
                    else if ( currentTexType == OuterCornerTex )
                    {
                        // 遇上左下角，变成下边缘
                        if ( info.layers[1].orientation == 2 || info.layers[1].orientation == 4 )
                        {
                            type = EdgeTex;
                            orientationType = 4;
                        }
                        // 如果碰上右上角，就变成右边缘
                        else if ( info.layers[1].orientation == 1 || info.layers[1].orientation == 7 )
                        {
                            type = EdgeTex;
                            orientationType = 1;
                        }
                        // 遇上左上角，变成对角
                        else if ( info.layers[1].orientation == 0 || info.layers[1].orientation == 5 )
                        {
                            type = DuijiaoTex;
                            orientationType = 1;
                        }
                        // 碰到右下角，也就是自身
                        else if ( info.layers[1].orientation == 3 || info.layers[1].orientation == 6 )
                        {
                            continue;
                        }
                    }             
                    // 如果碰上非相对的（如左上外角与右下内角为相对的）内角，直接跳过，否则，就变成底面纹理
                    else if ( currentTexType == InnerCornerTex )
                    {
                        if ( info.layers[1].orientation == 0 || info.layers[1].orientation == 5 )
                        {
                            type = InnerTex;
							orientationType = 0;      
						}
                        else
                            continue;
                    }
                    // 如果碰上位置相同的对角（对角之前的位置是外角），就变成内角
                    else if ( currentTexType == DuijiaoTex )
                    {
                        if ( info.layers[1].orientation == 0 || info.layers[1].orientation == 3
							 || info.layers[1].orientation == 5 || info.layers[1].orientation == 6 )
                        {
                            type = InnerCornerTex;
                            orientationType = 3;
                        }
                        else
                            continue;
                    }
                    // 以上情况都不是，就进行正常情况下外角的绘制
                    else
                    {
                        type = OuterCornerTex;
                        orientationType = 3;
                    }
                }
                // 以上是处理四个边缘
                // LEFT
                else
                if ( x == mCurrentBoxSelectionMinX )
                {
                    // 如果当前格的第1层的纹理类型为Other，有两种情况：1 这个格的第1层还没贴过纹理
                    // 2 这个格的第1层贴过纹理了，不过不属于当前组
                    // 如果是第一种情况，可以直接贴上去，如果是第二种情况，直接贴上去，在不同组纹理间没有平滑的过渡，这是一个
                    // 待处理的地方 FIX IT
                    if ( currentTexType == Other )
                    {         
                            type = EdgeTex;
                            orientationType = 0;                        
                    }
                    else if ( currentTexType == EdgeTex )
                    {
                        // self
                        if ( info.layers[1].orientation == 0 || info.layers[1].orientation == 2 )
                        {
                            continue;
                        }
                        // top
                        else if ( info.layers[1].orientation == 5 || info.layers[1].orientation == 7 )
                        {
                            type = InnerCornerTex;

                            orientationType = 0;
                        }
                        // bottom
                        else if ( info.layers[1].orientation == 4 || info.layers[1].orientation == 6 )
                        {
                            type = InnerCornerTex;

                            orientationType = 2;
                        }
                        // right
                        else if ( info.layers[1].orientation == 1 || info.layers[1].orientation == 3 )
                        {
                            type = InnerTex;
							orientationType = 0;      
                        }
                    }
                    else if ( currentTexType == OuterCornerTex )
                    {
                        // 遇上右上角
                        if ( info.layers[1].orientation == 1 || info.layers[1].orientation == 7 )
                        {
                            type = InnerCornerTex;

                            orientationType = 0;
                        }
                        // 遇上右下角
                        else if ( info.layers[1].orientation == 3 || info.layers[1].orientation == 6 )
                        {
                            type = InnerCornerTex;

                            orientationType = 2;
                        }
                        // 如果遇上左上角或左下角
                        else if ( (info.layers[1].orientation == 0 ) || (info.layers[1].orientation == 2)
							 || info.layers[1].orientation == 4  || info.layers[1].orientation == 5 )
                        {
                            type = EdgeTex;
                            orientationType = 0;
                        }
                    }                    
                    // 如果遇上非相对的内角（比如说，左边缘与右上内角和右下内角就是相对的），就跳过，否则，要变成底面纹理
                    else if ( currentTexType == InnerCornerTex )
                    {
                        if ( info.layers[1].orientation == 1 || info.layers[1].orientation == 3 
							 || info.layers[1].orientation == 6 || info.layers[1].orientation == 7 )
                        {
                            type = InnerTex;
							orientationType = 0;
                        }
                        else
                            continue;
                    }
                    // 如果碰上位置相同的对角（对角之前的位置是外角），就变成内角
                    else if ( currentTexType == DuijiaoTex )
                    {
                        type = InnerCornerTex;

                        if ( info.layers[1].orientation == 0 || info.layers[1].orientation == 3
							 || info.layers[1].orientation == 5 || info.layers[1].orientation == 6 )
                        {
                           orientationType = 0;
                        }
                        else
                        {
                           orientationType = 2;
                        }
                    }
                    

                }
                // TOP
                else if ( z == mCurrentBoxSelectionMinZ )
                {                    
                    if ( currentTexType == Other )
                    {
                        type = EdgeTex;
                        orientationType = 5;
                    }
                    else if ( currentTexType == EdgeTex )
                    {
                        // self
                        if ( info.layers[1].orientation == 5 || info.layers[1].orientation == 7)
                        {
                            continue;
                        }
                        // right
                        else if ( info.layers[1].orientation == 1 || info.layers[1].orientation == 3 )
                        {
                            type = InnerCornerTex;

                            orientationType = 1;
                        }                    
                        // bottom
                        else if ( info.layers[1].orientation == 4 || info.layers[1].orientation == 6 )
                        {
                            type = InnerTex;
							orientationType = 0;
                        }
                        // left
                        else if ( info.layers[1].orientation == 0 || info.layers[1].orientation == 2 )
                        {
                            type = InnerCornerTex;

                            orientationType = 0;
                        }
                    }
                    else if ( currentTexType == OuterCornerTex )
                    {
                        // 遇上左下角
                        if ( info.layers[1].orientation == 2 || info.layers[1].orientation == 4 )
                        {
                            type = InnerCornerTex;

                            orientationType = 0;
                        }
                        // 遇上右下角
                        else if ( info.layers[1].orientation == 3 || info.layers[1].orientation == 6 )
                        {
                            type = InnerCornerTex;

                            orientationType = 1;
                        }
                        // 如果遇上右上角或左上角
                        else if ( (info.layers[1].orientation == 1 ) || (info.layers[1].orientation == 0 )
							 || info.layers[1].orientation == 5  || info.layers[1].orientation == 7 )
                        {
                            type = EdgeTex;
                            orientationType = 5;
                        }
                    }                    
                    // 如果遇上非相对的内角（比如说，左边缘与右上内角和右下内角就是相对的），就跳过，否则，要变成底面纹理
                    else if ( currentTexType == InnerCornerTex )
                    {
                        if ( info.layers[1].orientation == 2 || info.layers[1].orientation == 3 
							 || info.layers[1].orientation == 4 || info.layers[1].orientation == 6 )
                        {
                            type = InnerTex;
							orientationType = 0;
                        }
                        else
                            continue;
                    }
                    // 如果碰上位置相同的对角（对角之前的位置是外角），就变成内角
                    else if ( currentTexType == DuijiaoTex )
                    {
                        type = InnerCornerTex;

                        if ( info.layers[1].orientation == 0 || info.layers[1].orientation == 5 
							 || info.layers[1].orientation == 3  || info.layers[1].orientation == 6 )
                        {
                            orientationType = 0;
                        }
                        else
                        {
                            orientationType = 1;
                        }
                    }

                }
                // RIGHT
               // else if ( isTexInCurrentGroup( findTexTypeByPixmapId(leftInfo.layers[0].pixmapId) ) )
                else if ( x == maxx )
                {                    
                    if ( currentTexType == Other )
                    {
                        type = EdgeTex;
                        orientationType = 1;
                    }
                    else if ( currentTexType == EdgeTex )
                    {
                        // self
                        if ( info.layers[1].orientation == 1 || info.layers[1].orientation == 3 )
                        {
                            continue;
                        }
                        // bottom
                        else if ( info.layers[1].orientation == 4 || info.layers[1].orientation == 6 )
                        {
                            type = InnerCornerTex;

                            orientationType = 3;
                        }
                        // top
                        else if ( info.layers[1].orientation == 5 || info.layers[1].orientation == 7 )
                        {
                            type = InnerCornerTex;

                            orientationType = 1;
                        }
                        // left
                        else if ( info.layers[1].orientation == 0 || info.layers[1].orientation == 2 )
                        {
                            type = InnerTex;
							orientationType = 0;
                        }
                    }
                    else if ( currentTexType == OuterCornerTex )
                    {
                        // 遇上左上角
                        if ( info.layers[1].orientation == 0 || info.layers[1].orientation == 5 )
                        {
                            type = InnerCornerTex;

                            orientationType = 1;
                        }
                        // 遇上左下角
                        else if ( info.layers[1].orientation == 2 || info.layers[1].orientation == 4 )
                        {
                            type = InnerCornerTex;

                            orientationType = 3;
                        }
                        // 如果遇上右上角或右下角
                        else if ( (info.layers[1].orientation == 1 ) || (info.layers[1].orientation == 3 ) 
							 || info.layers[1].orientation == 6 || info.layers[1].orientation == 7 )
                        {
                            type = EdgeTex;
                            orientationType = 1;
                        }
                    }                    
                    // 如果遇上非相对的内角（比如说，左边缘与右上内角和右下内角就是相对的），就跳过，否则，要变成底面纹理
                    else if ( currentTexType == InnerCornerTex )
                    {
                        if ( info.layers[1].orientation == 2 || info.layers[1].orientation == 0
							 || info.layers[1].orientation == 4 || info.layers[1].orientation == 5 )
                        {
                            type = InnerTex;
							orientationType = 0;
                        }
                        else
                            continue;
                    }
                    // 如果碰上位置相同的对角（对角之前的位置是外角），就变成内角
                    else if ( currentTexType == DuijiaoTex )
                    {
                        type = InnerCornerTex;

                        if ( info.layers[1].orientation == 0 || info.layers[1].orientation == 3
							 || info.layers[1].orientation == 5 || info.layers[1].orientation == 6 )
                        {
                            orientationType = 3;
                        }
                        else
                        {
                            orientationType = 1;
                        }
                    }

                }
                // BOTTOM
               // else if ( isTexInCurrentGroup( findTexTypeByPixmapId(topInfo.layers[0].pixmapId) ) )
                else if ( z == maxz )
                {             
                    // 如果当前的1层没贴过，就直接贴正常的边缘？？？？？？？？？？？？
                  //  if ( info.layers[1].pixmapId < 1)
                    if ( currentTexType == Other )
                    {
                        type = EdgeTex;
                        orientationType = 4;
                    }
                    else if ( currentTexType == EdgeTex )
                    {
                        // self
                        if ( info.layers[1].orientation == 4 || info.layers[1].orientation == 6 )
                        {
                            continue;
                        }
                        // left edge
                        else if ( info.layers[1].orientation == 0 || info.layers[1].orientation == 2 )
                        {
                            type = InnerCornerTex;

                            orientationType = 2;
                        }
                        // right edge
                        else if ( info.layers[1].orientation == 1 || info.layers[1].orientation == 3 )
                        {
                            type = InnerCornerTex;

                            orientationType = 3;
                        }
                        // 如果当前格为相对的边缘，就画成底
                        // top edge
                        else if ( info.layers[1].orientation == 5 || info.layers[1].orientation == 7 )
                        {
                            type = InnerTex;
							orientationType = 0;
                        }
                    }
                    else if ( currentTexType == OuterCornerTex )
                    {
                        // 遇上左上角
                        if ( info.layers[1].orientation == 0 || info.layers[1].orientation == 5 )
                        {
                            type = InnerCornerTex;

                            orientationType = 2;
                        }
                        // 遇上右上角
                        else if ( info.layers[1].orientation == 1 || info.layers[1].orientation == 7 )
                        {
                            type = InnerCornerTex;

                            orientationType = 3;
                        }
                        // 如果遇上左下角或右下角
                        else if ( (info.layers[1].orientation == 2 ) || (info.layers[1].orientation == 3 )
							 || info.layers[1].orientation == 4  || info.layers[1].orientation == 6 )
                        {
                            type = EdgeTex;
                            orientationType = 4;
                        }
                    }                    
                    // 如果遇上非相对的内角（比如说，左边缘与右上内角和右下内角就是相对的），就跳过，否则，要变成底面纹理
                    else if ( currentTexType == InnerCornerTex )
                    {
                        if ( info.layers[1].orientation == 1 || info.layers[1].orientation == 0 
							 || info.layers[1].orientation == 5  || info.layers[1].orientation == 7 )
                        {
                            type = InnerTex;
							orientationType = 0;
                        }
                        else
                            continue;
                    }
                    // 如果碰上位置相同的对角（对角之前的位置是外角），就变成内角
                    else if ( currentTexType == DuijiaoTex )
                    {
                        type = InnerCornerTex;

                        if ( info.layers[1].orientation == 0 || info.layers[1].orientation == 3 
							 || info.layers[1].orientation == 5 || info.layers[1].orientation == 6 )
                        {
                            orientationType = 3;
                        }
                        else
                        {
                            orientationType = 2;
                        }
                    }
                }
              
				assert ( (type >= InnerTex) && (type < Other) );
				assert ( (orientationType >= 0) && (orientationType < 8) );

                switch( type )
                {
                    // 用内部纹理来渲染本格
                case InnerTex:
                    {
                        PaintPixmap pixmap;  

						assert ( false == mCurrentInnerArray->empty() );

                        if ( modified != mHintModified )
                        {
							int index = static_cast<int>(Ogre::Math::RangeRandom(0,innerSize));
							if (index == innerSize)
							{
								index = 0;
							}
							assert (index < innerSize);
                            pixmap = (*mCurrentInnerArray)[ index ];
                        }
                        else
                        {
                            pixmap = (*mCurrentInnerArray)[0];
                        }
                        // 用这个id来索引pixmap
                        setPixmap(pixmap);

                        pixmapId = getPixmapId();


                        info.layers[0].pixmapId = pixmapId;

						assert ( orientationType == 0 );
                        info.layers[0].orientation = orientationType;
                     
                        info.layers[1].pixmapId = 0;
                        info.layers[1].orientation = 0;
                    
                        break;
                    }
                    // 用边缘纹理来渲染本格
                case EdgeTex:
                    {
						AutoTexGenPixmapArray *workingArray = NULL;
						int workingArraySize = 0;

						// 
						if ( orientationType == 0 )
						{
							if ( mCurrentLEdgeArray->empty() )
							{
								workingArray = mCurrentEdgeArray;
								workingArraySize = edgeSize;
							}
							else
							{
								workingArray = mCurrentLEdgeArray;
								workingArraySize = lEdgeSize;
							}
						}
						else if ( orientationType == 1 )
						{
							if ( mCurrentREdgeArray->empty() )
							{
								workingArray = mCurrentEdgeArray;
								workingArraySize = edgeSize;
							}
							else
							{
								workingArray = mCurrentREdgeArray;
								workingArraySize = rEdgeSize;
							}
						}	
						else if ( orientationType == 4 )
						{
							if ( mCurrentBEdgeArray->empty() )
							{
								workingArray = mCurrentEdgeArray;
								workingArraySize = edgeSize;
							}
							else
							{
								workingArray = mCurrentBEdgeArray;
								workingArraySize = bEdgeSize;
							}
						}	
						else if ( orientationType == 5 )
						{
							if ( mCurrentTEdgeArray->empty() )
							{
								workingArray = mCurrentEdgeArray;
								workingArraySize = edgeSize;
							}
							else
							{
								workingArray = mCurrentTEdgeArray;
								workingArraySize = tEdgeSize;
							}
						}

                        PaintPixmap pixmap;

						if ( modified != mHintModified )
						{
							int index = static_cast<int>(Ogre::Math::RangeRandom(0,workingArraySize));
							if (index == workingArraySize)
							{
								index = 0;
							}
							assert (index < workingArraySize);
							pixmap = (*workingArray)[ index ];
						}
						else
						{	
							pixmap = (*workingArray)[0];
						}

                        // 用这个id来索引pixmap
                        setPixmap(pixmap);

                        pixmapId = getPixmapId();

                        assert ( pixmapId >=1 );

                        if (info.layers[0].pixmapId > 0)
                        {
							/*assert ( orientationType == 5 ||
									 orientationType == 0 ||
									 orientationType == 4 ||
									 orientationType == 1 );*/

                            info.layers[1].pixmapId = pixmapId;

                            info.layers[1].orientation = orientationType;

							if ( info.layers[1].pixmapId < 1 )
							{
								OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR,
							" info.layers[1].pixmapId < 1 EdgeTex",
							"AutoTexPaintAction::_doPaint");
							}
                        }

                        break;
                    }
                    // 用内角纹理来渲染本格
                case InnerCornerTex:
                    {        
						AutoTexGenPixmapArray *workingArray = NULL;
						int workingArraySize = 0;

						if ( orientationType == 0 )
						{
							if ( mCurrentLTInnerCornerArray->empty() )
							{
								workingArray = mCurrentInnerCornerArray;
								workingArraySize = innercornerSize;
							}
							else
							{
								workingArray = mCurrentLTInnerCornerArray;
								workingArraySize = ltInnerCornerSize;
							}
						}
						else if ( orientationType == 1 )
						{
							if ( mCurrentRTInnerCornerArray->empty() )
							{
								workingArray = mCurrentInnerCornerArray;
								workingArraySize = innercornerSize;
							}
							else
							{
								workingArray = mCurrentRTInnerCornerArray;
								workingArraySize = rtInnerCornerSize;
							}
						}
						else if ( orientationType == 2 )
						{
							if ( mCurrentLBInnerCornerArray->empty() )
							{
								workingArray = mCurrentInnerCornerArray;
								workingArraySize = innercornerSize;
							}
							else
							{
								workingArray = mCurrentLBInnerCornerArray;
								workingArraySize = lbInnerCornerSize;
							}
						}
						else if ( orientationType == 3 )
						{
							if ( mCurrentRBInnerCornerArray->empty() )
							{
								workingArray = mCurrentInnerCornerArray;
								workingArraySize = innercornerSize;
							}
							else
							{
								workingArray = mCurrentRBInnerCornerArray;
								workingArraySize = rbInnerCornerSize;
							}
						}

                        PaintPixmap pixmap;  

                        if ( modified != mHintModified )
                        {
							int index = static_cast<int>(Ogre::Math::RangeRandom(0,workingArraySize));
							if (index == workingArraySize)
							{
								index = 0;
							}
                            pixmap = (*workingArray)[ index ];
                        }
                        else
                        {
                            pixmap = (*workingArray)[0];
                        }
                        // 用这个id来索引pixmap
                        setPixmap(pixmap);

                        pixmapId = getPixmapId();


                        assert ( pixmapId >=1 );

                        if (info.layers[0].pixmapId > 0)
                        {
							/*assert ( orientationType == 0 ||
									 orientationType == 1 ||
									 orientationType == 2 ||
									 orientationType == 3 );*/

                            info.layers[1].pixmapId = pixmapId;

                            info.layers[1].orientation = orientationType;

							if ( info.layers[1].pixmapId < 1 )
							{
								OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR,
							" info.layers[1].pixmapId < 1 InnerCornerTex",
							"AutoTexPaintAction::_doPaint");
							}
                        }

                        break;
                    }
                    // 外角
                case OuterCornerTex:
                    {    
						AutoTexGenPixmapArray *workingArray = NULL;
						int workingArraySize = 0;

						if ( orientationType == 0 )
						{
							if ( mCurrentLTOuterCornerArray->empty() )
							{
								workingArray = mCurrentOuterCornerArray;
								workingArraySize = outercornerSize;
							}
							else
							{
								workingArray = mCurrentLTOuterCornerArray;
								workingArraySize = ltOutercornerSize;
							}
						}
						else if ( orientationType == 1 )
						{
							if ( mCurrentRTOuterCornerArray->empty() )
							{
								workingArray = mCurrentOuterCornerArray;
								workingArraySize = outercornerSize;
							}
							else
							{
								workingArray = mCurrentRTOuterCornerArray;
								workingArraySize = rtOutercornerSize;
							}
						}
						else if ( orientationType == 2 )
						{
							if ( mCurrentLBOuterCornerArray->empty() )
							{
								workingArray = mCurrentOuterCornerArray;
								workingArraySize = outercornerSize;
							}
							else
							{
								workingArray = mCurrentLBOuterCornerArray;
								workingArraySize = lbOutercornerSize;
							}
						}
						else if ( orientationType == 3 )
						{
							if ( mCurrentRBOuterCornerArray->empty() )
							{
								workingArray = mCurrentOuterCornerArray;
								workingArraySize = outercornerSize;
							}
							else
							{
								workingArray = mCurrentRBOuterCornerArray;
								workingArraySize = rbOutercornerSize;
							}
						}

                        PaintPixmap pixmap;  

						if ( modified != mHintModified )
                        {
							int index = static_cast<int>(Ogre::Math::RangeRandom(0,workingArraySize));
							if (index == workingArraySize)
							{
								index = 0;
							}
							assert (index < workingArraySize);
                            pixmap = (*workingArray)[ index ];
                        }
                        else
                        {
                            pixmap = (*workingArray)[0];
                        }

                        // 用这个id来索引pixmap
                        setPixmap(pixmap);

                        pixmapId = getPixmapId();


                        assert ( pixmapId >=1 );

                        if (info.layers[0].pixmapId > 0)
                        {
							/*assert ( orientationType == 0 ||
									 orientationType == 1 ||
									 orientationType == 2 ||
									 orientationType == 3 );*/

                            info.layers[1].pixmapId = pixmapId;

                            info.layers[1].orientation = orientationType;

							if ( info.layers[1].pixmapId < 1 )
							{
								OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR,
							" info.layers[1].pixmapId < 1 OuterCornerTex",
							"AutoTexPaintAction::_doPaint");
							}
                        }

                        break;
                    }
                    // 对角
                case DuijiaoTex:
                    {
                        PaintPixmap pixmap;  

						assert ( false == mCurrentDuijiaoArray->empty() );

                        if ( modified != mHintModified )
                        {
							int index = static_cast<int>(Ogre::Math::RangeRandom(0,duijiaoSize));
							if (index == duijiaoSize)
							{
								index = 0;
							}
							assert (index < duijiaoSize);
                            pixmap = (*mCurrentDuijiaoArray)[ index ];
                        }
                        else
                        {
                            pixmap = (*mCurrentDuijiaoArray)[0];
                        }
                        // 用这个id来索引pixmap
                        setPixmap(pixmap);

                        pixmapId = getPixmapId();


                        assert ( pixmapId >=1 );

                        if (info.layers[0].pixmapId > 0)
                        {
							/*assert ( orientationType == 0 ||
									 orientationType == 1 );*/

                            info.layers[1].pixmapId = pixmapId;

                            info.layers[1].orientation = orientationType;

							if ( info.layers[1].pixmapId < 1 )
							{
								OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR,
							" info.layers[1].pixmapId < 1 DuijiaoTex",
							"AutoTexPaintAction::_doPaint");
							}
                        }

                        break;
                    }
                case Other:
					{
						/*OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS,
							"The texture type is wrong ",
							"AutoTexPaintAction::_doPaint");*/
                        Ogre::LogManager::getSingleton().logMessage("The texture type is wrong "
                            "AutoTexPaintAction::_doPaint");
                        return;
					}
                    break;
              
                }      

                assert(info.layers[0].pixmapId != 0 || info.layers[1].pixmapId == 0);
			
                assert(info.layers[0].pixmapId <= getTerrainData()->mPixmaps.size());

				if (info.layers[0].pixmapId > getTerrainData()->mPixmaps.size())
				{
					OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR,
							"info.layers[0].pixmapId > getTerrainData()->mPixmaps.size()",
							"AutoTexPaintAction::_doPaint");
				}

                assert(info.layers[1].pixmapId <= getTerrainData()->mPixmaps.size());

				if (info.layers[1].pixmapId > getTerrainData()->mPixmaps.size())
				{
					OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR,
							"info.layers[1].pixmapId > getTerrainData()->mPixmaps.size()",
							"AutoTexPaintAction::_doPaint");
				}

				if (getTerrainData()->mPixmaps.size() >= 256)
				{
					;
				}

                if (info == grid.info)
                    continue;

                anyModified = true;
                modified->add(grid.x, grid.z);
                getTerrainData()->setGridInfo(grid.x, grid.z, info);

                assert (info.layers[0].pixmapId !=0 || info.layers[1].pixmapId == 0);				
            }         
        }
               
        if (anyModified)
        {
            mCurrentInnerGrids->notifyModified();
            mCurrentOuterGrids->notifyModified();
        }
    }
    //-----------------------------------------------------------------------
    void AutoTexPaintAction::_onActive(bool active)
    {
        if (!active)
        {
            mHintModified->apply();
            mHintModified->reset();
        }
    }
    //-----------------------------------------------------------------------
    void AutoTexPaintAction::_onMove(const Point& pt)
    {      
        mHintModified->apply();
        mHintModified->reset();
        _buildHitIndicator(pt);
        _doPaint(mHintModified);   
    }
    //-----------------------------------------------------------------------
    void AutoTexPaintAction::_onBegin(const Point& pt)
    {
        mHintModified->apply();
        mHintModified->reset();
        mModifiedGrids->reset();
        _buildHitIndicator(pt);
        _doPaint(mModifiedGrids);
    }
    //-----------------------------------------------------------------------
    void AutoTexPaintAction::_onDrag(const Point& pt)
    {
        _buildHitIndicator(pt);
        _doPaint(mModifiedGrids);
    }
    //-----------------------------------------------------------------------
    void AutoTexPaintAction::_onEnd(const Point& pt, bool canceled)
    {
        /*if (canceled)
        {
            mModifiedGrids->apply();
        }*/
        doFinish(mModifiedGrids, canceled);
        mModifiedGrids->reset();
        _buildHitIndicator(pt);
    }
    //-----------------------------------------------------------------------
    // 用于判断纹理名称是否相等的仿函数
    namespace {
        struct IsCaseInsensitiveEqual
        {
            const String& value;
            Real left;
            Real top;

#define error 0.000001

            IsCaseInsensitiveEqual(const String& value, Real left, Real top) : value(value), left(left), top(top) {}

            bool operator() (const PaintPixmap& rhs) const
            {

                return (stricmp(value.c_str(), rhs.textureName.c_str()) == 0 &&
                    left <= rhs.left+error && left >= rhs.left-error &&
                    top <= rhs.top+error && top >= rhs.top-error );
            }
        };
    }
    //-----------------------------------------------------------------------
    bool AutoTexPaintAction::isTexInCurrentGroup( ushort id )
    {
        if ( id < 1 )
            return false;
        else
        {
            TerrainData::Pixmap pixmap = getTerrainData()->mPixmaps[id-1];
            String texName = getTerrainData()->mTextures[ pixmap.textureId ];

            return 
                std::find_if( mCurrentInnerCornerArray->begin(), mCurrentInnerCornerArray->end(),
                IsCaseInsensitiveEqual(texName,pixmap.left,pixmap.top) ) != mCurrentInnerCornerArray->end() ||
				std::find_if( mCurrentLTInnerCornerArray->begin(), mCurrentLTInnerCornerArray->end(),
				IsCaseInsensitiveEqual(texName,pixmap.left,pixmap.top) ) != mCurrentLTInnerCornerArray->end() ||
				std::find_if( mCurrentRTInnerCornerArray->begin(), mCurrentRTInnerCornerArray->end(),
				IsCaseInsensitiveEqual(texName,pixmap.left,pixmap.top) ) != mCurrentRTInnerCornerArray->end() ||
				std::find_if( mCurrentLBInnerCornerArray->begin(), mCurrentLBInnerCornerArray->end(),
				IsCaseInsensitiveEqual(texName,pixmap.left,pixmap.top) ) != mCurrentLBInnerCornerArray->end() ||
				std::find_if( mCurrentRBInnerCornerArray->begin(), mCurrentRBInnerCornerArray->end(),
				IsCaseInsensitiveEqual(texName,pixmap.left,pixmap.top) ) != mCurrentRBInnerCornerArray->end() ||

                std::find_if( mCurrentOuterCornerArray->begin(), mCurrentOuterCornerArray->end(),
                IsCaseInsensitiveEqual(texName,pixmap.left,pixmap.top) ) != mCurrentOuterCornerArray->end() ||
				std::find_if( mCurrentLTOuterCornerArray->begin(), mCurrentLTOuterCornerArray->end(),
				IsCaseInsensitiveEqual(texName,pixmap.left,pixmap.top) ) != mCurrentLTOuterCornerArray->end() ||
				std::find_if( mCurrentRTOuterCornerArray->begin(), mCurrentRTOuterCornerArray->end(),
				IsCaseInsensitiveEqual(texName,pixmap.left,pixmap.top) ) != mCurrentRTOuterCornerArray->end() ||
				std::find_if( mCurrentLBOuterCornerArray->begin(), mCurrentLBOuterCornerArray->end(),
				IsCaseInsensitiveEqual(texName,pixmap.left,pixmap.top) ) != mCurrentLBOuterCornerArray->end() ||
				std::find_if( mCurrentRBOuterCornerArray->begin(), mCurrentRBOuterCornerArray->end(),
				IsCaseInsensitiveEqual(texName,pixmap.left,pixmap.top) ) != mCurrentRBOuterCornerArray->end() ||

                std::find_if( mCurrentInnerArray->begin(), mCurrentInnerArray->end(),
                IsCaseInsensitiveEqual(texName,pixmap.left,pixmap.top) ) != mCurrentInnerArray->end() ||

                std::find_if( mCurrentEdgeArray->begin(), mCurrentEdgeArray->end(), 
                IsCaseInsensitiveEqual(texName,pixmap.left,pixmap.top) ) != mCurrentEdgeArray->end() ||
				std::find_if( mCurrentLEdgeArray->begin(), mCurrentLEdgeArray->end(), 
				IsCaseInsensitiveEqual(texName,pixmap.left,pixmap.top) ) != mCurrentLEdgeArray->end() ||
				std::find_if( mCurrentREdgeArray->begin(), mCurrentREdgeArray->end(), 
				IsCaseInsensitiveEqual(texName,pixmap.left,pixmap.top) ) != mCurrentREdgeArray->end() ||
				std::find_if( mCurrentTEdgeArray->begin(), mCurrentTEdgeArray->end(), 
				IsCaseInsensitiveEqual(texName,pixmap.left,pixmap.top) ) != mCurrentTEdgeArray->end() ||
				std::find_if( mCurrentBEdgeArray->begin(), mCurrentBEdgeArray->end(), 
				IsCaseInsensitiveEqual(texName,pixmap.left,pixmap.top) ) != mCurrentBEdgeArray->end() ||

                std::find_if( mCurrentDuijiaoArray->begin(), mCurrentDuijiaoArray->end(),
                IsCaseInsensitiveEqual(texName,pixmap.left,pixmap.top) ) != mCurrentDuijiaoArray->end();
        }        
    }   
   
    TexType 
    AutoTexPaintAction::findTexTypeByPixmapId( ushort id )
    {       
        if (id < 1)
        {
            return Other;
        }
        else
        {
            TerrainData::Pixmap pixmap = getTerrainData()->mPixmaps[id-1];
            String texName = getTerrainData()->mTextures[ pixmap.textureId ];

            if ( std::find_if( mCurrentInnerCornerArray->begin(), mCurrentInnerCornerArray->end(), IsCaseInsensitiveEqual(texName,pixmap.left,pixmap.top) ) != mCurrentInnerCornerArray->end() ||
				 std::find_if( mCurrentLTInnerCornerArray->begin(), mCurrentLTInnerCornerArray->end(), IsCaseInsensitiveEqual(texName,pixmap.left,pixmap.top) ) != mCurrentLTInnerCornerArray->end() ||
				 std::find_if( mCurrentRTInnerCornerArray->begin(), mCurrentRTInnerCornerArray->end(), IsCaseInsensitiveEqual(texName,pixmap.left,pixmap.top) ) != mCurrentRTInnerCornerArray->end() ||
				 std::find_if( mCurrentLBInnerCornerArray->begin(), mCurrentLBInnerCornerArray->end(), IsCaseInsensitiveEqual(texName,pixmap.left,pixmap.top) ) != mCurrentLBInnerCornerArray->end() ||
				 std::find_if( mCurrentRBInnerCornerArray->begin(), mCurrentRBInnerCornerArray->end(), IsCaseInsensitiveEqual(texName,pixmap.left,pixmap.top) ) != mCurrentRBInnerCornerArray->end()
				)
                return InnerCornerTex;

            else if ( std::find_if( mCurrentOuterCornerArray->begin(), mCurrentOuterCornerArray->end(), IsCaseInsensitiveEqual(texName,pixmap.left,pixmap.top) ) != mCurrentOuterCornerArray->end() ||
				      std::find_if( mCurrentLTOuterCornerArray->begin(), mCurrentLTOuterCornerArray->end(), IsCaseInsensitiveEqual(texName,pixmap.left,pixmap.top) ) != mCurrentLTOuterCornerArray->end() ||
					  std::find_if( mCurrentRTOuterCornerArray->begin(), mCurrentRTOuterCornerArray->end(), IsCaseInsensitiveEqual(texName,pixmap.left,pixmap.top) ) != mCurrentRTOuterCornerArray->end() ||
					  std::find_if( mCurrentLBOuterCornerArray->begin(), mCurrentLBOuterCornerArray->end(), IsCaseInsensitiveEqual(texName,pixmap.left,pixmap.top) ) != mCurrentLBOuterCornerArray->end() ||
					  std::find_if( mCurrentRBOuterCornerArray->begin(), mCurrentRBOuterCornerArray->end(), IsCaseInsensitiveEqual(texName,pixmap.left,pixmap.top) ) != mCurrentRBOuterCornerArray->end()
				)
                return OuterCornerTex;

            else if ( std::find_if( mCurrentInnerArray->begin(), mCurrentInnerArray->end(), IsCaseInsensitiveEqual(texName,pixmap.left,pixmap.top) ) != mCurrentInnerArray->end() )
                return InnerTex;

            else if ( std::find_if( mCurrentEdgeArray->begin(), mCurrentEdgeArray->end(), IsCaseInsensitiveEqual(texName,pixmap.left,pixmap.top) ) != mCurrentEdgeArray->end() ||
				      std::find_if( mCurrentLEdgeArray->begin(), mCurrentLEdgeArray->end(), IsCaseInsensitiveEqual(texName,pixmap.left,pixmap.top) ) != mCurrentLEdgeArray->end() ||
					  std::find_if( mCurrentREdgeArray->begin(), mCurrentREdgeArray->end(), IsCaseInsensitiveEqual(texName,pixmap.left,pixmap.top) ) != mCurrentREdgeArray->end() ||
					  std::find_if( mCurrentTEdgeArray->begin(), mCurrentTEdgeArray->end(), IsCaseInsensitiveEqual(texName,pixmap.left,pixmap.top) ) != mCurrentTEdgeArray->end() ||
					  std::find_if( mCurrentBEdgeArray->begin(), mCurrentBEdgeArray->end(), IsCaseInsensitiveEqual(texName,pixmap.left,pixmap.top) ) != mCurrentBEdgeArray->end()
				)
                return EdgeTex;

            else if ( std::find_if( mCurrentDuijiaoArray->begin(), mCurrentDuijiaoArray->end(), IsCaseInsensitiveEqual(texName,pixmap.left,pixmap.top) ) != mCurrentDuijiaoArray->end() )
                return DuijiaoTex;

            else
            {
                //Ogre::LogManager::getSingleton().logMessage("textype is Other, maybe something is wrong! texture name : "
                //    + texName + " pixmapid : " + Ogre::StringConverter::toString(id) + " AutoTexPaintAction::findTexTypeByPixmapId");
                return Other; 
            }
        }
    }
}
