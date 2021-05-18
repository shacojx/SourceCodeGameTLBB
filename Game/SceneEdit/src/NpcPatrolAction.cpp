#include "NpcPatrolAction.h"
#include "SceneManipulator.h"
#include "HitIndicator.h"
#include "IntersectPoint.h"
#include "UnreachRegion.h"
#include "CustomShape.h"
#include "NpcPatrolEditOperator.h"	// 不可行走区域的redo undo 操作.
#include "WXOperatorManager.h"       // redo undo 管理器.
#include "ScanLine.h"
#include "OgreCamera.h"

#include "Core/TerrainData.h"
#include "Core/WXResources.h"

#include <windows.h>

namespace WX 
{
	using namespace Ogre;

	 CNpcPatrolAction::CNpcPatrolAction(SceneManipulator* sceneManipulator)
            : Action(sceneManipulator)
			,m_pSceneNode(NULL)
			,m_pMaterialPoint(NULL)
			,m_pMaterialPointSel(NULL)
			,m_pMaterialPointTemp(NULL)
			,m_pMaterialLine(NULL)	
			,m_pMaterialLineSel(NULL)
			,m_pMaterialRegion(NULL)	
			,m_pMaterialRegionSel(NULL)
			,m_ulCurMaxId(0)
    {
		
		// 初始化
		InitNpcPatrol();

		// 测试使用.
		//Test_AddRegion();
		m_bIsShow = false;

		//m_iPreOperatorType = NO_OPERATOR;
		
    }

	CNpcPatrolAction::~CNpcPatrolAction(void)
	{
		ClearRegionData();

        if (m_pSceneNode)
        {
            m_pSceneNode->destroy();
        }
	}

	const String& CNpcPatrolAction::getName(void) const
	{
		//返回一个字符串名字， 用来判断当前操作的类型.
		static const String name = "NpcPatrol";
		return name;
	}

    
	// 鼠标移动的操作
    void CNpcPatrolAction::_onMove(const Point& pt)
	{
		getSceneManipulator()->getHitIndicator("IntersectPoint")->setHitPoint(pt.x, pt.y);
		ClearFuncKey();
		
	}


	// 鼠标按下的操作.
	void CNpcPatrolAction::_onBegin(const Point& pt)
	{
		// 记录当前操作， 的点的位置.
		RecordPreDoPointPos();
	}


    void CNpcPatrolAction::_onDrag(const Point& pt)
	{
		if(m_FuncKeyState & FUNC_KEY_SHIFT)
		{
			// 如果shift键按下允许修改不可行走区域点的位置.
			ChangeCurEditPointPos(pt);
			//m_iPreOperatorType = MODITY_POINT_POSITION;
		}
		else//
		{
			getSceneManipulator()->getHitIndicator("IntersectPoint")->setHitPoint(pt.x, pt.y);
		}

		// 每次执行结束清空功能键
		ClearFuncKey();
			
	}


