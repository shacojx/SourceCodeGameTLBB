#include "StdAfx.h"
#include "CGLockTarget.h"

uint CGLockTargetHandler::Execute( CGLockTarget* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
