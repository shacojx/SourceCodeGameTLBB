#include "StdAfx.h"
#include "CLAskCharLogin.h"

UINT CLAskCharLoginHandler::Execute(CLAskCharLogin* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION
	
		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;
}