#include "StdAfx.h"
#include "CGDoEvent.h"

using namespace Packets;

uint CGDoEventHandler::Execute( CGDoEvent* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
