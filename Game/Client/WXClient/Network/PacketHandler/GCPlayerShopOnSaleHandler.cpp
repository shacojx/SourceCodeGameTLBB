#include "StdAfx.h"
#include "GCPlayerShopOnSale.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\DataPool\GMDataPool.h"
#include "..\..\Event\GMEventSystem.h"
#include "..\..\Object\Item\Obj_Item.h"
#include "..\..\Action\GMActionSystem.h"
#include "TDException.h"

UINT GCPlayerShopOnSaleHandler::Execute( GCPlayerShopOnSale* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
		{
			_PLAYERSHOP_GUID nShopID			=	pPacket->GetShopID();			//商店ID
			BYTE		nStallIndex		=	pPacket->GetStallIndex();		//柜台ID
			_ITEM_GUID	ItemGuid		=	pPacket->GetObjGUID();			//物品GUID
			PET_GUID_t	PetGuid			=	pPacket->GetPetGuid();			//宠物Guid
			UINT		nSerial			=	pPacket->GetSerial();			//当前序列号
			BYTE		bIsOnSale		=	pPacket->GetIsOnSale();			//是否上架
			UINT		uPrice			=	pPacket->GetPrice();			//商品价格

			INT			nType			=	0;//0表示处理宠物，1表示处理物品
			if(PetGuid.IsNull())
			{
				//表示现在是处理物品
				nType = 1;
			}


			if(nType == 1)
			{
				//转换GUID
				tObject_Item::ITEM_GUID tempitemGuid;
				tempitemGuid.m_idOrg.m_idWorld	=	ItemGuid.m_World;
				tempitemGuid.m_idOrg.m_idServer =	ItemGuid.m_Server;
				tempitemGuid.m_idOrg.m_uSerial	=	ItemGuid.m_Serial;

				//在商店中找到该物品
				INT	nIndexInShop = CDataPool::GetMe()->PlayerShop_GetItemIndexByGUID(TRUE, nStallIndex, tempitemGuid.m_idUnion);
				if(nIndexInShop<0)
				{
					Assert(0);
					return PACKET_EXE_CONTINUE ;
				}
				if(bIsOnSale)
				{//上架
					CDataPool::GetMe()->PlayerShop_SetItemSerial(TRUE, nStallIndex, nIndexInShop, nSerial);
					CDataPool::GetMe()->PlayerShop_SetItemPrice(TRUE, nStallIndex, nIndexInShop, uPrice);
					CDataPool::GetMe()->PlayerShop_SetItemOnSale(TRUE, nStallIndex, nIndexInShop, TRUE);
				}
				else
				{
					CDataPool::GetMe()->PlayerShop_SetItemSerial(TRUE, nStallIndex, nIndexInShop, nSerial);
					CDataPool::GetMe()->PlayerShop_SetItemPrice(TRUE, nStallIndex, nIndexInShop, uPrice);
					CDataPool::GetMe()->PlayerShop_SetItemOnSale(TRUE, nStallIndex, nIndexInShop, FALSE);
				}

				CActionSystem::GetMe()->UserPS_Self_Update();
				CEventSystem::GetMe()->PushEvent(GE_PS_UPDATE_MY_SHOP);
			}
			else
			{
				//在商店中找到该宠物
				INT	nIndex = CDataPool::GetMe()->PlayerShop_GetPetByGuid(TRUE, nStallIndex, PetGuid);
				if(nIndex<0)
				{
					Assert(0);
					return PACKET_EXE_CONTINUE ;
				}
				if(bIsOnSale)
				{//上架
					CDataPool::GetMe()->PlayerShop_SetPetSerial(TRUE, nStallIndex, nIndex, nSerial);
					CDataPool::GetMe()->PlayerShop_SetPetPrice(TRUE, nStallIndex, nIndex, uPrice);
					CDataPool::GetMe()->PlayerShop_SetItemOnSale(TRUE, nStallIndex, nIndex, TRUE);
				}
				else
				{
					CDataPool::GetMe()->PlayerShop_SetPetSerial(TRUE, nStallIndex, nIndex, nSerial);
					CDataPool::GetMe()->PlayerShop_SetPetPrice(TRUE, nStallIndex, nIndex, uPrice);
					CDataPool::GetMe()->PlayerShop_SetItemOnSale(TRUE, nStallIndex, nIndex, FALSE);
				}

				CEventSystem::GetMe()->PushEvent(GE_PS_UPDATE_MY_SHOP);
			}
		}
		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
