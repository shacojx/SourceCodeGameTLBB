#include "StdAfx.h"
#include "GCMyBagList.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\Object\ObjectManager.h"
#include "..\..\Object\Item\Obj_Item.h"
#include "..\..\DataPool\GMDataPool.h"
#include "..\..\Action\GMActionSystem.h"
#include "..\..\Event\GMEventSystem.h"


uint GCMyBagListHandler::Execute(GCMyBagList* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

	// b
	//AxTrace(0, 2, "GCMyBagList::Execute");
	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		CObjectManager* pObjectManager = CObjectManager::GetMe();
	
		CDataPool::GetMe()->UserBag_Clear();
		
		INT nItemCount =(INT) pPacket->GetAskCount(); 
		for(INT i=0; i<nItemCount; i++)
		{
			_BAG_ITEM* pItem = pPacket->GetItemData() + i;
			if(!pItem) continue;

			CObject_Item* pItemObj = CObject_Item::NewItem(pItem->m_ItemTableIndex);
			if(!pItemObj) continue;

			pItemObj->SetGUID(
				(pPacket->GetItemData())[i].m_ItemID.m_World, 
				(pPacket->GetItemData())[i].m_ItemID.m_Server, 
				(pPacket->GetItemData())[i].m_ItemID.m_Serial);
			pItemObj->SetNumber(pItem->m_Count);
			pItemObj->SetTypeOwner(tObject_Item::IO_MYSELF_PACKET);
			pItemObj->SetPosIndex(pItem->m_nndex);

			CDataPool::GetMe()->UserBag_SetItem(pItem->m_nndex, pItemObj );

		}

		//¸üÐÂµ½ActionSystem
		if(nItemCount > 0) CActionSystem::GetMe()->UserBag_Update();

		//UI
		CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);
	}

	return PACKET_EXE_CONTINUE ;
	__LEAVE_FUNCTION
	return	PACKET_EXE_ERROR;
}