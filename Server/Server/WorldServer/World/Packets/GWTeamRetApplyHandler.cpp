#include "stdafx.h"


#include "GWTeamRetApply.h"
#include "Log.h"
#include "OnlineUser.h"
#include "ServerPlayer.h"
#include "WGTeamError.h"
#include "Team.h"
#include "ServerManager.h"
#include "WGTeamResult.h"



uint GWTeamRetApplyHandler::Execute( GWTeamRetApply* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	GUID_t sGuid = pPacket->GetSourGUID();//申请人的GUID
	GUID_t lGuid = pPacket->GetLeaderGUID();//队长的GUID

	USER* pLeaderUser = g_pOnlineUser->FindUser( lGuid );
	if( pLeaderUser==NULL )
	{
		Log::SaveLog( WORLD_LOGFILE, "GWTeamRetApplyHandler...User Leader GUID=%X not find!", 
			lGuid );

		return PACKET_EXE_CONTINUE;
	}

	ServerPlayer* pLeaderServerPlayer = (ServerPlayer*)pPlayer;

	USER* pSourUser = g_pOnlineUser->FindUser( sGuid );
	if( pSourUser==NULL )
	{
		if( pPacket->GetReturn()==TRUE )
		{
			WGTeamError dMsg;
			dMsg.SetPlayerID( pLeaderUser->GetPlayerID() );
			dMsg.SetGUID( pLeaderUser->GetGUID() );
			dMsg.SetErrorCode( TEAM_ERROR_TARGETNOTONLINE );
			pLeaderServerPlayer->SendPacket( &dMsg );
		}

		Log::SaveLog( WORLD_LOGFILE, "GWTeamRetApplyHandler...User Sour GUID=%X not find!", 
			sGuid );

		return PACKET_EXE_CONTINUE;
	}

	ID_t SourServerID = pSourUser->GetServerID();
	ServerPlayer* pSourServerPlayer = g_pServerManager->GetServerPlayer( SourServerID );
	if( pSourServerPlayer==NULL )
	{
		Assert(FALSE);
		return PACKET_EXE_CONTINUE;
	}

	if( pLeaderUser->IsEnemy( pSourUser->GetUserCampData() ) )
	{
		Log::SaveLog( WORLD_LOGFILE, "GWTeamRetApplyHandler...Lead Name=%s, Invite Name=%s are enemy!", 
			pLeaderUser->GetName(), pSourUser->GetName() );

		return PACKET_EXE_CONTINUE;
	}

	if( pPacket->GetReturn()==FALSE )
	{//队长不同意
		WGTeamError sMsg;
		sMsg.SetPlayerID( pSourUser->GetPlayerID() );
		sMsg.SetGUID( pLeaderUser->GetGUID() );
		sMsg.SetErrorCode( TEAM_ERROR_APPLYLEADERREFUSE );
		pSourServerPlayer->SendPacket( &sMsg );

		Log::SaveLog( WORLD_LOGFILE, "GWTeamRetApplyHandler...Leader lGUID=%X Refuse sGUID=%X join team!", 
			lGuid, sGuid );
	}
	else if( pSourUser->GetTeamID() != INVALID_ID )
	{//申请人已经属于某个队伍
		//WGTeamError sMsg;
		//sMsg.SetPlayerID( pSourUser->m_PlayerID );
		//sMsg.SetErrorCode( TEAM_ERROR_APPLYSOURHASTEAM );
		//pSourServerPlayer->SendPacket( &sMsg );

		WGTeamError lMsg;
		lMsg.SetPlayerID( pLeaderUser->GetPlayerID() );
		lMsg.SetGUID( pLeaderUser->GetGUID() );
		lMsg.SetErrorCode( TEAM_ERROR_APPLYSOURHASTEAM );
		pLeaderServerPlayer->SendPacket( &lMsg );

		Log::SaveLog( WORLD_LOGFILE, "GWTeamRetApplyHandler...Leader lGUID=%X, sGUID=%X has team!", 
			lGuid, sGuid );
	}
	else
	{//队伍存在
		TeamID_t tid = pLeaderUser->GetTeamID();
		Team* pTeam = g_pTeamList->GetTeam( tid );
		Assert( pTeam );

		if( pTeam->IsFull() )
		{//队伍人数已经满了
			WGTeamError sMsg;
			sMsg.SetPlayerID( pSourUser->GetPlayerID() );
			sMsg.SetGUID( pLeaderUser->GetGUID() );
			sMsg.SetErrorCode( TEAM_ERROR_APPLYTEAMFULL );
			pSourServerPlayer->SendPacket( &sMsg );

			WGTeamError lMsg;
			lMsg.SetPlayerID( pLeaderUser->GetPlayerID() );
			lMsg.SetGUID( pLeaderUser->GetGUID() );
			lMsg.SetErrorCode( TEAM_ERROR_TEAMFULL );
			pLeaderServerPlayer->SendPacket( &lMsg );
	
			Log::SaveLog( WORLD_LOGFILE, "GWTeamRetApplyHandler...Team Full  sGUID=%X lGUID=%X", 
				sGuid, lGuid );
		}
		else if( pTeam->Leader()->m_Member != lGuid )
		{//队长GUID和队伍中记录的队长GUID不符
			WGTeamError sMsg;
			sMsg.SetPlayerID( pSourUser->GetPlayerID() );
			sMsg.SetGUID( pLeaderUser->GetGUID() );
			sMsg.SetErrorCode( TEAM_ERROR_APPLYLEADERGUIDERROR );
			pSourServerPlayer->SendPacket( &sMsg );

			WGTeamError lMsg;
			lMsg.SetPlayerID( pLeaderUser->GetPlayerID() );
			lMsg.SetGUID( pLeaderUser->GetGUID() );
			lMsg.SetErrorCode( TEAM_ERROR_APPLYLEADERGUIDERROR );
			pLeaderServerPlayer->SendPacket( &lMsg );
	
			Log::SaveLog( WORLD_LOGFILE, "GWTeamRetApplyHandler...Team Full  sGUID=%X lGUID=%X", 
				sGuid, lGuid );
		}
		else 
		{//加入
			//将玩家加入队伍
			TEAMMEMBER Member;
			Member.m_Member = sGuid;
			pTeam->AddMember( &Member );

			//设置玩家队伍信息
			pSourUser->SetTeamID( pTeam->GetTeamID() );
			WGTeamResult Msg1; // 发给新队员的
			Msg1.SetPlayerID( pSourUser->GetPlayerID() );
			Msg1.SetTeamID( tid );
			WGTeamResult Msg2; // 发给每个队员的
			Msg2.SetReturn( TEAM_RESULT_MEMBERENTERTEAM );
			Msg2.SetTeamID( tid );
			Msg2.SetGUID( sGuid );
			Msg2.SetGUIDEx( pSourUser->GetPlayerID() ); // 将玩家的 PlayerID 传回
			Msg2.SetSceneID( pSourUser->GetSceneID() );
			Msg2.SetName( pSourUser->GetName() );
			Msg2.SetIcon( pSourUser->GetPortrait() );
			Msg2.SetDataID( pSourUser->GetSex() );

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
				{
					Assert(FALSE);
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

				if( pMember->m_Member != sGuid )
				{ // 将自己以外的玩家传给新玩家
					Msg1.SetReturn( TEAM_RESULT_TEAMREFRESH );
					Msg1.SetName( pUser->GetName() );
					Msg1.SetIcon( pUser->GetPortrait() );
					Msg1.SetDataID( pUser->GetSex() );

					Msg1.SetGUID( pMember->m_Member );
					Msg1.SetGUIDEx( pUser->GetPlayerID() ); // 将玩家的 PlayerID 传回
					Msg1.SetSceneID( pUser->GetSceneID() );

					pSourServerPlayer->SendPacket( &Msg1 );
				}
			}//end for

			Log::SaveLog( WORLD_LOGFILE, "GWTeamRetApplyHandler...Success!  sGUID=%X lGUID=%X", 
				sGuid, lGuid );
		}

	}




	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}
