#include "stdafx.h"


#include "GWTeamDismiss.h"
#include "Log.h"
#include "OnlineUser.h"
#include "ServerPlayer.h"
#include "WGTeamError.h"
#include "Team.h"
#include "WGTeamResult.h"
#include "ServerManager.h"



uint GWTeamDismissHandler::Execute( GWTeamDismiss* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	ServerPlayer* pServerPlayer = (ServerPlayer*)pPlayer ;

	GUID_t guid = pPacket->GetGUID() ;//解散队伍的人

	USER* pUser = g_pOnlineUser->FindUser( guid ) ;
	if( pUser==NULL )
	{
		Log::SaveLog( WORLD_LOGFILE, "GWTeamDismissHandler...User GUID=%X not find!", 
			guid ) ;

		return PACKET_EXE_CONTINUE ;
	}

	TeamID_t tid = pUser->GetTeamID() ;
	Team* pTeam = g_pTeamList->GetTeam( tid ) ;
	if( pTeam==NULL )
	{
		Log::SaveLog( WORLD_LOGFILE, "GWTeamDismissHandler...User GUID=%X TeamID=%d not find team!", 
			guid, tid ) ;

		return PACKET_EXE_CONTINUE ;
	}

	if( pTeam->Leader()->m_Member != guid )
	{//解散队伍的人不是队长
		WGTeamError Msg ;
		Msg.SetPlayerID( pUser->GetPlayerID() ) ;
		Msg.SetGUID(pPacket->GetGUID());
		Msg.SetErrorCode( TEAM_ERROR_DISMISSNOTLEADER ) ;

		pServerPlayer->SendPacket( &Msg ) ;

		Log::SaveLog( WORLD_LOGFILE, "GWTeamDismissHandler...User GUID=%X TeamID=%d not leader!", 
			guid, tid ) ;

		return PACKET_EXE_CONTINUE ;
	}

	WGTeamResult Msg ;
	Msg.SetReturn( TEAM_RESULT_TEAMDISMISS ) ;
	Msg.SetTeamID( tid ) ;

	//通知所有组内的玩家
	for( int i=0; i<pTeam->MemberCount(); i++ )
	{
		TEAMMEMBER* pMember = pTeam->Member( i ) ;
		if( pMember==NULL )
		{
			Assert(FALSE) ;
			continue ;
		}

		USER* pUser = g_pOnlineUser->FindUser( pMember->m_Member ) ;
		if( pUser==NULL )
		{//如果队员离线,则用户数据是空
			continue ;
		}
		
		ID_t ServerID = pUser->GetServerID() ;
		ServerPlayer* pServerPlayer = g_pServerManager->GetServerPlayer( ServerID ) ;
		if( pServerPlayer==NULL )
		{
			Assert(FALSE) ;
			continue ;
		}

		Msg.SetPlayerID( pUser->GetPlayerID() ) ;

		pServerPlayer->SendPacket( &Msg ) ;

		//设置用户队伍信息
		pUser->SetTeamID( INVALID_ID );
	}
	
	//清除队伍信息
	g_pTeamList->DestoryTeam( tid );

	Log::SaveLog( WORLD_LOGFILE, "GWTeamDismissHandler...User GUID=%X TeamID=%d Success!", 
		guid, tid ) ;

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
