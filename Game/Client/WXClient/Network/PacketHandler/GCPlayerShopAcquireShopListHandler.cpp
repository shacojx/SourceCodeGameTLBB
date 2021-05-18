/*
通知客户端商店列表
*/
#include "stdAfx.h"
#include "GCPlayerShopAcquireShopList.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\DataPool\GMDataPool.h"
#include "..\..\Event\GMEventSystem.h"
#include "TDException.h"
#include "..\..\Object\ObjectManager.h"

UINT GCPlayerShopAcquireShopListHandler::Execute( GCPlayerShopAcquireShopList* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION
	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{

		UINT			ShopNum			=	pPacket->GetShopNum();		//本场景商店个数
		FLOAT			m_ComFactor		=	pPacket->GetComFactor();
		SHOP_INFO_t*	ShopList		=	pPacket->GetShopList();		//商店信息列表
		ObjID_t			NpcId			=	pPacket->GetNpcId();		//Npc Id

		INT ObjectID = (CObjectManager::GetMe()->FindServerObject(NpcId))->GetID();

		CHAR			szTime[20];
		memset(szTime, 0, 20);
	
        CDataPool::GetMe()->PlayerShop_SetShopNum(ShopNum);
		CDataPool::GetMe()->PlayerShop_SetCommercialFactor(m_ComFactor);
		CDataPool::GetMe()->PlayerShop_SetNpcID(ObjectID);
		//商店列表存入数据池
		for(UINT i = 0; i<ShopNum; i++)
		{
			_snprintf(szTime, 20, "%d-%d-%d", ShopList[i].m_Year, ShopList[i].m_Month, ShopList[i].m_Day);
			CDataPool::GetMe()->PlayerShop_SetIDByIndex(i, ShopList[i].m_ID);
			CDataPool::GetMe()->PlayerShop_SetIsFavorByIndex(i, ShopList[i].m_bIsInFavor);
			CDataPool::GetMe()->PlayerShop_SetShopNameByIndex(i, ShopList[i].m_ShopName);
			CDataPool::GetMe()->PlayerShop_SetOwnerNameByIndex(i, ShopList[i].m_OwnerName);
			CDataPool::GetMe()->PlayerShop_SetOwnerGuidByIndex(i, ShopList[i].m_OwnerGuid);
			CDataPool::GetMe()->PlayerShop_SetStallNumOnSale(i, ShopList[i].m_StallNumOnSale);
			CDataPool::GetMe()->PlayerShop_SetStallNumOpened(i, ShopList[i].m_StallNumOpened);
			CDataPool::GetMe()->PlayerShop_SetTypeByIndex(i, ShopList[i].m_Type);
			CDataPool::GetMe()->PlayerShop_SetShopFoundedTimeByIndex(i, szTime);
			CDataPool::GetMe()->PlayerShop_SetShopDescByIndex(i, ShopList[i].m_ShopDesc);
		}

		//打开商店列表界面
		CEventSystem::GetMe()->PushEvent(GE_PS_OPEN_SHOPLIST, ObjectID);
	}

	return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
