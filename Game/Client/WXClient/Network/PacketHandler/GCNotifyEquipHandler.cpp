#include "StdAfx.h"
#include "GCNotifyEquip.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\Object\ObjectManager.h"
#include "..\..\Object\Item\Obj_Item.h"
#include "..\..\DataPool\GMDataPool.h"
#include "..\..\Action\GMActionSystem.h"
#include "..\..\Event\GMEventSystem.h"

uint GCNotifyEquipHandler::Execute( GCNotifyEquip* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION
	//AxTrace(0, 2, "GCNotifyEquipHandler::Execute");
	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		CObjectManager* pObjectManager = CObjectManager::GetMe();

		WORD wBagIndex = pPacket->GetBagIndex();
		_ITEM* pItem = pPacket->GetItem();

		CObject_Item* pItemObj = CObject_Item::NewItem(pItem->m_ItemIndex);
		if(!pItemObj) return PACKET_EXE_CONTINUE;

		pItemObj->SetGUID(
			pItem->m_ItemGUID.m_World, 
			pItem->m_ItemGUID.m_Server,
			pItem->m_ItemGUID.m_Serial);

		pItemObj->SetExtraInfo(pItem);

//		AxTrace(0, 2, "Item %s, count=%d, %d", pItemObj->GetName(), pItem->GetItemCount(), pItemObj->GetNumber());

		CDataPool::GetMe()->UserBag_SetItem(wBagIndex, pItemObj );
		CActionSystem::GetMe()->UserBag_Update();

		//UI
		CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);
	}

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
