#include "StdAfx.h"
#include "CGMissionContinue.h"

using namespace Packets;

uint CGMissionContinueHandler::Execute(CGMissionContinue* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;

}