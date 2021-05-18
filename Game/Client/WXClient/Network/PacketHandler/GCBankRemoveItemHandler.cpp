#include"stdafx.h"

#include "GCBankRemoveItem.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\Object\ObjectManager.h"
#include "..\..\DataPool\GMDataPool.h"
#include "..\..\Event\GMEventSystem.h"
#include "..\..\Object\Item\Obj_Item.h"
#include "..\..\Action\GMActionSystem.h"
#include "TDException.h"
uint GCBankRemoveItemHandler::Execute( GCBankRemoveItem* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
		{
			BYTE	indexTo = pPacket->GetIndexTo();
			BYTE	indexFrom = pPacket->GetIndexFrom();

			if(pPacket->GetToType() == GCBankRemoveItem::EQUIP_POS)
			{

			}
			else if(pPacket->GetToType() == GCBankRemoveItem::BAG_POS)
			{
				switch( pPacket->GetOperateType() )
				{
				case GCBankRemoveItem::OPERATE_MOVE:		// 移动物品到空格
					{
						tObject_Item*	pItem = CDataPool::GetMe()->UserBank_GetItem(indexFrom);
						CDataPool::GetMe()->UserBag_SetItem(indexTo, pItem);
						CDataPool::GetMe()->UserBank_SetItem(indexFrom, NULL, FALSE);
						CActionSystem::GetMe()->UserBank_Update();
						CActionSystem::GetMe()->UserBag_Update();

						CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"物品取出银行成功");
						CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);
						CEventSystem::GetMe()->PushEvent(GE_UPDATE_BANK);
					}
					break;
				case GCBankRemoveItem::OPERATE_SPLICE:		// 叠加物品
					{
						
					}
					break;
				case GCBankRemoveItem::OPERATE_SPLIT:		// 拆分物品
					{
						
					}
					break;
				case GCBankRemoveItem::OPERATE_SWAP:		// 交换物品
					{
						tObject_Item*	pItem  = CDataPool::GetMe()->UserBank_GetItem(indexFrom);
						tObject_Item*	pItem1 = CDataPool::GetMe()->UserBag_GetItem(indexTo);
						CDataPool::GetMe()->UserBag_SetItem(indexTo, pItem, FALSE);
						CDataPool::GetMe()->UserBank_SetItem(indexFrom, pItem1, FALSE);
						CActionSystem::GetMe()->UserBank_Update();
						CActionSystem::GetMe()->UserBag_Update();

						CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"银行和背包交换物品成功");
						CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);
						CEventSystem::GetMe()->PushEvent(GE_UPDATE_BANK);
					}
					break;
				default:
					break;
				}

			}
			else if(pPacket->GetToType() == GCBankRemoveItem::ERROR_POS)
			{
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"移动物品失败");
			}
		}


		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
