#include "StdAfx.h"


#include "WGTeamResult.h"



uint WGTeamResultHandler::Execute( WGTeamResult* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
