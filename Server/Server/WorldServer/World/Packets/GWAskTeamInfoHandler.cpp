#include "stdafx.h"
// GWAskTeamInfoHandler.cpp

#include "GWAskTeamInfo.h"

#include "ServerPlayer.h"
#include "ServerManager.h"
#include "OnlineUser.h"
#include "Log.h"
#include "Team.h"
#include "WGTeamResult.h"
#include "WGTeamList.h"
#include "WGTeamFollowList.h"

uint GWAskTeamInfoHandler::Execute( GWAskTeamInfo* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	ServerPlayer* pServerPlayer = (ServerPlayer*)pPlayer;
	GUID_t guid = pPacket->GetGUID(); // 被请求信息的玩家的 GUID

	USER* pUser = g_pOnlineUser->FindUser( guid );
	if( pUser == NULL )
	{
		Log::SaveLog( WORLD_LOGFILE, "GWAskTeamInfoHandler...User GUID=%X not find!", 
			guid );
		return PACKET_EXE_CONTINUE;
	}

	if( pUser->GetTeamID() == INVALID_ID )
	{
		return PACKET_EXE_CONTINUE;
	}

	Team* pTeam = g_pTeamList->GetTeam( pUser->GetTeamID() );
	if( pTeam==NULL )
	{
		Assert(FALSE) ;
		return PACKET_EXE_CONTINUE ;
	}
	else
	{
		TEAMMEMBER Member;

		Member.m_Member = pUser->GetGUID();

		if ( pTeam->IsMember( &Member ) == FALSE )
		{ // 某些情况下，玩家保存了过期的队伍号
			AssertEx( FALSE, "过期队伍号，忽略。" );
			return PACKET_EXE_CONTINUE;
		}
	}

	WGTeamList Msg;
	Msg.SetPlayerID(pPacket->GetPlayerID());
	Msg.SetTeamID( pUser->GetTeamID() );
	Msg.SetGUID( guid );
	WGTeamResult MsgtoEveryMember; // 通知其他队员某人进入新场景了
	MsgtoEveryMember.SetReturn( TEAM_RESULT_ENTERSCENE );
	MsgtoEveryMember.SetTeamID( pTeam->GetTeamID() );
	MsgtoEveryMember.SetGUID( pUser->GetGUID() );
	MsgtoEveryMember.SetGUIDEx( pUser->GetPlayerID() );
	MsgtoEveryMember.SetIcon( pUser->GetPortrait() );
	MsgtoEveryMember.SetSceneID( pUser->GetSceneID() );

	for(int i=0; i<pTeam->MemberCount(); ++i)
	{
		TEAMMEMBER* pMember = pTeam->Member( i );
		if( pMember==NULL )
		{
			Assert(FALSE);
			continue;
		}

		USER* pDestUser = g_pOnlineUser->FindUser( pMember->m_Member );
		if( pDestUser==NULL )
		{
			Assert(FALSE);
			continue;
		}

		ID_t ServerID = pDestUser->GetServerID() ;
		ServerPlayer* pDestServerPlayer = g_pServerManager->GetServerPlayer( ServerID ) ;
		if( pDestServerPlayer==NULL )
		{
			Assert(FALSE) ;
			continue ;
		}

		if( pUser->GetGUID() != pDestUser->GetGUID() )
		{
			MsgtoEveryMember.SetPlayerID( pDestUser->GetPlayerID() ) ;
			pDestServerPlayer->SendPacket( &MsgtoEveryMember );
		}

		TEAM_LIST_ENTRY entry;

		entry.SetGUID( pDestUser->GetGUID() );
		entry.SetSceneID( pDestUser->GetSceneID() );
		entry.SetExtraID( pDestUser->GetPlayerID() );
		entry.SetName( pDestUser->GetName() );
		entry.SetIcon( pDestUser->GetPortrait() );
		entry.SetDataID( pDestUser->GetSex() );
		Msg.AddTeamMember( entry );

	}

	pServerPlayer->SendPacket( &Msg );

	if ( pTeam->GetFollowedmemberCount()>0 )
	{
		GUID_t* FollowedMembers = pTeam->GetFollowedMembers();
		BOOL flag;

		flag = FALSE;

		for( INT i=0; i<pTeam->GetFollowedmemberCount(); ++i )
		{
			if ( FollowedMembers[i] == guid )
			{ // 如果身处组队跟随状态则发送组队列表
				flag = TRUE;
				break;
			}
		}

		if ( flag == TRUE )
		{
			WGTeamFollowList FollowListMsg;
			FollowListMsg.SetPlayerID(pPacket->GetPlayerID());
			FollowListMsg.SetGUID( pPacket->GetGUID() );

			for( INT i=0; i<pTeam->GetFollowedmemberCount(); ++i )
			{
				FollowListMsg.AddFollowMember(FollowedMembers[i]);
			}

			pServerPlayer->SendPacket( &FollowListMsg );
		}
	}

	Log::SaveLog( WORLD_LOGFILE, "GWAskTeamInfoHandler...Success!  GUID=%X", guid );

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}
