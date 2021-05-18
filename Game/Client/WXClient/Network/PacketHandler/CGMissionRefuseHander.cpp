#include "StdAfx.h"
#include "CGMissionRefuse.h"

using namespace Packets;

uint CGMissionRefuseHandler::Execute(CGMissionRefuse* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;

}