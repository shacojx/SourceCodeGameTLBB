#include "StdAfx.h"
#include "CGUseItem.h"


uint CGUseItemHandler::Execute(CGUseItem* pPacket,Player* pPlayer)
{
	__ENTER_FUNCTION
		
		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return	PACKET_EXE_ERROR;
}

