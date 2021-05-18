#include "StdAfx.h"
#include "CLAskCharList.h"


UINT	CLAskCharListHandler::Execute(CLAskCharList* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;
}