	// 鼠标抬起的操作
    void CNpcPatrolAction::_onEnd(const Point& pt, bool canceled)
	{
		if(!getActive())
		{
			return;
		}

		if(m_FuncKeyState & FUNC_KEY_SHIFT)
		{
			// 如果shift键按下允许修改不可行走区域点的位置.
			CNpcPatrolEditModifyOperator* pNpcPatrolModifyOperator = NULL;
			pNpcPatrolModifyOperator = new CNpcPatrolEditModifyOperator;

			pNpcPatrolModifyOperator->SetRegionAction(this);
			pNpcPatrolModifyOperator->SetPointId(m_ulCurEditPointId);
			pNpcPatrolModifyOperator->SetRegionId(m_ulCurEditRegionId);
			pNpcPatrolModifyOperator->SetPosition(m_preDoPointPos, m_curPointPos);//
			getOperatorManager()->addOperator(pNpcPatrolModifyOperator);//
		
		}
		else if(m_FuncKeyState & FUNC_KEY_CTRL)
		{
			UNREACHREGIONMAP::iterator it;
			UNREACHREGIONMAP::iterator itBegin;
			UNREACHREGIONMAP::iterator itEnd;
			CUnreachRegion* pUnReachRegion = NULL;
	
			itEnd  = m_UnreachRegionMap.end();
			for(it = m_UnreachRegionMap.begin();  it != itEnd; it++)
			{
				pUnReachRegion = it->second;

				if(pUnReachRegion)
				{
					if(pUnReachRegion->IsMouseSel(pt.x, pt.y))
					{
						// 选中
						UnSelRegion(m_ulCurEditRegionId);
						SelRegion(pUnReachRegion->m_ulId, false);

						// 通知界面
						getSceneManipulator()->_fireRegionSel(pUnReachRegion->m_ulId);
						break;
						
					}
				}
			}

			// 没有选中
		}
		else
		{
			// 鼠标抬起时对点的操作.
			if(EditPoint(pt))
			{
				CNpcPatrolEditAddOperator* pAddNewPointOperator = NULL;
				pAddNewPointOperator = new CNpcPatrolEditAddOperator;

				pAddNewPointOperator->SetRegionAction(this);
				pAddNewPointOperator->SetPointId(m_ulCurEditPointId);
				pAddNewPointOperator->SetRegionId(m_ulCurEditRegionId);
				pAddNewPointOperator->SetPosition(m_preDoPointPos, m_curPointPos);//
				getOperatorManager()->addOperator(pAddNewPointOperator);
	 
			}
		}


		// 清空功能键
		ClearFuncKey();

	}
    void CNpcPatrolAction::_onActive(bool active)
	{
		
		if (!active)
        {
            IntersectPoint* pPoint = (IntersectPoint*)getSceneManipulator()->getHitIndicator("IntersectPoint");
			pPoint->SetMaterial();
            pPoint->hide();
        }
		else
		{
			IntersectPoint* pPoint = (IntersectPoint*)getSceneManipulator()->getHitIndicator("IntersectPoint");
			String strMaterialName = "Region/PointTemp";
			pPoint->SetMaterial(strMaterialName);//
		}
	}


	////////////////////////////////////////////////////////////////////////////////////
	//
	//npc巡逻域管理函数
	//

	// 添加一个新的区域
	unsigned long CNpcPatrolAction::AddNewRegion()
	{
		m_ulCurMaxId++;
		unsigned long ulId = m_ulCurMaxId;

		CUnreachRegion* pRegion = NULL;
		pRegion = new CUnreachRegion(ulId, m_pSceneNode, getSceneManipulator());

		if(pRegion)
		{
			m_UnreachRegionMap[ulId] = pRegion;
			m_ulCurEditRegionId = ulId;
			return m_ulCurEditRegionId;
		}
		
		return 0xfffffff;
	}

	// 通过指定的id， 删除一个区域
	unsigned long CNpcPatrolAction::DelRegion(unsigned long ulId)
	{

		CUnreachRegion* pRegion = NULL;
		if(m_UnreachRegionMap.count(ulId))
		{
			pRegion = m_UnreachRegionMap[ulId];
			if(pRegion)
			{
				if(m_pSceneNode)
				{
					pRegion->DetachAllEntity();
					
					//pRegion->m_pSceneNode->removeAndDestroyAllChildren();
					pRegion->DestroyAllChildSceneNode();
					m_pSceneNode->removeAndDestroyChild(pRegion->m_pSceneNode->getName());
					//
					
				}

				delete pRegion;

				UNREACHREGIONMAP::iterator it;
				it = m_UnreachRegionMap.find(ulId);
				m_UnreachRegionMap.erase(it);

			}

			return 0;
		}

		return 0;
	}

		
	// 根据指定的id， 得到区域
	CUnreachRegion* CNpcPatrolAction::GetRegionById(unsigned long ulId)
	{

		if(m_UnreachRegionMap.count(ulId))
		{
			CUnreachRegion* pRegion = NULL;
			pRegion = m_UnreachRegionMap[ulId];

			return pRegion;
		}

        return NULL;
	}


	void CNpcPatrolAction::InitNpcPatrol()
	{
		CreateSceneNode();
	}//

	void CNpcPatrolAction::SetOldHitPointMaterial()
	{
		IntersectPoint* pPoint = (IntersectPoint*)getSceneManipulator()->getHitIndicator("IntersectPoint");
		pPoint->SetMaterial();
	}

