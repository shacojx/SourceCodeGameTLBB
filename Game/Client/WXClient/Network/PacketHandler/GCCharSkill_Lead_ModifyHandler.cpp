#include "StdAfx.h"
#include "TDTimeSystem.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\Object\ObjectManager.h"
#include "..\..\Object\Object.h"
#include "..\..\GameCommand.h"
#include "..\..\Object\ObjectCommandDef.h"
#include "GCCharSkill_Lead_Modify.h"

uint GCCharSkill_Lead_ModifyHandler :: Execute( GCCharSkill_Lead_Modify* pPacket, Player* pPlayer ) 
{
__ENTER_FUNCTION

//	AxTrace(0, 2, "GCCharSkill_Lead_ModifyHandler::Execute");
	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		CObjectManager* pObjectManager = CObjectManager::GetMe();

		CObject* pObj = (CObject*)(pObjectManager->FindServerObject( (INT)pPacket->getObjID() ));
		if ( pObj == NULL )
			return PACKET_EXE_CONTINUE;

		SCommand_Object cmdTemp;
		cmdTemp.m_wID			= OC_SKILL_LEAD_MODIFY;
		cmdTemp.m_afParam[0]	= ((FLOAT)pPacket->getSubTime())/1000.f;
		pObj->PushCommand(&cmdTemp );

		pObj->PushDebugString("GCCharSkill_Lead_Modify");
		pObj->SetMsgTime(CGameProcedure::s_pTimeSystem->GetTimeNow());
	}

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}

