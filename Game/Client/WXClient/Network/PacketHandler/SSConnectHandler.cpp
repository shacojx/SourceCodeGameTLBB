#include "StdAfx.h"
#include "SSConnect.h"

uint SSConnectHandler::Execute( SSConnect* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
