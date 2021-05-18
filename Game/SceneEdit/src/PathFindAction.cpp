#include "PathFindAction.h"
#include "SceneManipulator.h"
#include "HitIndicator.h"
#include "IntersectPoint.h"
#include "UnreachRegion.h"
#include "CustomShape.h"
#include "PathFindOperator.h"	
#include "WXOperatorManager.h"       // redo undo 管理器.
#include "ScanLine.h"
#include "OgreCamera.h"

#include "Core/TerrainData.h"
#include "Core/WXResources.h"
#include "RegionEditAction.h"

#include <windows.h>

namespace WX 
{
	using namespace Ogre;

	CPathFindAction::CPathFindAction(SceneManipulator* sceneManipulator)
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
		,m_ulCurEditRegionId(0)
	{

		// 初始化
		InitPathFind();

		// 测试使用.
		//Test_AddRegion();
		m_bIsShow = false;

		//m_iPreOperatorType = NO_OPERATOR;

	}

	CPathFindAction::~CPathFindAction(void)
	{
		ClearRegionData();

		if (m_pSceneNode)
		{
			m_pSceneNode->destroy();
		}
	}

	const String& CPathFindAction::getName(void) const
	{
		//返回一个字符串名字， 用来判断当前操作的类型.
		static const String name = "PathFind";
		return name;
	}


	// 鼠标移动的操作
	void CPathFindAction::_onMove(const Point& pt)
	{
		getSceneManipulator()->getHitIndicator("IntersectPoint")->setHitPoint(pt.x, pt.y);
		ClearFuncKey();

	}


	// 鼠标按下的操作.
	void CPathFindAction::_onBegin(const Point& pt)
	{
		// 记录当前操作， 的点的位置.
		RecordPreDoPointPos();
	}


	void CPathFindAction::_onDrag(const Point& pt)
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
	void CPathFindAction::_onEnd(const Point& pt, bool canceled)
	{
		if(!getActive())
		{
			return;
		}

		if(m_FuncKeyState & FUNC_KEY_SHIFT)
		{
			// 如果shift键按下允许修改不可行走区域点的位置.
			CPathFindEditModifyOperator* pPathFindModifyOperator = NULL;
			pPathFindModifyOperator = new CPathFindEditModifyOperator;

			pPathFindModifyOperator->SetRegionAction(this);
			pPathFindModifyOperator->SetPointId(m_ulCurEditPointId);
			pPathFindModifyOperator->SetPosition(m_preDoPointPos, m_curPointPos);//
			getOperatorManager()->addOperator(pPathFindModifyOperator);//

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
				CPathFindEditAddOperator* pAddNewPointOperator = NULL;
				pAddNewPointOperator = new CPathFindEditAddOperator;

				pAddNewPointOperator->SetRegionAction(this);
				pAddNewPointOperator->SetPointId(m_ulCurEditPointId);
				pAddNewPointOperator->SetPosition(m_preDoPointPos, m_curPointPos);//
				getOperatorManager()->addOperator(pAddNewPointOperator);

			}
		}


