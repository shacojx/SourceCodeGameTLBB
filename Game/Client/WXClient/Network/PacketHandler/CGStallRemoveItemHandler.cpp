#include "stdafx.h"
#include "CGStallRemoveItem.h"


uint CGStallRemoveItemHandler::Execute( CGStallRemoveItem* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
