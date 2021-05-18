#include "StdAfx.h"
#include "CLAskCreateChar.h"


UINT	CLAskCreateCharHandler::Execute(CLAskCreateChar* pPacket, Player* pPlayer)
{
	__ENTER_FUNCTION
		
		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;
}