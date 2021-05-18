#include "StdAfx.h"

#include "..\..\Procedure\GameProcedure.h"
#include "..\..\Object\ObjectManager.h"
#include "..\..\Object\Character\Obj_PlayerMySelf.h"

#include "..\..\DataPool\GMUIDataPool.h"
#include "..\..\event\GMEventSystem.h"

#include "GCAskTeamFollow.h"

uint GCAskTeamFollowHandler::Execute( GCAskTeamFollow* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		CObject_PlayerMySelf* pMe;

		pMe = CObjectManager::GetMe()->GetMySelf();
		CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, "请选择是否加入组队模式。" );

		GUID_t LeaderGUID;
		LeaderGUID = CUIDataPool::GetMe()->GetTeamOrGroup()->GetLeaderGUID();

		CHAR LeaderName[MAX_CHARACTER_NAME];
		strncpy(LeaderName,
			CUIDataPool::GetMe()->GetTeamOrGroup()->GetMember(LeaderGUID)->m_szNick,
			MAX_CHARACTER_NAME);

		CEventSystem::GetMe()->PushEvent(GE_TEAM_FOLLOW_INVITE, LeaderName);

	}

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}
