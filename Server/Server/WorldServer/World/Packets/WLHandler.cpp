#include "stdafx.h"

#include "WLRetCharLogin.h"
UINT WLRetCharLoginHandler::Execute(WLRetCharLogin* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION
	
		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;
}

#include "WLRetDeleteChar.h"
UINT WLRetDeleteCharHandler::Execute(WLRetDeleteChar* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;
}