	// 创建区域编辑的场景节点
	void CNpcPatrolAction::CreateSceneNode()
	{
		m_pSceneNode = getIndicatorRootSceneNode()->createChildSceneNode();
		if(m_pSceneNode)
		{
			m_pSceneNode->setVisible(false);
		}
		
	}

	// 在当前编辑的区域中添加一个新的点
	unsigned long CNpcPatrolAction::AddNewPoint(Ogre::Vector3 position)
	{
		if(0 == m_UnreachRegionMap.count(m_ulCurEditRegionId))
		{
			return 0 ;
		}

		CUnreachRegion* pRegion = NULL;

		pRegion = m_UnreachRegionMap[m_ulCurEditRegionId];
		if(pRegion)
		{
			
			m_ulCurEditPointId = pRegion->AddNewPoint(position);
			if( m_ulCurEditPointId > 0 )
			{
				//添加成功了
				if( position.y == -100000.0f )
				{
					pRegion->GetPointInfoById( m_ulCurEditPointId )->SetValuedY( false );
				}
			}
			return m_ulCurEditPointId;
		}

		return 0;
		
	}

	bool CNpcPatrolAction::EditPoint(const Point& pt)
	{
		Ogre::Vector3 position;
		unsigned long ulId = 0;

        bool hit = getSceneManipulator()->getTerrainIntersects(pt.x, pt.y, position);
		if(hit)
		{
			ulId = AddNewPoint(position);
			if(0 == ulId)
			{
				return false;
			}

			m_preDoPointPos = position;
			m_curPointPos   = position;

			getSceneManipulator()->_fireRegionAddNewPoint(m_ulCurEditRegionId, ulId, position);

			return true;
		}

		return false;
	}

	void CNpcPatrolAction::Test_AddRegion()
	{
		AddNewRegion();
	}

	void CNpcPatrolAction::HideScene(bool bHide)
	{
		if(m_pSceneNode)
		{
			m_pSceneNode->setVisible(bHide);
			m_bIsShow = bHide;
		}
	}

	void CNpcPatrolAction::ClearRegionData()
	{
		UNREACHREGIONMAP::iterator it;
		UNREACHREGIONMAP::iterator itEnd;

		CUnreachRegion* pRegion = NULL;
		itEnd = m_UnreachRegionMap.end();
		for(it = m_UnreachRegionMap.begin(); it != itEnd; it++)
		{
			pRegion = it->second;
			if(pRegion)
			{
				// 如果场景节点存在
				if(m_pSceneNode)
				{
					// 删除这个区域的所有实体.
					pRegion->DetachAllEntity();
					//pRegion->m_pSceneNode->removeAndDestroyAllChildren();
					
					// 删除区域的所有子节点
					pRegion->DestroyAllChildSceneNode();

					// 删除场景节点中区域节点.
					m_pSceneNode->removeAndDestroyChild(pRegion->m_pSceneNode->getName());
					//
					
				}

				delete pRegion;
			}
		}

		m_UnreachRegionMap.clear();

		m_ulCurMaxId = 0;

	}

	// 选择一个区域
	void CNpcPatrolAction::SelRegion(unsigned long ulId, bool bIsChangeCamera)
	{
		if(m_UnreachRegionMap.count(ulId))
		{
			m_ulCurEditRegionId = ulId;
			CUnreachRegion* pRegion = NULL;
			pRegion = m_UnreachRegionMap[ulId];
			pRegion->SetRegionSelMaterial();

			if(bIsChangeCamera)
			{
				if((pRegion->m_pLineShape)&&(pRegion->m_pLineShape->m_pointArray.size()))
				{
					
					Ogre::Vector3 point = pRegion->m_pLineShape->m_pointArray[0];
					SetCamera(point.x, point.z);
					
				}
			}

		}
	}

	// 不选择一个区域.
	void CNpcPatrolAction::UnSelRegion(unsigned long ulId)
	{
		if(m_UnreachRegionMap.count(ulId))
		{
			CUnreachRegion* pRegion = NULL;
			pRegion = m_UnreachRegionMap[ulId];
			pRegion->SetRegionNorMaterial();

		}
	}


