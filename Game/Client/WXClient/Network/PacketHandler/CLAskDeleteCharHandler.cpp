#include "StdAfx.h"
#include "CLAskDeleteChar.h"


UINT CLAskDeleteCharHandler::Execute(CLAskDeleteChar* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;
}