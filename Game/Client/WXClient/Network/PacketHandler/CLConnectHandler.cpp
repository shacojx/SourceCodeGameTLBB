#include "StdAfx.h"
#include "CLConnect.h"


UINT CLConnectHandler::Execute(CLConnect* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

	
		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;

}