	// 从区域读取文件
	unsigned long CNpcPatrolAction::ReadRegionFromFile(std::string& strFileName)
	{
		FILE* pFile = NULL;
		pFile = ::fopen(strFileName.data(), "r+b");

		if(NULL == pFile) return 0xfffffffe;
		fclose( pFile );

		// 清空以前的数据.
		ClearRegionData();

		Ogre::Vector3   position;

		float fScale = 1.0;
		float fTerrainDataPosX = getTerrainData()->mPosition.x;
		float fTerrainDataPosZ = getTerrainData()->mPosition.z;

		if(getSceneManipulator())
		{
			fScale = getSceneManipulator()->getBaseScale();
			if(abs((float)fScale) < 0.00001)
			{
				fScale = 1.0;
			}
		}

		char szPATROL[20];
		char szPOS[10];
		char szBUFFER[20];

		int iRegionCount = GetPrivateProfileInt("INFO", "PATROLNUMBER", 0, strFileName.c_str());
		for(int i = 0; i < iRegionCount; i++)
		{
			m_ulCurEditRegionId = AddNewRegion();

			sprintf( szPATROL, "PATROL%d", i );
			int iPointCount = GetPrivateProfileInt(szPATROL, "PATROLPOINTNUM", 0, strFileName.c_str());
			for( int j = 0; j < iPointCount; j ++ )
			{
				sprintf( szPOS, "POSX%d", j );
				DWORD dwRet = GetPrivateProfileString(szPATROL, szPOS, "", szBUFFER, 20, strFileName.c_str());
				position.x = dwRet > 0 ? atof( szBUFFER ) : 0.0f;

				sprintf( szPOS, "POSZ%d", j );
				dwRet = GetPrivateProfileString(szPATROL, szPOS, "", szBUFFER, 20, strFileName.c_str());
				position.z = dwRet > 0 ? atof( szBUFFER ) : 0.0f;

				sprintf( szPOS, "POSY%d", j );
				dwRet = GetPrivateProfileString(szPATROL, szPOS, "", szBUFFER, 20, strFileName.c_str());
				position.y = dwRet > 0 ? atof( szBUFFER ) : 0.0f;

				position.x = position.x * fScale + fTerrainDataPosX;
				position.z = position.z * fScale + fTerrainDataPosZ;
				if( position.y != -100000.0f && dwRet > 0 )
				{
					position.y = position.y * fScale;
				}
				else
				{
					position.y = -100000.0f;
				}

				AddNewPoint(position);
			}
		}

		// 根据显示状态， 显示场景.
		HideScene(m_bIsShow);

		return 0;
	}
		
