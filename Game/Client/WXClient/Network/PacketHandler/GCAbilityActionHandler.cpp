#include "StdAfx.h"

#include "..\..\Procedure\GameProcedure.h"
#include "..\..\Object\ObjectManager.h"
#include "..\..\Object\Object.h"
#include "..\..\GameCommand.h"
#include "..\..\Object\ObjectCommandDef.h"

#include "GCAbilityAction.h"

uint GCAbilityActionHandler::Execute( GCAbilityAction* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION
	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		//AxTrace(0,0,"GCAbilityActionHandler,有人使用生活技能");
		CObjectManager* pObjectManager = CObjectManager::GetMe();
		if(!pObjectManager)
			return PACKET_EXE_ERROR;
		CObject* pObj = (CObject*)(pObjectManager->FindServerObject( (INT)pPacket->getObjID() ));
		if ( pObj == NULL )
		{
			return PACKET_EXE_CONTINUE;
		}

		if(pPacket->getBeginOrEnd() == GCAbilityAction::ABILITY_BEGIN)
		{
			SCommand_Object cmdTemp;
			cmdTemp.m_wID			= OC_ABILITY;
			cmdTemp.m_auParam[0]	= 0;
			cmdTemp.m_anParam[1]	= pPacket->getLogicCount();
			cmdTemp.m_anParam[2]	= pPacket->getAction();
			cmdTemp.m_anParam[3]	= pPacket->getPrescriptionID();
			cmdTemp.m_anParam[4]	= pPacket->getTargetID();
			pObj->PushCommand(&cmdTemp );
		}
	}
	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
