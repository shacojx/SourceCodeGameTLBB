#include "StdAfx.h"
#include "CGCharJump.h"


uint CGCharJumpHandler::Execute( CGCharJump* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
