/*
通知客户端同步物品移动
*/
#include "stdafx.h"
#include "GCItemSynch.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\DataPool\GMDataPool.h"
#include "..\..\Event\GMEventSystem.h"
#include "..\..\Object\Item\Obj_Item.h"
#include "..\..\Action\GMActionSystem.h"
#include "..\..\Object\Item\Obj_Item_Equip.h"
#include "TDException.h"
using namespace ITEM_SYNCH_STRUCT;
uint GCItemSynchHandler::Execute( GCItemSynch* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		BYTE		Opt				=	pPacket->GetOpt();	
		BYTE		FromType		=	pPacket->GetFromType();
		BYTE		ToType			=	pPacket->GetToType();
		BYTE		ToIndex			=	pPacket->GetToIndex();
		_ITEM_GUID	ItemGuid		=	pPacket->GetItemGuid();
		PET_GUID_t	PetGuid			=	pPacket->GetPetGUID();
		BYTE		ExtraInfoL		=	pPacket->GetExtraInfoLength();
		BYTE*		pExtraInfo		=	pPacket->GetExtraInfoData();

		//转换GUID
		tObject_Item::ITEM_GUID tempitemGuid;
		tempitemGuid.m_idOrg.m_idWorld	=	ItemGuid.m_World;
		tempitemGuid.m_idOrg.m_idServer =	ItemGuid.m_Server;
		tempitemGuid.m_idOrg.m_uSerial	=	ItemGuid.m_Serial;

		switch(Opt)
		{
			case GCItemSynch::OPT_MOVE_ITEM:
				{
					switch(FromType)
					{
					case GCItemSynch::POS_BAG:
						{
							switch(ToType)
							{
							case GCItemSynch::POS_PLAYERSHOP:
								{//背包->商店
									GCMoveItemFromBagToPlayerShop_t ExtraInfoData;
									Assert(ExtraInfoL == ExtraInfoData.GetSize());
									memcpy(&ExtraInfoData, pExtraInfo, ExtraInfoL);

									_PLAYERSHOP_GUID	nShopID		= ExtraInfoData.m_ShopGuid;
									BYTE				nStallIndex	= ExtraInfoData.m_nStallIndex;
									UINT				uSerial		= ExtraInfoData.m_uSerial;

									//在背包中找到该物品
									INT	nIndexInBag = CDataPool::GetMe()->UserBag_GetItemIndexByGUID(tempitemGuid.m_idUnion);
									if(nIndexInBag<0)
									{
										Assert(0);
										return PACKET_EXE_CONTINUE ;
									}
									tObject_Item* pItemInBag = CDataPool::GetMe()->UserBag_GetItem(nIndexInBag);

									CDataPool::GetMe()->PlayerShop_SetItem(TRUE, nStallIndex, ToIndex, pItemInBag);
									CDataPool::GetMe()->UserBag_SetItem(nIndexInBag,NULL,FALSE);
									CDataPool::GetMe()->PlayerShop_SetItemSerial(TRUE, nStallIndex, ToIndex, uSerial);

									//更新背包
									CActionSystem::GetMe()->UserBag_Update();
									CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);

									//更新玩家商店
									CActionSystem::GetMe()->UserPS_Self_Update();
									CEventSystem::GetMe()->PushEvent(GE_PS_SELF_ITEM_CHANGED);

								}
								break;
							default:
								Assert(0);
								break;
							}
						}
						break;
					case GCItemSynch::POS_PET:
						{
							switch(ToType)
							{
							case GCItemSynch::POS_PLAYERSHOPPET:
								{
									GCMovePetFromBagToPlayerShop_t ExtraInfoData;
									Assert(ExtraInfoL == ExtraInfoData.GetSize());
									memcpy(&ExtraInfoData, pExtraInfo, ExtraInfoL);

									_PLAYERSHOP_GUID	nShopID		= ExtraInfoData.m_ShopGuid;
									BYTE				nStallIndex	= ExtraInfoData.m_nStallIndex;
									UINT				uSerial		= ExtraInfoData.m_uSerial;

									INT IndexInBag = CDataPool::GetMe()->Pet_GetPetByGuid(PetGuid);
									//锁住这个位置的宠物
									SDATA_PET*	pPetDestData	=	CDataPool::GetMe()->PlayerShop_GetPet(TRUE, nStallIndex, ToIndex);
									SDATA_PET*	pPetSourceData	=	CDataPool::GetMe()->Pet_GetPet(IndexInBag);

									if(!pPetDestData)
									{
										pPetDestData = new SDATA_PET;
										CDataPool::GetMe()->PlayerShop_SetPet(TRUE, nStallIndex, ToIndex, pPetDestData, FALSE);
									}

									pPetDestData->CleanUp();
									CActionSystem::GetMe()->CleanInvalidAction();

									pPetDestData->m_nIsPresent		= pPetSourceData->m_nIsPresent;
									pPetDestData->m_GUID			= pPetSourceData->m_GUID;
									pPetDestData->m_idServer		= pPetSourceData->m_idServer;
									pPetDestData->m_nDataID			= pPetSourceData->m_nDataID;
									pPetDestData->m_nAIType			= pPetSourceData->m_nAIType;
									pPetDestData->m_szName			= pPetSourceData->m_szName;
									pPetDestData->m_nLevel			= pPetSourceData->m_nLevel;
									pPetDestData->m_nExp			= pPetSourceData->m_nExp;
									pPetDestData->m_nHP				= pPetSourceData->m_nHP;
									pPetDestData->m_nHPMax			= pPetSourceData->m_nHPMax;
									pPetDestData->m_nAge			= pPetSourceData->m_nAge;
									pPetDestData->m_nEraCount		= pPetSourceData->m_nEraCount;
									pPetDestData->m_nHappiness		= pPetSourceData->m_nHappiness;
									pPetDestData->m_SpouseGUID		= pPetSourceData->m_SpouseGUID;
									pPetDestData->m_nModelID		= pPetSourceData->m_nModelID;
									pPetDestData->m_nMountID		= pPetSourceData->m_nMountID;
									pPetDestData->m_nAttPhysics		= pPetSourceData->m_nAttPhysics;
									pPetDestData->m_nAttMagic		= pPetSourceData->m_nAttMagic;
									pPetDestData->m_nDefPhysics		= pPetSourceData->m_nDefPhysics;
									pPetDestData->m_nDefMagic		= pPetSourceData->m_nDefMagic;
									pPetDestData->m_nHit			= pPetSourceData->m_nHit;
									pPetDestData->m_nMiss			= pPetSourceData->m_nMiss;
									pPetDestData->m_nCritical		= pPetSourceData->m_nCritical;
									pPetDestData->m_nAttrStrApt		= pPetSourceData->m_nAttrStrApt;
									pPetDestData->m_nAttrConApt		= pPetSourceData->m_nAttrConApt;
									pPetDestData->m_nAttrDexApt		= pPetSourceData->m_nAttrDexApt;
									pPetDestData->m_nAttrSprApt		= pPetSourceData->m_nAttrSprApt;
									pPetDestData->m_nAttrIntApt		= pPetSourceData->m_nAttrIntApt;
									pPetDestData->m_nAttrStr		= pPetSourceData->m_nAttrStr;
									pPetDestData->m_nAttrCon		= pPetSourceData->m_nAttrCon;
									pPetDestData->m_nAttrDex		= pPetSourceData->m_nAttrDex;
									pPetDestData->m_nAttrSpr		= pPetSourceData->m_nAttrSpr;
									pPetDestData->m_nAttrInt		= pPetSourceData->m_nAttrInt;
									pPetDestData->m_nBasic			= pPetSourceData->m_nBasic;
									pPetDestData->m_nPot			= pPetSourceData->m_nPot;

									for(INT i = 0; i<PET_MAX_SKILL_COUNT; i++ )
									{
										CDataPool::GetMe()->PlayerShop_SetSkill(TRUE, nStallIndex, ToIndex, i, NULL, TRUE);
										if( pPetSourceData->m_aSkill[i])
										{
											PET_SKILL* pPetSkill = CDataPool::GetMe()->PlayerShop_GetSkill(TRUE, nStallIndex, ToIndex, i);
											if(!pPetSkill)
											{
												pPetSkill = new PET_SKILL;
												pPetSkill->m_bCanUse	= pPetSourceData->m_aSkill[i]->m_bCanUse;
												pPetSkill->m_nPetNum	= pPetSourceData->m_aSkill[i]->m_nPetNum;
												pPetSkill->m_nPosIndex	= pPetSourceData->m_aSkill[i]->m_nPosIndex;
												pPetSkill->m_pDefine	= pPetSourceData->m_aSkill[i]->m_pDefine;

												CDataPool::GetMe()->PlayerShop_SetSkill(TRUE, nStallIndex, ToIndex, i, pPetSkill, TRUE);

											}
											else
											{
												Assert(0);
												/*pPetSkill->m_bCanUse	= pPetSourceData->m_aSkill[i]->m_bCanUse;
												pPetSkill->m_nPetNum	= pPetSourceData->m_aSkill[i]->m_nPetNum;
												pPetSkill->m_nPosIndex	= pPetSourceData->m_aSkill[i]->m_nPosIndex;
												pPetSkill->m_pDefine	= pPetSourceData->m_aSkill[i]->m_pDefine;*/
											}

											//CDataPool::GetMe()->Pet_SetSkill()
										}
									}
									CDataPool::GetMe()->PlayerShop_SetPetSerial(TRUE, nStallIndex, ToIndex, uSerial);
									//清空自己背包中的宠物
									CDataPool::GetMe()->Pet_ClearPet(IndexInBag);

									CActionSystem::GetMe()->CleanInvalidAction();
									//刷新界面,锁住宠物栏中的宠物
									CEventSystem::GetMe()->PushEvent(GE_PS_UPDATE_MY_SHOP);
									CEventSystem::GetMe()->PushEvent(GE_OPEN_PET_LIST);
								}
								break;
							default:
								break;
							}

						}
						break;
					case GCItemSynch::POS_PLAYERSHOP:
						{
							switch(ToType)
							{
							case GCItemSynch::POS_BAG:
								{
									GCMoveItemFromPlayerShopToBag_t ExtraInfoData;
									Assert(ExtraInfoL == ExtraInfoData.GetSize());
									memcpy(&ExtraInfoData, pExtraInfo, ExtraInfoL);

									_PLAYERSHOP_GUID nShopID	= ExtraInfoData.m_ShopGuid;
									BYTE nStallIndex			= ExtraInfoData.m_nStallIndex;
									BYTE bIsMine				= ExtraInfoData.m_Flag;
									UINT uSerial				= ExtraInfoData.m_uSerial;
									
									//在商店中找到该物品
									INT	nIndexInShop = CDataPool::GetMe()->PlayerShop_GetItemIndexByGUID(bIsMine, nStallIndex, tempitemGuid.m_idUnion);
									if(nIndexInShop<0)
									{
										Assert(0);
										return PACKET_EXE_CONTINUE ;
									}
									tObject_Item* pItemInShop = CDataPool::GetMe()->PlayerShop_GetItem(bIsMine, nStallIndex, nIndexInShop);

									CDataPool::GetMe()->UserBag_SetItem(ToIndex, pItemInShop);
									CDataPool::GetMe()->PlayerShop_SetItem(bIsMine, nStallIndex, nIndexInShop, NULL, FALSE);
									CDataPool::GetMe()->PlayerShop_SetItemSerial(bIsMine, nStallIndex, nIndexInShop, uSerial);
									
									//更新背包
									CActionSystem::GetMe()->UserBag_Update();
									CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);

									if(bIsMine)
									{
										//更新自己的商店界面
										CActionSystem::GetMe()->UserPS_Self_Update();
										CEventSystem::GetMe()->PushEvent(GE_PS_UPDATE_MY_SHOP);
									}
									else
									{
										//更新别人的商店界面
										CActionSystem::GetMe()->UserPS_Other_Update();
										CEventSystem::GetMe()->PushEvent(GE_PS_UPDATE_OTHER_SHOP);
									}
								}
								break;
							case GCItemSynch::POS_PLAYERSHOP:
								{//商店到商店

									GCMoveItemFromPlayerShopToPlayerShop_t ExtraInfoData;
									Assert(ExtraInfoL == ExtraInfoData.GetSize());
									memcpy(&ExtraInfoData, pExtraInfo, ExtraInfoL);

									_PLAYERSHOP_GUID	nShopID = ExtraInfoData.m_ShopGuid;
									BYTE nStallIndex			= ExtraInfoData.m_nStallIndex;
									UINT uSerialSource			= ExtraInfoData.m_uSerialSource;
									UINT uSerialDest			= ExtraInfoData.m_uSerialDest;
								
									INT	nIndexInShop = CDataPool::GetMe()->PlayerShop_GetItemIndexByGUID(TRUE, nStallIndex, tempitemGuid.m_idUnion);
									if(nIndexInShop<0)
									{
										Assert(0);
										return PACKET_EXE_CONTINUE ;
									}

									tObject_Item* pItemInShop = CDataPool::GetMe()->PlayerShop_GetItem(TRUE, nStallIndex, nIndexInShop);

									CDataPool::GetMe()->PlayerShop_SetItem(TRUE, nStallIndex, ToIndex, pItemInShop);
									CDataPool::GetMe()->PlayerShop_SetItem(TRUE, nStallIndex, nIndexInShop, NULL, FALSE);
									CDataPool::GetMe()->PlayerShop_SetItemSerial(TRUE, nStallIndex, ToIndex, uSerialDest);
									CDataPool::GetMe()->PlayerShop_SetItemSerial(TRUE, nStallIndex, nIndexInShop, uSerialSource);
									
									//更新玩家商店
									CActionSystem::GetMe()->UserPS_Self_Update();
									CEventSystem::GetMe()->PushEvent(GE_PS_SELF_ITEM_CHANGED);
								}
								break;

							default:
								Assert(0);
								break;
							}
						}
						break;
					case GCItemSynch::POS_PLAYERSHOPPET:
						{
							switch(ToType)
							{
							case GCItemSynch::POS_PET:
								{
									GCMovePetFromPlayerShopToBag_t ExtraInfoData;
									Assert(ExtraInfoL == ExtraInfoData.GetSize());
									memcpy(&ExtraInfoData, pExtraInfo, ExtraInfoL);

									_PLAYERSHOP_GUID nShopID	= ExtraInfoData.m_ShopGuid;
									BYTE nStallIndex			= ExtraInfoData.m_nStallIndex;
									BYTE bIsMine				= ExtraInfoData.m_Flag;
									UINT uSerial				= ExtraInfoData.m_uSerial;

									INT	IndexInStall = CDataPool::GetMe()->PlayerShop_GetPetByGuid(bIsMine, nStallIndex, PetGuid);
									if(IndexInStall<0)
										return PACKET_EXE_CONTINUE;

									SDATA_PET*	pPetSourceData	=	CDataPool::GetMe()->PlayerShop_GetPet(bIsMine, nStallIndex, IndexInStall);
									SDATA_PET*	pPetDestData	=	CDataPool::GetMe()->Pet_GetPet(ToIndex);

									pPetDestData->CleanUp();

									CActionSystem::GetMe()->CleanInvalidAction();

									pPetDestData->m_nIsPresent		= pPetSourceData->m_nIsPresent;
									pPetDestData->m_GUID			= pPetSourceData->m_GUID;
									pPetDestData->m_idServer		= pPetSourceData->m_idServer;
									pPetDestData->m_nDataID			= pPetSourceData->m_nDataID;
									pPetDestData->m_nAIType			= pPetSourceData->m_nAIType;
									pPetDestData->m_szName			= pPetSourceData->m_szName;
									pPetDestData->m_nLevel			= pPetSourceData->m_nLevel;
									pPetDestData->m_nExp			= pPetSourceData->m_nExp;
									pPetDestData->m_nHP				= pPetSourceData->m_nHP;
									pPetDestData->m_nHPMax			= pPetSourceData->m_nHPMax;
									pPetDestData->m_nAge			= pPetSourceData->m_nAge;
									pPetDestData->m_nEraCount		= pPetSourceData->m_nEraCount;
									pPetDestData->m_nHappiness		= pPetSourceData->m_nHappiness;
									pPetDestData->m_SpouseGUID		= pPetSourceData->m_SpouseGUID;
									pPetDestData->m_nModelID		= pPetSourceData->m_nModelID;
									pPetDestData->m_nMountID		= pPetSourceData->m_nMountID;
									pPetDestData->m_nAttPhysics		= pPetSourceData->m_nAttPhysics;
									pPetDestData->m_nAttMagic		= pPetSourceData->m_nAttMagic;
									pPetDestData->m_nDefPhysics		= pPetSourceData->m_nDefPhysics;
									pPetDestData->m_nDefMagic		= pPetSourceData->m_nDefMagic;
									pPetDestData->m_nHit			= pPetSourceData->m_nHit;
									pPetDestData->m_nMiss			= pPetSourceData->m_nMiss;
									pPetDestData->m_nCritical		= pPetSourceData->m_nCritical;
									pPetDestData->m_nAttrStrApt		= pPetSourceData->m_nAttrStrApt;
									pPetDestData->m_nAttrConApt		= pPetSourceData->m_nAttrConApt;
									pPetDestData->m_nAttrDexApt		= pPetSourceData->m_nAttrDexApt;
									pPetDestData->m_nAttrSprApt		= pPetSourceData->m_nAttrSprApt;
									pPetDestData->m_nAttrIntApt		= pPetSourceData->m_nAttrIntApt;
									pPetDestData->m_nAttrStr		= pPetSourceData->m_nAttrStr;
									pPetDestData->m_nAttrCon		= pPetSourceData->m_nAttrCon;
									pPetDestData->m_nAttrDex		= pPetSourceData->m_nAttrDex;
									pPetDestData->m_nAttrSpr		= pPetSourceData->m_nAttrSpr;
									pPetDestData->m_nAttrInt		= pPetSourceData->m_nAttrInt;
									pPetDestData->m_nBasic			= pPetSourceData->m_nBasic;
									pPetDestData->m_nPot			= pPetSourceData->m_nPot;

									for(INT i = 0; i<PET_MAX_SKILL_COUNT; i++ )
									{
										CDataPool::GetMe()->Pet_SetSkill(ToIndex, i, NULL, TRUE);
										if( pPetSourceData->m_aSkill[i])
										{
											PET_SKILL* pPetSkill = CDataPool::GetMe()->Pet_GetSkill(ToIndex, i);
											if(!pPetSkill)
											{
												pPetSkill = new PET_SKILL;
												pPetSkill->m_bCanUse	= pPetSourceData->m_aSkill[i]->m_bCanUse;
												pPetSkill->m_nPetNum	= pPetSourceData->m_aSkill[i]->m_nPetNum;
												pPetSkill->m_nPosIndex	= pPetSourceData->m_aSkill[i]->m_nPosIndex;
												pPetSkill->m_pDefine	= pPetSourceData->m_aSkill[i]->m_pDefine;

												CDataPool::GetMe()->Pet_SetSkill(ToIndex, i, pPetSkill, TRUE);
											}
											else
											{
												Assert(0);
												/*pPetSkill->m_bCanUse	= pPetSourceData->m_aSkill[i]->m_bCanUse;
												pPetSkill->m_nPetNum	= pPetSourceData->m_aSkill[i]->m_nPetNum;
												pPetSkill->m_nPosIndex	= pPetSourceData->m_aSkill[i]->m_nPosIndex;
												pPetSkill->m_pDefine	= pPetSourceData->m_aSkill[i]->m_pDefine;*/
											}
										}
									}

									CDataPool::GetMe()->PlayerShop_SetPet(bIsMine, nStallIndex, IndexInStall, NULL);
									CDataPool::GetMe()->PlayerShop_SetPetPrice(bIsMine, nStallIndex, IndexInStall, 0);
									CDataPool::GetMe()->PlayerShop_SetPetSerial(bIsMine, nStallIndex, IndexInStall, uSerial);

									CActionSystem::GetMe()->CleanInvalidAction();

									//通知界面更新
									if(bIsMine)
									{
										//更新管理界面
										CEventSystem::GetMe()->PushEvent(GE_PS_UPDATE_MY_SHOP);
									}
									else
									{
										//更新购买界面
										CEventSystem::GetMe()->PushEvent(GE_PS_UPDATE_OTHER_SHOP);
									}
									CEventSystem::GetMe()->PushEvent(GE_OPEN_PET_LIST);
								}
								break;
							default:
								break;
							}
						}
						break;

					default:
						Assert(0);
						break;
					}
				}
				break;
			case GCItemSynch::OPT_REMOVE_ITEM:
				{
					switch(FromType)
					{
					case GCItemSynch::POS_BAG:
						{

						}
						break;
					case GCItemSynch::POS_PET:
						{

						}
						break;
					case GCItemSynch::POS_PLAYERSHOP:
						{

						}
						break;
					default:
						Assert(0);
						break;
					}
				}
				break;
			case GCItemSynch::OPT_EXCHANGE_ITEM:
				{
					switch(FromType)
					{
					case GCItemSynch::POS_BAG:
						{
							switch(ToType)
							{
							case GCItemSynch::POS_PLAYERSHOP:
								{//背包<->商店

									GCExchangeItemFromBagToPlayerShop_t ExtraInfoData;
									Assert(ExtraInfoL == ExtraInfoData.GetSize());
									memcpy(&ExtraInfoData, pExtraInfo, ExtraInfoL);

									_PLAYERSHOP_GUID	nShopID = ExtraInfoData.m_ShopGuid;
									BYTE nStallIndex			= ExtraInfoData.m_nStallIndex;
									UINT uSerial				= ExtraInfoData.m_uSerial;
									
									//在背包中找到该物品
									INT	nIndexInBag = CDataPool::GetMe()->UserBag_GetItemIndexByGUID(tempitemGuid.m_idUnion);
									if(nIndexInBag<0)
									{
										Assert(0);
										return PACKET_EXE_CONTINUE ;
									}
									tObject_Item* pItemInBag = CDataPool::GetMe()->UserBag_GetItem(nIndexInBag);
									tObject_Item* pItemInShop = CDataPool::GetMe()->PlayerShop_GetItem(TRUE, nStallIndex, ToIndex);

									CDataPool::GetMe()->PlayerShop_SetItem(TRUE, nStallIndex, ToIndex, pItemInBag, FALSE);
									CDataPool::GetMe()->UserBag_SetItem(nIndexInBag, pItemInShop, FALSE);
									CDataPool::GetMe()->PlayerShop_SetItemSerial(TRUE, nStallIndex, ToIndex, uSerial);
									
									//更新背包
									CActionSystem::GetMe()->UserBag_Update();
									CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);

									//更新自己的商店界面
									CActionSystem::GetMe()->UserPS_Self_Update();
									CEventSystem::GetMe()->PushEvent(GE_PS_UPDATE_MY_SHOP);
								}
								break;
							default:
								Assert(0);
								break;
							}
						}
						break;
					case GCItemSynch::POS_PET:
						{

						}
						break;
					case GCItemSynch::POS_PLAYERSHOP:
						{
							switch(ToType)
							{
							case GCItemSynch::POS_BAG:
								{//商店<->背包

									GCExchangeItemFromPlayerShopToBag_t ExtraInfoData;
									Assert(ExtraInfoL == ExtraInfoData.GetSize());
									memcpy(&ExtraInfoData, pExtraInfo, ExtraInfoL);

									_PLAYERSHOP_GUID	nShopID	= ExtraInfoData.m_ShopGuid;
									BYTE nStallIndex			= ExtraInfoData.m_nStallIndex;
									UINT uSerial				= ExtraInfoData.m_uSerial;
	
									//在背包中找到该物品
									INT	nIndexInShop = CDataPool::GetMe()->PlayerShop_GetItemIndexByGUID(TRUE, nStallIndex, tempitemGuid.m_idUnion);
									if(nIndexInShop<0)
									{
										Assert(0);
										return PACKET_EXE_CONTINUE ;
									}
									tObject_Item* pItemInBag = CDataPool::GetMe()->UserBag_GetItem(ToIndex);
									tObject_Item* pItemInShop = CDataPool::GetMe()->PlayerShop_GetItem(TRUE, nStallIndex, nIndexInShop);

									CDataPool::GetMe()->PlayerShop_SetItem(TRUE, nStallIndex, nIndexInShop, pItemInBag, FALSE);
									CDataPool::GetMe()->UserBag_SetItem(ToIndex,pItemInShop,FALSE);
									CDataPool::GetMe()->PlayerShop_SetItemSerial(TRUE, nStallIndex, nIndexInShop, uSerial);

									//更新背包
									CActionSystem::GetMe()->UserBag_Update();
									CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);

									//更新自己的商店界面
									CActionSystem::GetMe()->UserPS_Self_Update();
									CEventSystem::GetMe()->PushEvent(GE_PS_UPDATE_MY_SHOP);
								}
								break;
							case GCItemSynch::POS_PLAYERSHOP:
								{//商店<->商店

									GCExchangeItemFromPlayerShopToPlayerShop_t ExtraInfoData;
									Assert(ExtraInfoL == ExtraInfoData.GetSize());
									memcpy(&ExtraInfoData, pExtraInfo, ExtraInfoL);


									_PLAYERSHOP_GUID	nShopID	= ExtraInfoData.m_ShopGuid;
									BYTE nStallIndex			= ExtraInfoData.m_nStallIndex;
									UINT uSerialSource			= ExtraInfoData.m_uSerialSource;
									UINT uSerialDest			= ExtraInfoData.m_uSerialDest;

									INT	nIndexInShop = CDataPool::GetMe()->PlayerShop_GetItemIndexByGUID(TRUE, nStallIndex, tempitemGuid.m_idUnion);
									if(nIndexInShop<0)
									{
										Assert(0);
										return PACKET_EXE_CONTINUE ;
									}

									tObject_Item* pItemInShopSource = CDataPool::GetMe()->PlayerShop_GetItem(TRUE, nStallIndex, nIndexInShop);
									tObject_Item* pItemInShopDest = CDataPool::GetMe()->PlayerShop_GetItem(TRUE, nStallIndex, ToIndex);

									CDataPool::GetMe()->PlayerShop_SetItem(TRUE, nStallIndex, ToIndex, pItemInShopSource, FALSE);
									CDataPool::GetMe()->PlayerShop_SetItem(TRUE, nStallIndex, nIndexInShop, pItemInShopDest, FALSE);
									CDataPool::GetMe()->PlayerShop_SetItemSerial(TRUE, nStallIndex, ToIndex, uSerialDest);
									CDataPool::GetMe()->PlayerShop_SetItemSerial(TRUE, nStallIndex, nIndexInShop, uSerialSource);

									//更新玩家商店
									CActionSystem::GetMe()->UserPS_Self_Update();
									CEventSystem::GetMe()->PushEvent(GE_PS_SELF_ITEM_CHANGED);
								}
								break;
							default:
								Assert(0);
								break;
							}
						}
						break;
					default:
						Assert(0);
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
