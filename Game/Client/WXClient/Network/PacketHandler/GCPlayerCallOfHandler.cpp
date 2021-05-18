// GCPlayerCallOfHandler.cpp

#include "StdAfx.h"

#include "GCPlayerCallOf.h"
#include "..\..\Event\GMEventSystem.h"
#include "..\..\DataPool\TeamOrGroup.h"
#include "..\..\DataPool\GMUIDataPool.h"


using namespace Packets;

uint GCPlayerCallOfHandler::Execute( GCPlayerCallOf* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION
	//显示对话框，
	//确定和取消的时候发送 CGPlayerCallOfResult
	GUID_t PlayerGuid = pPacket->GetCallerGUID();

	// 到好友列表中取找到这个玩家的名字 
	TeamMemberInfo* pPlayerInfo;
	pPlayerInfo = CUIDataPool::GetMe()->GetTeamOrGroup()->GetMember(PlayerGuid);
	if(!pPlayerInfo)
	{
		return PACKET_EXE_CONTINUE;
	}

	CEventSystem::GetMe()->PushEvent(GE_OPEN_CALLOF_PLAYER, pPlayerInfo->m_szNick, "", pPacket->GetTime());

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
