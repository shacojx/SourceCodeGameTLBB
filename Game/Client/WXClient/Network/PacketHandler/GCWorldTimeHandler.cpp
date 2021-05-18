#include "StdAfx.h"
#include "GCWorldTime.h"

uint GCWorldTimeHandler::Execute( GCWorldTime* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
