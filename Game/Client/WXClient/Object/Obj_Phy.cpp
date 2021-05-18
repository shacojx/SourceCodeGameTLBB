/**
物理层obj
负责重力，碰撞的处理tick
临时的一个物理物体实现
*/
#include "StdAfx.h"
#include <math.h>
#include "Obj_Phy.h"
#include "..\Global.h"
#include "TDTimeSystem.h"
#include "..\World\WorldManager.h"
#include "..\Procedure\GameProcedure.h"
#include "TDException.h"
#include <limits>
#include "..\WxRender\RenderSystem.h"

WX_IMPLEMENT_DYNAMIC(CObject_Phy, GETCLASS(CObject));
 
#define PHY_MACRO_GRAVITY					(9.8f)
#define PHY_MACRO_MSECONDS_PER_FRAME		(10)
static	const FLOAT	SF_Factor = (FLOAT)((FLOAT)PHY_MACRO_MSECONDS_PER_FRAME/(FLOAT)1000);

CObject_Phy::CObject_Phy()
{
	m_fvLinearSpeed =	fVector3(0.0f, 0.0f, 0.0f);
	m_fvRotSpeed	=	fVector3(0.0f, 0.0f, 0.0f);
	m_bIsEnable		=	FALSE;
	m_nLastTickTime	=	0;
	m_bIsInAir		=	FALSE;
}

CObject_Phy::~CObject_Phy()
{
}

VOID CObject_Phy::Initial(VOID* pParma)
{
	for(UINT i =0; i<MAX_REGISTER_EVENTS_NUM; i++)
	{
		m_aEventList[i] = PE_NONE;
	}
	m_nEventListNum = 0;

	CObject::Initial(pParma);
	return;
}

VOID CObject_Phy::AddLinearSpeed(fVector3 vSpeed)
{
	if(m_bIsInAir == TRUE)
		return;
	m_fvLinearSpeed = m_fvLinearSpeed+vSpeed;
}

VOID CObject_Phy::PhyEnable(BOOL bFlag)
{
	if(bFlag == FALSE)
	{
		m_fvLinearSpeed =	fVector3(0.0f, 0.0f, 0.0f);
		m_fvRotSpeed	=	fVector3(0.0f, 0.0f, 0.0f);
	}
	m_nLastTickTime	=	0;
	m_bIsEnable		=	bFlag;
}


VOID CObject_Phy::Tick(VOID)
{
	//调用物理系统对物体位置进行矫正
	if(m_bIsEnable == FALSE)
	{
		return;
	}

	//当前位置
	fVector3	fvTempPos	= CObject::GetPosition();
	fVector3	fvCurPos	= fvTempPos;
	//物理每桢 50毫秒
	UINT	nCurTime	=	CGameProcedure::s_pTimeSystem->GetTimeNow();

	//第一次不跑tick
	if(m_nLastTickTime == 0)
	{
		//记录上次时间点
		m_nLastTickTime	=	nCurTime;
		return;
	}

	UINT	nDeltaTime	=	CGameProcedure::s_pTimeSystem->CalSubTime(m_nLastTickTime, nCurTime);
	if(nDeltaTime<PHY_MACRO_MSECONDS_PER_FRAME)
	{
		return;
	}

	UINT	nStridTimes	= (UINT)(nDeltaTime/PHY_MACRO_MSECONDS_PER_FRAME);
	FLOAT	fUsedGravity	=	PHY_MACRO_GRAVITY;

	while(nStridTimes != 0)
	{
		//下落速度要有个增量（真实物理显得人物太重了）
		if(m_fvLinearSpeed.y > 0)
		{
			fUsedGravity = PHY_MACRO_GRAVITY + 25.0f;
		}
		else
		{
			fUsedGravity = PHY_MACRO_GRAVITY + 70.0f;
		}

		//计算垂直的速度变化
		m_fvLinearSpeed.y -= fUsedGravity*SF_Factor;

		//更新位置
		fvCurPos = fvCurPos+m_fvLinearSpeed*SF_Factor;

		//
		nStridTimes--;
	}
	
	//记录上次时间点
	m_nLastTickTime	=	nCurTime;

	//更新位置
	SetPosition(fvCurPos);

	//更新位置
	SetMapPosition(fVector2(fvCurPos.x, fvCurPos.z));

	CObject::Tick();
}

