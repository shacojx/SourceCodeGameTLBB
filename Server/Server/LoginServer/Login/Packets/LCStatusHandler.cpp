#include "stdafx.h"

#include "LCStatus.h"


UINT LCStatusHandler::Execute(LCStatus* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;
}