#include "StdAfx.h"
#include "CGEventRequest.h"

using namespace Packets;

uint CGEventRequestHandler::Execute(CGEventRequest* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;

}