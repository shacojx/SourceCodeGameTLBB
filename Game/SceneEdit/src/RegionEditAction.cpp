#include "RegionEditAction.h"
#include "SceneManipulator.h"
#include "HitIndicator.h"
#include "IntersectPoint.h"
#include "UnreachRegion.h"
#include "CustomShape.h"
#include "regioneditoperator.h"		// 不可行走区域的redo undo 操作.
#include "WXOperatorManager.h"       // redo undo 管理器.
#include "ScanLine.h"
#include "OgreCamera.h"

#include "Core/TerrainData.h"
#include "Core/WXResources.h"

namespace WX 
{
	using namespace Ogre;

	 CRegionEditAction::CRegionEditAction(SceneManipulator* sceneManipulator)
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
		InitRegionEdit();

		// 测试使用.
		//Test_AddRegion();
		m_bIsShow = false;

		m_iPreOperatorType = NO_OPERATOR;

		m_bShowDeepBuf = true;
		
    }

	CRegionEditAction::~CRegionEditAction(void)
	{
		ClearRegionData();

        if (m_pSceneNode)
        {
            m_pSceneNode->destroy();
        }
	}

	const String& CRegionEditAction::getName(void) const
	{
		//返回一个字符串名字， 用来判断当前操作的类型.
		static const String name = "RegionEdit";
		return name;
	}

    
	// 鼠标移动的操作
    void CRegionEditAction::_onMove(const Point& pt)
	{
		getSceneManipulator()->getHitIndicator("IntersectPoint")->setHitPoint(pt.x, pt.y);
		ClearFuncKey();
		
	}


	// 鼠标按下的操作.
	void CRegionEditAction::_onBegin(const Point& pt)
	{
		// 记录当前操作， 的点的位置.
		RecordPreDoPointPos();
	}


    void CRegionEditAction::_onDrag(const Point& pt)
	{
		if(m_FuncKeyState & FUNC_KEY_SHIFT)
		{
			// 如果shift键按下允许修改不可行走区域点的位置.
			ChangeCurEditPointPos(pt);
			m_iPreOperatorType = MODITY_POINT_POSITION;
		}
		else//
		{
			getSceneManipulator()->getHitIndicator("IntersectPoint")->setHitPoint(pt.x, pt.y);
		}

		// 每次执行结束清空功能键
		ClearFuncKey();
			
	}


	// 鼠标抬起的操作
    void CRegionEditAction::_onEnd(const Point& pt, bool canceled)
	{
		if(!getActive())
		{
			return;
		}

		if(m_FuncKeyState & FUNC_KEY_SHIFT)
		{
			// 如果shift键按下允许修改不可行走区域点的位置.
			CRegionEditModifyOperator* pRegionEditModifyOperator = NULL;
			pRegionEditModifyOperator = new CRegionEditModifyOperator;

			pRegionEditModifyOperator->SetRegionAction(this);
			pRegionEditModifyOperator->SetPointId(m_ulCurEditPointId);
			pRegionEditModifyOperator->SetRegionId(m_ulCurEditRegionId);
			pRegionEditModifyOperator->SetPosition(m_preDoPointPos, m_curPointPos);
			getOperatorManager()->addOperator(pRegionEditModifyOperator);

		
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
				CRegionEditAddPointOperator* pAddNewPointOperator = NULL;
				pAddNewPointOperator = new CRegionEditAddPointOperator;

				pAddNewPointOperator->SetRegionAction(this);
				pAddNewPointOperator->SetPointId(m_ulCurEditPointId);
				pAddNewPointOperator->SetRegionId(m_ulCurEditRegionId);
				pAddNewPointOperator->SetPosition(m_preDoPointPos, m_curPointPos);
				getOperatorManager()->addOperator(pAddNewPointOperator);
	            
				m_iPreOperatorType = ADD_NEW_POINT;
			}
		}


		// 清空功能键
		ClearFuncKey();

	}
    void CRegionEditAction::_onActive(bool active)
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
	// 不可行走区域管理函数
	//

	// 添加一个新的区域
	unsigned long CRegionEditAction::AddNewRegion()
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
	unsigned long CRegionEditAction::DelRegion(unsigned long ulId)
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
	CUnreachRegion* CRegionEditAction::GetRegionById(unsigned long ulId)
	{

		if(m_UnreachRegionMap.count(ulId))
		{
			CUnreachRegion* pRegion = NULL;
			pRegion = m_UnreachRegionMap[ulId];

			return pRegion;
		}

        return NULL;
	}


	void CRegionEditAction::InitRegionEdit()
	{
		CreateSceneNode();
	}

	void CRegionEditAction::SetOldHitPointMaterial()
	{
		IntersectPoint* pPoint = (IntersectPoint*)getSceneManipulator()->getHitIndicator("IntersectPoint");
		pPoint->SetMaterial();
	}

	// 创建区域编辑的场景节点
	void CRegionEditAction::CreateSceneNode()
	{
		m_pSceneNode = getIndicatorRootSceneNode()->createChildSceneNode();
		if(m_pSceneNode)
		{
			m_pSceneNode->setVisible(false);
		}
		
	}

	// 在当前编辑的区域中添加一个新的点
	unsigned long CRegionEditAction::AddNewPoint(Ogre::Vector3 position)
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
			return m_ulCurEditPointId;
		}

		return 0;
		
	}

	bool CRegionEditAction::EditPoint(const Point& pt)
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

	void CRegionEditAction::Test_AddRegion()
	{
		AddNewRegion();
	}

	void CRegionEditAction::HideScene(bool bHide)
	{
		if(m_pSceneNode)
		{
			m_pSceneNode->setVisible(bHide);
			m_bIsShow = bHide;
		}
	}

	void CRegionEditAction::ClearRegionData()
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
	void CRegionEditAction::SelRegion(unsigned long ulId, bool bIsChangeCamera)
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
	void CRegionEditAction::UnSelRegion(unsigned long ulId)
	{
		if(m_UnreachRegionMap.count(ulId))
		{
			CUnreachRegion* pRegion = NULL;
			pRegion = m_UnreachRegionMap[ulId];
			pRegion->SetRegionNorMaterial();


		}
	}

	// 从区域读取文件
	unsigned long CRegionEditAction::ReadRegionFromFileVer1(std::string strFileName)
	{
		FILE* pFile = fopen( strFileName.c_str(), "rb" );

		if(NULL == pFile)
		{
			// 不能打开文件.
			return 0xfffffffe;
		}

		int			iVerLen = (int)sizeof(REGION_EDIT_VER1);
		char*		pVer = new char[iVerLen];

		fread( pVer, 1, iVerLen-1, pFile );
		pVer[iVerLen-1] = 0;
		if( strcmp( pVer, REGION_EDIT_VER1 ) != 0 )
		{
			delete [] pVer;
			fclose( pFile );
			return 0xfffffffe;
		}

		delete [] pVer;

		// 清空以前的数据.
		ClearRegionData();

		int			iRegionCount = 0;
		int			iPointCount  = 0;
		int         iId			 = 0;
		int         iFlag        = 0;
		unsigned long ulId       = 0;
		CUnreachRegion* pRegion  = NULL;
		Ogre::Vector3   position;


		////////////////////////////////////////////////////////////////////
		//
		//  坐标转换参数
		//

		//SaveX
		float x = 0;
		//SaveZ
		float z = 0;

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


		// 读取区域的个数
		::fread(&iRegionCount, sizeof(iRegionCount), 1, pFile);

		for(int i = 0; i < iRegionCount; i++)
		{
			// 读取区域的id
			::fread(&iId, sizeof(iId), 1, pFile);

			// 读取区域的Flag
			::fread(&iFlag, sizeof(iFlag), 1, pFile);

			// 设置当前正在编辑的id。
			m_ulCurEditRegionId = AddNewRegion();
			CUnreachRegion* pRegion = NULL;
			pRegion = m_UnreachRegionMap[m_ulCurEditRegionId];
			if( pRegion ) pRegion->SetFlyable( iFlag );

			// 读取点的个数.
			::fread(&iPointCount, sizeof(iPointCount), 1, pFile);
			for(int k = 0; k < iPointCount; k++)
			{
				// 读取x， z 数据
				::fread(&position.x, sizeof(position.x), 1, pFile);
				::fread(&position.z, sizeof(position.z), 1, pFile);
				::fread(&position.y, sizeof(position.y), 1, pFile);

				position.x = position.x * fScale + fTerrainDataPosX;
				position.z = position.z * fScale + fTerrainDataPosZ;

				AddNewPoint(position);
			}

			//if( i != 133)
			{
				// 刷新新的区域
				ReshapeArea(m_ulCurEditRegionId);
			}

		}

		::fclose(pFile);

		// 根据显示状态， 显示场景.
		HideScene(m_bIsShow);

		return 0;
	}

	// 把区域存储到文件
	unsigned long CRegionEditAction::SaveRegionToFileVer1(std::string strFileName)
	{
		FILE* pFile = NULL;
		pFile = ::fopen(strFileName.data(), "wb");

		if(NULL == pFile)
		{
			// 不能打开文件.
			return 0xfffffffe;
		}

		fwrite( REGION_EDIT_VER1, 1, strlen(REGION_EDIT_VER1), pFile );

		int			iRegionCount = 0;
		int			iPointCount  = 0;
		int         iId			 = 0;
		int			iFlag        = 0;
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
		::fwrite(&iRegionCount, sizeof(iRegionCount), 1, pFile);

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

		for(itRegion = m_UnreachRegionMap.begin(); itRegion != itEndRegion; itRegion++)
		{
			pRegion = itRegion->second;
			if(pRegion)
			{
				// 存储区域的id
				iId = pRegion->m_ulId;
				::fwrite(&iId, sizeof(iId), 1, pFile);

				// 存储区域的iFlag
				iFlag = pRegion->GetFlyable();
				if( pRegion->GetFlyable() ) SET_FLYABLE(iFlag);
				::fwrite(&iFlag, sizeof(iFlag), 1, pFile);

				// 存储点的个数.
				iPointCount = pRegion->m_pointMap.size();
				::fwrite(&iPointCount, sizeof(iPointCount), 1, pFile);

				// 存储每个点的信息。
				itEndPoint = pRegion->m_pointMap.end();
				for(itPoint = pRegion->m_pointMap.begin(); itPoint != itEndPoint; itPoint++)
				{
					pPoint = itPoint->second;
					if(pPoint)
					{
						position  = pPoint->m_Position;
						x = position.x;
						z = position.z;

						x = (x - fTerrainDataPosX) / fScale;
						z = (z - fTerrainDataPosZ) / fScale;

						::fwrite(&x, sizeof(x), 1, pFile);
						::fwrite(&z, sizeof(z), 1, pFile);
						::fwrite(&position.y, sizeof(position.y), 1, pFile);
					}
				}
			}
		}

		::fclose(pFile);

		return 0;
	}

	// 从区域读取文件，旧版本
	unsigned long CRegionEditAction::ReadRegionFromFileVer0(std::string strFileName)
	{
		FILE* pFile = fopen( strFileName.c_str(), "rb" );

		if(NULL == pFile)
		{
			// 不能打开文件.
			return 0xfffffffe;
		}

		// 清空以前的数据.
		ClearRegionData();

		int			iRegionCount = 0;
		int			iPointCount  = 0;
		int         iId			 = 0;
		unsigned long ulId       = 0;
		CUnreachRegion* pRegion  = NULL;
		Ogre::Vector3   position;


		////////////////////////////////////////////////////////////////////
		//
		//  坐标转换参数
		//

		//SaveX
		float x = 0;
		//SaveZ
		float z = 0;

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


		// 读取区域的个数
		::fread(&iRegionCount, sizeof(iRegionCount), 1, pFile);

		for(int i = 0; i < iRegionCount; i++)
		{
			// 读取区域的id
			::fread(&iId, sizeof(iId), 1, pFile);

			// 设置当前正在编辑的id。
			m_ulCurEditRegionId = AddNewRegion();

			// 读取点的个数.
			::fread(&iPointCount, sizeof(iPointCount), 1, pFile);
			for(int k = 0; k < iPointCount; k++)
			{
				// 读取x， z 数据
				::fread(&position.x, sizeof(position.x), 1, pFile);
				::fread(&position.z, sizeof(position.z), 1, pFile);
				::fread(&position.y, sizeof(position.y), 1, pFile);

				position.x = position.x * fScale + fTerrainDataPosX;
				position.z = position.z * fScale + fTerrainDataPosZ;

				AddNewPoint(position);
			}

			//if( i != 133)
			{
				// 刷新新的区域
				ReshapeArea(m_ulCurEditRegionId);
			}

		}

		::fclose(pFile);

		// 根据显示状态， 显示场景.
		HideScene(m_bIsShow);

		return 0;
	}

	// 把区域存储到文件，旧版本
	unsigned long CRegionEditAction::SaveRegionToFileVer0(std::string strFileName)
	{
		FILE* pFile = NULL;
		pFile = ::fopen(strFileName.data(), "wb");

		if(NULL == pFile)
		{
			// 不能打开文件.
			return 0xfffffffe;
		}

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
		::fwrite(&iRegionCount, sizeof(iRegionCount), 1, pFile);

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

		for(itRegion = m_UnreachRegionMap.begin(); itRegion != itEndRegion; itRegion++)
		{
			pRegion = itRegion->second;
			if(pRegion)
			{
				// 存储区域的id
				iId = pRegion->m_ulId;
				::fwrite(&iId, sizeof(iId), 1, pFile);

				// 存储点的个数.
				iPointCount = pRegion->m_pointMap.size();
				::fwrite(&iPointCount, sizeof(iPointCount), 1, pFile);

				// 存储每个点的信息。
				itEndPoint = pRegion->m_pointMap.end();
				for(itPoint = pRegion->m_pointMap.begin(); itPoint != itEndPoint; itPoint++)
				{
					pPoint = itPoint->second;
					if(pPoint)
					{
						position  = pPoint->m_Position;
						x = position.x;
						z = position.z;

						x = (x - fTerrainDataPosX) / fScale;
						z = (z - fTerrainDataPosZ) / fScale;

						::fwrite(&x, sizeof(x), 1, pFile);
						::fwrite(&z, sizeof(z), 1, pFile);
						::fwrite(&position.y, sizeof(position.y), 1, pFile);
					}
				}
			}
		}

		::fclose(pFile);

		return 0;
	}

	// 从区域读取文件
	unsigned long CRegionEditAction::ReadRegionFromFile(std::string strFileName)
	{
		FILE* fp = fopen( strFileName.c_str(), "rb" );
		if( !fp ) return 0xfffffffe;

		char buffer[20];
		fread( buffer, 1, 20, fp );
		fclose( fp );
		if( strncmp( buffer, REGION_EDIT_VER1, strlen(REGION_EDIT_VER1) ) == 0 )
		{
			return ReadRegionFromFileVer1( strFileName );
		}
		else
		{
			return ReadRegionFromFileVer0( strFileName );
		}
	}
		
	// 把区域存储到文件
	unsigned long CRegionEditAction::SaveRegionToFile(std::string strFileName)
	{
		if( strcmp( REGION_EDIT_VER1, REGION_EDIT_VER1 ) == 0 )
		{
			return SaveRegionToFileVer1( strFileName );
		}
		else
		{
			return SaveRegionToFileVer0( strFileName );
		}
	}

	//根据Region计算出格子是否行走信息
	void CRegionEditAction::calculateNavInfo(std::vector<ScanLine>& lines,int PassLevel)
	{
		int minx,maxx,miny,maxy;
		minx = miny = 1024;
		maxx = maxy = 0;

		for(int i=0;i<(int)lines.size();i++)
		{
			Point ptStart = lines[i].getStartPoint(); 

			minx = Ogre::Math::Floor( std::min((float)minx,ptStart.x));
			miny = Ogre::Math::Floor( std::min((float)miny,ptStart.y));
			maxx = Ogre::Math::Ceil( std::max((float)maxx,ptStart.x));
			maxy = Ogre::Math::Ceil( std::max((float)maxy,ptStart.y));
		}

		for(int y = miny;y<maxy;y++)
		{	
			for(int x = minx;x<maxx;x++)
			{
				//得到穿过的边数
				int count_left0 = 0;
				int count_right0 = 0;
				int count_left1 = 0;
				int count_right1 = 0;

				//当前的y值
				for(int i=0;i<(int)lines.size();i++)
				{
					const Point ptStart =lines[i].getStartPoint(); 		
					const Point ptEnd =lines[i].getEndPoint();

					
					if(Ogre::Math::Abs(ptEnd.y-ptStart.y) <0.01)//与X轴平行
						continue;

					float k = (ptEnd.x - ptStart.x) / (ptEnd.y-ptStart.y);
					float curx0 = ptStart.x + (y - ptStart.y) * k;
					float curx1 = ptStart.x + (y+1 - ptStart.y) * k;

					float startx  = std::min(ptStart.x,ptEnd.x);
					float endx = std::max(ptStart.x,ptEnd.x);

					if(curx0> startx && curx0 < endx) //如果与Y=y直线有交点(交点就是curx0)
					{
						if( curx0 > x )
							count_left0 ++;
						if( curx0 < x + 1 )
							count_right0 ++;
					}
					if(curx1> startx && curx1 < endx) //如果与Y=y+1直线有交点(交点就是curx1)
					{
						if( curx1 > x )
							count_left1 ++;
						if( curx1 < x + 1 )
							count_right1 ++;
					}
				}
//fujia	2007.12.30
				if(count_left0 % 2 && count_right0 % 2 && count_left1 % 2 && count_right1 % 2)
				{
					mWorld[ y * mWidth + x] = PassLevel;
				}
			}
		}
	}

	int CRegionEditAction::dumpToNavMapFile(std::string& strFileName)
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
	unsigned long CRegionEditAction::SaveRegionToNavMapFile(std::string& strFileName)
	{

		int width = getTerrainData()->getXSize();
		int height = getTerrainData()->getZSize();
		width *= 2;
		height *= 2;

		mHeight = height;
		mWidth = width;

		mWorld = new int[width * height];
		memset((void*)mWorld,0,width * height * 4);

		int			iRegionCount = 0;
		int			iPointCount  = 0;
		int         iId			 = 0;
		unsigned long ulId       = 0;

		CUnreachRegion* pRegion  = NULL;
		CPointInfo*		pStartPoint	 = NULL;
		CPointInfo*		pEndPoint	 = NULL;

		Ogre::Vector3   position;

		UNREACHREGIONMAP::iterator itRegion;
		UNREACHREGIONMAP::iterator itEndRegion;

		POINT_MAP::iterator itPoint;
		POINT_MAP::iterator itBeginPoint;
		POINT_MAP::iterator itEndPoint;
		POINT_MAP::iterator itNextPoint;
		POINT_MAP::iterator itPrevPoint;

		//SaveX
		float x = 0;
		//SaveZ
		float z = 0;

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
			fScale *= 0.5;
		}

		itEndRegion = m_UnreachRegionMap.end();
		for(itRegion = m_UnreachRegionMap.begin(); itRegion != itEndRegion; itRegion++)
		{
			pRegion = itRegion->second;

			//如果轻功可以飞越,则不考虑这个区域
			int Level = pRegion->GetFlyable();
			if(pRegion)
			{
				int j = 0;
				std::vector<ScanLine> scanLines;
				int count;
				
				itBeginPoint = pRegion->m_pointMap.begin();
				itEndPoint = pRegion->m_pointMap.end();
				for( itPoint = itBeginPoint ; itPoint != itEndPoint; itPoint++)
				{
					ScanLine line;
					count = 0;
					
					itPrevPoint = itPoint;
					itNextPoint = ++itPrevPoint;
					
					if(itNextPoint == itEndPoint)
						itNextPoint = itBeginPoint;

					pStartPoint = itPoint->second;
					if(pStartPoint)
					{
						position  = pStartPoint->m_Position;
						x = position.x;
						z = position.z;

						x = (x - fTerrainDataPosX) / fScale;
						z = (z - fTerrainDataPosZ) / fScale;
						line.setStartPoint(Point(x,z));
						count++;
					}

					pEndPoint = itNextPoint->second;
					if(pEndPoint)
					{
						position  = pEndPoint->m_Position;
						x = position.x;
						z = position.z;

						x = (x - fTerrainDataPosX) / fScale;
						z = (z - fTerrainDataPosZ) / fScale;
						line.setEndPoint(Point(x,z));
						count++;
					}

					assert(count == 2);
					if(count == 2)
						scanLines.push_back(line);
				}

				calculateNavInfo(scanLines,Level);
			}
		}

		dumpToNavMapFile(strFileName);

		delete [] mWorld; 

		return 0;
	}

	// 根据地形的高度改变编辑的区域。
	void CRegionEditAction::SetRegionHeight()
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


	void CRegionEditAction::SetCamera(float fx, float fy)
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

	void CRegionEditAction::SetCamera()
	{

	}

	// 选择一个点.
	void CRegionEditAction::SelPoint(unsigned long ulPointId)
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
	void CRegionEditAction::SetFuncKey(unsigned long ulFuncKey)
	{
		m_FuncKeyState |= ulFuncKey;
	}

	//  清空功能键.
	void CRegionEditAction::ClearFuncKey()
	{
		m_FuncKeyState = FUNCTION_KEY;
	}


	void CRegionEditAction::setParameter(const String& name, const String& value)
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
		   else if("ATL"  == value)
		   {
			   SetFuncKey(FUNC_KEY_ALT);
		   }
		   else if("ESC" == value)
		   {
				ChangeNoDeepMaterial();
		   }
        }
    
    }

	// 改变当前编辑的点的位置
	void CRegionEditAction::ChangeCurEditPointPos(const Point& pt)
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
					if(pRegion->CanChangePointPos(iIndex, position))
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



