#include "StdAfx.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\Event\GMEventSystem.h"
#include "..\..\DataPool\GMDataPool.h"
#include "..\..\Object\Object.h"
#include "..\..\Object\Item\Obj_Item.h"

#include "GCShopBuy.h"
#include "..\..\Sound\GMSoundSystem.h"

uint GCShopBuyHandler::Execute( GCShopBuy* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		BYTE		bIsBuyOk	=	pPacket->GetBuyOk(); 
		UINT		ItemIndex	=	pPacket->GetIndex();
		BYTE		ItemNum		=	pPacket->GetNum();

		//已经关上了就算了
		if(CDataPool::GetMe()->Booth_IsClose())
			return PACKET_EXE_CONTINUE ;

		if(bIsBuyOk == GCShopBuy::BUY_MONEY_FAIL)
		{
			CGameProcedure::s_pEventSystem->PushEvent(GE_NEW_DEBUGMESSAGE,"金钱不足.");
		}
		else if(bIsBuyOk == GCShopBuy::BUY_RMB_FAIL)
		{
			CGameProcedure::s_pEventSystem->PushEvent(GE_NEW_DEBUGMESSAGE,"元宝不足.");
		}
		else if(bIsBuyOk == GCShopBuy::BUY_BAG_FULL)
		{
			CGameProcedure::s_pEventSystem->PushEvent(GE_NEW_DEBUGMESSAGE,"包裹已满");
		}
		else if(bIsBuyOk == GCShopBuy::BUY_NO_MERCH)
		{
			CGameProcedure::s_pEventSystem->PushEvent(GE_NEW_DEBUGMESSAGE,"商品已改变，请重新打开");
		}
		else if(bIsBuyOk == GCShopBuy::BUY_OK)
		{
	        tObject_Item* pItem = CObject_Item::NewItem( ItemIndex );
			CHAR szMsg[256];
			
			_snprintf(szMsg, 256, "您购买了%d个%s",	ItemNum, pItem->GetName());

			ADDTALKMSG(szMsg);
			CObject_Item::DestroyItem((CObject_Item*)pItem);
			//CGameProcedure::s_pEventSystem->PushEvent(GE_NEW_DEBUGMESSAGE,"交易成功");
			CSoundSystemFMod::_PlayUISoundFunc(25+59);
		}

		return PACKET_EXE_CONTINUE ;

		__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
