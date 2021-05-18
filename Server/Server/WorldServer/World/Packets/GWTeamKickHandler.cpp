#include "stdafx.h"
#include "GWTeamKick.h"
#include "Log.h"
#include "OnlineUser.h"
#include "ServerPlayer.h"
#include "WGTeamError.h"
#include "Team.h"
#include "WGTeamResult.h"
#include "ServerManager.h"

uint GWTeamKickHandler::Execute( GWTeamKick* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	ServerPlayer* pSourServerPlayer = (ServerPlayer*)pPlayer;

	GUID_t sGuid = pPacket->GetSourGUID(); // 队长
	GUID_t dGuid = pPacket->GetDestGUID(); // 倒霉蛋

	USER* pSourUser = g_pOnlineUser->FindUser( sGuid );
	if( pSourUser==NULL )
	{
		Log::SaveLog( WORLD_LOGFILE, "GWTeamKickHandler...User Sour GUID=%X not find!", 
			sGuid );

		return PACKET_EXE_CONTINUE;
	}

	TeamID_t tid = pSourUser->GetTeamID();
	Team* pTeam = g_pTeamList->GetTeam( tid );
	if( pTeam==NULL )
	{
		Log::SaveLog( WORLD_LOGFILE, "GWTeamKickHandler...User sGUID=%X TeamID=%d not find team!", 
			sGuid, tid );

		return PACKET_EXE_CONTINUE;
	}

	INT nMemberCount = pTeam->MemberCount();
	INT i;
	for( i=0; i<nMemberCount; ++i )
	{
		if ( pTeam->Member(i)->m_Member == dGuid )
		{
			break;
		}
	}

	if( i >= nMemberCount )
	{ // 被踢者不在队伍里
		Log::SaveLog( WORLD_LOGFILE, "GWTeamKickHandler...User dGUID=%X not in sTeamID=%d(sGUID=%X)!", 
			dGuid, tid, sGuid );

		return PACKET_EXE_CONTINUE;
	}

	if( pTeam->Leader()->m_Member != sGuid )
	{ // 踢人者不是队长
		WGTeamError Msg;
		Msg.SetPlayerID( pSourUser->GetPlayerID() );
		Msg.SetGUID( pSourUser->GetGUID() );
		Msg.SetErrorCode( TEAM_ERROR_KICKNOTLEADER );

		pSourServerPlayer->SendPacket( &Msg );

		Log::SaveLog( WORLD_LOGFILE, "GWTeamKickHandler...User sGUID=%X TeamID=%d not leader!", 
			sGuid, tid );

		return PACKET_EXE_CONTINUE;
	}

	WGTeamResult Msg;
	Msg.SetReturn( TEAM_RESULT_TEAMKICK );
	Msg.SetGUID( dGuid );
	Msg.SetTeamID( tid );

	//通知所有组内的玩家
	for( int i=0; i<pTeam->MemberCount(); i++ )
	{
		TEAMMEMBER* pMember = pTeam->Member( i );
		if( pMember==NULL )
		{
			Assert(FALSE);
			continue;
		}

		USER* pUser = g_pOnlineUser->FindUser( pMember->m_Member );
		if( pUser==NULL )
		{//如果队员离线,则用户数据是空
			continue;
		}
		
		ID_t ServerID = pUser->GetServerID();
		ServerPlayer* pServerPlayer = g_pServerManager->GetServerPlayer( ServerID );
		if( pServerPlayer==NULL )
		{
			Assert(FALSE);
			continue;
		}

		Msg.SetPlayerID( pUser->GetPlayerID() );

		pServerPlayer->SendPacket( &Msg );
	}

	//剔除队员
	TEAMMEMBER Member;
	Member.m_Member = dGuid;
	pTeam->DelMember( &Member );

	//设置被踢者的队伍信息
	USER* pDestUser = g_pOnlineUser->FindUser( dGuid );
	if ( pDestUser!=NULL )
	{
		pDestUser->SetTeamID( INVALID_ID );
	}

	Log::SaveLog( WORLD_LOGFILE, "GWTeamKickHandler...User sGUID=%X TeamID=%d Success!", 
		sGuid, tid );

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}
