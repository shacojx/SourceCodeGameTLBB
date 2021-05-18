#include "StdAfx.h"
#include "CGCommand.h"

uint CGCommandHandler::Execute( CGCommand* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