void CRegionEditAction::ChangeCurEditPointPos(unsigned long ulRegionId, unsigned long ulPointId, Ogre::Vector3 position)
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
void CRegionEditAction::RecordPreDoPointPos()
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
void CRegionEditAction::AddNewPoint(unsigned long ulRegionId, unsigned long ulPointId, Ogre::Vector3 position)
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
void CRegionEditAction::DelPoint(unsigned long ulRegionId, unsigned long ulPointId)
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
bool CRegionEditAction::IsRegionValidate(unsigned long ulRegionId)
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
void CRegionEditAction::HideRegionArea(unsigned long ulRegionId, bool bHide)
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
void CRegionEditAction::ReshapeArea(unsigned long ulRegionId)
{
	CUnreachRegion* pRegion = NULL;
	// 得到当前正在编辑的区域的信息.
	pRegion = GetRegionById(ulRegionId);
	if(pRegion)
	{	
		
		pRegion->ReShapeArea();
		pRegion->SetRegionNorMaterial();

	}
}

// 改变没有深度缓冲区的材质
void CRegionEditAction::ChangeNoDeepMaterial()
{
	if(m_UnreachRegionMap.size())
	{
		CUnreachRegion* pRegion = NULL;
		pRegion = m_UnreachRegionMap.begin()->second;

		if(pRegion)
		{
			m_bShowDeepBuf = !m_bShowDeepBuf;
			pRegion->ChangeNoDeepMaterial(m_bShowDeepBuf);
		}
		
	}
}


} 