	// 把区域存储到文件
	unsigned long CNpcPatrolAction::SaveRegionToFile(std::string strFileName)
	{

		FILE* pFile = NULL;
		//pFile = ::fopen(strFileName.data(), "wb");

		//if(NULL == pFile)
		//{
		//	// 不能打开文件.
		//	return 0xfffffffe;
		//}

		int			iRegionCount = 0;
		int			iPointCount  = 0;
		int         iId			 = 0;
		unsigned long ulId       = 0;

		CUnreachRegion* pRegion  = NULL;
		CPointInfo*		pPoint	 = NULL;
		Ogre::Vector3   position;

		UNREACHREGIONMAP::iterator itRegion;
		UNREACHREGIONMAP::iterator itEndRegion;

		POINT_MAP::iterator itPoint;
		POINT_MAP::iterator itEndPoint;

		// 存储区域的个数
		iRegionCount = m_UnreachRegionMap.size();
		//::fwrite(&iRegionCount, sizeof(iRegionCount), 1, pFile);

		// 存储每个区域的信息
		itEndRegion = m_UnreachRegionMap.end();

		///////////////////////////////////////////////////////////////////////////////////////////////
		//
		// 保存把场景坐标转换成游戏逻辑坐标.
		//

		//SaveX
		float x = 0;
		//SaveZ
		float z = 0;
		//SaveY
		float y = 0;

		// 缩放系数
		float fScale = 1.0;
		float fTerrainDataPosX = 0;
		float fTerrainDataPosZ = 0;

		fTerrainDataPosX = getTerrainData()->mPosition.x;
		fTerrainDataPosZ = getTerrainData()->mPosition.z;

		if(getSceneManipulator())
		{
			fScale = getSceneManipulator()->getBaseScale();
			if(abs((float)fScale) < 0.00001)
			{
				fScale = 1.0;
			}
		}
		

		std::string strNpcPatrolFileIni = "";
		std::string strNpcPatrolFile    = "";

		strNpcPatrolFile    = strFileName;
		int iPos = strNpcPatrolFile.rfind(".");

		strNpcPatrolFileIni = strNpcPatrolFile.substr(0, iPos);
		strNpcPatrolFileIni += "_patrolpoint.ini";

		// 删除旧的文件。
		::DeleteFile(strNpcPatrolFileIni.c_str());

		char bufInfo[1024];
		memset(bufInfo, 0, sizeof(bufInfo));

		// 存储区域的个数
		iRegionCount = m_UnreachRegionMap.size();
		sprintf(bufInfo, "%d", iRegionCount);
		WritePrivateProfileString("INFO", "PATROLNUMBER", bufInfo, strNpcPatrolFileIni.c_str());
		

		// 存储每个区域的信息
		itEndRegion = m_UnreachRegionMap.end();
		///////////////////////////////////////////////////////////////////////////////////////////////
		//
		// 保存把场景坐标转换成游戏逻辑坐标.
		//
		fTerrainDataPosX = getTerrainData()->mPosition.x;
		fTerrainDataPosZ = getTerrainData()->mPosition.z;
		if(getSceneManipulator())
		{
			fScale = getSceneManipulator()->getBaseScale();
			if(abs((float)fScale) < 0.00001)
			{
				fScale = 1.0;
			}
		}

		char bufPosInfo[1024];
		char bufPosValue[1024];

		int iRegionIndex = 0;
		int iPointIndex  = 0;
		for(itRegion = m_UnreachRegionMap.begin(); itRegion != itEndRegion; itRegion++)
		{
			pRegion = itRegion->second;
			if(pRegion)
			{
				memset(bufInfo, 0, sizeof(bufInfo));
				sprintf(bufInfo, "PATROL%d", iRegionIndex);
				iRegionIndex++;
			
				// 存储点的个数.
				iPointCount = pRegion->m_pointMap.size();
				sprintf(bufPosInfo, "%d", iPointCount);
				WritePrivateProfileString(bufInfo, "PATROLPOINTNUM", bufPosInfo, strNpcPatrolFileIni.c_str());

				// 存储每个点的信息。
				itEndPoint = pRegion->m_pointMap.end();
				iPointIndex = 0;
				for(itPoint = pRegion->m_pointMap.begin(); itPoint != itEndPoint; itPoint++)
				{
					pPoint = itPoint->second;
					if(pPoint)
					{
						position  = pPoint->m_Position;
						x = position.x;
						y = position.y;
						z = position.z;

						x = (x - fTerrainDataPosX) / fScale;
						y = (y) / fScale;
						z = (z - fTerrainDataPosZ) / fScale;

						memset(bufPosInfo, 0, sizeof(bufPosInfo));
						sprintf(bufPosInfo, "POSX%d", iPointIndex);

						memset(bufPosValue, 0, sizeof(bufPosValue));
						sprintf(bufPosValue, "%.4f", x);
						WritePrivateProfileString(bufInfo, bufPosInfo, bufPosValue, strNpcPatrolFileIni.c_str());

						if( pPoint->GetValuedY() )
						{
							memset(bufPosInfo, 0, sizeof(bufPosInfo));
							sprintf(bufPosInfo, "POSY%d", iPointIndex);

							memset(bufPosValue, 0, sizeof(bufPosValue));
							sprintf(bufPosValue, "%.4f", y);
							WritePrivateProfileString(bufInfo, bufPosInfo, bufPosValue, strNpcPatrolFileIni.c_str());
						}
						else
						{
							//memset(bufPosInfo, 0, sizeof(bufPosInfo));
							//sprintf(bufPosInfo, "POSY%d", iPointIndex);

							//memset(bufPosValue, 0, sizeof(bufPosValue));
							//sprintf(bufPosValue, "%.4f", -100000.0f);
							//WritePrivateProfileString(bufInfo, bufPosInfo, bufPosValue, strNpcPatrolFileIni.c_str());
						}

						memset(bufPosInfo, 0, sizeof(bufPosInfo));
						sprintf(bufPosInfo, "POSZ%d", iPointIndex);

						memset(bufPosValue, 0, sizeof(bufPosValue));
						sprintf(bufPosValue, "%.4f", z);
						WritePrivateProfileString(bufInfo, bufPosInfo, bufPosValue, strNpcPatrolFileIni.c_str());
						iPointIndex++;
					}
				}

				//iRegionIndex++;
			}
		}
	
		//
		// 存储服务器需要的配置文件。
		//
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		return 0;

	}// unsigned long CNpcPatrolAction::SaveRegionToFile(std::string strFileName)

