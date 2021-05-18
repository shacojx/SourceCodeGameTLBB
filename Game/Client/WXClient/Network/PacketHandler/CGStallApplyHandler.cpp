#include "stdafx.h"

#include "CGStallApply.h"


uint CGStallApplyHandler::Execute( CGStallApply* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
