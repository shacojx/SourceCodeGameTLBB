#include "StdAfx.h"
#include "TDTimeSystem.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\Object\ObjectManager.h"
#include "..\..\Object\Object.h"
#include "..\..\Object\Character\Obj_Character.h"
#include "..\..\GameCommand.h"
#include "..\..\Global.h"
#include "..\..\Object\ObjectCommandDef.h"
#include "GCCharDoAction.h"

uint GCCharDoActionHandler :: Execute( GCCharDoAction* pPacket, Player* pPlayer ) 
{
__ENTER_FUNCTION

	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		CObjectManager* pObjectManager = CObjectManager::GetMe();

		CObject* pObj = (CObject*)(pObjectManager->FindServerObject( (INT)pPacket->getObjID() ));
		if ( pObj == NULL )
			return PACKET_EXE_CONTINUE;

		if(g_theKernel.IsKindOf(pObj->GetClass(), GETCLASS(CObject_Character)))
		{
			CObject_Character *pCharacter = (CObject_Character*)pObj;

			CHAR szText[128];
			sprintf(szText, "%d;0", (INT)(pPacket->getActionID()));
			pCharacter->SetChatMoodAction(STRING(szText));
		}

		pObj->PushDebugString("GCCharDoAction");
		pObj->SetMsgTime(CGameProcedure::s_pTimeSystem->GetTimeNow());
	}

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}

