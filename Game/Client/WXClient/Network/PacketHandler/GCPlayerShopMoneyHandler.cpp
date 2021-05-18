#include "StdAfx.h"
#include "GCPlayerShopMoney.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\DataPool\GMDataPool.h"
#include "..\..\Event\GMEventSystem.h"
#include "..\..\Object\Item\Obj_Item.h"
#include "..\..\Action\GMActionSystem.h"
#include "TDException.h"

UINT GCPlayerShopMoneyHandler::Execute( GCPlayerShopMoney* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
		{
			_PLAYERSHOP_GUID	nShopID	=	pPacket->GetShopID();		//商店ID
			BYTE	nType	=	pPacket->GetType();						//存到哪
			UINT	uAmount	=	pPacket->GetAmount();					//数量
		
			if(nType == GCPlayerShopMoney::TYPE_BASE_MONEY)
			{
				CDataPool::GetMe()->PlayerShop_SetBaseMoney(TRUE, uAmount);
			}
			else if(nType == GCPlayerShopMoney::TYPE_PROFIT_MONEY)
			{
				CDataPool::GetMe()->PlayerShop_SetProfitMoney(TRUE, uAmount);
			}
		}

		//通知商店更新
		CEventSystem::GetMe()->PushEvent(GE_PS_SELF_ITEM_CHANGED);

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
