#include "StdAfx.h"
#include "GCUseEquipResult.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\Object\ObjectManager.h"
#include "..\..\Object\Item\Obj_Item_Equip.h"
#include "..\..\Object\Character\Obj_PlayerMySelf.h"
#include "..\..\DataPool\GMUIDataPool.h"
#include "..\..\DataPool\GMDataPool.h"
#include "..\..\Action\GMActionSystem.h"
#include "..\..\Event\GMEventSystem.h"
#include "..\..\DataPool\GMDP_CharacterData.h"
#include "..\..\Sound\GMSoundSystem.h"

using namespace Packets;
uint GCUseEquipResultHandler::Execute(GCUseEquipResult* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

	//AxTrace(0, 2, "GCUseEquipResultHandler::Execute");
	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		CObjectManager* pObjectManager = CObjectManager::GetMe();

		UseEquipResultCode ret = (UseEquipResultCode)pPacket->getResult();
		switch(ret)
		{
		case USEEQUIP_SUCCESS:
			{
				//---------------------------------------------------------
				//UI数据池
				CDataPool* pDataPool = CDataPool::GetMe();

				//背包里的装备
				tObject_Item* pItemAtBag = pDataPool->UserBag_GetItem(pPacket->getBagIndex());
				if(!pItemAtBag) return PACKET_EXE_CONTINUE;

				//身上的装备
				tObject_Item* pItemAtUser= pDataPool->UserEquip_GetItem( (HUMAN_EQUIP)pPacket->getEquipPoint());

				//装配信息不对
				if(pItemAtBag->GetItemClass() != ICLASS_EQUIP || 
					((CObject_Item_Equip*)pItemAtBag)->GetItemType() != pPacket->getEquipPoint()) return PACKET_EXE_CONTINUE;

				//如果发生物品转移，则改变客户端id，以表示在客户端是不同物体
				((CObject_Item*)pItemAtBag)->ChangeClientID();
				if(pItemAtUser)	((CObject_Item*)pItemAtUser)->ChangeClientID();
				
				//---------------------------------------
				//刷新角色属性
				CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Set_Equip(
					((const CObject_Item_Equip*)pItemAtBag)->GetItemType(),
					pItemAtBag->GetIdTable());

				//刷新数据池
				pDataPool->UserEquip_SetItem(((const CObject_Item_Equip*)pItemAtBag)->GetItemType(), (tObject_Item*)pItemAtBag, FALSE);

				//---------------------------------------
				//刷新包裹数据
				if(pItemAtUser)
					pItemAtUser->SetGUID(	pPacket->getItemId().m_World, 
											pPacket->getItemId().m_Server,
											pPacket->getItemId().m_Serial);
				pDataPool->UserBag_SetItem(pPacket->getBagIndex(), (tObject_Item*)pItemAtUser, FALSE);
				CActionSystem::GetMe()->UserBag_Update();

				//产生事件
				CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED, pPacket->getBagIndex());

				CSoundSystemFMod::_PlayUISoundFunc(67);
			}
			break;

		case USEEQUIP_LEVEL_FAIL:
			{
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "物品的装配等级高于您目前的等级");
			}
			break;

		case USEEQUIP_TYPE_FAIL:
			{
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "装配类型错误");
			}
			break;

		case USEEQUIP_JOB_FAIL:
			{
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "职业不匹配");
			}
			break;

		default:
			{
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "未知错误");
			}
			break;
		}
		
		// 更新主角身上的装备到ActionSystem
		CActionSystem::GetMe()->UserEquip_Update();

		// 通知界面事件
		CEventSystem::GetMe()->PushEvent(GE_UPDATE_EQUIP);
	}

	return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}