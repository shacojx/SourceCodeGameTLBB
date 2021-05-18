#include "StdAfx.h"
#include "CGShopClose.h"

uint CGShopCloseHandler::Execute( CGShopClose* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
