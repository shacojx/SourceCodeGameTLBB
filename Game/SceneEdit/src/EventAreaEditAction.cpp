#include "EventAreaEditAction.h"
#include "SceneManipulator.h"
#include "HitIndicator.h"
#include "IntersectPoint.h"
#include "UnreachRegion.h"
#include "CustomShape.h"
#include "EventAreaEditOperator.h"
#include "WXOperatorManager.h"       // redo undo 管理器.
#include "ScanLine.h"
#include "OgreCamera.h"


#include "Core/TerrainData.h"
#include "Core/WXResources.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace WX 
{
	using namespace Ogre;


	/////////////////////////////////////////////////////////////////////////////////
	//
	// 区域管理操作
	//


	CEventAreaRectInfo::CEventAreaRectInfo()
		: m_pParentSceneNode(NULL)
		, m_pSceneNode(NULL)
		, m_RectRenderShape(NULL)
		, m_pSceneManipulator(NULL)
	{
		m_iEventAreaId = -1;
		m_iScriptId    = -1;
		
		m_rect.m_LeftTop.x = 0;
		m_rect.m_LeftTop.y = 0;
		m_rect.m_RightBottom.x = 0;
		m_rect.m_RightBottom.y = 0;


	
	}

	CEventAreaRectInfo::~CEventAreaRectInfo()
	{

	}

	// 设置父场景节点.
	void CEventAreaRectInfo::SetParentSceneNode(Ogre::SceneNode* pSceneNode)
	{
		m_pParentSceneNode = pSceneNode;
	}

	// 设置SceneManipulator句柄
	void CEventAreaRectInfo::SetSceneManipulator(SceneManipulator*  SceneManipulator)
	{
		m_pSceneManipulator = SceneManipulator;
	}

	// 创建场景节点模型.
	unsigned long CEventAreaRectInfo::CreateSceneNode()
	{
		if(m_pSceneNode)
		{
			return 0;
		}

		if(m_pParentSceneNode)
		{
			 m_pSceneNode = (SceneNode*)m_pParentSceneNode->createChildSceneNode();
			 if(NULL == m_pSceneNode)
			 {
				 // 创建失败.
				 return 0xffffffff;
			 }
		}

		// 创建成功.
		return 0;
	}

	//// 创建显示的矩形
	//void CEventAreaRectInfo::BuildEventAreaRect()
	//{
	//	if(NULL == m_RectRenderShape)
	//	{
	//		return;
	//	}

	//	m_RectRenderShape->ClearRenderShape();

	//	float fStepX = 100.0f;
	//	float fStepZ = 100.0f;
	//	int   iStartX = m_rect.m_LeftTop.x;
	//	int   iStartZ = m_rect.m_LeftTop.y;
	//	int   iEndX   = m_rect.m_RightBottom.x;
	//	int   iEndZ   = m_rect.m_RightBottom.y;

	//	float fx = 0;
	//	float fz = 0;
	//		
	//	Ogre::Vector3 NewPoint;
	//	for(fz = iStartZ; fz < iEndZ; fz += fStepZ)
	//		for(fx = iStartX; fx < iEndX; fx += fStepX)
	//		{
	//			// 三角行1
	//			NewPoint.x = fx;
	//			NewPoint.z = fz;
	//			NewPoint.y = getTerrainData()->getHeightAt(NewPoint.x, NewPoint.z);
	//			m_pCurOperatorRect->AddNewPoint(NewPoint);

	//			NewPoint.x = fx;
	//			NewPoint.z = fz + fStepZ;
	//			NewPoint.y = getTerrainData()->getHeightAt(NewPoint.x, NewPoint.z);
	//			m_pCurOperatorRect->AddNewPoint(NewPoint);


	//			NewPoint.x = fx + fStepX;
	//			NewPoint.z = fz;
	//			NewPoint.y = getTerrainData()->getHeightAt(NewPoint.x, NewPoint.z);
	//			m_pCurOperatorRect->AddNewPoint(NewPoint);

	//		
	//			// 三角行2
	//			NewPoint.x = fx + fStepX;
	//			NewPoint.z = fz;
	//			NewPoint.y = getTerrainData()->getHeightAt(NewPoint.x, NewPoint.z);
	//			m_pCurOperatorRect->AddNewPoint(NewPoint);

	//			NewPoint.x = fx;
	//			NewPoint.z = fz + fStepZ;
	//			NewPoint.y = getTerrainData()->getHeightAt(NewPoint.x, NewPoint.z);
	//			m_pCurOperatorRect->AddNewPoint(NewPoint);

	//			NewPoint.x = fx + fStepX;
	//			NewPoint.z = fz + fStepZ;
	//			NewPoint.y = getTerrainData()->getHeightAt(NewPoint.x, NewPoint.z);
	//			m_pCurOperatorRect->AddNewPoint(NewPoint);
	//		}

	//	m_pRectRenderShape->FillPointData();
	//	m_pRectRenderShape->SetShapeType(Ogre::RenderOperation::OT_TRIANGLE_LIST);
	//	m_pRectRenderShape->setMaterial("EventAreaEdit/SelectRect");
	//	
	//}//

































///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  事件区域action 编辑.
//
//



	 CEventAreaEditAction::CEventAreaEditAction(SceneManipulator* sceneManipulator)
            : Action(sceneManipulator)
			, m_bIsShow(false)
			, m_FuncKeyState(0)
			, m_pSceneNode(NULL)
			, m_pLineShape(NULL)
			, m_pCurOperatorRect(NULL)
	 {
		// 初始化事件区域编辑
		InitEventArea();
    }

	CEventAreaEditAction::~CEventAreaEditAction(void)
	{
		ReleaseRectInfoMap();
	}


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// 键盘输入操作。
	//
		
	// 设置按下的功能键
	void CEventAreaEditAction::SetFuncKey(unsigned long ulFuncKey)
	{

	}

	//  清空功能键.
	void CEventAreaEditAction::ClearFuncKey()
	{

	}

		
	////////////////////////////////////////////////////////////////////////////////////
	//
	// 存盘和读盘操作
	//

	// 从区域读取文件
	unsigned long CEventAreaEditAction::ReadEventAreaFromFile(std::string strFileName)
	{

		// 清空矩形信息.
		ReleaseRectInfoMap();

		std::string strEventAreaFileIni = "";
		std::string strEventAreaFile    = "";

		strEventAreaFile    = strFileName;
		int iPos = strEventAreaFile.rfind(".");

		strEventAreaFileIni = strEventAreaFile.substr(0, iPos);
		strEventAreaFileIni += "_area.ini";

		char bufInfo[1024];
		memset(bufInfo, 0, sizeof(bufInfo));

		// 读取区域的个数
		int iRectCount = 0;
		iRectCount = ::GetPrivateProfileInt("area_info", "area_count",  0, strEventAreaFileIni.c_str());
		

		///////////////////////////////////////////////////////////////////////////////////////////////
		// 保存把场景坐标转换成游戏逻辑坐标.
		float fScale = 0;
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


		char bufPosInfo[1024];
		int iEventRectIndex = 0;
		CEventAreaInfo* pEventAreaInfo = NULL;

		float fx = 0;
		m_iMaxEventAreaId = 0;
		for(int i = 0; i < iRectCount; i++)
		{
			pEventAreaInfo = new CEventAreaInfo;
			if(pEventAreaInfo)
			{
				pEventAreaInfo->m_iId = i;
				memset(bufInfo, 0, sizeof(bufInfo));
				sprintf(bufInfo, "area%d", i);
			
				// 事件区域id
				pEventAreaInfo->m_iEventAreaId = GetPrivateProfileInt(bufInfo, "guid", 0, strEventAreaFileIni.c_str());
			
				// 脚本id
				pEventAreaInfo->m_iScriptId = GetPrivateProfileInt(bufInfo, "script_id", 0, strEventAreaFileIni.c_str());
			

				// 记录矩形区域位置
				memset(bufPosInfo, 0, sizeof(bufPosInfo));
				::GetPrivateProfileString(bufInfo, "left",  "none", bufPosInfo, sizeof(bufPosInfo), strEventAreaFileIni.c_str());
				fx = atof(bufPosInfo);
				fx = fx * fScale + fTerrainDataPosX;
				pEventAreaInfo->m_rect.m_LeftTop.x = fx;

				memset(bufPosInfo, 0, sizeof(bufPosInfo));
				::GetPrivateProfileString(bufInfo, "top",  "none", bufPosInfo, sizeof(bufPosInfo), strEventAreaFileIni.c_str());
				fx = atof(bufPosInfo);
				fx = fx * fScale + fTerrainDataPosZ;
				pEventAreaInfo->m_rect.m_LeftTop.y = fx;
			

				memset(bufPosInfo, 0, sizeof(bufPosInfo));
				::GetPrivateProfileString(bufInfo, "right",  "none", bufPosInfo, sizeof(bufPosInfo), strEventAreaFileIni.c_str());
				fx = atof(bufPosInfo);
				fx = fx * fScale + fTerrainDataPosX;
				pEventAreaInfo->m_rect.m_RightBottom.x = fx;
			

				memset(bufPosInfo, 0, sizeof(bufPosInfo));
				::GetPrivateProfileString(bufInfo, "bottom",  "none", bufPosInfo, sizeof(bufPosInfo), strEventAreaFileIni.c_str());
				fx = atof(bufPosInfo);
				fx = fx * fScale + fTerrainDataPosZ;
				pEventAreaInfo->m_rect.m_RightBottom.y = fx;

				// 保存信息
				m_RectShowMap[i] = pEventAreaInfo;

				// 读取id
				//m_iCurEditEventAreaId = pEventAreaInfo->m_iEventAreaId;
				//m_iCurEditScriptid    = pEventAreaInfo->m_iScriptId;

				// 设置当前编辑的矩形id
				m_iEditRectId = i;

				// 设置当前的最大eventid
				if(m_iMaxEventAreaId < m_iCurEditEventAreaId)
				{
					m_iMaxEventAreaId = m_iCurEditEventAreaId;
				}

				
				// 创建显示的数据
				pEventAreaInfo->SetParentSceneNode(m_pSceneNode);
				pEventAreaInfo->SetSceneManipulator(getSceneManipulator());
				pEventAreaInfo->CreateSceneNode();
				pEventAreaInfo->CreateRenderShape();
				pEventAreaInfo->m_pRectRenderShape->SetShapeType(Ogre::RenderOperation::OT_TRIANGLE_LIST);
				
				m_pCurOperatorRect = pEventAreaInfo;

				m_PrePoint.x  = pEventAreaInfo->m_rect.m_LeftTop.x;
				m_PrePoint.z = pEventAreaInfo->m_rect.m_LeftTop.y;

				m_CurPoint.x =  pEventAreaInfo->m_rect.m_RightBottom.x;
				m_CurPoint.z =  pEventAreaInfo->m_rect.m_RightBottom.y;
				NormalRect();
				BuildEventAreaRect();
				m_pCurOperatorRect->m_pRectRenderShape->setMaterial("Core/StatsBlockCenter");//fujia EventAreaEdit/UnSelectRect
										
			}
		}

		HideScene(m_bIsShow);
		return 0;
	}
	
	// 把区域存储到文件
	unsigned long CEventAreaEditAction::SaveEventAreaToFile(std::string strFileName)
	{
		SHOW_RECT_MAP::iterator itEventAreaEnd;
		SHOW_RECT_MAP::iterator it;

		std::string strEventAreaFileIni = "";
		std::string strEventAreaFile    = "";

		strEventAreaFile    = strFileName;
		int iPos = strEventAreaFile.rfind(".");

		strEventAreaFileIni = strEventAreaFile.substr(0, iPos);
		strEventAreaFileIni += "_area.ini";

		// 删除旧的文件。
		::DeleteFile(strEventAreaFileIni.c_str());

		char bufInfo[1024];
		memset(bufInfo, 0, sizeof(bufInfo));

		// 存储区域的个数
		int iRectCount = m_RectShowMap.size();
		sprintf(bufInfo, "%d", iRectCount);
		WritePrivateProfileString("area_info", "area_count", bufInfo, strEventAreaFileIni.c_str());
		

		// 存储每个区域的信息
		itEventAreaEnd = m_RectShowMap.end();
		///////////////////////////////////////////////////////////////////////////////////////////////
		//
		// 保存把场景坐标转换成游戏逻辑坐标.
		//
		
		float fScale = 0;
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

		char bufPosInfo[1024];
		int iEventRectIndex = 0;
		CEventAreaInfo* pEventAreaInfo = NULL;

		float x = 0;
		float y = 0;
		for(it = m_RectShowMap.begin(); it != itEventAreaEnd; it++)
		{
			pEventAreaInfo = it->second;
			if(pEventAreaInfo)
			{
				memset(bufInfo, 0, sizeof(bufInfo));
				sprintf(bufInfo, "area%d", iEventRectIndex);
				iEventRectIndex++;
			
				// 事件区域id
				memset(bufPosInfo, 0, sizeof(bufPosInfo));
				sprintf(bufPosInfo, "%d", pEventAreaInfo->m_iEventAreaId);
				WritePrivateProfileString(bufInfo, "guid", bufPosInfo, strEventAreaFileIni.c_str());

				// 脚本id
				memset(bufPosInfo, 0, sizeof(bufPosInfo));
				sprintf(bufPosInfo, "%d", pEventAreaInfo->m_iScriptId);
				WritePrivateProfileString(bufInfo, "script_id", bufPosInfo, strEventAreaFileIni.c_str());


				// 记录矩形区域位置
				memset(bufPosInfo, 0, sizeof(bufPosInfo));
				x = pEventAreaInfo->m_rect.m_LeftTop.x;
				x = (x - fTerrainDataPosX) / fScale;
				sprintf(bufPosInfo, "%.4f", x);
				WritePrivateProfileString(bufInfo, "left", bufPosInfo, strEventAreaFileIni.c_str());

				memset(bufPosInfo, 0, sizeof(bufPosInfo));
				x = pEventAreaInfo->m_rect.m_LeftTop.y;
				x = (x - fTerrainDataPosZ) / fScale;
				sprintf(bufPosInfo, "%.4f",x);
				WritePrivateProfileString(bufInfo, "top", bufPosInfo, strEventAreaFileIni.c_str());

				memset(bufPosInfo, 0, sizeof(bufPosInfo));
				x = pEventAreaInfo->m_rect.m_RightBottom.x;
				x = (x - fTerrainDataPosX) / fScale;
				sprintf(bufPosInfo, "%.4f",x);
				WritePrivateProfileString(bufInfo, "right", bufPosInfo, strEventAreaFileIni.c_str());

				memset(bufPosInfo, 0, sizeof(bufPosInfo));
				x = pEventAreaInfo->m_rect.m_RightBottom.y;
				x = (x - fTerrainDataPosZ) / fScale;
				sprintf(bufPosInfo, "%.4f",x);
				WritePrivateProfileString(bufInfo, "bottom", bufPosInfo, strEventAreaFileIni.c_str());



					
			}
		}
	
		return 0;
	}
		
			
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// action 操作。
	//
		
	const String& CEventAreaEditAction::getName(void) const
	{
		//返回一个字符串名字， 用来判断当前操作的类型.
		static const String name = "EventAreaEdit";
		return name;
	}

	// 设置操作参数.
	void CEventAreaEditAction::setParameter(const String& name, const String& value)
	{

	}


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//
	// 鼠标操作的虚函数.
	//
    //protected:
        
		
	void CEventAreaEditAction::_onMove(const Point& pt)
	{

	}
	void CEventAreaEditAction::_onBegin(const Point& pt)
	{
		OnMouseLBnDown(pt);
	}
	void CEventAreaEditAction::_onDrag(const Point& pt)
	{
		OnMouseDrag(pt);
	}
	void CEventAreaEditAction::_onEnd(const Point& pt, bool canceled)
	{
		OnMouseLBnUp(pt);
	}

	// 当前激活active操作.
	void CEventAreaEditAction::_onActive(bool active)
	{

	}


	
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// 响应相应的鼠标操作.
	//

	void CEventAreaEditAction::OnMouseMove(const Point& pt)
	{
		switch(m_iOperator)
		{
		case SELECT:
			{
				getSceneManipulator()->getHitIndicator("IntersectPoint")->setHitPoint(pt.x, pt.y);
				ClearFuncKey();
			}
			break;
		case DRAW:
			{
				
			}
			break;
		default:
			{
			}
			break;
		}

		
	}

	void CEventAreaEditAction::OnMouseLBnDown(const Point& pt)
	{
		switch(m_iOperator)
		{
		case SELECT:
			{
				
			}
			break;
		case DRAW:
			{
				DoDrawLBnDown(pt);
			}
			break;
		default:
			{
			}
			break;
		}


		
	}

	void CEventAreaEditAction::OnMouseLBnUp(const Point& pt)
	{
		switch(m_iOperator)
		{
		case SELECT:
			{
				DoSelectLBnUp(pt);
			}
			break;
		case DRAW:
			{
				DoDrawLBnUp(pt);
			}
			break;
		default:
			{
			}
			break;
		}
		
	}

	void CEventAreaEditAction::OnMouseRBnDown(const Point& pt)
	{

	}

	void CEventAreaEditAction::OnMouseRBnUp(const Point& pt)
	{

	}

	void CEventAreaEditAction::OnMouseDrag(const Point& pt)
	{
		switch(m_iOperator)
		{
		case SELECT:
			{
				
			}
			break;
		case DRAW:
			{
				DoDrawLBnDrag(pt);
			}
			break;
		default:
			{
			}
			break;
		}
		
	}


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// 与渲染层相关操作
	//

	// 创建区域编辑的场景节点
	void CEventAreaEditAction::CreateSceneNode()
	{
		m_pSceneNode = getIndicatorRootSceneNode()->createChildSceneNode();
		if(m_pSceneNode)
		{
			HideScene(false);
		}
		
	}

	// 隐藏当前编辑的action
	void CEventAreaEditAction::HideScene(bool bHide)
	{
		if(m_pSceneNode)
		{
			m_pSceneNode->setVisible(bHide);
			m_bIsShow = bHide;
		}
	}

	// 创建显示的区域
	void CEventAreaEditAction::CreateShowEventArea()
	{
		if(NULL == m_pCurOperatorRect)
		{
			return;
		}

		if(NULL == m_pCurOperatorRect->m_pRectRenderShape)
		{

		}
		else
		{

		}

	}


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// 逻辑数据操作
	//

	// 初始化事件区域编辑
	void CEventAreaEditAction::InitEventArea()
	{
		// 创建场景节点
		CreateSceneNode();

		// 初试化逻辑数据.
		InitLogicData();
	}

	void CEventAreaEditAction::NormalRect()
	{
		int iX1 = 0;
		int iY1 = 0;
		int iX2 = 0;
		int iY2 = 0;

		if(m_PrePoint.x < m_CurPoint.x)
		{

			iX1 = m_PrePoint.x;
            iX2 = m_CurPoint.x;

			m_AreaRect.m_LeftTop.x      = iX1;
			m_AreaRect.m_RightBottom.x  = iX2;

			
		}
		else
		{
			iX1 = m_CurPoint.x;
            iX2 = m_PrePoint.x;

			m_AreaRect.m_LeftTop.x      = iX1;
			m_AreaRect.m_RightBottom.x  = iX2;

			
		}

		if(m_PrePoint.z < m_CurPoint.z)
		{
			iY1 = m_PrePoint.z;
            iY2 = m_CurPoint.z;

			m_AreaRect.m_LeftTop.y      = iY1 ;
			m_AreaRect.m_RightBottom.y  = iY2 ;

			

		}
		else
		{
			iY1 = m_CurPoint.z;
            iY2 = m_PrePoint.z;

			m_AreaRect.m_LeftTop.y      = iY1;
			m_AreaRect.m_RightBottom.y  = iY2;

			
		}

		int   iStartX = m_AreaRect.m_LeftTop.x;
		int   iStartZ = m_AreaRect.m_LeftTop.y;
		int   iEndX   = m_AreaRect.m_RightBottom.x;
		int   iEndZ   = m_AreaRect.m_RightBottom.y;


		if(iStartX > 0)
		{
			iStartX = (iStartX / 100) * 100;
		}
		else
		{
			iStartX = ((iStartX - 99 )/ 100) * 100;
		}

		if(iStartZ > 0)
		{
			iStartZ = (iStartZ / 100) * 100;
		}
		else
		{
			iStartZ = ((iStartZ - 99) / 100) * 100;
		}

		if(iEndX > 0)
		{
			iEndX   = ((iEndX + 99) / 100) * 100;
		}
		else
		{
			iEndX   = (iEndX / 100) * 100;
		}

		if(iEndZ > 0)
		{
			iEndZ   = ((iEndZ + 99) / 100) * 100;
		}
		else
		{
			iEndZ   = (iEndZ / 100) * 100;
		}


		m_AreaRect.m_LeftTop.x		= iStartX;
		m_AreaRect.m_LeftTop.y		= iStartZ;
		m_AreaRect.m_RightBottom.x	= iEndX;
		m_AreaRect.m_RightBottom.y	= iEndZ;
	
	}

	// 得到地形上的坐标点
	bool  CEventAreaEditAction::GetTerrainPos(const Point& pt, Ogre::Vector3& position)
	{
			
		bool hit = getSceneManipulator()->getTerrainIntersects(pt.x, pt.y, position);

		return hit;
		
	}

	// 做画矩形的鼠标按下操作
	void CEventAreaEditAction::DoDrawLBnDown(const Point& pt)
	{
		

		Ogre::Vector3 TerrainPosition;
		if(GetTerrainPos(pt, TerrainPosition))
		{

			if(m_pCurOperatorRect)
			{
				m_pCurOperatorRect->m_pRectRenderShape->setMaterial("Core/StatsBlockCenter");//fujia EventAreaEdit/UnSelectRect
			}

			m_iEditRectId++;
			m_PrePoint = TerrainPosition;
			m_CurPoint = TerrainPosition;
			m_CurPoint.x += 100;
			m_CurPoint.z += 100;
			NormalRect();
			
			CEventAreaInfo* pRectInfo = new CEventAreaInfo;
			pRectInfo->m_iEventAreaId = m_iCurEditEventAreaId;
			pRectInfo->m_iScriptId    = m_iCurEditScriptid;
			pRectInfo->m_iId          = m_iEditRectId;
			m_pCurOperatorRect        = pRectInfo;

			if(m_pCurOperatorRect)
			{
				m_pCurOperatorRect->m_rect = m_AreaRect;
				//m_pCurOperatorRect->m_rect.m_LeftTop.x = m_AreaRect.m_LeftTop.x;
				//m_pCurOperatorRect->m_rect.m_LeftTop.y = m_AreaRect.m_LeftTop.y;
				//m_pCurOperatorRect->m_rect.m_RightBottom.x = m_AreaRect.m_RightBottom.x;
				//m_pCurOperatorRect->m_rect.m_RightBottom.y = m_AreaRect.m_RightBottom.y;//
			}

			m_RectShowMap[m_iEditRectId] = pRectInfo;

			// 创建显示的数据
			pRectInfo->SetParentSceneNode(m_pSceneNode);
			pRectInfo->SetSceneManipulator(getSceneManipulator());
			pRectInfo->CreateSceneNode();

			// 要创建渲染缓冲区不然在, release下回出现问题.
			
			pRectInfo->CreateRenderShape();
			BuildEventAreaRect();
			pRectInfo->m_pRectRenderShape->SetShapeType(Ogre::RenderOperation::OT_TRIANGLE_LIST);

			getSceneManipulator()->_fireAddNewRect(0, m_iEditRectId);

		}
	}
	

	// 做画矩形的鼠标托拽操作
	void CEventAreaEditAction::DoDrawLBnDrag(const Point& pt)
	{
		Ogre::Vector3 TerrainPosition;
		if(GetTerrainPos(pt, TerrainPosition))
		{
			m_CurPoint = TerrainPosition;
			NormalRect();
			
			if(m_pCurOperatorRect)
			{
				m_pCurOperatorRect->m_rect = m_AreaRect;
				//m_pCurOperatorRect->m_rect.m_LeftTop.x = m_AreaRect.m_LeftTop.x;
				//m_pCurOperatorRect->m_rect.m_LeftTop.y = m_AreaRect.m_LeftTop.y;
				//m_pCurOperatorRect->m_rect.m_RightBottom.x = m_AreaRect.m_RightBottom.x;
				//m_pCurOperatorRect->m_rect.m_RightBottom.y = m_AreaRect.m_RightBottom.y;//
			}
			BuildEventAreaRect();

		}//
	}

	
	// 初始化逻辑数据
	void CEventAreaEditAction::InitLogicData()
	{
		m_iEditRectId    = 0;
		m_pCurOperatorRect = NULL;
		m_iOperator = SELECT;

		m_iCurEditEventAreaId = 0;
		m_iCurEditScriptid    = 0;
		m_iMaxEventAreaId     = 0;
	}

	// 根据矩形创建显示的事件区域
	void CEventAreaEditAction::BuildEventAreaRect()
	{
		if(NULL == m_pCurOperatorRect)
		{
			return;
		}

		m_pCurOperatorRect->ClearRenderShape();

		float fStepX = 100.0f;
		float fStepZ = 100.0f;
		int   iStartX = m_AreaRect.m_LeftTop.x;
		int   iStartZ = m_AreaRect.m_LeftTop.y;
		int   iEndX   = m_AreaRect.m_RightBottom.x;
		int   iEndZ   = m_AreaRect.m_RightBottom.y;

		float fx = 0;
		float fz = 0;
			
		Ogre::Vector3 NewPoint;
		for(fz = iStartZ; fz < iEndZ; fz += fStepZ)
			for(fx = iStartX; fx < iEndX; fx += fStepX)
			{
				// 三角行1
				NewPoint.x = fx;
				NewPoint.z = fz;
				NewPoint.y = getTerrainData()->getHeightAt(NewPoint.x, NewPoint.z);
				m_pCurOperatorRect->AddNewPoint(NewPoint);

				NewPoint.x = fx;
				NewPoint.z = fz + fStepZ;
				NewPoint.y = getTerrainData()->getHeightAt(NewPoint.x, NewPoint.z);
				m_pCurOperatorRect->AddNewPoint(NewPoint);


				NewPoint.x = fx + fStepX;
				NewPoint.z = fz;
				NewPoint.y = getTerrainData()->getHeightAt(NewPoint.x, NewPoint.z);
				m_pCurOperatorRect->AddNewPoint(NewPoint);

			
				// 三角行2
				NewPoint.x = fx + fStepX;
				NewPoint.z = fz;
				NewPoint.y = getTerrainData()->getHeightAt(NewPoint.x, NewPoint.z);
				m_pCurOperatorRect->AddNewPoint(NewPoint);

				NewPoint.x = fx;
				NewPoint.z = fz + fStepZ;
				NewPoint.y = getTerrainData()->getHeightAt(NewPoint.x, NewPoint.z);
				m_pCurOperatorRect->AddNewPoint(NewPoint);

				NewPoint.x = fx + fStepX;
				NewPoint.z = fz + fStepZ;
				NewPoint.y = getTerrainData()->getHeightAt(NewPoint.x, NewPoint.z);
				m_pCurOperatorRect->AddNewPoint(NewPoint);
			}

		m_pCurOperatorRect->m_pRectRenderShape->FillPointData();
		m_pCurOperatorRect->m_pRectRenderShape->SetShapeType(Ogre::RenderOperation::OT_TRIANGLE_LIST);
		m_pCurOperatorRect->m_pRectRenderShape->setMaterial("EventAreaEdit/SelectRect");
		
	}





		// 做画矩形的鼠标抬起操作
	void CEventAreaEditAction::DoDrawLBnUp(const Point& pt)
	{
		if(m_pCurOperatorRect)
		{
			m_pCurOperatorRect->m_pRectRenderShape->setMaterial("EventAreaEdit/PreSelectRect");
			
			// 添加redo undo
			CEventAreaEditAddOperator* pAddyOperator = NULL;
			pAddyOperator = new CEventAreaEditAddOperator;

			pAddyOperator->SetAction(this);
			pAddyOperator->SetInstanceId(m_iEditRectId);
			pAddyOperator->SetPosition(m_PrePoint, m_CurPoint);//
			getOperatorManager()->addOperator(pAddyOperator);//
		}
	}


	// 清空信息列表
	void CEventAreaEditAction::ReleaseRectInfoMap()
	{
		
		CEventAreaInfo* pEventAreaInfo = NULL;
		SHOW_RECT_MAP::iterator itEventAreaEnd;
		SHOW_RECT_MAP::iterator it;

		itEventAreaEnd = m_RectShowMap.end();
		for(it = m_RectShowMap.begin(); it != itEventAreaEnd; it++)
		{
			pEventAreaInfo = it->second;
			if(pEventAreaInfo)
			{
				delete pEventAreaInfo;					
			}
		}

		m_RectShowMap.clear();

		m_pCurOperatorRect = NULL;

	}

	// 根据矩形id删除一个矩形
	void CEventAreaEditAction::DeleteRect(int iId)
	{
		SHOW_RECT_MAP::iterator itEventAreaEnd;
		SHOW_RECT_MAP::iterator it;

		it = m_RectShowMap.find(iId);

		Ogre::Vector3	p1;
		Ogre::Vector3   p2;

		int iRectid = 0;
		if(it != m_RectShowMap.end())
		{
			CEventAreaInfo* pEventAreaInfo = NULL;
			pEventAreaInfo = it->second;
			if(pEventAreaInfo)
			{
				iRectid = pEventAreaInfo->m_iId;
				p1.x = pEventAreaInfo->m_rect.m_LeftTop.x;
				p1.z = pEventAreaInfo->m_rect.m_LeftTop.y;

				p2.x = pEventAreaInfo->m_rect.m_RightBottom.x;
				p2.z = pEventAreaInfo->m_rect.m_RightBottom.y;
				delete pEventAreaInfo;
			}
			
 			m_RectShowMap.erase(it);
			getSceneManipulator()->_fireDelRect(0, iRectid);

			// 删除redo undo
			CEventAreaEditDelOperator* pDelOperator = NULL;
			pDelOperator = new CEventAreaEditDelOperator;

			pDelOperator->SetAction(this);
			pDelOperator->SetInstanceId(m_iEditRectId);
			pDelOperator->SetPosition(p1, p2);//
			getOperatorManager()->addOperator(pDelOperator);//

			m_pCurOperatorRect = NULL;
			
		}

		
	}

	// 不带redo undo 操作的删除
	void CEventAreaEditAction::DeleteRectNoOperator(int iId)
	{
		SHOW_RECT_MAP::iterator itEventAreaEnd;
		SHOW_RECT_MAP::iterator it;

		it = m_RectShowMap.find(iId);

		int iRectid = 0;
		if(it != m_RectShowMap.end())
		{
			CEventAreaInfo* pEventAreaInfo = NULL;
			pEventAreaInfo = it->second;
			if(pEventAreaInfo)
			{
				iRectid = pEventAreaInfo->m_iId;
				delete pEventAreaInfo;
			}
			
			m_RectShowMap.erase(it);
			getSceneManipulator()->_fireDelRect(0, iRectid);

			m_pCurOperatorRect = NULL;
			
		}

		
	}


	// 添加一个矩形.
	int CEventAreaEditAction::AddRect(Ogre::Vector3 vectorUnDoPos, Ogre::Vector3 vectorReDoPos, int iId)
	{
		int iReturnId= 0;
		if(-1 == iId)
		{
			m_iEditRectId++;
			m_PrePoint = vectorUnDoPos;
			m_CurPoint = vectorReDoPos;
			NormalRect();
			
			CEventAreaInfo* pRectInfo = new CEventAreaInfo;
			pRectInfo->m_iEventAreaId = m_iCurEditEventAreaId;
			pRectInfo->m_iScriptId    = m_iCurEditScriptid;
			pRectInfo->m_iId          = m_iEditRectId;

			// 保存信息
			m_RectShowMap[m_iEditRectId]   = pRectInfo;
			m_pCurOperatorRect = pRectInfo;
		
			pRectInfo->m_rect = m_AreaRect;
			// 创建显示的数据
			pRectInfo->SetParentSceneNode(m_pSceneNode);
			pRectInfo->SetSceneManipulator(getSceneManipulator());
			pRectInfo->CreateSceneNode();
			pRectInfo->CreateRenderShape();
			pRectInfo->m_pRectRenderShape->SetShapeType(Ogre::RenderOperation::OT_TRIANGLE_LIST);
			BuildEventAreaRect();
			m_pCurOperatorRect->m_pRectRenderShape->setMaterial("Core/StatsBlockCenter");//fujia EventAreaEdit/UnSelectRect

			iReturnId = m_iEditRectId;
			getSceneManipulator()->_fireAddNewRect(0, iReturnId);
		}
		else
		{
			m_PrePoint = vectorUnDoPos;
			m_CurPoint = vectorReDoPos;
			NormalRect();
			
			CEventAreaInfo* pRectInfo = new CEventAreaInfo;
			pRectInfo->m_iEventAreaId = m_iCurEditEventAreaId;
			pRectInfo->m_iScriptId    = m_iCurEditScriptid;
			pRectInfo->m_iId          = iId;

			// 保存信息
			m_RectShowMap[m_iEditRectId]   = pRectInfo;
			m_pCurOperatorRect = pRectInfo;
		
			pRectInfo->m_rect = m_AreaRect;
			// 创建显示的数据
			pRectInfo->SetParentSceneNode(m_pSceneNode);
			pRectInfo->SetSceneManipulator(getSceneManipulator());
			pRectInfo->CreateSceneNode();
			pRectInfo->CreateRenderShape();
			pRectInfo->m_pRectRenderShape->SetShapeType(Ogre::RenderOperation::OT_TRIANGLE_LIST);
			BuildEventAreaRect();
			m_pCurOperatorRect->m_pRectRenderShape->setMaterial("Core/StatsBlockCenter");//fujia EventAreaEdit/UnSelectRect

			iReturnId = iId;
			getSceneManipulator()->_fireAddNewRect(0, iReturnId);
		}
			
		return iReturnId;

	}

	// 选中一个矩形
	void CEventAreaEditAction::SelectEventAreaRect(int iId)
	{
		
		if(m_RectShowMap.count(iId))
		{
			if(m_pCurOperatorRect)
			{
				m_pCurOperatorRect->m_pRectRenderShape->setMaterial("Core/StatsBlockCenter");//fujia EventAreaEdit/UnSelectRect
			}

			CEventAreaInfo* pRectInfo = NULL;
			pRectInfo = m_RectShowMap[iId];
			m_pCurOperatorRect = pRectInfo;

			if(m_pCurOperatorRect)
			{
				m_pCurOperatorRect->m_pRectRenderShape->setMaterial("Core/StatsBlockCenter");//fujia EventAreaEdit/UnSelectRect
			}

		}
	}

	// 做选择矩形的鼠标抬起操作
	void CEventAreaEditAction::DoSelectLBnUp(const Point& pt)
	{
		Ogre::Vector3 TerrainPosition;
		if(GetTerrainPos(pt, TerrainPosition))
		{

			if(m_pCurOperatorRect)
			{
				m_pCurOperatorRect->m_pRectRenderShape->setMaterial("Core/StatsBlockCenter");//fujia EventAreaEdit/UnSelectRect
			}

			CEventAreaInfo* pEventAreaInfo = NULL;
			SHOW_RECT_MAP::iterator itEventAreaEnd;
			SHOW_RECT_MAP::iterator it;

			itEventAreaEnd = m_RectShowMap.end();
			for(it = m_RectShowMap.begin(); it != itEventAreaEnd; it++)
			{
				pEventAreaInfo = it->second;
				if(pEventAreaInfo)
				{
					if( (TerrainPosition.x > pEventAreaInfo->m_rect.m_LeftTop.x)&&
						(TerrainPosition.x < pEventAreaInfo->m_rect.m_RightBottom.x)&&
						(TerrainPosition.z > pEventAreaInfo->m_rect.m_LeftTop.y)&&
						(TerrainPosition.z < pEventAreaInfo->m_rect.m_RightBottom.y))
					{
						m_pCurOperatorRect = pEventAreaInfo;
						m_pCurOperatorRect->m_pRectRenderShape->setMaterial("EventAreaEdit/SelectRect");
						break;
					}
				}
			}
			
			if(m_pCurOperatorRect)
			{
				getSceneManipulator()->_fireSelRect(0, m_pCurOperatorRect->m_iId);
			}

		}
	}



	// 更改区域
	void CEventAreaEditAction::ModifyEventArea(int iInstanceId, int iEventId, int iScripId)
	{

		if(0 == m_RectShowMap.count(iInstanceId))
		{
			return;
		}


	    CEventAreaInfo* pEventAreaInfo = m_RectShowMap[iInstanceId];
		if(pEventAreaInfo)
		{
			pEventAreaInfo->m_iEventAreaId  = iEventId;
			pEventAreaInfo->m_iScriptId     = iScripId;
			
		}

	}


}
