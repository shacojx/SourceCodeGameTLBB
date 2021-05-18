#include "StdAfx.h"
#include "GCDetailEquipList.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\GameCommand.h"
#include "..\..\DataPool\DataPoolCommandDef.h"
#include "..\..\DataPool\GMDataPool.h"
#include "..\..\Object\ObjectManager.h"
#include "..\..\Global.h"
#include "..\..\Object\Character\Obj_Character.h"
#include "..\..\Object\Character\Obj_PlayerOther.h"
#include "..\..\DataPool\GMDP_CharacterData.h"
#include "..\..\Object\Item\Obj_Item.h"
#include "..\..\Event\GMEventSystem.h"
#include "..\..\Action\GMActionSystem.h"

uint GCDetailEquipListHandler::Execute(GCDetailEquipList* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION
	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		//AxTrace(0, 2, "得到装备的详细信息.");
		CObject* pObj = (CObject*)CObjectManager::GetMe()->FindServerObject(pPacket->getObjID());
		if(!pObj || !g_theKernel.IsKindOf(pObj->GetClass(), GETCLASS(CObject_Character)))
		{
			return PACKET_EXE_CONTINUE;
		}

		CCharacterData* pCharacterData = ((CObject_Character*)pObj)->GetCharacterData();

		//刷入数据池
		UINT dwEquipMask = pPacket->GetPartFlags();
		_ITEM* pItemBuf = pPacket->GetEquipData();

		if(pObj != (CObject*)CObjectManager::GetMe()->GetMySelf())
		{
			for(INT i=0; i<HEQUIP_NUMBER; i++)
			{
				if(dwEquipMask&(1<<i))
				{
					CObject_Item* pItemObj = CObject_Item::NewItem(pItemBuf[i].m_ItemIndex);
					if(!pItemObj)
					{
						CDataPool::GetMe()->OtherPlayerEquip_SetItem((HUMAN_EQUIP)i, NULL, TRUE);
					}
					else
					{
						pItemObj->SetGUID(	pItemBuf[i].m_ItemGUID.m_World,
							pItemBuf[i].m_ItemGUID.m_Server,
							pItemBuf[i].m_ItemGUID.m_Serial);

						pItemObj->SetExtraInfo(pItemBuf+i);
						pItemObj->SetTypeOwner(tObject_Item::IO_PLAYEROTHER_EQUIP);
						pItemObj->SetPosIndex(i);
						
	//					pCharacterData->Set_Equip((HUMAN_EQUIP)i,pItemObj->GetID());
						CDataPool::GetMe()->OtherPlayerEquip_SetItem((HUMAN_EQUIP)i, pItemObj);
					}
				}
				else
				{
					CDataPool::GetMe()->OtherPlayerEquip_SetItem((HUMAN_EQUIP)i, NULL, TRUE);
				}

			}
			
			CObject_PlayerOther* TargetAvatar = CObjectManager::GetMe()->GetTarget_Avatar();

			TargetAvatar->GetCharacterData()->Set_RaceID( ((CObject_Character*)pObj)->GetCharacterData()->Get_RaceID() );
			for(INT i=0; i<HEQUIP_NUMBER; i++)
			{
				TargetAvatar->GetCharacterData()->Set_Equip((HUMAN_EQUIP)i,((CObject_Character*)pObj)->GetCharacterData()->Get_Equip((HUMAN_EQUIP)i));
			}
			TargetAvatar->SetFaceDir(0.0f);
			
			CDataPool::GetMe()->Copy_To_TargetEquip(pObj);

			// 更新其他玩家身上的装备到ActionSystem
			CActionSystem::GetMe()->OtherPlayerEquip_Update();

			// 通知界面事件
			CEventSystem::GetMe()->PushEvent(GE_OTHERPLAYER_UPDATE_EQUIP);
		}
		else
		{
			for(INT i=0; i<HEQUIP_NUMBER; i++)
			{
				if(dwEquipMask&(1<<i))
				{
					CObject_Item* pItemObj = CObject_Item::NewItem(pItemBuf[i].m_ItemIndex);
					if(!pItemObj) continue;

					pItemObj->SetGUID(	pItemBuf[i].m_ItemGUID.m_World,
						pItemBuf[i].m_ItemGUID.m_Server,
						pItemBuf[i].m_ItemGUID.m_Serial);

					pItemObj->SetExtraInfo(pItemBuf+i);
					pItemObj->SetTypeOwner(tObject_Item::IO_MYSELF_EQUIP);
					pItemObj->SetPosIndex(i);

					CDataPool::GetMe()->UserEquip_SetItem((HUMAN_EQUIP)i, pItemObj);
				}
			}

			// 更新主角身上的装备到ActionSystem
			CActionSystem::GetMe()->UserEquip_Update();

			// 通知界面事件
			CEventSystem::GetMe()->PushEvent(GE_UPDATE_EQUIP);
		
		}
	}

	return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}