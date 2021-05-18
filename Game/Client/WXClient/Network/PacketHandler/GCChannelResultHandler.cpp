#include "StdAfx.h"


#include "GCChannelResult.h"
#include "..\..\Procedure\GameProcedure.h"



uint GCChannelResultHandler::Execute( GCChannelResult* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION
	//当前流程是主流程
	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		SCRIPT_SANDBOX::Talk::s_Talk.HandleChannelResultPacket(pPacket);
	}
	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
