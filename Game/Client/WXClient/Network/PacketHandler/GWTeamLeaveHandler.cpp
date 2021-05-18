#include "StdAfx.h"


#include "GWTeamLeave.h"



uint GWTeamLeaveHandler::Execute( GWTeamLeave* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
