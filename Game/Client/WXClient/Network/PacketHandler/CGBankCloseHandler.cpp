#include"stdafx.h"

#include "CGBankClose.h"


uint CGBankCloseHandler::Execute( CGBankClose* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
