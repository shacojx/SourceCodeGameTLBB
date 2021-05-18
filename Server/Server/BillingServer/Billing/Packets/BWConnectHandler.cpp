#include "stdafx.h"
#include "BWConnect.h"

UINT BWConnectHandler::Execute( BWConnect* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION	
	
		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION	

		return PACKET_EXE_ERROR;
}