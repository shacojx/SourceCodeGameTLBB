#include "stdafx.h"

#include "LCRetLogin.h"


UINT	LCRetLoginHandler::Execute(LCRetLogin* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION
	
		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;
}