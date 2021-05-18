#include "StdAfx.h"
#include "GCLevelUpResult.h"

#include "..\..\Procedure\GameProcedure.h"
#include "..\..\Object\ObjectManager.h"
#include "..\..\Object\Character\Obj_PlayerMySelf.h"

uint GCLevelUpResultHandler::Execute(GCLevelUpResult* pPacket,Player* pPlayer)
{
	__ENTER_FUNCTION
	//AxTrace(0, 2, "GCLevelUpResultHandler::Execute");
	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		if(pPacket->GetLevelUpResult() != LEVELUP_SUCCESS) return PACKET_EXE_CONTINUE;
	}

	return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

	return	PACKET_EXE_ERROR;
}