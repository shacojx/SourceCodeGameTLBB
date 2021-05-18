#include "stdafx.h"

#include "Scene.h"
#include "GamePlayer.h"
#include "Obj_Pet.h"
#include "Obj_Human.h"
#include "AI_Human.h"
#include "CGManipulatePet.h"
#include "GCManipulatePetRet.h"
#include "GCOperateResult.h"
#include "GCDetailAttrib_Pet.h"

using namespace Packets;


uint CGManipulatePetHandler::Execute( CGManipulatePet* pPacket, Player* pPlayer )
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

	Item *pPetItem = pHuman->GetPetItem(pPacket->GetGUID());
	if(pPetItem == NULL)
		return PACKET_EXE_CONTINUE;

	if(pPetItem->IsLock())
	{
		return PACKET_EXE_CONTINUE;
	}

	INT nType = pPacket->GetManipulateType();
	switch(nType)
	{
	case MANIPULATE_CREATEPET:
		{// 召唤宠物
			ORESULT oResult = pHuman->TestCallUpPet(pPacket->GetGUID() );
			if( OR_SUCCEEDED(oResult) )
			{
				AI_Human* pHumanAI = (AI_Human*)(pHuman->GetAIObj());
				if (pHumanAI)
				{
					ObjID_t idSkill = CALL_UP_PET;
					oResult = pHumanAI->PushCommand_UseSkill(idSkill, 1, -1, -1.f, -1.f, 0.f, INVALID_GUID);
				}
			}
			if( OR_FAILED(oResult) )
			{
				GCManipulatePetRet msg;
				msg.SetManipulateRet(GCManipulatePetRet::MANIPULATEPET_RET_CALLUPFALID);
				pGamePlayer->SendPacket(&msg);

				pHuman->SendOperateResultMsg(oResult);
				return PACKET_EXE_CONTINUE;
			}

			pHuman->ReCallPet();
			pHuman->SetGUIDOfCallUpPet(pPacket->GetGUID());
		}
		break;
	case MANIPULATE_DELETEPET:
		{// 收回宠物
			ORESULT oResult = pHuman->ReCallPet();
			GCManipulatePetRet msg;
			if (OR_FAILED(oResult))
			{
				msg.SetManipulateRet(GCManipulatePetRet::MANIPULATEPET_RET_RECALLFALID);
			}
			else if (OR_SUCCEEDED(oResult))
			{
				msg.SetManipulateRet(GCManipulatePetRet::MANIPULATEPET_RET_RECALLSUCC);
			}
			pGamePlayer->SendPacket(&msg);
		}
		break;
	case MANIPULATE_FREEPET:
		{// 放生宠物
			PET_LOG_PARAM	PetLogParam;
			ORESULT oResult = pHuman->FreePetToNature(&PetLogParam,pPacket->GetGUID());
			GCManipulatePetRet msg;
			if (OR_FAILED(oResult))
			{
				msg.SetManipulateRet(GCManipulatePetRet::MANIPULATEPET_RET_FREEFALID);
			}
			else if (OR_SUCCEEDED(oResult))
			{
				msg.SetManipulateRet(GCManipulatePetRet::MANIPULATEPET_RET_FREESUCC);
			}
			pGamePlayer->SendPacket(&msg);
		}
		break;
	case MANIPULATE_ASKOTHERPETINFO:
		{// 察看其他玩家的宠物信息
			ObjID_t objID = pPacket->GetObjID();
			Obj* pObj = pScene->GetObjManager()->GetObj(objID);
			if (pObj && pObj->GetObjType() == Obj::OBJ_TYPE_PET)
			{
				Obj_Human* pCreator = ((Obj_Pet*)pObj)->GetCreator();
				if (pCreator)
				{
					PET_GUID_t guidpet = ((Obj_Pet*)pObj)->GetPetGUID();
					if(guidpet.IsNull())
					{
						return PACKET_EXE_CONTINUE;
					}
					Item* pPetItem = pCreator->GetPetItem(guidpet);
					if(!pPetItem) 
					{
						return PACKET_EXE_CONTINUE;
					}

					GCDetailAttrib_Pet msg;
					pCreator->CalculatePetDetailAttrib(msg, pPetItem);
					msg.SetExtraInfoLength(1);
					BYTE bFlag;
					bFlag = TYPE_CONTEX_MENU_OTHER_PET;
					msg.SetExtraInfoData(&bFlag);
					pHuman->GetPlayer()->SendPacket(&msg);
				}
				else
				{
					pHuman->SendOperateResultMsg(OR_CANNOT_ASK_PETDETIAL);
				}
			}
		}
		break;
	default:
		break;
	}
	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
