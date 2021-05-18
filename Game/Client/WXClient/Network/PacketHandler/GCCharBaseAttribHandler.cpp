#include "StdAfx.h"
#include "..\..\NetWork\NetManager.h"
#include "TDException.h"
#include "TDTimeSystem.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\Object\ObjectManager.h"
#include "..\..\Object\Character\Obj_PlayerMyself.h"
#include "..\..\Object\Object.h"
#include "..\..\GameCommand.h"
#include "..\..\Object\ObjectCommandDef.h"
#include "GCCharBaseAttrib.h"
#include "..\..\DataPool\GMUIDataPool.h"
#include "..\..\DataPool\DataPoolCommandDef.h"
#include "..\..\DataPool\GMDP_CharacterData.h"
#include "..\..\Global.h"
#include "..\..\Event\GMEventSystem.h"
#include "AxProfile.h"
#include "TDVariable.h"
#include "..\..\Object\GMObjectLoadQueue.h"
#include "..\..\Interface\GMGameInterFace.h"

uint GCCharBaseAttribHandler::Execute( GCCharBaseAttrib* pPacket, Player* pPlayer ) 
{
__ENTER_FUNCTION
	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
//		AxProfile::AxProfile_PushNode("CharBaseAttrib");

		CObjectManager* pObjectManager = CObjectManager::GetMe();

		CObject* pObj = (CObject*)(pObjectManager->FindServerObject( (INT)pPacket->getObjID() ));
		if(pObj == NULL || !g_theKernel.IsKindOf(pObj->GetClass(), GETCLASS(CObject_Character)))
			return PACKET_EXE_CONTINUE;

		BOOL bAddLoadTask = FALSE;
		////如果需要更新资源，尝试放入缓冲队列
		//if(pPacket->IsUpdateAttrib(UPDATE_CHAR_ATT_DATA_ID)
		//	||pPacket->IsUpdateAttrib(UPDATE_CHAR_ATT_MODEL_ID)
		//	|| pPacket->IsUpdateAttrib(UPDATE_CHAR_ATT_MOUNT_ID)
		//	)
		//{
		//	GCCharBaseAttrib *pTaskPacket = (GCCharBaseAttrib*)(CNetManager::GetMe()->GetPacketFactory().CreatePacket(PACKET_GC_CHARBASEATTRIB));
		//	if(pTaskPacket != NULL)
		//	{
		//		pTaskPacket->setObjID(pPacket->getObjID());

		//		if(pPacket->IsUpdateAttrib(UPDATE_CHAR_ATT_DATA_ID))
		//			pTaskPacket->setDataID(pPacket->getDataID());

		//		if(pPacket->IsUpdateAttrib(UPDATE_CHAR_ATT_MODEL_ID))
		//			pTaskPacket->setModelID(pPacket->getModelID());

		//		if(pPacket->IsUpdateAttrib(UPDATE_CHAR_ATT_MOUNT_ID))
		//			pTaskPacket->setMountID(pPacket->getMountID());

		//		if(pObjectManager->GetLoadQueue()->TryAddLoadTask(pObj->GetID(), pTaskPacket, TRUE))
		//		{
		//			bAddLoadTask = TRUE;
		//		}
		//		else
		//		{
		//			CNetManager::GetMe()->GetPacketFactory().RemovePacket(pTaskPacket);
		//		}
		//	}
		//}

		//刷入数据池
		CCharacterData* pCharacterData = ((CObject_Character*)pObj)->GetCharacterData();

		if(pPacket->IsUpdateAttrib(UPDATE_CHAR_ATT_DATA_ID) && !bAddLoadTask)
		{
			pCharacterData->Set_RaceID(pPacket->getDataID());
		}
		if(pPacket->IsUpdateAttrib(UPDATE_CHAR_ATT_CAMP_ID))
		{
			pCharacterData->Set_CampData(pPacket->getCampData());
		}
		if(pPacket->IsUpdateAttrib(UPDATE_CHAR_ATT_NAME))
		{
			pCharacterData->Set_Name(pPacket->getName());
		}
		if(pPacket->IsUpdateAttrib(UPDATE_CHAR_ATT_TITLE))
		{
			pCharacterData->Set_CurTitle(pPacket->getTitle());
			pCharacterData->Set_CurTitleType(pPacket->getTitleType());
			CEventSystem::GetMe()->PushEvent(GE_CUR_TITLE_CHANGEED);
		}
		if(pPacket->IsUpdateAttrib(UPDATE_CHAR_ATT_LEVEL))
		{
			pCharacterData->Set_Level(pPacket->getLevel());
		}
		if(pPacket->IsUpdateAttrib(UPDATE_CHAR_ATT_HP_PERCENT))
		{
			pCharacterData->Set_HPPercent(pPacket->getHPPercent()/100.0f);
		}
		if(pPacket->IsUpdateAttrib(UPDATE_CHAR_ATT_MP_PERCENT))
		{
			pCharacterData->Set_MPPercent(pPacket->getMPPercent()/100.0f);
		}
		if(pPacket->IsUpdateAttrib(UPDATE_CHAR_ATT_RAGE))
		{
			pCharacterData->Set_Rage(pPacket->getRage() );
		}
		if(pPacket->IsUpdateAttrib(UPDATE_CHAR_ATT_STEALTH_LEVEL))
		{
			pCharacterData->Set_StealthLevel(pPacket->getStealthLevel() );
		}
		if(pPacket->IsUpdateAttrib(UPDATE_CHAR_ATT_SIT))
		{
			//pCharacterData->Set_Sit(pPacket->IsSit() );
		}
		if(pPacket->IsUpdateAttrib(UPDATE_CHAR_ATT_MOVE_SPEED))
		{
			pCharacterData->Set_MoveSpeed(pPacket->getMoveSpeed());
		}
		if(pPacket->IsUpdateAttrib(UPDATE_CHAR_ATT_ATTACK_SPEED))
		{
			//必须是主角
			if(pObj == CObjectManager::GetMe()->GetMySelf())
				pCharacterData->Set_AttackSpeed((INT)pPacket->getAttackSpeed());
		}
		
		if(pPacket->IsUpdateAttrib(UPDATE_CHAR_OWNER))
		{
			pCharacterData->Set_OwnerID(pPacket->getOwnerID());
		}
		if(pPacket->IsUpdateAttrib(UPDATE_CHAR_OCCUPANT))
		{
			pCharacterData->Set_OccupantGUID(pPacket->getOccupantGUID());
		}
		if(pPacket->IsUpdateAttrib(UPDATE_CHAR_ATT_PORTRAIT_ID))
		{
			pCharacterData->Set_PortraitID(pPacket->getPortraitID());
		}
		if(pPacket->IsUpdateAttrib(UPDATE_CHAR_ATT_MODEL_ID) && !bAddLoadTask)
		{
			pCharacterData->Set_ModelID(pPacket->getModelID());
		}
		if(pPacket->IsUpdateAttrib(UPDATE_CHAR_ATT_MOUNT_ID) && !bAddLoadTask)
		{
			pCharacterData->Set_MountID(pPacket->getMountID());
		}
		if(pPacket->IsUpdateAttrib(UPDATE_CHAR_ATT_PLAYER_DATA))
		{
			INT xx = pPacket->getFaceMeshID();
			pCharacterData->Set_FaceMesh(pPacket->getFaceMeshID());
			INT xx2 = pPacket->getHairMeshID();
			pCharacterData->Set_HairMesh(pPacket->getHairMeshID());
			INT xx3 = pPacket->getHairMatrixID();
			pCharacterData->Set_HairColor(pPacket->getHairMatrixID());
		}
		if(pPacket->IsUpdateAttrib(UPDATE_CHAR_IS_IN_STALL))
		{
			pCharacterData->Set_IsInStall(pPacket->getIsOpen());
		}
		if(pPacket->IsUpdateAttrib(UPDATE_CHAR_STALL_NAME))
		{
			pCharacterData->Set_StallName(pPacket->getStallName());
		}

		if(pPacket->IsUpdateAttrib(UPDATE_CHAR_STALL_NAME))
		{
			pCharacterData->Set_StallName(pPacket->getStallName());
		}

		pObj->PushDebugString("GCCharBaseAttrib");
		pObj->SetMsgTime(CGameProcedure::s_pTimeSystem->GetTimeNow());

//		AxProfile::AxProfile_PopNode("CharBaseAttrib");

		////如果是玩家自己
		//if(pObj == (CObject*)CObjectManager::GetMe()->GetMySelf())
		//{
		//	//得到名字
		//	((CObject_PlayerMySelf*)pObj)->GetCharacterData()->Set_Name(pPacket->getName());
		//	//得到称号
		//	((CObject_PlayerMySelf*)pObj)->GetCharacterData()->Set_CurTitle(pPacket->getTitle());

		//}
		//如果不是玩家自己
		if(pObj != (CObject*)CObjectManager::GetMe()->GetMySelf() && pPacket->IsUpdateAttrib(UPDATE_CHAR_ATT_AITYPE))
		{
			pCharacterData->Set_AIType(pPacket->getAIType());

		}
	}
	else
	{
		//TDAssert( FALSE );
	}

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}

