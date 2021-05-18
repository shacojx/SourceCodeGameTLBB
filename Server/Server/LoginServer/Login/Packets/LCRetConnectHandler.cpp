#include "stdafx.h"

#include "LCRetConnect.h"

UINT LCRetConnectHandler::Execute(LCRetConnect* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION
		
		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;
}