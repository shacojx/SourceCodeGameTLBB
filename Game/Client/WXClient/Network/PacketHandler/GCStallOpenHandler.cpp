#include "stdafx.h"

#include "GCStallOpen.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\DataPool\GMDataPool.h"
#include "..\..\Event\GMEventSystem.h"
#include "..\..\Object\ObjectManager.h"
#include "..\..\Object\Object.h"
#include "..\..\Object\Character\Obj_PlayerMySelf.h"
#include "..\..\Object\Character\Obj_Character.h"
#include "..\..\DataPool\GMDP_CharacterData.h"
#include "..\..\Object\Item\Obj_Item.h"
#include "..\..\Action\GMActionSystem.h"
#include "TDException.h"

uint GCStallOpenHandler::Execute( GCStallOpen* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION
	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{

		ObjID_t	OwnerObjId	=	pPacket->GetObjID();
		BYTE	ItemNum		=	pPacket->GetMerchadiseNum();

		//清空原有商品列表
		if(OwnerObjId == CObjectManager::GetMe()->GetMySelf()->GetServerID())
		{//看自己的摊位

			//清空摊位
			CDataPool::GetMe()->MyStallBox_Clear();

			//初始化自己摊位
			CDataPool::GetMe()->MyStallBox_SetIsOpen(TRUE);//设置摊主ID
			CDataPool::GetMe()->MyStallBox_SetObjID(OwnerObjId);
			CDataPool::GetMe()->MyStallBox_SetStallName(pPacket->GetShopName()); //设置摊位名
			//CDataPool::GetMe()->MyStallBox_SetOwnerName(pChar->); //设置摊主名
			CDataPool::GetMe()->MyStallBox_SetFirstPage(pPacket->GetFirstPage());//设置首页
			CDataPool::GetMe()->MyStallBox_SetOwnerGUID(pPacket->GetGUID());
			CDataPool::GetMe()->MyStallBox_SetDefaultPage(pPacket->GetFirstPage());

			//每个物品添加到数据池中
			for(INT i=0; i<ItemNum; i++)
			{

				GCStallOpen::_STALL_ITEM* pNewItem = &(pPacket->GetMerchadiseList()[i]);

				if(pNewItem->bIsPet == TRUE)
				{//自己背包里的宠物
					if(pNewItem->PetGuid.IsNull())
					{//此宠物GUID无效，只是序列号刷过来
						CDataPool::GetMe()->MyStallBox_SetPet(pNewItem->nIndex, NULL, FALSE);
						CDataPool::GetMe()->MyStallBox_SetPetPrice(pNewItem->nIndex, pNewItem->nPrice);
						CDataPool::GetMe()->MyStallBox_SetPetSerial(pNewItem->nIndex, pNewItem->nSerial);
					}
					else
					{//宠物和序列号一起刷过来
						INT IndexInBag = CDataPool::GetMe()->Pet_GetPetByGuid(pNewItem->PetGuid);
						//锁住这个位置的宠物
						SDATA_PET* pPetData = CDataPool::GetMe()->Pet_GetPet(IndexInBag);
						//验证这个宠物是不是被锁住了
						Assert(pPetData->m_nIsPresent == 2);
						CDataPool::GetMe()->MyStallBox_SetPet(pNewItem->nIndex, pPetData, FALSE);
						CDataPool::GetMe()->MyStallBox_SetPetPrice(pNewItem->nIndex, pNewItem->nPrice);
						CDataPool::GetMe()->MyStallBox_SetPetSerial(pNewItem->nIndex, pNewItem->nSerial);
					}
				}
				else
				{
					if(pNewItem->item.IsNullType())
					{//没有物品只是序列号刷过来
						CDataPool::GetMe()->MyStallBox_SetItem(pNewItem->nIndex, NULL);
						CDataPool::GetMe()->MyStallBox_SetItemPrice(pNewItem->nIndex, pNewItem->nPrice);//设置摊位物品价格
						CDataPool::GetMe()->MyStallBox_SetItemSerial(pNewItem->nIndex, pNewItem->nSerial);//设置摊位物品序列号
					}
					else
					{//物品和序列号一起刷过来
						//创建商品实例
						tObject_Item* pItem = CObject_Item::NewItem( pNewItem->item.m_ItemIndex);
						if(!pItem) continue;

						pItem->SetGUID(
							pNewItem->item.m_ItemGUID.m_World, 
							pNewItem->item.m_ItemGUID.m_Server, 
							pNewItem->item.m_ItemGUID.m_Serial);
						TDAssert(pItem);

						//放到货架的制定位置上
						CDataPool::GetMe()->MyStallBox_SetItem(pNewItem->nIndex, pItem);
						CDataPool::GetMe()->MyStallBox_SetItemExtraInfo(pNewItem->nIndex, FALSE, &(pNewItem->item));
						CDataPool::GetMe()->MyStallBox_SetItemPrice(pNewItem->nIndex, pNewItem->nPrice);//设置摊位物品价格
						CDataPool::GetMe()->MyStallBox_SetItemSerial(pNewItem->nIndex, pNewItem->nSerial);//设置摊位物品序列号
					}
				}
			}

			// 发送打开自己摊位的事件
			CActionSystem::GetMe()->UserStallSale_Update();
			CEventSystem::GetMe()->PushEvent(GE_OPEN_STALL_SALE);
			//打开宠物列表界面
			CEventSystem::GetMe()->PushEvent(GE_OPEN_PET_LIST,0);
		}
		else
		{//别人的摊位
			//清空目标摊位
			CDataPool::GetMe()->OtStallBox_Clear(TRUE);

			//初始化目标摊位
			CDataPool::GetMe()->OtStallBox_SetIsOpen(TRUE);//设置摊主ID
			CDataPool::GetMe()->OtStallBox_SetObjID(OwnerObjId);
			CDataPool::GetMe()->OtStallBox_SetStallName(pPacket->GetShopName()); //设置摊位名
			//CDataPool::GetMe()->MyStallBox_SetOwnerName(pPacket->Get); //设置摊主名
			CDataPool::GetMe()->OtStallBox_SetFirstPage(pPacket->GetFirstPage());//设置首页
			CDataPool::GetMe()->OtStallBox_SetOwnerGUID(pPacket->GetGUID());

			CObject* pObj = (CObject*)CObjectManager::GetMe()->FindServerObject(OwnerObjId);

			LPCSTR szName = ((CObject_Character*)pObj)->GetCharacterData()->Get_Name();
			CDataPool::GetMe()->OtStallBox_SetOwnerName(szName);
			CDataPool::GetMe()->OtStallBox_SetDefaultPage(pPacket->GetFirstPage());
			
			//每个物品添加到数据池中
			for(INT i=0; i<ItemNum; i++)
			{
				GCStallOpen::_STALL_ITEM* pNewItem = &(pPacket->GetMerchadiseList()[i]);
				//创建商品实例
				tObject_Item* pItem = CObject_Item::NewItem( pNewItem->item.m_ItemIndex);
				if(!pItem) continue;

				pItem->SetGUID(
					pNewItem->item.m_ItemGUID.m_World, 
					pNewItem->item.m_ItemGUID.m_Server, 
					pNewItem->item.m_ItemGUID.m_Serial);
				TDAssert(pItem);

				//放到货架的制定位置上
				CDataPool::GetMe()->OtStallBox_SetItem(pNewItem->nIndex, pItem);
				CDataPool::GetMe()->OtStallBox_SetItemExtraInfo(pNewItem->nIndex, FALSE, &(pNewItem->item));
				CDataPool::GetMe()->OtStallBox_SetItemPrice(pNewItem->nIndex, pNewItem->nPrice);//设置摊位物品价格
				CDataPool::GetMe()->OtStallBox_SetItemSerial(pNewItem->nIndex, pNewItem->nSerial);//设置摊位物品序列号
			}

			// 发送打开别人摊位的事件
			CActionSystem::GetMe()->UserStallBuy_Update();

			INT nObjId = CObjectManager::GetMe()->FindServerObject(pPacket->GetObjID())->GetID();
			CEventSystem::GetMe()->PushEvent(GE_OPEN_STALL_BUY,nObjId);
		}
	}
	return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
