// GCMenpaiInfoHandler.cpp

#include "StdAfx.h"
#include "GCMenpaiInfo.h"

#include "..\..\DataPool\GMDataPool.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\Event\GMEventSystem.h"

uint GCMenpaiInfoHandler::Execute( GCMenpaiInfo* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION
	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		//DISCARD!!!!!
/*
		TeamID_t idMenpai = pPacket->GetMenpaiInfo_ID();
		CDataPool::GetMe()->JoinMenpai_SetMenpaiInfo( idMenpai );
		CEventSystem::GetMe()->PushEvent(GE_TOGLE_JOINSCHOOL);
*/		
	}
	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