		// 清空功能键
		ClearFuncKey();

	}
	void CPathFindAction::_onActive(bool active)
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
	unsigned long CPathFindAction::AddNewRegion()
	{
		if( m_UnreachRegionMap.size() > 0 )
		{
			//ShowUnreachRegion();
			return m_ulCurEditRegionId;
		}

		m_ulCurMaxId++;
		unsigned long ulId = m_ulCurMaxId;

		CUnreachRegion* pRegion = NULL;
		pRegion = new CUnreachRegion(ulId, m_pSceneNode, getSceneManipulator());

		if(pRegion)
		{
			CRegionEditAction* pRegionAction = (CRegionEditAction*)getSceneManipulator()->_getAction("RegionEdit");
			pRegion->m_pLineShape1->SetRegion( pRegionAction->GetUnreachRegionMap() );

			m_UnreachRegionMap[ulId] = pRegion;
			m_ulCurEditRegionId = ulId;
			return m_ulCurEditRegionId;
		}

		return 0xfffffff;
	}

	void CPathFindAction::ShowUnreachRegion()
	{
		if( m_UnreachRegionMap.size() == 0 ) return;

		CUnreachRegion* pRegion = m_UnreachRegionMap[m_ulCurEditRegionId];
		if( pRegion )
		{
			pRegion->m_pLineShape1->FillAreaPointData();
		}
	}

	// 通过指定的id， 删除一个区域
	unsigned long CPathFindAction::DelRegion(unsigned long ulId)
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

					pRegion->DestroyAllChildSceneNode();
					m_pSceneNode->removeAndDestroyChild(pRegion->m_pSceneNode->getName());
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
	CUnreachRegion* CPathFindAction::GetRegionById(unsigned long ulId)
	{
		if( m_UnreachRegionMap.size() > 0 )
		{
			CUnreachRegion* pRegion = m_UnreachRegionMap[m_ulCurEditRegionId];
			return pRegion;
		}

		return NULL;
	}


	void CPathFindAction::InitPathFind()
	{
		CreateSceneNode();
	}

	void CPathFindAction::SetOldHitPointMaterial()
	{
		IntersectPoint* pPoint = (IntersectPoint*)getSceneManipulator()->getHitIndicator("IntersectPoint");
		pPoint->SetMaterial();
	}

	// 创建区域编辑的场景节点
	void CPathFindAction::CreateSceneNode()
	{
		m_pSceneNode = getIndicatorRootSceneNode()->createChildSceneNode();
		if(m_pSceneNode)
		{
			m_pSceneNode->setVisible(false);
		}

	}

	// 在当前编辑的区域中添加一个新的点
	unsigned long CPathFindAction::AddNewPoint(Ogre::Vector3 position)
	{
		if(0 == m_UnreachRegionMap.count(m_ulCurEditRegionId))
		{
			return 0 ;
		}

		CUnreachRegion* pRegion = NULL;

		pRegion = m_UnreachRegionMap[m_ulCurEditRegionId];
		if(pRegion)
		{

			m_ulCurEditPointId = pRegion->AddNewPoint1(position);
			return m_ulCurEditPointId;
		}

		return 0;

	}

	bool CPathFindAction::EditPoint(const Point& pt)
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

			getSceneManipulator()->_fireAddDijkPoint(ulId, position);

			return true;
		}

		return false;
	}

	void CPathFindAction::HideScene(bool bHide)
	{
		if(m_pSceneNode)
		{
			m_pSceneNode->setVisible(bHide);
			m_bIsShow = bHide;
		}
	}

	void CPathFindAction::ClearRegionData()
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

					// 删除区域的所有子节点
					pRegion->DestroyAllChildSceneNode();

					// 删除场景节点中区域节点.
					m_pSceneNode->removeAndDestroyChild(pRegion->m_pSceneNode->getName());
					

				}

				delete pRegion;
			}
		}

		m_UnreachRegionMap.clear();

		m_ulCurMaxId = 0;

	}

	// 选择一个区域
	void CPathFindAction::SelRegion(unsigned long ulId, bool bIsChangeCamera)
	{
		if(m_UnreachRegionMap.count(ulId))
		{
			m_ulCurEditRegionId = ulId;
			CUnreachRegion* pRegion = NULL;
			pRegion = m_UnreachRegionMap[ulId];
			pRegion->SetRegionSelMaterial1();

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
	void CPathFindAction::UnSelRegion(unsigned long ulId)
	{
		if( m_UnreachRegionMap.size() > 0 )
		{
			CUnreachRegion* pRegion = NULL;
			pRegion = m_UnreachRegionMap[ulId];
			pRegion->SetRegionNorMaterial1();
		}
	}
	// 从区域读取文件
	unsigned long CPathFindAction::ReadRegionFromFile(std::string& strFileName)
	{
		FILE* fp = fopen( strFileName.c_str(), "rb" );
		if( !fp ) return 0xfffffffe;

		char buffer[20];
		fread( buffer, 1, 20, fp );
		fclose( fp );
		if( strncmp( buffer, DIJKSTRA_EDIT_VER1, strlen(DIJKSTRA_EDIT_VER1) ) == 0 )
		{
			return ReadRegionFromFileVer0( strFileName ); //
		}
		else
		{
			return 0;
		}
	}

	// 从区域读取文件
	unsigned long CPathFindAction::ReadRegionFromFileVer0(std::string& strFileName)
	{
		FILE* pFile = fopen( strFileName.c_str(), "rb" );

		if(NULL == pFile)
		{
			// 不能打开文件.
			return 0xfffffffe;
		}

		// 清空以前的数据.
		ClearRegionData();
		int			iVerLen = (int)sizeof(DIJKSTRA_EDIT_VER1);
		char*		pVer = new char[iVerLen];
		fread( pVer, 1, iVerLen, pFile );
		delete [] pVer;

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


		//区域的个数
		iRegionCount = 1;
		for(int i = 0; i < iRegionCount; i++)
		{
			// 区域的id
			iId = 1;

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
				getSceneManipulator()->_fireAddDijkPoint(k+1, position);
			}

			//if( i != 133)
			//{
			//	// 刷新新的区域
				ReshapeArea(m_ulCurEditRegionId);
			//}

		}

		::fclose(pFile);

		// 根据显示状态， 显示场景.
		HideScene(m_bIsShow);

		return 0;
	}

	// 把区域存储到文件
	unsigned long CPathFindAction::SaveRegionToFile(std::string strFileName)
	{

		FILE* pFile = NULL;
		pFile = ::fopen(strFileName.data(), "wb");
		
		if(NULL == pFile)
		{
			// 不能打开文件.
			return 0xfffffffe;
		}
		::fwrite(DIJKSTRA_EDIT_VER1, sizeof(DIJKSTRA_EDIT_VER1), 1, pFile);
		int			iRegionCount	 = 0;
		int			iPointCount		 = 0;
		int         iId			 = 0;
		unsigned long ulId			 = 0;

		CUnreachRegion* pPathfind		 = NULL;
		CPointInfo*		pPoint		 = NULL;
		Ogre::Vector3   position;

		UNREACHREGIONMAP::iterator itRegion;
		UNREACHREGIONMAP::iterator itEndRegion;

		POINT_MAP::iterator itPoint;
		POINT_MAP::iterator itEndPoint;

		// 存储区域的个数
		iRegionCount = m_UnreachRegionMap.size();
		

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
			pPathfind = itRegion->second;
			if(pPathfind)
			{
				// 存储点的个数.
				iPointCount = pPathfind->m_pointMap1.size();
				::fwrite(&iPointCount, sizeof(iPointCount), 1, pFile);

				// 存储每个点的信息。
				itEndPoint = pPathfind->m_pointMap1.end();
				for(itPoint = pPathfind->m_pointMap1.begin(); itPoint != itEndPoint; itPoint++)
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

				float* JuLi = pPathfind->m_pLineShape1->m_Weights;
				::fwrite(JuLi, sizeof(float),iPointCount*iPointCount, pFile);

			}
		}
		
		::fclose(pFile);
		return 0;
	}


	// 根据地形的高度改变编辑的区域。
	void CPathFindAction::SetRegionHeight()
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
				pUnreachRegion->SetPointHeight1(getTerrainData());
				pUnreachRegion->SetLineHeight1(getTerrainData());
			}
		}
	}


	void CPathFindAction::SetCamera(float fx, float fy)
	{
		const Ogre::Vector3 OldPos = getSceneManipulator()->getCamera()->getPosition();
		Ogre::Vector3 NewPos = OldPos;
		NewPos.x = fx;
		NewPos.z = fy;
		getSceneManipulator()->getCamera()->setPosition(NewPos);
	}

	void CPathFindAction::SetCamera()
	{

	}

	// 选择一个点.
	void CPathFindAction::SelPoint(unsigned long ulPointId)
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
			pPoint = pRegion->GetPointInfoById1(ulPointId);
			if(pPoint)
			{
				pPoint->SetSelMaterial();
			}
		}
	}


	// 设置按下的功能键
	void CPathFindAction::SetFuncKey(unsigned long ulFuncKey)
	{
		m_FuncKeyState |= ulFuncKey;
	}

	//  清空功能键.
	void CPathFindAction::ClearFuncKey()
	{
		m_FuncKeyState = FUNCTION_KEY;
	}


	void CPathFindAction::setParameter(const String& name, const String& value)
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
	void CPathFindAction::ChangeCurEditPointPos(const Point& pt)
	{
		CUnreachRegion* pRegion = NULL;
		CPointInfo*     pPoint  = NULL;

		// 得到当前正在编辑的区域的信息.
		pRegion = GetRegionById(m_ulCurEditRegionId);
		if(pRegion)
		{
			pPoint = pRegion->GetPointInfoById1(m_ulCurEditPointId);
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
								pRegion->m_pLineShape->FillPointData1();
								//pRegion->ReShapeArea1();
							}
						}

					}
				}
			}
		}
	}

	void CPathFindAction::ChangeCurEditPointPos(unsigned long ulPointId, Ogre::Vector3 position)
	{
		CUnreachRegion* pRegion = NULL;
		CPointInfo*     pPoint  = NULL;

		// 得到当前正在编辑的区域的信息.
		pRegion = GetRegionById(m_ulCurEditRegionId);
		if(pRegion)
		{
			pPoint = pRegion->GetPointInfoById1(ulPointId);
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
					pRegion->m_pLineShape->FillPointData1();
				}

			}

		}

	}

	// 记录当前操作前一次点的位置.
	void CPathFindAction::RecordPreDoPointPos()
	{

		CUnreachRegion* pRegion = NULL;
		CPointInfo*     pPoint  = NULL;

		// 得到当前正在编辑的区域的信息.
		pRegion = GetRegionById(m_ulCurEditRegionId);
		if(pRegion)
		{
			pPoint = pRegion->GetPointInfoById1(m_ulCurEditPointId);
			if(pPoint)
			{	
				m_preDoPointPos = pPoint->m_Position;		
			}
		}
	}

	// 指定区域id， 指定点的id， 指定位置， 添加一个小球.
	void CPathFindAction::AddNewPoint(unsigned long ulPointId, Ogre::Vector3 position)
	{
		// 得到当前正在编辑的区域的信息.
		CUnreachRegion* pRegion = NULL;
		pRegion = GetRegionById(m_ulCurEditRegionId);
		if(pRegion)
		{
			pRegion->AddNewPoint1(ulPointId, position);

			getSceneManipulator()->_fireAddDijkPoint(ulPointId, position);
		}
	}


	// 指定区域id， 指定点的id， 指定位置， 删除一个小球.
	void CPathFindAction::DelPoint(unsigned long ulPointId)
	{
		// 得到当前正在编辑的区域的信息.
		CUnreachRegion* pRegion = NULL;
		CPointInfo*     pPoint  = NULL;

		pRegion = GetRegionById(m_ulCurEditRegionId);
		if(pRegion)
		{
			pRegion->DelPointFromTail1(ulPointId);

			getSceneManipulator()->_fireDelDijkPoint(ulPointId);
		}
	}


	// 当前的区域是否合法
	bool CPathFindAction::IsRegionValidate(unsigned long ulRegionId)
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
	void CPathFindAction::HideRegionArea(unsigned long ulRegionId, bool bHide)
	{
		CUnreachRegion* pRegion = NULL;

		// 得到当前正在编辑的区域的信息.
		pRegion = GetRegionById(ulRegionId);
		if(pRegion)
		{

			pRegion->HideAreaNode1(bHide);

		}
	}

	// 重新创建显示的面积区域
	void CPathFindAction::ReshapeArea(unsigned long ulRegionId)
	{
		CUnreachRegion* pRegion = NULL;
		// 得到当前正在编辑的区域的信息.
		pRegion = GetRegionById(ulRegionId);
		if(pRegion)
		{

			pRegion->ReShapeArea1();

		}
	}


}
