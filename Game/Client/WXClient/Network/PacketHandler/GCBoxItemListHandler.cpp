#include "StdAfx.h"
#include "GCBoxItemList.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\Object\ObjectManager.h"
#include "..\..\DataPool\GMDataPool.h"
#include "..\..\Object\Item\Obj_Item.h"
#include "..\..\Event\GMEventSystem.h"
#include "..\..\Action\GMActionSystem.h"

using namespace Packets;

uint GCBoxItemListHandler::Execute(GCBoxItemList* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION
	//AxTrace(0, 2, "GCBoxItemListHandler::Execute");

	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		CObjectManager* pObjectManager = CObjectManager::GetMe();

		// 清空
		CDataPool::GetMe()->ItemBox_Clear();		
		// 记录箱子id
		CDataPool::GetMe()->ItemBox_SetID( pPacket->getItemBoxId() );

		// 放置每一个物品
		for( INT i = 0; i < pPacket->getItemNum(); i ++ )
		{
			CObject_Item* pItemObj = CObject_Item::NewItem(pPacket->getItemList()[ i ].m_ItemIndex);
			if(!pItemObj) continue;

			pItemObj->SetGUID(
				(WORD)((pPacket->getItemList())[i].m_ItemGUID.m_World), 
				(WORD)((pPacket->getItemList())[i].m_ItemGUID.m_Server), 
				(GUID_t)((pPacket->getItemList())[i].m_ItemGUID.m_Serial));

			pItemObj->SetExtraInfo(&(pPacket->getItemList()[i]));
			pItemObj->SetTypeOwner(tObject_Item::IO_ITEMBOX);
			pItemObj->SetPosIndex(i);

			CDataPool::GetMe()->ItemBox_SetItem(i, pItemObj);
		}
	
		INT nObjId = CObjectManager::GetMe()->FindServerObject(pPacket->getItemBoxId())->GetID();

		//产生事件
		CActionSystem::GetMe()->ItemBox_Created(nObjId);
	}
	return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}