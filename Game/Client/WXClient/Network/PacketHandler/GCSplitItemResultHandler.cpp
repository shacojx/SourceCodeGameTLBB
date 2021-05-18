//GCSplitItemResult.h

#include "StdAfx.h"
#include "GCSplitItemResult.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\Object\ObjectManager.h"
#include "..\..\Object\Item\Obj_Item.h"
#include "..\..\DataPool\GMDataPool.h"
#include "..\..\Action\GMActionSystem.h"
#include "..\..\Event\GMEventSystem.h"



uint GCSplitItemResultHandler::Execute( GCSplitItemResult* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	//处理拆分完成后的结果显示
	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		CObjectManager* pObjectManager = CObjectManager::GetMe();

		//成功拆分
		if( GCSplitItemResult::RESULT_SUCCEED == pPacket->getResult())
		{
			//需要处理被拆分的物品和拆分生成的物品
			INT nCon = pPacket->getContainer();
			switch( nCon )
			{
			case GCSplitItemResult::BAG_CON:
				{
					// 生成新的物品
					WORD wBagIndex = pPacket->getConIndex();
					_ITEM* pItem = pPacket->getItem();

					CObject_Item* pItemObj = CObject_Item::NewItem(pItem->m_ItemIndex);
					if(!pItemObj) 
						return PACKET_EXE_CONTINUE;

					pItemObj->SetGUID(
						pItem->m_ItemGUID.m_World, 
						pItem->m_ItemGUID.m_Server,
						pItem->m_ItemGUID.m_Serial);

					pItemObj->SetExtraInfo(pItem);

					CDataPool::GetMe()->UserBag_SetItem(wBagIndex, pItemObj );
					CActionSystem::GetMe()->UserBag_Update();
			
					//修改被拆分的物品
					
				}
				break;
			case GCSplitItemResult::BANK_CON:
				{
					WORD wBankIndex = pPacket->getConIndex();
					_ITEM* pItem = pPacket->getItem();

					CObject_Item* pItemObj = CObject_Item::NewItem(pItem->m_ItemIndex);
					if(!pItemObj) 
						return PACKET_EXE_CONTINUE;

					pItemObj->SetGUID(
						pItem->m_ItemGUID.m_World, 
						pItem->m_ItemGUID.m_Server,
						pItem->m_ItemGUID.m_Serial);

					pItemObj->SetExtraInfo(pItem);

					CDataPool::GetMe()->UserBank_SetItem(wBankIndex, pItemObj );
					CActionSystem::GetMe()->UserBank_Update();

					//修改被拆分的物品
					
				}
				break;
			default:
				break;
			}
		}
		//拆分失败，没有空格来容纳拆分后的物品
		else if( GCSplitItemResult::RESULT_FALSE_NOGRID == pPacket->getResult() )
		{
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"容器已满，不能完成拆分");
		}
		//拆分失败，拆分规则上的原因
		else
		{
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"拆分失败");
		}

		//解除被拆分物品的锁定
		INT nTypeOwner	= CDataPool::GetMe()->Split_GetItemConta();
		INT nItemPos	= CDataPool::GetMe()->Split_GetItemPos();
		switch( nTypeOwner ) 
		{
		case tObject_Item::IO_MYSELF_PACKET:				// 来自玩家的背包
			{
				tObject_Item* pItem = CDataPool::GetMe()->UserBag_GetItem(nItemPos);

				if(pItem)
				{
					pItem->SetLock(FALSE);
				}
				CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);
			}
			break;
		case tObject_Item::IO_MYSELF_BANK:					// 玩家的银行
			{
				tObject_Item* pItem = CDataPool::GetMe()->UserBank_GetItem(nItemPos);

				if(pItem)
				{
					pItem->SetLock(FALSE);
				}
				CEventSystem::GetMe()->PushEvent(GE_UPDATE_BANK);
			}
			break;
		default:
			break;
		}
		CDataPool::GetMe()->Split_SetItemConta(-1);
		CDataPool::GetMe()->Split_SetItemPos(-1);
	}
	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