	//根据Region计算出格子是否行走信息
	void CNpcPatrolAction::calculateNavInfo(std::vector<ScanLine>& lines)
	{
		//int minx,maxx,miny,maxy;
		//minx = miny = 1024;
		//maxx = maxy = 0;

		//for(int i=0;i<(int)lines.size();i++)
		//{
		//	Point ptStart = lines[i].getStartPoint(); 

		//	minx = Ogre::Math::Floor( std::min((float)minx,ptStart.x));
		//	miny = Ogre::Math::Floor( std::min((float)miny,ptStart.y));
		//	maxx = Ogre::Math::Ceil( std::max((float)maxx,ptStart.x));
		//	maxy = Ogre::Math::Ceil( std::max((float)maxy,ptStart.y));
		//}

		//for(int y = miny;y<maxy;y++)
		//{	
		//	for(int x = minx;x<maxx;x++)
		//	{
		//		//得到穿过的边数
		//		int count = 0;
		//		//当前的y值
		//		for(int i=0;i<(int)lines.size();i++)
		//		{
		//			const Point ptStart =lines[i].getStartPoint(); 		
		//			const Point ptEnd =lines[i].getEndPoint();

		//			
		//			if(Ogre::Math::Abs(ptEnd.y-ptStart.y) <0.01)
		//				continue;


		//			float curx = ptStart.x + (y - ptStart.y) * (ptEnd.x - ptStart.x) / (ptEnd.y-ptStart.y);

		//			float startx  = std::min(ptStart.x,ptEnd.x);
		//			float endx = std::max(ptStart.x,ptEnd.x);

		//			if(curx> startx && curx < endx && curx < x)
		//				count ++;
		//		}

		//		if(count % 2)
		//		{
		//			mWorld[ y * mWidth + x] = 1;
		//		}
		//	}
		//}//
	}

	int CNpcPatrolAction::dumpToNavMapFile(std::string& strFileName)
	{
		FILE* pFile = fopen(strFileName.data(), "wb");

		if( NULL == pFile)
			return 0xfffffffe;

		NavMapHeader header;
		header.height = mHeight;
		header.width = mWidth;

		fwrite((void*)&header,sizeof(struct NavMapHeader),1,pFile);
		fwrite((void*)mWorld,4 * header.height * header.width,1,pFile);	

		fclose(pFile);

		return 0;
	}

	// 把区域信息转储为服务器需要的导航图文件
	// 注意:乘以2是因为1m的格子被切分为2
	unsigned long CNpcPatrolAction::SaveRegionToNavMapFile(std::string& strFileName)
	{

		return 0;
	}

	// 根据地形的高度改变编辑的区域。
	void CNpcPatrolAction::SetRegionHeight()
	{
		UNREACHREGIONMAP::iterator it;
		UNREACHREGIONMAP::iterator itEnd;

		CUnreachRegion* pUnreachRegion = NULL;

		itEnd = m_UnreachRegionMap.end();
		for(it = m_UnreachRegionMap.begin(); it != itEnd; it++)
		{
			pUnreachRegion = it->second;
			if(pUnreachRegion)
			{
				pUnreachRegion->SetPointHeight(getTerrainData());
				pUnreachRegion->SetLineHeight(getTerrainData());
			}
		}
	}


