#include "StdAfx.h"


#include "GWTeamKick.h"



uint GWTeamKickHandler::Execute( GWTeamKick* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
