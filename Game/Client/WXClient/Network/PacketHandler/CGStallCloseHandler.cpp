
#include "stdafx.h"
#include "CGStallClose.h"


uint CGStallCloseHandler::Execute( CGStallClose* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
