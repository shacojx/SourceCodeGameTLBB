#include"stdafx.h"

#include "GCBankAddItem.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\Object\ObjectManager.h"
#include "..\..\DataPool\GMDataPool.h"
#include "..\..\Event\GMEventSystem.h"
#include "..\..\Object\Item\Obj_Item.h"
#include "..\..\Action\GMActionSystem.h"
#include "TDException.h"
uint GCBankAddItemHandler::Execute( GCBankAddItem* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION
		if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
		{
			BYTE	indexTo = pPacket->GetIndexTo();
			BYTE	indexFrom = pPacket->GetIndexFrom();

			if(pPacket->GetFromType() == GCBankAddItem::EQUIP_POS)
			{

			}
			else if(pPacket->GetFromType() == GCBankAddItem::BAG_POS)
			{
				tObject_Item*	pItemFrom = CDataPool::GetMe()->UserBag_GetItem(indexFrom);
				tObject_Item*	pItemTo = CDataPool::GetMe()->UserBank_GetItem(indexTo);

				switch(pPacket->GetOperateType())
				{
				case GCBankAddItem::OPERATE_MOVE:		// 移动到空格
					{
						CDataPool::GetMe()->UserBank_SetItem(indexTo, pItemFrom);
						CDataPool::GetMe()->UserBag_SetItem(indexFrom, NULL, FALSE);
					}
					break;
				case GCBankAddItem::OPERATE_SPLICE:		// 合并
					{
						//CDataPool::GetMe()->UserBank_SetItem(indexTo, pItemFrom);
						//CDataPool::GetMe()->UserBag_SetItem(indexFrom, NULL, FALSE);
					}
					break;
				case GCBankAddItem::OPERATE_SWAP:		// 交换
					{
						CDataPool::GetMe()->UserBank_SetItem(indexTo, pItemFrom, FALSE);
						CDataPool::GetMe()->UserBag_SetItem(indexFrom, pItemTo, FALSE);
					}
					break;
				default:
					break;
				}
				CActionSystem::GetMe()->UserBank_Update();
				CActionSystem::GetMe()->UserBag_Update();
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"物品存入银行成功");
				CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);
				CEventSystem::GetMe()->PushEvent(GE_UPDATE_BANK);
			}
			else if(pPacket->GetFromType() == GCBankAddItem::ERROR_POS)
			{
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"物品存入银行失败");
			}

		}

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
