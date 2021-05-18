#include "StdAfx.h"
#include "GCEnterScene.h"
#include "..\..\Global.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\Procedure\GamePro_Enter.h"
#include "..\..\Procedure\GamePro_Main.h"
#include "..\..\Object\ObjectManager.h"
#include "..\..\Object\Character\Obj_PlayerMySelf.h"
#include "..\..\event\GMEventSystem.h"
#include "TDVariable.h"

uint GCEnterSceneHandler::Execute( GCEnterScene* pPacket, Player* pPlayer )
{
	//AxTrace(0, 2, "GCEnterSceneHandler::Execute");

	//当前流程是登录流程
	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcEnter)
	{
		//允许进入
		if(pPacket->getReturn() == 0)
		{
			//------------------------------------------------------------------
			//保存自身数据
			CGameProcedure::s_pVariableSystem->SetAs_Int("MySelf_ID", (INT)pPacket->getObjID());
			CGameProcedure::s_pVariableSystem->SetAs_Vector2("MySelf_Pos", pPacket->getEnterPos().m_fX, pPacket->getEnterPos().m_fZ);

			//是否是玩家城市
			//-- for debug

			BOOL bUserCityMode	= pPacket->getIsCity();
			int nCitySceneID	= pPacket->getSceneID();
			int nCityLevel		= pPacket->getCityLevel();

			//-- for debug

			//------------------------------------------------------------------
			//设置要进入的场景
			if(bUserCityMode)
			{
				CGameProcedure::s_pProcEnter->SetSceneID(nCitySceneID, nCityLevel);
			}
			else                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  
			{
				//普通场景，第二个参数（城市等级）必须为-1
				CGameProcedure::s_pProcEnter->SetSceneID(pPacket->getSceneID(), -1);
			}
			//设置登录流程状态,使之进入下一个状态
			CGameProcedure::s_pProcEnter->SetStatus(CGamePro_Enter::ENTERSCENE_OK);
			//进入场景
			CGameProcedure::s_pProcEnter->EnterScene();
		}
		else
		{
			//不允许进入
			CGameProcedure::s_pProcEnter->SetStatus(CGamePro_Enter::ENTERSCENE_FAILED );
			CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO_CLOSE_NET, "进入场景的请求被服务器拒绝");	
		}
	}

	return PACKET_EXE_CONTINUE ;
}