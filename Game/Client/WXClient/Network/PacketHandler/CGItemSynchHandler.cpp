/*
同步客户端物品变化
*/
#include "stdafx.h"
#include "CGItemSynch.h"

uint CGItemSynchHandler::Execute( CGItemSynch* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
