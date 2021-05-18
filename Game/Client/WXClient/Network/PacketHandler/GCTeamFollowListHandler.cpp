#include "StdAfx.h"
#include "GCTeamFollowList.h"

#include "..\..\Procedure\GameProcedure.h"
#include "..\..\Object\ObjectManager.h"
#include "..\..\Object\Object.h"

#include "..\..\GameCommand.h"
#include "..\..\Object\ObjectCommandDef.h"
#include "..\..\event\GMEventSystem.h"

uint GCTeamFollowListHandler::Execute( GCTeamFollowList* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		CObjectManager* pObjectManager = CObjectManager::GetMe();

		CObject* pObj = (CObject*)(pObjectManager->FindServerObject( (INT)pPacket->GetObjID() ));
		if ( pObj == NULL )
			return PACKET_EXE_CONTINUE;

		SCommand_Object cmdTemp;
		cmdTemp.m_wID				= OC_TEAM_FOLLOW_MEMBER;
		cmdTemp.m_adwParam[0]		= pPacket->GetMemberCount();

		for( INT i=1; i<=pPacket->GetMemberCount(); ++i )
		{
			cmdTemp.m_adwParam[i] = pPacket->GetFollowMember(i-1);
		}

		pObj->PushCommand(&cmdTemp );
		// CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, "GCTeamFollowList Received." );

	}

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}
