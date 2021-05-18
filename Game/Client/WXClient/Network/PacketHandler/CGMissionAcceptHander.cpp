#include "StdAfx.h"
#include "CGAskMissionDesc.h"

using namespace Packets;

uint CGAskMissionDescHandler::Execute(CGAskMissionDesc* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;

}