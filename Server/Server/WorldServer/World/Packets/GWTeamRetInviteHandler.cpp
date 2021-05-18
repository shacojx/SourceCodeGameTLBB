#include "stdafx.h"


#include "GWTeamRetInvite.h"
#include "Log.h"
#include "OnlineUser.h"
#include "ServerPlayer.h"
#include "WGTeamError.h"
#include "Team.h"
#include "ServerManager.h"
#include "WGTeamResult.h"
#include "WGTeamLeaderAskInvite.h"




uint GWTeamRetInviteHandler::Execute( GWTeamRetInvite* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	GUID_t sGuid = pPacket->GetSourGUID();//邀请人的GUID
	GUID_t dGuid = pPacket->GetDestGUID();//被邀请人的GUID

	USER* pDestUser = g_pOnlineUser->FindUser( dGuid );
	if( pDestUser==NULL )
	{
		Log::SaveLog( WORLD_LOGFILE, "GWTeamRetInviteHandler...User Dest GUID=%X not find!", 
			dGuid );

		return PACKET_EXE_CONTINUE;
	}

	ServerPlayer* pDestServerPlayer = (ServerPlayer*)pPlayer;

	USER* pSourUser = g_pOnlineUser->FindUser( sGuid );
	if( pSourUser==NULL )
	{
		if( pPacket->GetReturn()==TRUE )
		{
			WGTeamError dMsg;
			dMsg.SetPlayerID( pDestUser->GetPlayerID() );
			dMsg.SetGUID( pDestUser->GetGUID() );
			dMsg.SetErrorCode( TEAM_ERROR_TARGETNOTONLINE );
			pDestServerPlayer->SendPacket( &dMsg );
		}

		Log::SaveLog( WORLD_LOGFILE, "GWTeamRetInviteHandler...User Sour GUID=%X not find!", 
			sGuid );
		return PACKET_EXE_CONTINUE;
	}

	if( pSourUser->IsEnemy( pDestUser->GetUserCampData() ) )
	{
		Log::SaveLog( WORLD_LOGFILE, "GWTeamRetInviteHandler...Sour Name=%X, Dest Name=%s are enemy!", 
			pSourUser->GetName(), pDestUser->GetName() );

		return PACKET_EXE_CONTINUE;
	}

	ID_t SourServerID = pSourUser->GetServerID();
	ServerPlayer* pSourServerPlayer = g_pServerManager->GetServerPlayer( SourServerID );
	if( pSourServerPlayer==NULL )
	{
		Assert(FALSE);
		return PACKET_EXE_CONTINUE;
	}

	USER* pLeader = NULL;
	ServerPlayer* pLeaderServerPlayer;

	if ( pSourUser->GetTeamID() != INVALID_ID )
	{
		Team* pTeam = g_pTeamList->GetTeam( pSourUser->GetTeamID() );

		if ( pTeam == NULL )
		{
			Assert( pTeam );
			pSourUser->SetTeamID( INVALID_ID );
		}
		else if ( pTeam->Leader()->m_Member != sGuid )
		{ // 邀请人不是队长
			pLeader = g_pOnlineUser->FindUser( pTeam->Leader()->m_Member );

			pLeaderServerPlayer = g_pServerManager->GetServerPlayer( pLeader->GetServerID() );
			if ( pLeaderServerPlayer == NULL )
			{
				Assert( FALSE );
				pLeader = NULL;
			}
		}
	}


	if( pPacket->GetReturn()==FALSE )
	{//被邀请人不同意加入队伍
		WGTeamError Msg;
		Msg.SetPlayerID( pSourUser->GetPlayerID() );
		Msg.SetGUID( pDestUser->GetGUID() );
		Msg.SetErrorCode( TEAM_ERROR_INVITEREFUSE );

		pSourServerPlayer->SendPacket( &Msg );

		if ( pLeader != NULL )
		{
			Msg.SetPlayerID( pLeader->GetPlayerID() );
			pLeaderServerPlayer->SendPacket( &Msg );
		}

		Log::SaveLog( WORLD_LOGFILE, "GWTeamRetInviteHandler...Invite Refuse SourGUID=%X DestGUID=%X", 
			sGuid, dGuid );
	}
	else if( pDestUser->GetTeamID() != INVALID_ID )
	{//被邀请人已经有队伍
		WGTeamError Msg;
		Msg.SetPlayerID( pSourUser->GetPlayerID() );
		Msg.SetGUID( pDestUser->GetGUID() );
		Msg.SetErrorCode(TEAM_ERROR_INVITEDESTHASTEAM);

		pSourServerPlayer->SendPacket( &Msg );

		if ( pLeader != NULL )
		{
			Msg.SetPlayerID( pLeader->GetPlayerID() );
			pLeaderServerPlayer->SendPacket( &Msg );
		}

		Log::SaveLog( WORLD_LOGFILE, "GWTeamRetInviteHandler...DestInTeam SourGUID=%X DestGUID=%X", 
			sGuid, dGuid );
	}
	else if( pSourUser->GetTeamID()==INVALID_ID )
	{//邀请人和被邀请人都无队伍
		TeamID_t tid = g_pTeamList->CreateTeam( );
		Assert( tid!=INVALID_ID );
		Team* pTeam = g_pTeamList->GetTeam( tid );
		Assert( pTeam );

		if ( pSourUser->GetGUID() == pDestUser->GetGUID() )
		{

			//将玩家加入队伍中
			TEAMMEMBER Member;
			Member.m_Member = sGuid;
			pTeam->AddMember( &Member );

			//设置玩家队伍数据
			pSourUser->SetTeamID( tid );

			//返回结果
			WGTeamResult Msg;
			Msg.SetPlayerID( pSourUser->GetPlayerID() );
			Msg.SetReturn( TEAM_RESULT_MEMBERENTERTEAM );
			Msg.SetGUID( sGuid );
			Msg.SetTeamID( tid );
			Msg.SetGUIDEx( pSourUser->GetPlayerID() ); // 将玩家的 PlayerID 传回
			Msg.SetIcon( pSourUser->GetPortrait() );
			Msg.SetSceneID( pSourUser->GetSceneID() );
			Msg.SetDataID( pSourUser->GetSex() );


			pSourServerPlayer->SendPacket( &Msg );

			Log::SaveLog( WORLD_LOGFILE, "GWTeamRetInviteHandler...CreateTeam GUID=%X TeamID=%d", 
				sGuid, tid );
		}
		else
		{
			//将玩家加入队伍中
			TEAMMEMBER sMember;
			sMember.m_Member = sGuid;
			pTeam->AddMember( &sMember );
			TEAMMEMBER dMember;
			dMember.m_Member = dGuid;
			pTeam->AddMember( &dMember );

			//设置玩家队伍数据
			pSourUser->SetTeamID( tid );
			pDestUser->SetTeamID( tid );

			//返回结果
			WGTeamResult sMsg1;
			sMsg1.SetPlayerID( pSourUser->GetPlayerID() );
			sMsg1.SetReturn( TEAM_RESULT_TEAMREFRESH );
			sMsg1.SetGUID( sGuid );
			sMsg1.SetTeamID( tid );
			sMsg1.SetGUIDEx( pSourUser->GetPlayerID() ); // 将玩家的 PlayerID 传回
			sMsg1.SetSceneID( pSourUser->GetSceneID() );
			pSourServerPlayer->SendPacket( &sMsg1 );

			WGTeamResult sMsg2;
			sMsg2.SetPlayerID( pSourUser->GetPlayerID() );
			sMsg2.SetReturn( TEAM_RESULT_MEMBERENTERTEAM );
			sMsg2.SetGUID( dGuid );
			sMsg2.SetTeamID( tid );
			sMsg2.SetGUIDEx( pDestUser->GetPlayerID() ); // 将玩家的 PlayerID 传回
			sMsg2.SetSceneID( pDestUser->GetSceneID() );
			sMsg2.SetName( pDestUser->GetName() );
			sMsg2.SetIcon( pDestUser->GetPortrait() );
			sMsg2.SetDataID( pDestUser->GetSex() );
			pSourServerPlayer->SendPacket( &sMsg2 );

			WGTeamResult dMsg1;
			dMsg1.SetPlayerID( pDestUser->GetPlayerID() );
			dMsg1.SetReturn( TEAM_RESULT_TEAMREFRESH );
			dMsg1.SetGUID( sGuid );
			dMsg1.SetTeamID( tid );
			dMsg1.SetGUIDEx( pSourUser->GetPlayerID() ); // 将玩家的 PlayerID 传回
			dMsg1.SetSceneID( pSourUser->GetSceneID() );
			dMsg1.SetName( pSourUser->GetName() );
			dMsg1.SetIcon( pSourUser->GetPortrait() );
			dMsg1.SetDataID( pSourUser->GetSex() );
			pDestServerPlayer->SendPacket( &dMsg1 );

			WGTeamResult dMsg2;
			dMsg2.SetPlayerID( pDestUser->GetPlayerID() );
			dMsg2.SetReturn( TEAM_RESULT_MEMBERENTERTEAM );
			dMsg2.SetGUID( dGuid );
			dMsg2.SetTeamID( tid );
			dMsg2.SetGUIDEx( pDestUser->GetPlayerID() ); // 将玩家的 PlayerID 传回
			dMsg2.SetSceneID( pDestUser->GetSceneID() );
			pDestServerPlayer->SendPacket( &dMsg2 );

			Log::SaveLog( WORLD_LOGFILE, "GWTeamRetInviteHandler...CreateTeam sGUID=%X dGUID=%X TeamID=%d", 
				sGuid, dGuid, tid );
		}

	}
	else
	{//邀请人有队伍, 被邀请人无队伍
		TeamID_t tid = pSourUser->GetTeamID();
		Team* pTeam = g_pTeamList->GetTeam( tid );
		Assert( pTeam );
		if( pTeam->IsFull() )
		{//队伍人数已经满了
			WGTeamError sMsg;
			sMsg.SetPlayerID( pSourUser->GetPlayerID() );
			sMsg.SetGUID( pDestUser->GetGUID() );
			sMsg.SetErrorCode( TEAM_ERROR_INVITETEAMFULL );
			pSourServerPlayer->SendPacket( &sMsg );

			WGTeamError dMsg;
			dMsg.SetPlayerID( pDestUser->GetPlayerID() );
			dMsg.SetGUID( pDestUser->GetGUID() );
			dMsg.SetErrorCode( TEAM_ERROR_APPLYTEAMFULL );
			pDestServerPlayer->SendPacket( &dMsg );
	
			Log::SaveLog( WORLD_LOGFILE, "GWTeamRetInviteHandler...Team Full sGUID=%X dGUID=%X TeamID=%d", 
				sGuid, dGuid, tid );
		}
		else
		{ //将玩家加入队伍
			TEAMMEMBER Member;
			Member.m_Member = dGuid;
			pTeam->AddMember( &Member );

			//设置玩家队伍信息
			pDestUser->SetTeamID( pTeam->GetTeamID() );
			WGTeamResult Msg1; // 发给新队员的
			Msg1.SetPlayerID( pDestUser->GetPlayerID() );
			Msg1.SetTeamID( tid );
			WGTeamResult Msg2; // 发给每个队员的
			Msg2.SetReturn( TEAM_RESULT_MEMBERENTERTEAM );
			Msg2.SetTeamID( tid );
			Msg2.SetGUID( dGuid );
			Msg2.SetGUIDEx( pDestUser->GetPlayerID() ); // 将玩家的 PlayerID 传回
			Msg2.SetSceneID( pDestUser->GetSceneID() );
			Msg2.SetName( pDestUser->GetName() );
			Msg2.SetIcon( pDestUser->GetPortrait() );
			Msg2.SetDataID( pDestUser->GetSex() );

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

				Msg2.SetPlayerID( pUser->GetPlayerID() );
				pServerPlayer->SendPacket( &Msg2 );

				if( pMember->m_Member != dGuid )
				{ // 将自己以外的玩家传给新玩家
					Msg1.SetReturn( TEAM_RESULT_TEAMREFRESH );
					Msg1.SetName( pUser->GetName() );
					Msg1.SetIcon( pUser->GetPortrait() );
					Msg1.SetDataID( pUser->GetSex() );

					Msg1.SetGUID( pMember->m_Member );
					Msg1.SetGUIDEx( pUser->GetPlayerID() ); // 将玩家的 PlayerID 传回
					Msg1.SetSceneID( pUser->GetSceneID() );

					pDestServerPlayer->SendPacket( &Msg1 );
				}

			}

			Log::SaveLog( WORLD_LOGFILE, "GWTeamRetInviteHandler...Success! sGUID=%X dGUID=%X TeamID=%d", 
				sGuid, dGuid, tid );
		}
	}

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}
