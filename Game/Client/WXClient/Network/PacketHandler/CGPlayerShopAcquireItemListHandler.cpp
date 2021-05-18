/*
客户端申请商店的商品列表
*/
#include "StdAfx.h"
#include "CGPlayerShopAcquireItemList.h"

UINT CGPlayerShopAcquireItemListHandler::Execute( CGPlayerShopAcquireItemList* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
