// GCShopSoldListHandler.cpp
#include "stdafx.h"
#include "GCShopSoldList.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\Object\ObjectManager.h"
#include "..\..\Object\Character\Obj_PlayerOther.h"
#include "..\..\DataPool\GMDataPool.h"
#include "..\..\Event\GMEventSystem.h"
#include "..\..\Object\Item\Obj_Item.h"
#include "..\..\Action\GMActionSystem.h"
#include "TDException.h"
#include "..\..\Sound\GMSoundSystem.h"

uint GCShopSoldListHandler::Execute( GCShopSoldList* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		//已经关上了就算了
		//if(CDataPool::GetMe()->Booth_IsClose())
		//	return PACKET_EXE_CONTINUE ;

		INT nNum = pPacket->GetMerchadiseNum();
		if(nNum == 0)
		{//空的没东西
			if(CDataPool::GetMe()->Booth_GetSoldNumber() != 0)
			{
				CDataPool::GetMe()->Booth_Sold_Clear();
			}
		}
		else
		{
			//清空原有商品列表
			CDataPool::GetMe()->Booth_Sold_Clear();

			CSoundSystemFMod::_PlayUISoundFunc(25+59);

			//添加到数据池中
			for(INT i=0; i<nNum; i++)
			{
				GCShopSoldList::_MERCHANDISE_ITEM* pNewItem = &(pPacket->GetMerchadiseList()[i]);
				tObject_Item* pItem = NULL;

				pItem = CObject_Item::NewItem( pNewItem->item_data.m_ItemIndex);
				pItem->SetGUID(
					pNewItem->item_data.m_ItemGUID.m_World, 
					pNewItem->item_data.m_ItemGUID.m_Server, 
					pNewItem->item_data.m_ItemGUID.m_Serial);

				pItem->SetExtraInfo(&(pNewItem->item_data));

				TDAssert(pItem);
				
				//设置该物品在货架上的位置因为是回购物品它的位置应该是200
				pItem->SetPosIndex(i);
				AxTrace(0,0,"pItem->SetPosIndex ＝ %d",i);
				//设置该物品的数量（每一组的数量）
				pItem->SetNumber(pNewItem->item_data.GetItemCount());
				//回购物品当然只有一个
				pItem->SetMax(1);
				//加入数据池
				CDataPool::GetMe()->Booth_SetSoldItem(i, pItem);
				CDataPool::GetMe()->Booth_SetSoldPrice(i, pNewItem->iPrice);
			}
			//设置回购商品的数量
			CDataPool::GetMe()->Booth_SetSoldNumber(nNum);
		}
		
		//更新到ActionSystem
		CActionSystem::GetMe()->Booth_Update();
		//通知UI
		CEventSystem::GetMe()->PushEvent(GE_UPDATE_BOOTH);
	}

	return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
