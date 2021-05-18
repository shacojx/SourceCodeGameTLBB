#include"stdafx.h"

#include "CGBankAddItem.h"


uint CGBankAddItemHandler::Execute( CGBankAddItem* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
