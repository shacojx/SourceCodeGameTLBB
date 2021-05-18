#include "StdAfx.h"
#include "CGReturnTeamFollow.h"

uint CGReturnTeamFollowHandler::Execute( CGReturnTeamFollow* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}
