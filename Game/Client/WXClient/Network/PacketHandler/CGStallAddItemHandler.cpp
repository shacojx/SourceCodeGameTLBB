#include "stdafx.h"

#include "CGStallAddItem.h"


uint CGStallAddItemHandler::Execute( CGStallAddItem* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
