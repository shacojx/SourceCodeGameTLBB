#include "StdAfx.h"


#include "WGTeamError.h"



uint WGTeamErrorHandler::Execute( WGTeamError* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
