#include "StdAfx.h"
#include "GCNotifyTeamInfo.h"

#include "..\..\Procedure\GameProcedure.h"
#include "..\..\Object\ObjectManager.h"
#include "..\..\Object\Object.h"

#include "..\..\GameCommand.h"
#include "..\..\Object\ObjectCommandDef.h"
#include "..\..\event\GMEventSystem.h"

uint GCNotifyTeamInfoHandler::Execute( GCNotifyTeamInfo* pPacket, Player* pPlayer )
{
//__ENTER_FUNCTION

	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		CObjectManager* pObjectManager = CObjectManager::GetMe();

		CObject* pObj = (CObject*)(pObjectManager->FindServerObject( (INT)pPacket->GetObjID() ));
		if ( pObj == NULL )
			return PACKET_EXE_CONTINUE;

		SCommand_Object cmdTemp;
		cmdTemp.m_wID			= OC_UPDATE_TEAM_FLAG;
		cmdTemp.m_abParam[0]	= pPacket->GetHaveTeamFlag();
		cmdTemp.m_abParam[1]	= pPacket->GetTeamLeaderFlag();
		cmdTemp.m_abParam[2]	= pPacket->GetTeamFullFlag();
//		cmdTemp.m_abParam[3]	= pPacket->GetTeamFollowFlag();
		pObj->PushCommand( &cmdTemp );

		// CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, "GCNotifyTeamInfo Received." );
	}

	return PACKET_EXE_CONTINUE ;

//__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
