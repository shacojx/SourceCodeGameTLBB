#include "StdAfx.h"
#include "WGNotifyUser.h"

uint WGNotifyUserHandler::Execute( WGNotifyUser* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
