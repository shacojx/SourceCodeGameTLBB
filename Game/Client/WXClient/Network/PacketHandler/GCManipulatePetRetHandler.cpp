
#include "stdafx.h"
#include "GCManipulatePetRet.h"
#include "..\..\DataPool\GMDP_Struct_Pet.h"
#include "..\..\DataPool\GMDataPool.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\Object\ObjectManager.h"
#include "..\..\Object\Object.h"
#include "..\..\Object\Character\Obj_Character.h"
#include "..\..\GameCommand.h"
#include "..\..\DataPool\DataPoolCommandDef.h"
#include "..\..\DataPool\GMUIDataPool.h"
#include "..\..\DataPool\GMDP_CharacterData.h"
#include "..\..\Global.h"
#include "../../Event/GMEventSystem.h"


UINT GCManipulatePetRetHandler::Execute( GCManipulatePetRet* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		SDATA_PET* My_Pet = CDataPool::GetMe()->Pet_GetPet(pPacket->GetGUID());
		// ....

		switch (pPacket->GetManipulateRet())
		{
		case GCManipulatePetRet::MANIPULATEPET_RET_CAPTUREFALID:
			{
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "捕捉失败");
			}
			break;
		case GCManipulatePetRet::MANIPULATEPET_RET_CAPTURESUCC:
			{
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "捕捉成功");
			}
			break;
		case GCManipulatePetRet::MANIPULATEPET_RET_CALLUPFALID:
			{
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "召唤失败");
			}
			break;
		case GCManipulatePetRet::MANIPULATEPET_RET_CALLUPSUCC:
			{
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "召唤成功");
			}
			break;
		case GCManipulatePetRet::MANIPULATEPET_RET_RECALLFALID:
			{
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"收回宠物失败");
			}
			break;
		case GCManipulatePetRet::MANIPULATEPET_RET_RECALLSUCC:
			{
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"收回宠物成功");
			}
			break;
		case GCManipulatePetRet::MANIPULATEPET_RET_FREEFALID:
			{
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"放生宠物失败");
			}
			break;
		case GCManipulatePetRet::MANIPULATEPET_RET_FREESUCC:
			{
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"放生宠物成功");
			}
			break;
		default :
			break;
		}
		CEventSystem::GetMe()->PushEvent(GE_UPDATE_PET_PAGE);

	}

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
