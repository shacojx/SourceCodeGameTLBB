#include "stdafx.h"
#include "BLRetAuth.h"
#include "Log.h"
UINT BLRetAuthHandler::Execute(BLRetAuth* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}
