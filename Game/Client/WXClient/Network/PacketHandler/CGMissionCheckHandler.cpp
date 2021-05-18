#include "stdafx.h"
#include "CGMissionCheck.h"
using namespace Packets;
uint CGMissionCheckHandler::Execute(CGMissionCheck* pPacket,Player* pPlayer)
{
	__ENTER_FUNCTION
	
	return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

	return	PACKET_EXE_ERROR;
}

