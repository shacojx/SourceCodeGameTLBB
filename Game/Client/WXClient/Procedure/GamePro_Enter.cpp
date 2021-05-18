#include "StdAfx.h"
#include "GamePro_Enter.h"
#include "..\Network\NetManager.h"
#include "..\DataPool\GMUIDataPool.h"
#include "..\DataPool\DataPoolCommandDef.h"
#include "..\GameCommand.h"
#include "..\World\WorldManager.h"
#include "..\WxRender\RenderSystem.h"
#include "..\Object\ObjectManager.h"
#include "..\Object\Character\Obj_PlayerMySelf.h"
#include "CGConnect.h"
#include "CGEnterScene.h"
#include "..\Global.h"
#include "TDVariable.h"
#include "TDEventDefine.h"
#include "GamePro_Login.h"

CGamePro_Enter::CGamePro_Enter()
{
	m_Status = ENTERSCENE_READY;
	m_dwEnterType = ENTER_TYPE_FIRST;
}

CGamePro_Enter::~CGamePro_Enter()
{
}

VOID CGamePro_Enter::Init(VOID)
{
	((CObjectManager*)(s_pObjectManager))->SetLoadNPCDirect(TRUE);
}

VOID CGamePro_Enter::Tick(VOID)
{
	CGameProcedure::Tick();
	switch(GetStatus())
	{

	case ENTERSCENE_CONNECTING:
		{
			//等待服务器的回应
			SetStatus(ENTERSCENE_READY);
			break;
		}
	//发送进入场景的请求
	case ENTERSCENE_READY:
		{
			INT nSceneID = s_pVariableSystem->GetAs_Int("Scene_ID");

			//发送进入场景的请求
			CGEnterScene msg;
			msg.setEnterType((BYTE)m_dwEnterType);
			msg.setSceneID(nSceneID);
			fVector2 fvEnterPos = s_pVariableSystem->GetAs_Vector2("Scene_EnterPos");
			msg.setEnterPos(WORLD_POS(fvEnterPos.x, fvEnterPos.y));
			
			CNetManager::GetMe()->SendPacket(&msg);

			//等待服务器的回应
			SetStatus(ENTERSCENE_REQUESTING);
		}
		break;

	//发送进入场景的请求,等待服务器的回应...
	case ENTERSCENE_REQUESTING:
		break;

	//服务器允许进入场景
	case ENTERSCENE_OK:
		{
			// 关闭系统界面
			CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_CLOSE_SYSTEM_INFO);
			// 关闭帐号输入界面
			CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_CLOSE_COUNT_INPUT);
			// 关闭人物选择界面
			CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_CLOSE_SELECT_CHARACTOR);
			//设置场景摄像机
			CGameProcedure::s_pGfxSystem->Camera_SetCurrent(CRenderSystem::SCENE);
			
			EnterScene();
		}
		break;

	//服务器不允许进入场景
	case ENTERSCENE_FAILED:
		{
			CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO_CLOSE_NET, "服务器不允许进入场景");	
		}
		break;
	}
}

VOID CGamePro_Enter::EnterScene(void)
{
	//------------------------------------------------------------------
	//进入场景
	/*
	|
	|	如果当前有已经Load得场景，首先会退出并销毁场景上所有的Object，包括自身
	|
	*/
	if(!(s_pWorldManager->EnterScene(m_nSceneID, m_nCityLevel)))
	{
		return;
	}

	//------------------------------------------------------------------
	//创建玩家自身
	INT idMySelfServer = (INT)(CGameProcedure::s_pVariableSystem->GetAs_Int("MySelf_ID"));
	CObject_PlayerMySelf* pMySelf = CObjectManager::GetMe()->GetMySelf();
	if(!pMySelf)
	{
		pMySelf = (CObject_PlayerMySelf*)CObjectManager::GetMe()->NewObject(_T("CObject_PlayerMySelf"), idMySelfServer);
	}

	//------------------------------------------------------------------
	//设置玩家自身数据

	//设置ServerID
	CObjectManager::GetMe()->SetObjectServerID( pMySelf->GetID(), idMySelfServer );
	//设置 GUID
	pMySelf->SetServerGUID((INT)(CGameProcedure::s_pVariableSystem->GetAs_Int("User_GUID")));

	//设置初始位置
	fVector2 fvMapPos = CGameProcedure::s_pVariableSystem->GetAs_Vector2("MySelf_Pos");
	fVector3 fvGame;
	CGameProcedure::s_pGfxSystem->Axis_Trans(
		CRenderSystem::AX_PLAN, fVector3(fvMapPos.x, 0.0f, fvMapPos.y),
		CRenderSystem::AX_GAME, fvGame);

	SObjectInit initPlayerMySelf;
	initPlayerMySelf.m_fvPos = fvGame;
	CObjectManager::GetMe()->GetMySelf()->Initial(&initPlayerMySelf);

	//重新设置位置，防止进入行走面下面
	CObjectManager::GetMe()->GetMySelf()->SetMapPosition(fvMapPos);

	//同步摄像机
	CGameProcedure::s_pGfxSystem->Camera_SetLookAt(
		CObjectManager::GetMe()->GetMySelf()->GetPosition());

	//产生事件
	s_pEventSystem->PushEvent(GE_PLAYER_ENTER_WORLD);

	// 人物选择界面
	CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_CLOSE_SELECT_CHARACTOR);

	// 背景界面
	CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_CLOSE_BACK_GROUND);
	
	//转入主循环
	CGameProcedure::SetActiveProc((CGameProcedure*)CGameProcedure::s_pProcMain);

}

VOID CGamePro_Enter::Render(VOID)
{
	if(s_pGfxSystem) s_pGfxSystem->RenderFrame();
}

VOID CGamePro_Enter::Release(VOID)
{
	((CObjectManager*)(s_pObjectManager))->SetLoadNPCDirect(FALSE);
}

VOID CGamePro_Enter::MouseInput(VOID)
{

}