	void CNpcPatrolAction::SetCamera(float fx, float fy)
	{
		//float fHeight = getSceneManipulator()->getRealCameraHeight();
		//getSceneManipulator()->setRealCamera(fx, fy);
		//getSceneManipulator()->setRealCameraHeight(fHeight);//

		const Ogre::Vector3 OldPos = getSceneManipulator()->getCamera()->getPosition();
		Ogre::Vector3 NewPos = OldPos;
		//Ogre::Vector3 dir(0, 1.0f, 0);
		NewPos.x = fx;
		NewPos.z = fy;
		getSceneManipulator()->getCamera()->setPosition(NewPos);
		//getSceneManipulator()->getCamera()->setDirection(dir);

	}

	void CNpcPatrolAction::SetCamera()
	{

	}

	// 选择一个点.
	void CNpcPatrolAction::SelPoint(unsigned long ulPointId)
	{
		m_ulCurEditPointId = ulPointId;

		CUnreachRegion* pRegion = NULL;
		CPointInfo*     pPoint  = NULL;

		// 得到当前正在编辑的区域的信息.
		pRegion = GetRegionById(m_ulCurEditRegionId);
		if(pRegion)
		{
			pRegion->SetPrePointMaterial();
			pRegion->m_ulCurEditPointId = m_ulCurEditPointId;
			pPoint = pRegion->GetPointInfoById(ulPointId);
			if(pPoint)
			{
				pPoint->SetSelMaterial();
			}
		}
	}


	// 设置按下的功能键
	void CNpcPatrolAction::SetFuncKey(unsigned long ulFuncKey)
	{
		m_FuncKeyState |= ulFuncKey;
	}

	//  清空功能键.
	void CNpcPatrolAction::ClearFuncKey()
	{
		m_FuncKeyState = FUNCTION_KEY;
	}


	void CNpcPatrolAction::setParameter(const String& name, const String& value)
    {
        if (name == "FUNC_KEY")
        {
           if("SHIFT" == value)
		   {
			   SetFuncKey(FUNC_KEY_SHIFT);
		   }
		   else if("CTRL" == value)
		   {
			   SetFuncKey(FUNC_KEY_CTRL);
		   }
		   else if("ATL")
		   {
			   SetFuncKey(FUNC_KEY_ALT);
		   }
        }
    
    }

