
#include "stdafx.h"
#include "CGExchangeCancel.h"

uint CGExchangeCancelHandler::Execute( CGExchangeCancel* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
