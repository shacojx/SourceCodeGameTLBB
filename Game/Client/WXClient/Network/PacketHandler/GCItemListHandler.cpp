/*
物品列表发给客户端
*/
#include "stdafx.h"
#include "GCItemList.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\DataPool\GMDataPool.h"
#include "..\..\Event\GMEventSystem.h"
#include "..\..\Object\Item\Obj_Item.h"
#include "..\..\Action\GMActionSystem.h"
#include "..\..\Object\Item\Obj_Item_Equip.h"
#include "..\..\Object\ObjectManager.h"
#include "..\..\Object\Character\Obj_PlayerMySelf.h"
#include "TDException.h"
#include "CGPlayerShopAcquireItemList.h"
using namespace ITEM_SYNCH_STRUCT;

#define OPEN_TYPE_ITEM 1
#define OPEN_TYPE_PET  2

uint GCItemListHandler::Execute( GCItemList* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION
		if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
		{
			BYTE		Opt	= pPacket->GetOpt();	
			switch(Opt)
			{
			case GCItemList::OPT_ADD_ONE_ITEM:
				{

				}
				break;
			case GCItemList::OPT_ADD_ITEM_LIST:
				{
					BYTE					nItemNum	= pPacket->GetItemNum();
					GCItemList::_MSG_ITEM*	ItemData	= pPacket->GetItemList();
					BYTE					nExtraLen	= pPacket->GetExtraLength();
					CHAR*					ExtraData	= pPacket->GetExtraInfo();
					BYTE					bFlag		= 0;

					GCPlayerShopItemListForSelf_t ExtraInfoData;

					if(nExtraLen == GCPlayerShopItemListForOther_t::GetSize() && (BYTE)ExtraData[0] == GCPlayerShopItemListForSelf_t::FOR_BUYER)
					{
						bFlag = 1;//玩家商店中的商品列表(别人的店)
					}
					else if(nExtraLen == GCPlayerShopItemListForSelf_t::GetSize() && (BYTE)ExtraData[0] == GCPlayerShopItemListForSelf_t::FOR_MANAGER)
					{
						bFlag = 2;//玩家商店中的商品列表(自己的店)
					}
				
					switch(bFlag)
					{
					case 1:
						{//玩家商店中的商品列表，非本人接收到的
							//商店列表存入数据池

							//要查看商店的shopID存入数据池
							GCPlayerShopItemListForOther_t ExtraInfoData;
							memcpy(&ExtraInfoData, ExtraData, GCPlayerShopItemListForOther_t::GetSize());

							if(ExtraInfoData.m_bIsItemList)
							{
								//需要清空CObject_Item中的数据
								CDataPool::GetMe()->PlayerShop_CleanObjItem(FALSE);
								//清空已有数据
								CDataPool::GetMe()->PlayerShop_CleanUp(FALSE);
								CActionSystem::GetMe()->UserPS_Other_Update();
							}

							_PLAYERSHOP_GUID nShopID	= ExtraInfoData.m_ShopGuid;
							BYTE			bIsSaleOut	= ExtraInfoData.m_bIsSaleOut;
							UINT			uSaleOutPrice= ExtraInfoData.m_uSaleOutPrice;
							UINT			nBaseMoney	= ExtraInfoData.m_uBaseMoney;
							UINT			nProfitMoney= ExtraInfoData.m_uProfitMoney;
							BYTE			bSerial		= ExtraInfoData.m_Serial;
							CHAR			szShopName[MAX_SHOP_NAME_SIZE+1] = {0};
							CHAR			szShopDesc[MAX_SHOP_DESC_SIZE+1] = {0};
							CHAR			szOwnerName[MAX_CHARACTER_NAME+1] = {0};
							memcpy(szShopName, ExtraInfoData.m_ShopName, MAX_SHOP_NAME_SIZE);
							memcpy(szShopDesc, ExtraInfoData.m_ShopDesc, MAX_SHOP_DESC_SIZE);
							memcpy(szOwnerName, ExtraInfoData.m_OwnerName, MAX_CHARACTER_NAME);

							CDataPool::GetMe()->PlayerShop_SetShopID(FALSE, nShopID);
							CDataPool::GetMe()->PlayerShop_SetIsSaleOut(FALSE, bIsSaleOut);
							CDataPool::GetMe()->PlayerShop_SetSaleOutPrice(FALSE, uSaleOutPrice);
							CDataPool::GetMe()->PlayerShop_SetBaseMoney(FALSE, nBaseMoney);
							CDataPool::GetMe()->PlayerShop_SetProfitMoney(FALSE, nProfitMoney);
							CDataPool::GetMe()->PlayerShop_SetShopSerial(FALSE, bSerial);
							CDataPool::GetMe()->PlayerShop_SetShopName(FALSE, szShopName);
							CDataPool::GetMe()->PlayerShop_SetShopDesc(FALSE, szShopDesc);
							CDataPool::GetMe()->PlayerShop_SetOwnerName(FALSE, szOwnerName);
							CDataPool::GetMe()->PlayerShop_SetOwnerGuid(FALSE, ExtraInfoData.m_OwnerGuid);
							CDataPool::GetMe()->PlayerShop_SetOpenStallNum(FALSE, ExtraInfoData.m_bStallNum);
							for(UINT i = 0; i<MAX_STALL_NUM_PER_SHOP; i++)
							{
								CDataPool::GetMe()->PlayerShop_SetStallEnableByIndex( FALSE, i, ExtraInfoData.m_aStallIsOpen[i]);
							}

							for(UINT i = 0; i<nItemNum; i++)
							{
								BYTE		nBoxIndex	= ItemData[i].nIndex;			//界面中的位置
								_ITEM		item_data	= ItemData[i].item_data;	
								BYTE		ExtraLength	= ItemData[i].ExtraLength;

								GCPlayerShopItemListEachItemForOther_t ExtraInfoData;
								Assert(ItemData[i].ExtraLength == ExtraInfoData.GetSize());
								memcpy(&ExtraInfoData, ItemData[i].ExtraInfo, ItemData[i].ExtraLength); 
								BYTE		nStallIndex = ExtraInfoData.m_nStallIndex;
								BOOL		bIsMyShop	= ExtraInfoData.m_bIsMine;
								UINT		nCurPrice	= ExtraInfoData.m_uPrice;
								BYTE		nCurSerial	= ExtraInfoData.m_uSerial;

								Assert(bIsMyShop == FALSE);

								//创建商品实例
								tObject_Item* pItem = CObject_Item::NewItem( item_data.m_ItemIndex);
								if(!pItem) continue;

								pItem->SetGUID(
									item_data.m_ItemGUID.m_World, 
									item_data.m_ItemGUID.m_Server, 
									item_data.m_ItemGUID.m_Serial);
								TDAssert(pItem);

								//放到货架的制定位置上
								CDataPool::GetMe()->PlayerShop_SetItem(bIsMyShop, nStallIndex, nBoxIndex, pItem);
								CDataPool::GetMe()->PlayerShop_SetItemExtraInfo(bIsMyShop, nStallIndex, nBoxIndex, FALSE, &item_data);
								CDataPool::GetMe()->PlayerShop_SetItemPrice(bIsMyShop, nStallIndex, nBoxIndex, nCurPrice);//设置摊位物品价格
								CDataPool::GetMe()->PlayerShop_SetItemSerial(bIsMyShop, nStallIndex, nBoxIndex, nCurSerial);//设置摊位物品序列号

							}
							CActionSystem::GetMe()->UserPS_Other_Update();
							if(ExtraInfoData.m_bIsItemList)
							{
								if(ExtraInfoData.m_bUIFlag == CGPlayerShopAcquireItemList::ASK_OPEN_STALLL)
								{
									INT nObjId = CDataPool::GetMe()->PlayerShop_GetNpcID();
									if(CDataPool::GetMe()->PlayerShop_GetIsSaleOut(FALSE))
									{
										CEventSystem::GetMe()->PushEvent(GE_PS_OPEN_OTHER_TRANS,nObjId,OPEN_TYPE_ITEM);
									}
									else
									{
										CEventSystem::GetMe()->PushEvent(GE_PS_OPEN_OTHER_SHOP,nObjId,OPEN_TYPE_ITEM);
									}
								}
								else if(ExtraInfoData.m_bUIFlag == CGPlayerShopAcquireItemList::ASK_GET_STALL)
								{									
									if(CDataPool::GetMe()->PlayerShop_GetIsSaleOut(FALSE))
									{
										CEventSystem::GetMe()->PushEvent(GE_PS_UPDATE_OTHER_TRANS);
									}
									else
									{
										CEventSystem::GetMe()->PushEvent(GE_PS_UPDATE_OTHER_SHOP);
									}
								}
							}
							else
							{
								if(ExtraInfoData.m_bUIFlag == CGPlayerShopAcquireItemList::ASK_OPEN_STALLL)
								{
									INT nObjId = CDataPool::GetMe()->PlayerShop_GetNpcID();
									CEventSystem::GetMe()->PushEvent(GE_OPEN_PET_LIST);
									if(CDataPool::GetMe()->PlayerShop_GetIsSaleOut(FALSE))
									{
										CEventSystem::GetMe()->PushEvent(GE_PS_OPEN_OTHER_TRANS,nObjId,OPEN_TYPE_PET);
									}
									else
									{
										CEventSystem::GetMe()->PushEvent(GE_PS_OPEN_OTHER_SHOP,nObjId,OPEN_TYPE_PET);
									}
								}
								else if(ExtraInfoData.m_bUIFlag == CGPlayerShopAcquireItemList::ASK_GET_STALL)
								{
									if(CDataPool::GetMe()->PlayerShop_GetIsSaleOut(FALSE))
									{
										CEventSystem::GetMe()->PushEvent(GE_PS_UPDATE_OTHER_TRANS);
									}
									else
									{
										CEventSystem::GetMe()->PushEvent(GE_PS_UPDATE_OTHER_SHOP);
									}
								}
							}
						}
						break;
					case 2:
						{
							//商店列表存入数据池，本人接收到的

							//清空已有数据
							//需要清空CObject_Item中的数据

							GCPlayerShopItemListForSelf_t ExtraInfoData;
							memcpy(&ExtraInfoData, ExtraData, GCPlayerShopItemListForSelf_t::GetSize());

							if(ExtraInfoData.m_bIsItemList)
							{
								CDataPool::GetMe()->PlayerShop_CleanObjItem(TRUE);
								CDataPool::GetMe()->PlayerShop_CleanUp(TRUE);
								CActionSystem::GetMe()->UserPS_Self_Update();
							}

							_PLAYERSHOP_GUID nShopID	= ExtraInfoData.m_ShopGuid;
							BYTE			bIsSaleOut	= ExtraInfoData.m_bIsSaleOut;
							UINT			uSaleOutPrice= ExtraInfoData.m_uSaleOutPrice;
							UINT			nBaseMoney	= ExtraInfoData.m_uBaseMoney;
							UINT			nProfitMoney= ExtraInfoData.m_uProfitMoney;
							BYTE			bSerial		= ExtraInfoData.m_Serial;
							BYTE			ExRecListNum= ExtraInfoData.m_ExRecListNum;							
							BYTE			MaRecListNum= ExtraInfoData.m_MaRecListNum;
							BYTE			bType		= ExtraInfoData.m_bType;
							FLOAT			ComFactor	= ExtraInfoData.m_ComFactor;
							CHAR			szShopName[MAX_SHOP_NAME_SIZE+1] = {0};
							CHAR			szShopDesc[MAX_SHOP_DESC_SIZE+1] = {0};
							CHAR			szOwnerName[MAX_CHARACTER_NAME+1] = {0};
							memcpy(szShopName, ExtraInfoData.m_ShopName, MAX_SHOP_NAME_SIZE);
							memcpy(szShopDesc, ExtraInfoData.m_ShopDesc, MAX_SHOP_DESC_SIZE);
							memcpy(szOwnerName, ExtraInfoData.m_OwnerName, MAX_CHARACTER_NAME);

							BYTE			aStallIsOpen[MAX_STALL_NUM_PER_SHOP];
							memcpy(aStallIsOpen, ExtraInfoData.m_aStallIsOpen, MAX_STALL_NUM_PER_SHOP);

							if(ExtraInfoData.m_OwnerGuid == CObjectManager::GetMe()->GetMySelf()->GetServerGUID())
							{//自己就是店主
								CDataPool::GetMe()->PlayerShop_SetMeIsOwner(TRUE);
							}
							else
							{
								CDataPool::GetMe()->PlayerShop_SetMeIsOwner(FALSE);
							}

							CDataPool::GetMe()->PlayerShop_SetShopID(TRUE, nShopID);
							CDataPool::GetMe()->PlayerShop_SetIsSaleOut(TRUE, bIsSaleOut);
							CDataPool::GetMe()->PlayerShop_SetSaleOutPrice(TRUE, uSaleOutPrice);
							CDataPool::GetMe()->PlayerShop_SetBaseMoney(TRUE, nBaseMoney);
							CDataPool::GetMe()->PlayerShop_SetProfitMoney(TRUE, nProfitMoney);
							CDataPool::GetMe()->PlayerShop_SetShopSerial(TRUE, bSerial);
							CDataPool::GetMe()->PlayerShop_SetShopName(TRUE, szShopName);
							CDataPool::GetMe()->PlayerShop_SetShopDesc(TRUE, szShopDesc);
							CDataPool::GetMe()->PlayerShop_SetOwnerName(TRUE, szOwnerName);
							CDataPool::GetMe()->PlayerShop_SetOwnerGuid(TRUE, ExtraInfoData.m_OwnerGuid);
							CDataPool::GetMe()->PlayerShop_SetExRecNum(TRUE, ExRecListNum);
							CDataPool::GetMe()->PlayerShop_SetMaRecNum(TRUE, MaRecListNum);
							CDataPool::GetMe()->PlayerShop_SetOpenStallNum(TRUE, ExtraInfoData.m_bStallNum);
							CDataPool::GetMe()->PlayerShop_SetShopSubType(TRUE, ExtraInfoData.m_bType);

							CDataPool::GetMe()->PlayerShop_SetCommercialFactor(ComFactor);

							for(UINT i = 0; i<MAX_STALL_NUM_PER_SHOP; i++)
							{
								CDataPool::GetMe()->PlayerShop_SetStallEnableByIndex( TRUE, i, aStallIsOpen[i]);
							}

							for(UINT i = 0; i<nItemNum; i++)
							{
								if(ItemData[i].nType == GCItemList::TYPE_ITEM)
								{
									UINT		nCurPrice	= 0;
									UINT		nCurSerial	= 0;
									BYTE		nBoxIndex	= ItemData[i].nIndex;			//界面中的位置
									_ITEM		item_data	= ItemData[i].item_data;	
								
									GCPlayerShopItemListEachItemForSelf_t ExtraInfoData;
									Assert(ItemData[i].ExtraLength == ExtraInfoData.GetSize());
									memcpy(&ExtraInfoData, ItemData[i].ExtraInfo, ItemData[i].ExtraLength); 
									BYTE		nStallIndex =	ExtraInfoData.m_nStallIndex;
									BOOL		bIsMyShop	=	ExtraInfoData.m_IsMine;
												nCurPrice	=	ExtraInfoData.m_ItemPrice;
												nCurSerial	=	ExtraInfoData.m_ItemSerial;
									BYTE		bIsOnSale	=	ExtraInfoData.m_IsOnSale;

									Assert(bIsMyShop);

									//创建商品实例
									tObject_Item* pItem = CObject_Item::NewItem( item_data.m_ItemIndex);
									if(!pItem) continue;

									pItem->SetGUID(
										item_data.m_ItemGUID.m_World, 
										item_data.m_ItemGUID.m_Server, 
										item_data.m_ItemGUID.m_Serial);
									TDAssert(pItem);

									//放到货架的制定位置上
									CDataPool::GetMe()->PlayerShop_SetItem(bIsMyShop,nStallIndex, nBoxIndex, pItem);
									CDataPool::GetMe()->PlayerShop_SetItemExtraInfo(bIsMyShop,nStallIndex, nBoxIndex, FALSE, &item_data);
									CDataPool::GetMe()->PlayerShop_SetItemPrice(bIsMyShop,nStallIndex, nBoxIndex, nCurPrice);//设置摊位物品价格
									CDataPool::GetMe()->PlayerShop_SetItemSerial(bIsMyShop,nStallIndex, nBoxIndex, nCurSerial);//设置摊位物品序列号
									CDataPool::GetMe()->PlayerShop_SetItemOnSale(bIsMyShop,nStallIndex, nBoxIndex, bIsOnSale);//设置物品是否上架
								}
								else if(ItemData[i].nType == GCItemList::TYPE_PET)
								{

								}
								else if(ItemData[i].nType == GCItemList::TYPE_SERIALS)
								{
									BYTE		nBoxIndex	= ItemData[i].nIndex;			//界面中的位置
									GCPlayerShopItemListEachSerialForSelf_t ExtraInfoData;
									Assert(ItemData[i].ExtraLength == ExtraInfoData.GetSize());
									memcpy(&ExtraInfoData, ItemData[i].ExtraInfo, ItemData[i].ExtraLength); 
									BYTE		nStallIndex =	ExtraInfoData.m_nStallIndex;
									BYTE		nCurSerial	=	ExtraInfoData.m_ItemSerial;
									CDataPool::GetMe()->PlayerShop_SetItemSerial(TRUE, nStallIndex, nBoxIndex, nCurSerial);
								}
							}
							CActionSystem::GetMe()->UserPS_Self_Update();
							if(ExtraInfoData.m_bIsItemList)
							{
								if(ExtraInfoData.m_bUIFlag == CGPlayerShopAcquireItemList::ASK_OPEN_STALLL)
								{
									//打开物品商店
									INT ObjId = CDataPool::GetMe()->PlayerShop_GetNpcID();
									CDataPool::GetMe()->PlayerShop_SetShopType(TRUE,OPEN_TYPE_ITEM);
									CEventSystem::GetMe()->PushEvent(GE_PS_OPEN_MY_SHOP,ObjId,OPEN_TYPE_ITEM);
								}
								else
								{
									//更新物品商店
									CDataPool::GetMe()->PlayerShop_SetShopType(TRUE,OPEN_TYPE_ITEM);
									CEventSystem::GetMe()->PushEvent(GE_PS_UPDATE_MY_SHOP);
								}
							}
							else
							{
								if(ExtraInfoData.m_bUIFlag == CGPlayerShopAcquireItemList::ASK_OPEN_STALLL)
								{
									//打开宠物商店
									INT ObjId = CDataPool::GetMe()->PlayerShop_GetNpcID();
									CDataPool::GetMe()->PlayerShop_SetShopType(TRUE,OPEN_TYPE_PET);
									CEventSystem::GetMe()->PushEvent(GE_PS_OPEN_MY_SHOP,ObjId,OPEN_TYPE_PET);
									CEventSystem::GetMe()->PushEvent(GE_OPEN_PET_LIST);
								}
								else
								{
									//更新物品商店
									CDataPool::GetMe()->PlayerShop_SetShopType(TRUE,OPEN_TYPE_PET);
									CEventSystem::GetMe()->PushEvent(GE_PS_UPDATE_MY_SHOP);
								}
							}
						}
						break;
					default:
						break;
					}
				}
				break;
			default:
				break;
			}
		}
		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
