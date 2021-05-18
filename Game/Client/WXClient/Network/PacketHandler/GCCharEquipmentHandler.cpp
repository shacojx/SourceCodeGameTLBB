#include "StdAfx.h"
#include "TDTimeSystem.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\Object\ObjectManager.h"
#include "..\..\Object\Object.h"
#include "..\..\Object\Item\Obj_Item.h"
#include "..\..\GameCommand.h"
#include "..\..\Object\ObjectCommandDef.h"
#include "GCCharEquipment.h"
#include "TDUIDataPool.h"
#include "..\..\Global.h"
#include "..\..\Object\Character\Obj_Character.h"
#include "..\..\DataPool\GMDP_CharacterData.h"

uint GCCharEquipmentHandler :: Execute( GCCharEquipment* pPacket, Player* pPlayer ) 
{
__ENTER_FUNCTION
	//AxTrace(0, 2, "GCCharEquipmentHandler::Execute");
	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		CObjectManager* pObjectManager = CObjectManager::GetMe();

		CObject* pObj = (CObject*)(pObjectManager->FindServerObject( (INT)pPacket->getObjID() ));
		if(pObj == NULL || !g_theKernel.IsKindOf(pObj->GetClass(), GETCLASS(CObject_Character)))
			return PACKET_EXE_CONTINUE;
/*
		static UINT pBuf[UPDATE_CHAR_ATT_NUMBERS];
		pPacket->FillParamBuf( (VOID*)pBuf );

		SCommand_Object cmdTemp;
		cmdTemp.m_wID			= OC_UPDATE_EQUIPMENT;
		cmdTemp.m_adwParam[0]	= pPacket->getFlags();
		cmdTemp.m_apParam[1]	= (VOID*)pBuf;
		pObj->PushCommand(&cmdTemp );
*/
		//Ë¢ÈëÊý¾Ý³Ø
		CCharacterData * pCharacterData = ((CObject_Character*)pObj)->GetCharacterData();
//		if(pObj == (CObject*)CObjectManager::GetMe()->GetMySelf())
		{
			UINT dwEquipFlag = pPacket->getFlags();

			if(dwEquipFlag & (1<<HEQUIP_WEAPON)) 
			{
				pCharacterData->Set_EquipGem(HEQUIP_WEAPON, pPacket->getWeaponGemID());
				pCharacterData->Set_Equip(HEQUIP_WEAPON, pPacket->getWeaponID());
			}

			if(dwEquipFlag & (1<<HEQUIP_CAP))	 
			{
				pCharacterData->Set_Equip(HEQUIP_CAP, pPacket->getCapID());
			}

			if(dwEquipFlag & (1<<HEQUIP_ARMOR))
			{
				pCharacterData->Set_Equip(HEQUIP_ARMOR, pPacket->getArmourID());
			}

			if(dwEquipFlag & (1<<HEQUIP_CUFF)) 
			{
				pCharacterData->Set_Equip(HEQUIP_CUFF, pPacket->getCuffID());
			}

			if(dwEquipFlag & (1<<HEQUIP_BOOT))  
			{
				pCharacterData->Set_Equip(HEQUIP_BOOT, pPacket->getBootID());
			}
		}

		pObj->PushDebugString("GCCharEquipment");
		pObj->SetMsgTime(CGameProcedure::s_pTimeSystem->GetTimeNow());
	}
	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}

