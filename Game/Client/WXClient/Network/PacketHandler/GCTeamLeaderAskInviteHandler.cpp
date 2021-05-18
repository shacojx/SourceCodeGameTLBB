#include "StdAfx.h"

#include "..\..\Procedure\GameProcedure.h"
#include "..\..\DataPool\GMUIDataPool.h"
#include "..\..\event\GMEventSystem.h"

#include "GCTeamLeaderAskInvite.h"

uint GCTeamLeaderAskInviteHandler::Execute( GCTeamLeaderAskInvite* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION


	

/*
	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{ // 这个地方还不能直接抛出消息，没有地方存储信息
		char szTemp[MAX_PATH] = {0};
		_snprintf(szTemp, MAX_PATH, "%s 希望邀请 %s 加入队伍", pPacket->GetSourName(), pPacket->GetDestName());
		CGTeamLeaderRetInvite msg;
		msg.SetSourGUID( pPacket->GetSourGUID() );
		msg.SetDestGUID( pPacket->GetDestGUID() );

		if(IDYES == MessageBox(NULL, szTemp, "组队邀请!!!", MB_YESNO|MB_ICONQUESTION))
		{
			msg.SetReturn(1);
		}
		else
		{
			msg.SetReturn(0);
		}

		CNetManager::GetMe()->SendPacket(&msg);
	}
*/
	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		//CUIDataPool::GetMe()->SetSourceGUID( pPacket->GetSourGUID() );
		//CUIDataPool::GetMe()->SetDestinationGUID( pPacket->GetDestGUID() );//

		INT iPos = CUIDataPool::GetMe()->SetMemberInviteGUID(pPacket->GetSourGUID(), pPacket->GetDestGUID());
		CEventSystem::GetMe()->PushEvent(GE_TEAM_MEMBER_INVITE, pPacket->GetSourName(), pPacket->GetDestName(), iPos); 

		return PACKET_EXE_CONTINUE;//
	}



__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
