#include "StdAfx.h"
#include "GCLeaveScene.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\Object\ObjectManager.h"
#include "..\..\Object\Character\Obj_PlayerOther.h"

uint GCLeaveSceneHandler::Execute( GCLeaveScene* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION
	//AxTrace(0, 2, "GCLeaveSceneHandler");
	//当前流程是主流程
	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		CObjectManager* pObjectManager = CObjectManager::GetMe();

		//寻找该玩家
		CObject_Character* pChar = NULL;
		if(!pChar) pChar = (CObject_Character* )pObjectManager->FindServerObject((INT)pPacket->getObjID());
		if(!pChar)	return PACKET_EXE_CONTINUE;

		//删除玩家
		pChar->Release();
		CGameProcedure::s_pObjectManager->DestroyObject(pChar);
	}

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
