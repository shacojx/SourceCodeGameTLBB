#include "stdafx.h"
#include "GCPlayerShopOpenStall.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\DataPool\GMDataPool.h"
#include "..\..\Event\GMEventSystem.h"
#include "..\..\Object\Item\Obj_Item.h"
#include "..\..\Action\GMActionSystem.h"
#include "TDException.h"

UINT GCPlayerShopOpenStallHandler::Execute( GCPlayerShopOpenStall* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION
		if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
		{
			_PLAYERSHOP_GUID		ShopID		=	pPacket->GetShopID();			//ÉÌµêID
			BYTE					nStallIndex =	pPacket->GetStallIndex();
			BYTE					bOpen		=	pPacket->GetOpen();

			if(bOpen)
			{
				CDataPool::GetMe()->PlayerShop_SetStallEnableByIndex(TRUE, nStallIndex,  PLAYER_SHOP::STALL_OPEN);
			}
			else
			{
				CDataPool::GetMe()->PlayerShop_SetStallEnableByIndex(TRUE, nStallIndex,  PLAYER_SHOP::STALL_CLOSE);
			}
			CEventSystem::GetMe()->PushEvent(GE_PS_SELF_ITEM_CHANGED);
		}
		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
