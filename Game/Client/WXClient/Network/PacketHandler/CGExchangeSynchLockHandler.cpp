
#include "stdafx.h"
#include "CGExchangeSynchLock.h"


uint CGExchangeSynchLockHandler::Execute( CGExchangeSynchLock* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
