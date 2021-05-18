#include "StdAfx.h"
#include "Obj_Building.h"
#include "../DBC/GMDataBase.h"
#include "../Procedure/GameProcedure.h"
#include "TDException.h"
#include "TDUtil.h"
#include "..\WxRender\RenderSystem.h"

WX_IMPLEMENT_DYNAMIC(CObject_Building, GETCLASS(CObject_Static));
CObject_Building::CObject_Building() : 
	m_pResDefine(0),
	m_nLevel(-1),
	m_resType(RT_Mesh)
{

}

CObject_Building::~CObject_Building()
{

}

VOID CObject_Building::Release(VOID)
{
	if(m_pRenderInterface)
	{
		m_pRenderInterface->Destroy();
		m_pRenderInterface = 0;
	}
}

/********************************************
	资源定义ID发生改变
*********************************************/
void CObject_Building::SetLogicData_Define(const _DBC_CITY_BUILDING* pDefine)
{
	//相同
	if(m_pBuildingDefine == pDefine) return;

	m_pBuildingDefine = pDefine;

	//建筑物定义表
	DBC_DEFINEHANDLE(s_pBuildingDefineDBC, DBC_BUILDING_DEINFE);
	m_pResDefine = (const _DBC_BUILDING_DEFINE*)s_pBuildingDefineDBC->Search_Index_EQU(m_pBuildingDefine->nBuildingID);
	if(!m_pResDefine)
	{
		//TDU_Log("Invalid building define[%d]", m_pBuildingDefine->nID);
		return;
	}

	//取得渲染层位置
	const char* szGfxPos = pDefine->szGfxPosition;
	//分解
	std::vector< STRING > vGfxPos;
	TDU_ConvertStringToVector(szGfxPos, vGfxPos, " ", TRUE, TRUE);
	TDAssert(vGfxPos.size() == 3);

	fVector3 fvGfxPos(
		(float)atof(vGfxPos[0].c_str()), 
		(float)atof(vGfxPos[1].c_str()), 
		(float)atof(vGfxPos[2].c_str()));

	//转化坐标
	fVector3 fvGamePos;
	CGameProcedure::s_pGfxSystem->Axis_Trans(CRenderSystem::AX_GFX, fvGfxPos, CRenderSystem::AX_GAME, fvGamePos);

	//设置逻辑位置
	SetPosition(fvGamePos);

	//重新创建渲染层数据
	CreateRenderInterface();
}

/********************************************
	等级发生改变
*********************************************/
void CObject_Building::SetLogicData_Level(INT nLevel)
{
	if(m_nLevel == nLevel) return;

	//非法等级
	if(nLevel < 0 || nLevel >= _DBC_BUILDING_DEFINE::MAX_LEVEL)
	{
		//TDU_Log("Invalid building level[%d]", nLevel);
		return;
	}

	m_nLevel = nLevel;

	//重新创建渲染层数据
	CreateRenderInterface();
}

/********************************************
	在渲染层创建渲染指针
*********************************************/
void CObject_Building::CreateRenderInterface(void)
{
	//是否定义合法
	if(!m_pResDefine) return;
	if(m_nLevel < 0 || m_nLevel >= _DBC_BUILDING_DEFINE::MAX_LEVEL) return;

	//判断模型类型
	if(_stricmp(m_pResDefine->szResType, "Mesh") == 0)
	{
		m_resType = RT_Mesh;
	}
	else if(_stricmp(m_pResDefine->szResType, "Model") == 0)
	{
		m_resType = RT_Model;
	}
	else if(_stricmp(m_pResDefine->szResType, "Object") == 0)
	{
		m_resType = RT_Object;
	}
	else
	{
		//非法的资源类型
		TDThrow("Invalid building resource type:'%s' in %d", m_pResDefine->szResType, m_pResDefine->nID);
		return;
	}


	//销毁旧数据
	if(m_pRenderInterface)
	{
		m_pRenderInterface->Destroy();
		m_pRenderInterface = 0;
	}

	//资源定义
	const char* szRes = m_pResDefine->szLevel[m_nLevel];
	if(!szRes || szRes[0] == 0) return;

	//创建新渲染数据
	switch(m_resType)
	{
	case RT_Mesh:
		m_pRenderInterface = CGameProcedure::s_pGfxSystem->New_EntityObject(tEntityNode::ETYPE_MESH);
		m_pRenderInterface->StaObj_SetMeshFile(szRes);
		break;

	case RT_Model:
		m_pRenderInterface = CGameProcedure::s_pGfxSystem->New_EntityObject(tEntityNode::ETYPE_MODEL);
		m_pRenderInterface->StaObj_SetMeshFile(szRes);
		break;

	case RT_Object:
		m_pRenderInterface = CGameProcedure::s_pGfxSystem->New_EntityObject(tEntityNode::ETYPE_ACTOR);
		m_pRenderInterface->Actor_SetFile(szRes);
		break;

	default: return;
	}

	//鼠标无法选中
	m_pRenderInterface->SetRayQuery(FALSE);

	m_pRenderInterface->SetPosition( GetPosition() );
	//设置旋转
	m_pRenderInterface->SetWXObjectProperty("orientation", m_pBuildingDefine->szGfxOrientation);
}
