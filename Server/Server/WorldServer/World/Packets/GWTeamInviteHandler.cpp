#include "stdafx.h"


#include "GWTeamInvite.h"
#include "Log.h"
#include "OnlineUser.h"
#include "ServerPlayer.h"
#include "ServerManager.h"

#include "Team.h"
#include "WGTeamError.h"
#include "WGTeamResult.h"
#include "WGTeamAskInvite.h"
#include "WGTeamLeaderAskInvite.h"



uint GWTeamInviteHandler::Execute( GWTeamInvite* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	ServerPlayer* pSourServerPlayer = (ServerPlayer*)pPlayer;

	GUID_t sGuid = pPacket->GetSourGUID();

	USER* pSourUser = g_pOnlineUser->FindUser( sGuid );
	if( pSourUser==NULL )
	{
		Log::SaveLog( WORLD_LOGFILE, "GWTeamInviteHandler...User Sour GUID=%X not find!", 
			sGuid );

		return PACKET_EXE_CONTINUE;
	}

	USER* pDestUser = g_pOnlineUser->FindUser( pPacket->GetDestName() );
	if( pDestUser==NULL )
	{
		Log::SaveLog( WORLD_LOGFILE, "GWTeamInviteHandler...User Dest Name=%s not find!", 
			pPacket->GetDestName() );

		return PACKET_EXE_CONTINUE;
	}

	GUID_t dGuid = pDestUser->GetGUID();

	if( pSourUser->IsEnemy( pDestUser->GetUserCampData() ) )
	{
		Log::SaveLog( WORLD_LOGFILE, "GWTeamInviteHandler...Sour Name=%s, Dest Name=%s are enemy!", 
			pSourUser->GetName(), pDestUser->GetName() );

		return PACKET_EXE_CONTINUE;
	}

	// 设置了拒绝被邀请
	if( pDestUser->IsRefuseTeamInvite() )
	{
		WGTeamError Msg;
		Msg.SetPlayerID( pSourUser->GetPlayerID() );
		Msg.SetGUID(sGuid);
		Msg.SetErrorCode(TEAM_ERROR_REFUSEINVITESETTING);
		pSourServerPlayer->SendPacket( &Msg );

		Log::SaveLog( WORLD_LOGFILE, "GWTeamInviteHandler...DestGUID=%X refuse to be invited. ", 
			dGuid );
		return PACKET_EXE_CONTINUE;
	}

	if( pDestUser->GetTeamID() != INVALID_ID )
	{//被邀请人已经属于某个队伍了
		WGTeamError Msg;
		Msg.SetPlayerID( pSourUser->GetPlayerID() );
		Msg.SetGUID(sGuid);
		Msg.SetErrorCode(TEAM_ERROR_INVITEDESTHASTEAM);
		pSourServerPlayer->SendPacket( &Msg );

		Log::SaveLog( WORLD_LOGFILE, "GWTeamInviteHandler...DestInTeam SourGUID=%X DestGUID=%X", 
			sGuid, dGuid );
	}
	else if( sGuid==dGuid && pSourUser->GetTeamID()==INVALID_ID )
	{//邀请人和被邀请人属于同一个人, 且不属于某个队伍
		//自建队伍
		TeamID_t tid = g_pTeamList->CreateTeam( );
		Assert( tid!=INVALID_ID );
		Team* pTeam = g_pTeamList->GetTeam( tid );
		Assert( pTeam );

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
		Msg.SetSceneID( pSourUser->GetSceneID() );
		Msg.SetDataID( pSourUser->GetSex() );
		Msg.SetIcon( pSourUser->GetPortrait() );

		pSourServerPlayer->SendPacket( &Msg );

		Log::SaveLog( WORLD_LOGFILE, "GWTeamInviteHandler...CreateTeam GUID=%X TeamID=%d", 
			sGuid, tid );
	}
	else
	{
		if ( pSourUser->GetTeamID() != INVALID_ID )
		{
			//邀请人已经有一个队伍
			Team* pTeam = g_pTeamList->GetTeam( pSourUser->GetTeamID() );
			Assert( pTeam );

			GUID_t tlGUID = pTeam->Leader()->m_Member; // 队长GUID

			//邀请人不是队长，要给队长发送请求
			if ( tlGUID != sGuid )
			{ // 不是队长的情况
				USER* pLeaderUser = g_pOnlineUser->FindUser( tlGUID );
				if( pLeaderUser == NULL )
				{
					Assert(FALSE);
				}

				ID_t LeaderServerID = pLeaderUser->GetServerID();
				ServerPlayer* pLeaderServerPlayer = g_pServerManager->GetServerPlayer( LeaderServerID );
				if( pLeaderServerPlayer==NULL )
				{
					Assert(FALSE);
				}

				WGTeamLeaderAskInvite Msg;
				Msg.SetPlayerID( pLeaderUser->GetPlayerID() );//队长的Player_t
				Msg.SetSourGUID( sGuid );		//邀请人
				Msg.SetDestGUID( dGuid );		//被要请人
				Msg.SetSourName( pSourUser->GetName() );
				Msg.SetDestName( pDestUser->GetName() );
				Msg.SetGUID(tlGUID);

				pLeaderServerPlayer->SendPacket( &Msg );

				Log::SaveLog( WORLD_LOGFILE, "GWTeamInviteHandler...TeamLeaderAskInvite SourGUID=%X DestGUID=%X", 
					sGuid, dGuid );
				return PACKET_EXE_CONTINUE;
			}
		}

		ID_t DestServerID = pDestUser->GetServerID();
		ServerPlayer* pDestServerPlayer = g_pServerManager->GetServerPlayer( DestServerID );
		if( pDestServerPlayer )
		{//向被邀请人发送邀请消息
			WGTeamAskInvite Msg;
			Msg.SetPlayerID( pDestUser->GetPlayerID() );
			Msg.SetdGUID( dGuid );
			Msg.SetsGUID( sGuid );

			if ( pSourUser->GetTeamID() != INVALID_ID )
			{
				Team* pTeam = g_pTeamList->GetTeam( pSourUser->GetTeamID() );
				Assert( pTeam );

			//	Msg.SetMemberCount( pTeam->MemberCount() );

				for( INT i=0; i<pTeam->MemberCount(); ++i )
				{
					WGTeamAskInvite::InviterInfo info;
					TEAMMEMBER* pMember = pTeam->Member( i );
					if( pMember == NULL )
					{
						Assert(FALSE);
						continue ;
					}

					USER* pUser = g_pOnlineUser->FindUser( pMember->m_Member );
					if( pUser == NULL )
					{//如果队员离线,则用户数据是空
						continue ;
					}

					info.m_PlayerID = pUser->GetPlayerID();
					info.m_NickSize = (UCHAR)strlen(pUser->GetName());
					strncpy( (CHAR*)info.m_szNick, pUser->GetName(), info.m_NickSize );
					info.m_uFamily = pUser->GetMenpai();
					info.m_Scene = pUser->GetSceneID();
					info.m_Level = pUser->GetLevel();
					info.m_uDataID = pUser->GetSex();

					Msg.AddInviterInfo( info );
				}
			}
			else
			{
			//	Msg.SetMemberCount( 1 );
				WGTeamAskInvite::InviterInfo info;
				info.m_PlayerID = pSourUser->GetPlayerID();
				info.m_NickSize = (UCHAR)strlen(pSourUser->GetName());
				strncpy( (CHAR*)info.m_szNick, pSourUser->GetName(), info.m_NickSize );
				info.m_uFamily = pSourUser->GetMenpai();
				info.m_Scene = pSourUser->GetSceneID();
				info.m_Level = pSourUser->GetLevel();
				info.m_uDataID = pSourUser->GetSex();

				Msg.AddInviterInfo( info );
			}

			pDestServerPlayer->SendPacket( &Msg );
		}
		else
		{
			Assert(FALSE);
		}

		Log::SaveLog( WORLD_LOGFILE, "GWTeamInviteHandler...AskInvite SourGUID=%X DestGUID=%X", 
			sGuid, dGuid );

	}


	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}
