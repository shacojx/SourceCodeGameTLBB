#include "StdAfx.h"
#include "CGStopTeamFollow.h"

uint CGStopTeamFollowHandler::Execute( CGStopTeamFollow* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}
