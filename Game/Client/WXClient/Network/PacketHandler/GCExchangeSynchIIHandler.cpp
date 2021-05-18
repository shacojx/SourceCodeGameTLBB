/*
客户端收到对方客户端交易盒中物品,金钱的变化
*/
#include "stdafx.h"
#include "GCExchangeSynchII.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\DataPool\GMDataPool.h"
#include "..\..\Event\GMEventSystem.h"
#include "..\..\Object\Item\Obj_Item.h"
#include "..\..\Action\GMActionSystem.h"
#include "..\..\Object\Item\Obj_Item_Equip.h"
#include "TDException.h"

using namespace EXCHANGE_MSG;
uint GCExchangeSynchIIHandler::Execute( GCExchangeSynchII* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION
	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		BYTE	IsMyself	=	pPacket->GetIsMyself();		
		BYTE	Opt			=	pPacket->GetOpt();	
		BYTE	FromType	=	pPacket->GetFromType();
		BYTE	ToType		=	pPacket->GetToType();
		BYTE	FromIndex	=	pPacket->GetFromIndex();
		BYTE	ToIndex		=	pPacket->GetToIndex();
		UINT	Money		=	pPacket->GetMoney();
		PET_GUID_t	PetGuid = 	pPacket->GetPetGuid(); 

		if(IsMyself)
		{//修改自己的交易盒
			switch(Opt)
			{
				case OPT_ADDITEM:
					{
						switch(FromType)
						{
						case POS_BAG:
							{
								tObject_Item* pBagItem = CDataPool::GetMe()->UserBag_GetItem(FromIndex);
								// 锁定FromIndex中的物品
								pBagItem->SetLock(TRUE);

								//克隆物品
								tObject_Item* pNewItem  = CObject_Item::NewItem( pBagItem->GetIdTable());
								((CObject_Item*)pNewItem)->Clone((CObject_Item*)pBagItem);

								CDataPool::GetMe()->MyExBox_SetItem(ToIndex, pNewItem);
		
							}
							break;
						case POS_EQUIP:
							{
							}
							break;
						case POS_PET:
							{
							}
							break;
						default:
							break;
						}
					}
					break;
				case OPT_REMOVEITEM:
					{
						switch(ToType)
						{
						case POS_BAG:
							{
								//需要解除背包对应物品的锁定(注意，克隆的物品GUID相同，所以可以这样操作)
								if(CDataPool::GetMe()->MyExBox_GetItem(FromIndex))
								{
									__int64 ItemGUID = CDataPool::GetMe()->MyExBox_GetItem(FromIndex)->GetGUID();
									tObject_Item* pBagItem = CDataPool::GetMe()->UserBag_GetItem(CDataPool::GetMe()->UserBag_GetItemIndexByGUID(ItemGUID));
									if(pBagItem)
									{
										//去除锁定
										pBagItem->SetLock(FALSE);
									}
								}

								CDataPool::GetMe()->MyExBox_SetItem(FromIndex, NULL);
							}
							break;
						case POS_EQUIP:
							{
							}
							break;
						case POS_PET:
							{
							}
							break;
						default:
							break;
						}
					}
					break;
				case OPT_MONEY:
					{//金钱改变不发给自己了，省条消息
					}
					break;
				case OPT_REMOVEMONEY:
					{//金钱改变不发给自己了，省条消息
					}
					break;
				case OPT_ADDPET:
					{//加入宠物到交易盒中
						INT IndexInBag = CDataPool::GetMe()->Pet_GetPetByGuid(PetGuid);
						//锁住这个位置的宠物
						SDATA_PET* pPetData = CDataPool::GetMe()->Pet_GetPet(IndexInBag);
						CDataPool::GetMe()->MyExBox_SetPet(ToIndex, pPetData, FALSE);

						//需要通知界面标记这个宠物已经锁定
						// …………
					}
					break;
				case OPT_REMOVEPET:
					{//从交易盒中拿出宠物
						INT IndexInExBox = CDataPool::GetMe()->MyExBox_GetPetByGuid(PetGuid);
						CDataPool::GetMe()->MyExBox_SetPet(IndexInExBox, NULL, FALSE);
						INT IndexInBag = CDataPool::GetMe()->Pet_GetPetByGuid(PetGuid);
						//解锁这个位置的宠
						//......
					}
					break;
				default:
					break;
			}
			//更新
			CActionSystem::GetMe()->UserExchangeSelf_Update();
		}
		else
		{//修改对方的交易盒
			switch(Opt)
			{
			case OPT_ADDITEM:
				{
					EXCHANGE_MSG::_EXCHANGE_ITEM* pNewItem = pPacket->GetItem();
					tObject_Item* pItem = NULL;

					if(pNewItem->isBlueEquip)
					{
						pItem = CObject_Item::NewItem( pNewItem->item_data.m_ItemIndex);
						pItem->SetGUID(
							pNewItem->item_data.m_ItemGUID.m_World, 
							pNewItem->item_data.m_ItemGUID.m_Server, 
							pNewItem->item_data.m_ItemGUID.m_Serial);
						TDAssert(pItem);
						CDataPool::GetMe()->OtExBox_SetItem(ToIndex, pItem);
						CDataPool::GetMe()->OtExBox_SetItemExtraInfo(ToIndex, FALSE, &pNewItem->item_data);
					}
					else
					{
						pItem = CObject_Item::NewItem( pNewItem->item_guid);
						TDAssert(pItem);
						CDataPool::GetMe()->OtExBox_SetItem(ToIndex, pItem);
					}
				}
				break;
			case OPT_REMOVEITEM:
				{
					CDataPool::GetMe()->OtExBox_SetItem(FromIndex, NULL);
				}
				break;
			case OPT_MONEY:
				{
					CDataPool::GetMe()->OtExBox_SetMoney(Money);
				}
				break;
			case OPT_REMOVEMONEY:
				{//废了
				}
				break;
			case OPT_ADDPET:
				{

				}
				break;
			case OPT_REMOVEPET:
				{
					INT IndexInExBox = CDataPool::GetMe()->OtExBox_GetPetByGuid(PetGuid);
					SDATA_PET* pPetData = CDataPool::GetMe()->OtExBox_GetPet(IndexInExBox);
					SAFE_DELETE(pPetData);
					CDataPool::GetMe()->OtExBox_SetPet(IndexInExBox, NULL, FALSE);
				}
				break;
			default:
				break;
			}
			//更新
			CActionSystem::GetMe()->UserExchangeOther_Update();
		}
	}

	//刷新交易盒子
	CEventSystem::GetMe()->PushEvent(GE_UPDATE_EXCHANGE);
	//刷新背包（）
	CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);
	//更新宠物列表界面

	return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
