#include "StdAfx.h"
#include "CGCharDefaultEvent.h"


using namespace Packets;


uint CGCharDefaultEventHandler::Execute(CGCharDefaultEvent* pPacket, Player* pPlayer )
{

	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;

}