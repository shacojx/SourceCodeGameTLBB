#include "StdAfx.h"
#include "GCAbilityResult.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\Event\GMEventSystem.h"
#include "..\..\Object\ObjectManager.h"
#include "..\..\Object\Character\Obj_PlayerMySelf.h"
#include "TDUIDataPool.h"
#include "OResultDef.h"
#include "..\..\Object\ObjectCommandDef.h"
#include "..\..\Sound\GMSoundSystem.h"

uint GCAbilityResultHandler::Execute( GCAbilityResult* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION
	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		CObjectManager* pObjectManager = CObjectManager::GetMe();
//		//失败！
		if( pPacket->getResult() != OR_OK)
		{
			INT nResult = pPacket->getResult();
//			CGameProcedure::s_pGfxSystem->PushDebugString("ERROR");
//			static CHAR szTemp[MAX_PATH];
//			_snprintf(szTemp, MAX_PATH, "Error No:%d",nResult);
//			GetOResultText((OPERATE_RESULT)pPacket->getResult());
			CGameProcedure::s_pEventSystem->PushEvent(GE_NEW_DEBUGMESSAGE,GetOResultText((OPERATE_RESULT)nResult));
			if(nResult == OR_FAILURE)
			{
				switch(pPacket->GetAbilityID())
				{
				case 3:
					CSoundSystemFMod::_PlayUISoundFunc(22+59);
					break;
				default:
					break;				
				}

				CEventSystem::GetMe()->PushEvent(GE_CLOSE_SYNTHESIZE_ENCHASE);
			}
			// 这里是做什么呢，？
			//SCommand_Object cmdCharacter;
			//cmdCharacter.m_wID = OC_LIFE_ABILITE_ACTION;
			//cmdCharacter.m_adwParam[0] = pPacket->GetAbilityID();
			//cmdCharacter.m_adwParam[1] = -1;
			//cmdCharacter.m_adwParam[2] = 2;
			//cmdCharacter.m_adwParam[3] = pPacket->GetPrescriptionID();
	
			//pObjectManager->GetMySelf()->PushCommand(&cmdCharacter);
			return PACKET_EXE_CONTINUE;
		}

		// 这里是做什么呢，？
		//SCommand_Object cmdCharacter;
		//cmdCharacter.m_wID = OC_LIFE_ABILITE_ACTION;
		//cmdCharacter.m_adwParam[0] = pPacket->GetAbilityID();
		//cmdCharacter.m_adwParam[1] = -1;
		//cmdCharacter.m_adwParam[2] = 1;
		//cmdCharacter.m_adwParam[3] = pPacket->GetPrescriptionID();

		//pObjectManager->GetMySelf()->PushCommand(&cmdCharacter);
	}

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}
