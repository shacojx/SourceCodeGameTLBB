#include "stdafx.h"


#include "CGCharAskEquipment.h"
#include "Log.h"
#include "GamePlayer.h"
#include "GCCharEquipment.h"
#include "Scene.h"

extern UINT GetEquipmentMaxLevelGemID(Item *pEquipment);

uint CGCharAskEquipmentHandler::Execute( CGCharAskEquipment* pPacket, Player* pPlayer )
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



		ObjID_t ObjID = pPacket->getObjID();
		if( ObjID == INVALID_ID )
		{
			Assert(FALSE) ;
			return PACKET_EXE_ERROR ;
		}

		Obj* pObj = pScene->GetObjManager()->GetObj( ObjID ) ;
		if( pObj==NULL )
		{
			g_pLog->FastSaveLog( LOG_FILE_1, "CGCharAskEquipmentHandler: not find obj=%d", ObjID ) ;
			return PACKET_EXE_CONTINUE ;
		}

		if( !IsCharacterObj( pObj->GetObjType() ) )
		{
			g_pLog->FastSaveLog( LOG_FILE_1, "CGCharAskEquipmentHandler: not character obj=%d", ObjID ) ;
			return PACKET_EXE_CONTINUE ;
		}

		Obj_Character* pCharacter = (Obj_Character*)pObj ;

		GCCharEquipment	Msg;
		Msg.setObjID(pCharacter->GetID());
		if ( pCharacter->GetObjType() == Obj::OBJ_TYPE_HUMAN )
		{
			Obj_Human* pDest = (Obj_Human*)pCharacter ;
			if(pDest->GetEquipID(HEQUIP_WEAPON)> 0)
			{
				Msg.setWeaponID(pDest->GetEquipID(HEQUIP_WEAPON));
				UINT uGemID = GetEquipmentMaxLevelGemID(pDest->GetEquipContain()->GetItem(HEQUIP_WEAPON));
				Msg.setWeaponGemID(uGemID);
			}
			if(pDest->GetEquipID(HEQUIP_BOOT)>0)
			{
				Msg.setBootID(pDest->GetEquipID(HEQUIP_BOOT));
				UINT uGemID = GetEquipmentMaxLevelGemID(pDest->GetEquipContain()->GetItem(HEQUIP_BOOT));
				Msg.setBootGemID(uGemID);
			}
			if(pDest->GetEquipID(HEQUIP_CUFF)>0)
			{
				Msg.setCuffID(pDest->GetEquipID(HEQUIP_CUFF));
				UINT uGemID = GetEquipmentMaxLevelGemID(pDest->GetEquipContain()->GetItem(HEQUIP_CUFF));
				Msg.setCuffGemID(uGemID);
			}
			if(pDest->GetEquipID(HEQUIP_CAP)>0)
			{
				Msg.setCapID(pDest->GetEquipID(HEQUIP_CAP));
				UINT uGemID = GetEquipmentMaxLevelGemID(pDest->GetEquipContain()->GetItem(HEQUIP_CAP));
				Msg.setCapGemID(uGemID);
			}
			if(pDest->GetEquipID(HEQUIP_ARMOR)>0)
			{
				Msg.setArmourID(pDest->GetEquipID(HEQUIP_ARMOR));
				UINT uGemID = GetEquipmentMaxLevelGemID(pDest->GetEquipContain()->GetItem(HEQUIP_ARMOR));
				Msg.setArmourGemID(uGemID);
			}

			pGamePlayer->SendPacket( &Msg ) ;
		}
		g_pLog->FastSaveLog( LOG_FILE_1, "CGCharAskEquipmentHandler: ok obj=%d scene=%d", 
		ObjID, pScene->SceneID() ) ;

		return PACKET_EXE_CONTINUE ;
		

		__LEAVE_FUNCTION
		return PACKET_EXE_ERROR ;
}
