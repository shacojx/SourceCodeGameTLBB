#include "StdAfx.h"
#include "TDTimeSystem.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\Object\ObjectManager.h"
#include "..\..\Object\Object.h"
#include "..\..\GameCommand.h"
#include "..\..\Object\ObjectCommandDef.h"
#include "GCCharSkill_Gather_Modify.h"

uint GCCharSkill_Gather_ModifyHandler :: Execute( GCCharSkill_Gather_Modify* pPacket, Player* pPlayer ) 
{
__ENTER_FUNCTION

//	AxTrace(0, 2, "GCCharSkill_Gather_ModifyHandler::Execute");
	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		CObjectManager* pObjectManager = CObjectManager::GetMe();

		CObject* pObj = (CObject*)(pObjectManager->FindServerObject( (INT)pPacket->getObjID() ));
		if ( pObj == NULL )
			return PACKET_EXE_CONTINUE;

		SCommand_Object cmdTemp;
		cmdTemp.m_wID			= OC_SKILL_GATHER_MODIFY;
		cmdTemp.m_afParam[0]	= ((FLOAT)pPacket->getSubTime())/1000.f;
		pObj->PushCommand(&cmdTemp );

		pObj->PushDebugString("GCCharSkill_Gather_Modify");
		pObj->SetMsgTime(CGameProcedure::s_pTimeSystem->GetTimeNow());
	}

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}