	// 改变当前编辑的点的位置
	void CNpcPatrolAction::ChangeCurEditPointPos(const Point& pt)
	{
		CUnreachRegion* pRegion = NULL;
		CPointInfo*     pPoint  = NULL;

		// 得到当前正在编辑的区域的信息.
		pRegion = GetRegionById(m_ulCurEditRegionId);
		if(pRegion)
		{
			pPoint = pRegion->GetPointInfoById(m_ulCurEditPointId);
			if(pPoint)
			{	
				
				// 查找这个点的下标值.
				int iIndex = -1;
				POINT_MAP::iterator it;
				POINT_MAP::iterator itEnd;
				itEnd = pRegion->m_pointMap.end();
				for(it = pRegion->m_pointMap.begin(); it != itEnd; it++)
				{
					iIndex++;
					if(m_ulCurEditPointId == it->first)
					{
						break;
					}
				}

				
				// 得到修改后的位置.
				Ogre::Vector3 position;
		        bool hit = getSceneManipulator()->getTerrainIntersects(pt.x, pt.y, position);
				if(hit)
				{
					
					//暂时把判断能否添加一个点去掉.
					//if(pRegion->CanChangePointPos(iIndex, position))
					{
						// 修改新的点的位置.
						pPoint->SetPos(position);

						// 当前编辑的点的位置.
						m_curPointPos = position;
						
						if(it != itEnd)
						{
							// 如果下标存在
							if((-1 != iIndex)&&(iIndex < (int)pRegion->m_pLineShape->m_pointArray.size()))
							{
								pRegion->m_pLineShape->m_pointArray[iIndex] = position;
								pRegion->m_pLineShape->FillPointData();
								//pRegion->ReShapeArea();
							}
						}

					}// if(pRegion->CanChangePointPos(iIndex, position))

				}// if(hit)

			}// if(pPoint)

		}// if(pRegion)
	}



void CNpcPatrolAction::ChangeCurEditPointPos(unsigned long ulRegionId, unsigned long ulPointId, Ogre::Vector3 position)
{

	CUnreachRegion* pRegion = NULL;
	CPointInfo*     pPoint  = NULL;

	// 得到当前正在编辑的区域的信息.
	pRegion = GetRegionById(ulRegionId);
	if(pRegion)
	{
		pPoint = pRegion->GetPointInfoById(ulPointId);
		if(pPoint)
		{	
			
			// 查找这个点的下标值.
			int iIndex = -1;
			POINT_MAP::iterator it;
			POINT_MAP::iterator itEnd;
			itEnd = pRegion->m_pointMap.end();
			for(it = pRegion->m_pointMap.begin(); it != itEnd; it++)
			{
				iIndex++;
				if(ulPointId == it->first)
				{
					break;
				}
			}

			pPoint->SetPos(position);
			
			// 如果下标存在
			if((-1 != iIndex)&&(iIndex < (int)pRegion->m_pLineShape->m_pointArray.size()))
			{
				pRegion->m_pLineShape->m_pointArray[iIndex] = position;
				pRegion->m_pLineShape->FillPointData();
				//pRegion->ReShapeArea();
			}
					
		}// if(pPoint)

	}// if(pRegion)

}

// 记录当前操作前一次点的位置.
void CNpcPatrolAction::RecordPreDoPointPos()
{
	
	CUnreachRegion* pRegion = NULL;
	CPointInfo*     pPoint  = NULL;

	// 得到当前正在编辑的区域的信息.
	pRegion = GetRegionById(m_ulCurEditRegionId);
	if(pRegion)
	{
		pPoint = pRegion->GetPointInfoById(m_ulCurEditPointId);
		if(pPoint)
		{	
			m_preDoPointPos = pPoint->m_Position;		

		}// if(pPoint)

	}// if(pRegion)

}

// 指定区域id， 指定点的id， 指定位置， 添加一个小球.
void CNpcPatrolAction::AddNewPoint(unsigned long ulRegionId, unsigned long ulPointId, Ogre::Vector3 position)
{
	// 得到当前正在编辑的区域的信息.
	CUnreachRegion* pRegion = NULL;
	pRegion = GetRegionById(ulRegionId);
	if(pRegion)
	{
		pRegion->AddNewPoint(ulPointId, position);

		getSceneManipulator()->_fireRegionAddNewPoint(ulRegionId, ulPointId, position);
	}
}

	
// 指定区域id， 指定点的id， 指定位置， 删除一个小球.
void CNpcPatrolAction::DelPoint(unsigned long ulRegionId, unsigned long ulPointId)
{
	// 得到当前正在编辑的区域的信息.
	CUnreachRegion* pRegion = NULL;
	CPointInfo*     pPoint  = NULL;

	pRegion = GetRegionById(ulRegionId);
	if(pRegion)
	{
		pRegion->DelPointFromTail(ulPointId);

		getSceneManipulator()->_fireRegionDelPoint(ulRegionId, ulPointId);
	}
}


// 当前的区域是否合法
bool CNpcPatrolAction::IsRegionValidate(unsigned long ulRegionId)
{
	CUnreachRegion* pRegion = NULL;

	// 得到当前正在编辑的区域的信息.
	pRegion = GetRegionById(ulRegionId);
	if(pRegion)
	{
		if(pRegion->IsRegionValidate())
		{
			return true;
		}
	}

	return false;
}

	// 隐藏一个区域的面积
void CNpcPatrolAction::HideRegionArea(unsigned long ulRegionId, bool bHide)
{
	CUnreachRegion* pRegion = NULL;

	// 得到当前正在编辑的区域的信息.
	pRegion = GetRegionById(ulRegionId);
	if(pRegion)
	{
		
		pRegion->HideAreaNode(bHide);

	}
}

// 重新创建显示的面积区域
void CNpcPatrolAction::ReshapeArea(unsigned long ulRegionId)
{
	CUnreachRegion* pRegion = NULL;
	// 得到当前正在编辑的区域的信息.
	pRegion = GetRegionById(ulRegionId);
	if(pRegion)
	{
		
		pRegion->ReShapeArea();

	}
}


}
