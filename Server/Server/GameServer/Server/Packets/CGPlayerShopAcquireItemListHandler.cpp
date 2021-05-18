#include "stdafx.h"
/*
客户端申请商店的商品列表
*/

#include "CGPlayerShopAcquireItemList.h"

#include "Type.h"
#include "Log.h"
#include "GamePlayer.h"
#include "Scene.h"
#include "Obj_Human.h"

#include "GCItemList.h"
#include "GCDetailAttrib_Pet.h"
#include "GameTable.h"

#include "RecordOpt.h"
#include "GCPlayerShopUpdatePartners.h"

using namespace ITEM_SYNCH_STRUCT;
UINT CGPlayerShopAcquireItemListHandler::Execute( CGPlayerShopAcquireItemList* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

	GamePlayer* pGamePlayer = (GamePlayer*)pPlayer ;
	Assert( pGamePlayer ) ;

	Obj_Human* pHuman = pGamePlayer->GetHuman() ;
	Assert( pHuman ) ;

	Scene* pScene = pHuman->getScene() ;
	if( pScene==NULL )
	{
		Assert(FALSE) ;
		return PACKET_EXE_ERROR ;
	}
	//检查线程执行资源是否正确
	Assert( MyGetCurrentThreadID()==pScene->m_ThreadID ) ;
	GCItemList	MsgItemList;
	_PLAYERSHOP_GUID nShopID		=	pPacket->GetShopID();
	BYTE			nStallIndex		=	pPacket->GetStallIndex();
	BYTE			nSign			=	pPacket->GetSign();

	PlayerShopManager*	pPlayerShopManager = pScene->GetPlayerShopManager();
	PlayerShop*	pPlayerShop	= pPlayerShopManager->GetPlayerShopByGUID(nShopID);
	Assert(pPlayerShop);

	//这个商店是否已经盘出
	BOOL bIsSaleOut = (pPlayerShop->GetShopStatus() == PLAYER_SHOP::STATUS_PLAYER_SHOP_ON_SALE)? TRUE:FALSE;

	//这个商店是否是宠物店
	BOOL bIsPetShop = (pPlayerShop->GetShopType() == PLAYER_SHOP::TYPE_PLAYER_SHOP_PET)? TRUE:FALSE;

	//是不是自己的店
	BOOL bIsMine = (pHuman->GetGUID() == pPlayerShop->GetOwnerGuid())? TRUE:FALSE;

	//是不是自己可以管理的店
	BOOL bCanManager = pPlayerShop->IsPartner(pHuman->GetGUID());

	PlayerStallBox*		pCurStallBox		= NULL;
	ItemContainer*		pCurContainer		= NULL;
	ItemContainer*		pCurPetContainer	= NULL;

	GCItemList::_MSG_ITEM ItemInMsg[STALL_BOX_SIZE*MAX_STALL_NUM_PER_SHOP];
	UINT				ItemNumInMsg = 0;
	UINT				uCurItemPrice = 0;
	UINT				uCurItemSerial = 0;
	Item*				pCurItem = NULL;
	BYTE	aStallIsOpen[MAX_STALL_NUM_PER_SHOP];
	memset(aStallIsOpen, 0, MAX_STALL_NUM_PER_SHOP);

	if(bIsMine == FALSE && bCanManager == FALSE)
	{//别人察看
		if(pPlayerShop->GetShopStatus() == PLAYER_SHOP::STATUS_PLAYER_SHOP_OPEN ||
			bIsSaleOut)
		{
			for(UINT i = 0; i<MAX_STALL_NUM_PER_SHOP; i++)
			{
				PlayerStallBox* pTmpStallBox = pPlayerShop->GetPlayerStallBoxByIndex(i);
				aStallIsOpen[i] = (BYTE)pTmpStallBox->GetStallStatus();
			}

			GCPlayerShopItemListForOther_t ExtraInfoData;
			pCurStallBox = pPlayerShop->GetPlayerStallBoxByIndex(nStallIndex);

			if(bIsPetShop)
			{//宠物商店
				ExtraInfoData.m_bIsItemList = 0;
				ExtraInfoData.m_bUIFlag = nSign;

				if(pCurStallBox->GetStallStatus() == PLAYER_SHOP::STALL_OPEN ||
					(bIsSaleOut && pCurStallBox->GetStallStatus() != PLAYER_SHOP::STALL_INVALID))
				{
					//循环写入
					pCurPetContainer = pCurStallBox->GetPetContainer();
					for(INT i = 0; i<pCurPetContainer->GetContainerSize(); i++)
					{
						if( pCurPetContainer->GetItem(i)->IsEmpty() == FALSE &&
							(pCurStallBox->CanSale(i) || bIsSaleOut)
							)
						{//有东西
							INT	nPrice	= pCurStallBox->GetPriceByIndex(i);
							INT	nSerial	= pCurStallBox->GetSerialByIndex(i);

							Item* pIt = pCurPetContainer->GetItem(i);
							GCDetailAttrib_Pet PetMsgDetail;

							//组装GCDetailAttrib_Pet结构
							Obj_Human::CalculatePetDetailAttrib(PetMsgDetail, pIt);
							PetMsgDetail.SetTradeIndex( i );
							PET_EXTRA_STRUCT::GCPlayerShopPetView_t ExtraPetInfo;
							ExtraPetInfo.m_bFlag			= TYPE_PLAYERSHOP;
							ExtraPetInfo.m_nPrice			= nPrice;
							ExtraPetInfo.m_nSerial			= nSerial;
							ExtraPetInfo.m_bIsMine			= FALSE;
							ExtraPetInfo.m_nStallIndex		= nStallIndex;
							ExtraPetInfo.m_bIsOnSale		= FALSE;

							PetMsgDetail.SetExtraInfoLength(ExtraPetInfo.GetSize());
							PetMsgDetail.SetExtraInfoData((BYTE*)&ExtraPetInfo);
							pGamePlayer->SendPacket( &PetMsgDetail );		
						}
					}
				}
			}
			else
			{//物品店
				ExtraInfoData.m_bIsItemList = 1;
				ExtraInfoData.m_bUIFlag = nSign;

				if(pCurStallBox->GetStallStatus() == PLAYER_SHOP::STALL_OPEN ||
					(bIsSaleOut && pCurStallBox->GetStallStatus() != PLAYER_SHOP::STALL_INVALID)
					)
				{
					pCurContainer = pCurStallBox->GetContainer();
					for(UINT j = 0; j<STALL_BOX_SIZE; j++)
					{
						pCurItem = pCurContainer->GetItem(j);
						if( !pCurItem->IsEmpty() && 
							(pCurStallBox->CanSale(j) || bIsSaleOut)
							)
						{//此物品上架了
							uCurItemPrice	=	pCurStallBox->GetPriceByIndex(j);
							uCurItemSerial	=	pCurStallBox->GetSerialByIndex(j);
							pCurItem->SaveValueTo(&ItemInMsg[ItemNumInMsg].item_data);
							ItemInMsg[ItemNumInMsg].nIndex			= j;

							GCPlayerShopItemListEachItemForOther_t ExtraInfoData;
							ExtraInfoData.m_nStallIndex =	nStallIndex;
							ExtraInfoData.m_uPrice		=	uCurItemPrice;
							ExtraInfoData.m_uSerial		=	uCurItemSerial;
							ExtraInfoData.m_bIsMine		=	bIsMine;
							ItemInMsg[ItemNumInMsg].ExtraLength		= ExtraInfoData.GetSize();//附加信息长度10个字节
							memcpy(ItemInMsg[ItemNumInMsg].ExtraInfo, &ExtraInfoData, ItemInMsg[ItemNumInMsg].ExtraLength);
							ItemNumInMsg++;
						}
					}
				}
			}

			MsgItemList.SetOpt(GCItemList::OPT_ADD_ITEM_LIST);
			MsgItemList.SetItemNum(ItemNumInMsg);
			MsgItemList.SetItemList(ItemInMsg);
			
			ExtraInfoData.m_ShopGuid		=	nShopID;
			ExtraInfoData.m_bFlag			=	GCPlayerShopItemListForSelf_t::FOR_BUYER;
			ExtraInfoData.m_OwnerGuid		=	pPlayerShop->GetOwnerGuid();
			ExtraInfoData.m_uBaseMoney		=	pPlayerShop->GetBaseMoney();
			ExtraInfoData.m_uProfitMoney	=	pPlayerShop->GetProfitMoney();
			ExtraInfoData.m_bIsSaleOut		=	bIsSaleOut;
			ExtraInfoData.m_uSaleOutPrice	=	pPlayerShop->GetSaleOutPrice();
			ExtraInfoData.m_Serial			=	pPlayerShop->GetSerial();
			ExtraInfoData.m_bStallNum		=	pPlayerShop->GetNumStallOpened();
			memcpy((CHAR*)ExtraInfoData.m_OwnerName, pPlayerShop->GetOwnerName(), MAX_CHARACTER_NAME);
			memcpy((CHAR*)ExtraInfoData.m_ShopName, pPlayerShop->GetShopName(), MAX_SHOP_NAME_SIZE);
			memcpy((CHAR*)ExtraInfoData.m_ShopDesc, pPlayerShop->GetShopDesc(), MAX_SHOP_DESC_SIZE);
			memcpy((CHAR*)ExtraInfoData.m_aStallIsOpen, (CHAR*)aStallIsOpen, MAX_STALL_NUM_PER_SHOP);
			MsgItemList.SetExtraLength(ExtraInfoData.GetSize());
			MsgItemList.SetExtraInfo((CHAR*)&ExtraInfoData);
			pGamePlayer->SendPacket(&MsgItemList);
		}
	}
	else if(bIsMine == TRUE || bCanManager == TRUE)
	{//自己或者其他管理人员察看
		UINT	uBaseMoney	 = 0;
		UINT	uProfitMoney = 0;

		uBaseMoney		= pPlayerShop->GetBaseMoney();
		uProfitMoney	= pPlayerShop->GetProfitMoney();
		
		GCPlayerShopItemListForSelf_t ExtraInfoData;
		pCurStallBox = pPlayerShop->GetPlayerStallBoxByIndex(nStallIndex);

		if(TRUE)
		{//所有物品都发送
			for(UINT i = 0; i<MAX_STALL_NUM_PER_SHOP; i++)
			{
				PlayerStallBox* pTmpStallBox = pPlayerShop->GetPlayerStallBoxByIndex(i);
				aStallIsOpen[i] = (BYTE)pTmpStallBox->GetStallStatus();
			}

			if(bIsPetShop)
			{
				ExtraInfoData.m_bIsItemList = 0;
				ExtraInfoData.m_bUIFlag = nSign;

				pCurPetContainer = pCurStallBox->GetPetContainer();

				//循环写入
				for(INT i = 0; i<pCurPetContainer->GetContainerSize(); i++)
				{
					if( pCurPetContainer->GetItem(i)->IsEmpty() == FALSE )
					{//有东西
						INT	nPrice	= pCurStallBox->GetPriceByIndex(i);
						INT	nSerial	= pCurStallBox->GetSerialByIndex(i);

						Item* pIt = pCurPetContainer->GetItem(i);
						GCDetailAttrib_Pet PetMsgDetail;
						//组装GCDetailAttrib_Pet结构
						Obj_Human::CalculatePetDetailAttrib(PetMsgDetail, pIt);
						PetMsgDetail.SetTradeIndex( i );
						BYTE nOnSale = pCurStallBox->CanSale(i);

						PET_EXTRA_STRUCT::GCPlayerShopPetView_t ExtraPetInfo;
						ExtraPetInfo.m_bFlag			= TYPE_PLAYERSHOP;
						ExtraPetInfo.m_nPrice			= nPrice;
						ExtraPetInfo.m_nSerial			= nSerial;
						ExtraPetInfo.m_bIsMine			= TRUE;
						ExtraPetInfo.m_nStallIndex		= nStallIndex;
						ExtraPetInfo.m_bIsOnSale		= nOnSale;
	
						PetMsgDetail.SetExtraInfoLength(ExtraPetInfo.GetSize());
						PetMsgDetail.SetExtraInfoData((BYTE*)&ExtraPetInfo);
						pGamePlayer->SendPacket( &PetMsgDetail );		
					}
					else
					{//没有宠物发序列号
						uCurItemSerial	=	pCurStallBox->GetSerialByIndex(i);
						ItemInMsg[ItemNumInMsg].nType		= GCItemList::TYPE_SERIALS;//序列号
						ItemInMsg[ItemNumInMsg].nIndex		= i;
						GCPlayerShopItemListEachSerialForSelf_t ExtraInfoData;
						ExtraInfoData.m_nStallIndex			= nStallIndex;
						ExtraInfoData.m_ItemSerial			= uCurItemSerial;
						ItemInMsg[ItemNumInMsg].ExtraLength	= ExtraInfoData.GetSize();
						memcpy(ItemInMsg[ItemNumInMsg].ExtraInfo, &ExtraInfoData, ItemInMsg[ItemNumInMsg].ExtraLength);
						ItemNumInMsg++;
					}
				}
			}
			else
			{
				ExtraInfoData.m_bIsItemList = 1;
				ExtraInfoData.m_bUIFlag = nSign;

				pCurContainer = pCurStallBox->GetContainer();
				for(UINT j = 0; j<STALL_BOX_SIZE; j++)
				{
					pCurContainer = pCurStallBox->GetContainer();
					pCurItem = pCurContainer->GetItem(j);
					if( !pCurItem->IsEmpty() )
					{//此物品存在
						uCurItemPrice	=	pCurStallBox->GetPriceByIndex(j);
						uCurItemSerial	=	pCurStallBox->GetSerialByIndex(j);
						pCurItem->SaveValueTo(&ItemInMsg[ItemNumInMsg].item_data);
						ItemInMsg[ItemNumInMsg].nType			= GCItemList::TYPE_ITEM;//物品
						ItemInMsg[ItemNumInMsg].nIndex			= j;

						GCPlayerShopItemListEachItemForSelf_t ExtraInfoData;
						ExtraInfoData.m_nStallIndex				=	nStallIndex;
						ExtraInfoData.m_ItemPrice				=	uCurItemPrice;	
						ExtraInfoData.m_ItemSerial				=	uCurItemSerial;
						ExtraInfoData.m_IsMine					=	TRUE;
						ExtraInfoData.m_IsOnSale				=	(BYTE)pCurStallBox->CanSale(j);	
						ItemInMsg[ItemNumInMsg].ExtraLength		=	ExtraInfoData.GetSize();
						memcpy(ItemInMsg[ItemNumInMsg].ExtraInfo, &ExtraInfoData, ItemInMsg[ItemNumInMsg].ExtraLength);
						ItemNumInMsg++;
					}
					else
					{//没有物品发序列号
						uCurItemSerial	=	pCurStallBox->GetSerialByIndex(j);
						ItemInMsg[ItemNumInMsg].nType		= GCItemList::TYPE_SERIALS;//序列号
						ItemInMsg[ItemNumInMsg].nIndex		= j;
						GCPlayerShopItemListEachSerialForSelf_t ExtraInfoData;
						ExtraInfoData.m_nStallIndex			= nStallIndex;
						ExtraInfoData.m_ItemSerial			= uCurItemSerial;
						ItemInMsg[ItemNumInMsg].ExtraLength	= ExtraInfoData.GetSize();
						memcpy(ItemInMsg[ItemNumInMsg].ExtraInfo, &ExtraInfoData, ItemInMsg[ItemNumInMsg].ExtraLength);
						ItemNumInMsg++;
					}
				}
			}
		}

		MsgItemList.SetOpt(GCItemList::OPT_ADD_ITEM_LIST);
		MsgItemList.SetItemNum(ItemNumInMsg);
		MsgItemList.SetItemList(ItemInMsg);
		
		ExtraInfoData.m_ShopGuid		=	nShopID;
		ExtraInfoData.m_bFlag			=	GCPlayerShopItemListForSelf_t::FOR_MANAGER;
		ExtraInfoData.m_uBaseMoney		=	uBaseMoney;
		ExtraInfoData.m_uProfitMoney	=	uProfitMoney;
		ExtraInfoData.m_OwnerGuid		=	pPlayerShop->GetOwnerGuid();
		ExtraInfoData.m_bIsSaleOut		=	bIsSaleOut;
		ExtraInfoData.m_uSaleOutPrice	=	pPlayerShop->GetSaleOutPrice();
		ExtraInfoData.m_Serial			=	pPlayerShop->GetSerial();
		ExtraInfoData.m_ExRecListNum	=	RecordOpt::GetRecordNum(pPlayerShop->GetExchangeRecord());
		ExtraInfoData.m_MaRecListNum	=	RecordOpt::GetRecordNum(pPlayerShop->GetManagerRecord());
		ExtraInfoData.m_bStallNum		=	pPlayerShop->GetNumStallOpened();
		ExtraInfoData.m_bType			=	pPlayerShop->GetShopType();
		pPlayerShopManager->ClampComFactor();
		ExtraInfoData.m_ComFactor		=	pPlayerShopManager->GetComFactor();
		memcpy((CHAR*)ExtraInfoData.m_OwnerName, pPlayerShop->GetOwnerName(), MAX_CHARACTER_NAME);
		memcpy((CHAR*)ExtraInfoData.m_ShopName, pPlayerShop->GetShopName(), MAX_SHOP_NAME_SIZE);
		memcpy((CHAR*)ExtraInfoData.m_ShopDesc, pPlayerShop->GetShopDesc(), MAX_SHOP_DESC_SIZE);
		memcpy((CHAR*)ExtraInfoData.m_aStallIsOpen, (CHAR*)aStallIsOpen, MAX_STALL_NUM_PER_SHOP);
		MsgItemList.SetExtraLength(ExtraInfoData.GetSize());
		MsgItemList.SetExtraInfo((CHAR*)&ExtraInfoData);

		pGamePlayer->SendPacket(&MsgItemList);

		//发送合伙人信息
		GCPlayerShopUpdatePartners					MsgPartnerToClient;
		GCPlayerShopUpdatePartners::MsgPartner_t	SendingArray[MAX_PARTNER_PER_SHOP];

		Partner_t*	pCurParList = pPlayerShop->GetPartnerList();
		INT k = 0;
		for(INT i = 0; i<MAX_PARTNER_PER_SHOP; i++)
		{
			if(pCurParList[i].m_Guid != INVALID_GUID)
			{
				SendingArray[k].CleanUp();
				SendingArray[k].m_Guid = pCurParList[i].m_Guid;
				memcpy(SendingArray[k++].m_szName, pCurParList[i].m_szName, MAX_CHARACTER_NAME);
			}
		}

		MsgPartnerToClient.SetShopID(nShopID);
		MsgPartnerToClient.SetPartnerNum(k);
		MsgPartnerToClient.SetPartnerList(SendingArray);

		pGamePlayer->SendPacket(&MsgPartnerToClient);

	}
	
	g_pLog->FastSaveLog( LOG_FILE_1, "CGPlayerShopAcquireItemListHandler::ObjName=%s",pHuman->GetName()) ;

	return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
