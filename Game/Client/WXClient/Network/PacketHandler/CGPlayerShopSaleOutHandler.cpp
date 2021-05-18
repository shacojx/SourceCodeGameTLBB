/*
通知服务器此商店已经被盘出
*/
#include "stdafx.h"
#include "CGPlayerShopSaleOut.h"

UINT CGPlayerShopSaleOutHandler::Execute( CGPlayerShopSaleOut* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
