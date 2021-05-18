#include "stdafx.h"

#include "CGStallOpen.h"


uint CGStallOpenHandler::Execute( CGStallOpen* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
