// GCShopSellHandler.cpp
#include "stdafx.h"
#include "GCShopSell.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\Sound\GMSoundSystem.h"

uint GCShopSellHandler::Execute( GCShopSell* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION
		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
