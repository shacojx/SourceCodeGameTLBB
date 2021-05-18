#include "StdAfx.h"
#include "GCEnvPlayerList.h"
#include "..\..\Object\ObjectManager.h"
#include "..\..\Object\Obj_PlayerOther.h"
#include "..\..\Procedure\GameProcedure.h"

uint GCEnvPlayerListHandler::Execute( GCEnvPlayerList* pPacket, Player* pPlayer )
{
	AxTrace(0, 2, "GCEnvPlayerListHandler::Execute");

	//当前流程是主流程
	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		//接受周围环境玩家列表
		int nPlayerCount = pPacket->GetCount();
		for(int i=0; i<nPlayerCount; i++)
		{
			const PLAYER_S& newPlayer = (pPacket->GetPlayerList())[i];
			
			//通过ObjManager创建其他玩家
			TDObject_PlayerOther* pNewPlayer = (TDObject_PlayerOther*)TDObjectManager::GetMe()->NewObject(_T("TDObject_PlayerOther"), _T("TDObject_PlayerOther"));
			pNewPlayer->Build(newPlayer);
			pNewPlayer->Initial();
		}
	}
	return PACKET_EXE_CONTINUE ;
}