VOID CObject_Phy::SetMapPosition(const fVector2& fvPosition)
{
	CScene* pActiveScene = (CScene*)CWorldManager::GetMe()->GetActiveScene();
	if(pActiveScene==0)
    {
        //场景尚未加载，一般会出现在登陆流程时Avatar的显示，直接处理
		SetPosition(fVector3(fvPosition.x, 0.0, fvPosition.y));		//在行走面上
		return;
    }

	//当前位置
	fVector3	fvCurObjPos	=	CObject::GetPosition();
	FLOAT		fInAirHeight=	fvCurObjPos.y;

	//---------------------------------------------------
	//首先取得在地形上的高度
	fVector3 fvAtTerrain;
	CGameProcedure::s_pGfxSystem->Axis_Trans(
		CRenderSystem::AX_PLAN, fVector3(fvPosition.x, 0.0f, fvPosition.y),
		CRenderSystem::AX_GAME, fvAtTerrain);

	//---------------------------------------------------
	//取得在行走面上高度
	FLOAT fHeight = -FLT_MAX;
	{
		if(!(pActiveScene->m_WalkCollisionMng.GetHeight(fvPosition.x, fvPosition.y, fHeight)))
		{
			fHeight = -FLT_MAX;
		}
	}

	FLOAT	fRealHeight	= (fHeight > fvAtTerrain.y) ? fHeight:fvAtTerrain.y;

	if(IsPhyEnable() == TRUE )
	{
		if(m_fvLinearSpeed.y < 0)
		{//下降过程
			//---------------------------------------------------
			if(fRealHeight>fInAirHeight)
			{//已经落地了
				SetPosition(fVector3(fvPosition.x, fRealHeight, fvPosition.y));		//在行走面上
				DispatchPhyEvent(PE_COLLISION_WITH_GROUND);
				m_bIsInAir = FALSE;
			}
			else
			{//在空中设置空中的高度
				SetPosition(fVector3(fvPosition.x, fInAirHeight, fvPosition.y));	//在空中
				m_bIsInAir = TRUE;
			}
		}
		else
		{//上升过程，一定在空中
			if(fRealHeight>fInAirHeight)
			{
				SetPosition(fVector3(fvPosition.x, fRealHeight, fvPosition.y));	//在空中
			}
			else
			{
				SetPosition(fVector3(fvPosition.x, fInAirHeight, fvPosition.y));	//在空中
			}
			m_bIsInAir = TRUE;
		}
	}
	else
	{
		SetPosition(fVector3(fvPosition.x, fRealHeight, fvPosition.y));		//在行走面上
	}
	SetFootPosition(fVector3(fvPosition.x, fRealHeight, fvPosition.y));		//更新地表位置，供摄像机瞄准
}

//注册一个物理事件
VOID CObject_Phy::RegisterPhyEvent(PHY_EVENT_ID eventid)
{
	if( m_nEventListNum == MAX_REGISTER_EVENTS_NUM )
	{
		return;
	}
	for(UINT i = 0; i<m_nEventListNum; i++)
	{
		if(m_aEventList[i] == eventid)
			return;
	}
	m_aEventList[m_nEventListNum++] = eventid;
}

//注销一个物理事件
VOID CObject_Phy::UnRegisterPhyEvent(PHY_EVENT_ID eventid)
{
	if( m_nEventListNum==0)
	{
		return;
	}
	for(UINT i = 0; i<m_nEventListNum; i++)
	{
		if(m_aEventList[i] == eventid)
		{
			m_aEventList[i] = m_aEventList[m_nEventListNum-1];
			m_nEventListNum--;
			break;
		}
	}
}

//通知一个物理事件发生
VOID CObject_Phy::NotifyPhyEvent(PHY_EVENT_ID eventid, VOID* pParam)
{
	return;
}

//分发一个物理事件
VOID CObject_Phy::DispatchPhyEvent(PHY_EVENT_ID eventid, VOID* pParam)
{
	for(UINT i = 0; i<m_nEventListNum; i++)
	{
		if(m_aEventList[i] == eventid)
		{//已被注册
			NotifyPhyEvent(eventid, pParam);
			return;
		}
	}
}
