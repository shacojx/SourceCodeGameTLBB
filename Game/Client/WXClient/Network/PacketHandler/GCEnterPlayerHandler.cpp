#include "StdAfx.h"
#include "GCEnterPlayer.h"
#include "..\..\Object\ObjectManager.h"
#include "..\..\Object\Obj_PlayerOther.h"
#include "..\..\Procedure\GameProcedure.h"

uint GCEnterPlayerHandler::Execute( GCEnterPlayer* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION
	AxTrace(0, 2, "GCEnterPlayerHandler::Execute");

	//新玩家进入
	//当前流程是主流程
	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		const PLAYER_S& newPlayer = *(pPacket->GetPlayerInfo());
			
		//通过ObjManager创建其他玩家
		TDObject_PlayerOther* pNewPlayer = (TDObject_PlayerOther*)TDObjectManager::GetMe()->NewObject(_T("TDObject_PlayerOther"), _T("TDObject_PlayerOther"));
		pNewPlayer->Build(newPlayer);
		pNewPlayer->Initial();
	}

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
