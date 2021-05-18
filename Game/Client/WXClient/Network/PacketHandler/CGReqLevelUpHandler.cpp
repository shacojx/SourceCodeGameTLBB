#include "StdAfx.h"
#include "CGReqLevelUp.h"


uint CGReqLevelUpHandler::Execute(CGReqLevelUp* pPacket,Player* pPlayer)
{
	__ENTER_FUNCTION
		
		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return	PACKET_EXE_ERROR;
}