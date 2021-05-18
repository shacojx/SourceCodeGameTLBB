#include "StdAfx.h"
#include "CGAttack.h"


uint CGAttackHandler::Execute( CGAttack* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
