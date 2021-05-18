#include "stdafx.h"


#include "GWTeamApply.h"
#include "Log.h"
#include "OnlineUser.h"
#include "ServerPlayer.h"
#include "WGTeamError.h"
#include "Team.h"
#include "WGTeamAskApply.h"
#include "ServerManager.h"

uint GWTeamApplyHandler::Execute( GWTeamApply* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	ServerPlayer* pSourServerPlayer = (ServerPlayer*)pPlayer;

	GUID_t sGuid = pPacket->GetSourGUID();

	USER* pSourUser = g_pOnlineUser->FindUser( sGuid );
	if( pSourUser==NULL )
	{
		Log::SaveLog( WORLD_LOGFILE, "GWTeamApplyHandler...User Sour GUID=%X not find!", 
			sGuid );

		return PACKET_EXE_CONTINUE;
	}

	USER* pDestUser = g_pOnlineUser->FindUser( pPacket->GetDestName() );
	if( pDestUser==NULL )
	{
		Log::SaveLog( WORLD_LOGFILE, "GWTeamApplyHandler...User DestName=%s not find!", 
			pPacket->GetDestName() );

		return PACKET_EXE_CONTINUE;
	}

	GUID_t dGuid = pDestUser->GetGUID();

	if( pSourUser->GetTeamID() != INVALID_ID )
	{//申请人已经属于某个队伍了
		WGTeamError Msg;
		Msg.SetPlayerID( pSourUser->GetPlayerID() );
		Msg.SetGUID(pPacket->GetGUID());
		Msg.SetErrorCode(TEAM_ERROR_APPLYWHENINTEAM);
		pSourServerPlayer->SendPacket( &Msg );

		Log::SaveLog( WORLD_LOGFILE, "GWTeamApplyHandler...DestInTeam sGUID=%X dGUID=%X", 
			sGuid, dGuid );

		return PACKET_EXE_CONTINUE;
	}
	else if( pDestUser->GetTeamID() == INVALID_ID )
	{//被申请者不属于某个组
		WGTeamError Msg;
		Msg.SetPlayerID( pSourUser->GetPlayerID() );
		Msg.SetGUID(pPacket->GetGUID());
		Msg.SetErrorCode(TEAM_ERROR_APPLYDESTHASNOTTEAM);
		pSourServerPlayer->SendPacket( &Msg );

		Log::SaveLog( WORLD_LOGFILE, "GWTeamApplyHandler...DestInTeam sGUID=%X dGUID=%X", 
			sGuid, dGuid );

		return PACKET_EXE_CONTINUE;
	}

	TeamID_t tid = pDestUser->GetTeamID();
	Team* pTeam = g_pTeamList->GetTeam( tid );
	if( pTeam==NULL )
	{
		Assert(FALSE);
		return PACKET_EXE_CONTINUE;
	}

	USER* pUser = g_pOnlineUser->FindUser( pTeam->Leader()->m_Member );
	if( pUser==NULL )
	{//异常, 队长不能是离线玩家
		Assert(FALSE);
		return PACKET_EXE_CONTINUE;
	}

	if ( pUser->UserStatus() != US_NORMAL )
	{
		WGTeamError Msg;
		Msg.SetPlayerID( pSourUser->GetPlayerID() );
		Msg.SetGUID(pPacket->GetGUID());
		Msg.SetErrorCode( TEAM_ERROR_APPLYLEADERCANTANSWER );
		pSourServerPlayer->SendPacket( &Msg );

		Log::SaveLog( WORLD_LOGFILE, "GWTeamApplyHandler...Leader not on line sGUID=%X dGUID=%X", 
			sGuid, dGuid );
		return PACKET_EXE_CONTINUE;
	}
		
	ID_t ServerID = pUser->GetServerID();
	ServerPlayer* pServerPlayer = g_pServerManager->GetServerPlayer( ServerID );
	if( pServerPlayer==NULL )
	{
		Assert(FALSE);
		return PACKET_EXE_CONTINUE;
	}

	WGTeamAskApply Msg;
	Msg.SetPlayerID( pUser->GetPlayerID() );
	Msg.SetSourPlayerID( pSourUser->GetPlayerID() );
	Msg.SetSourGUID( sGuid );
	Msg.SetDestGUID( dGuid );
	Msg.SetSourName( pSourUser->GetName() );
	Msg.SetDestName( pDestUser->GetName() );
	Msg.SetFamily( pSourUser->GetMenpai() );
	Msg.SetScene( pSourUser->GetSceneID() );
	Msg.SetLevel( pSourUser->GetLevel() );
	Msg.SetSex( pSourUser->GetSex() );
	Msg.SetGUID( pUser->GetGUID() );

	pServerPlayer->SendPacket( &Msg );

	Log::SaveLog( WORLD_LOGFILE, "GWTeamApplyHandler...sGUID=%X dGUID=%X Success!", 
		sGuid, dGuid );

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}
