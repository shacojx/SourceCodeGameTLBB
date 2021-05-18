#include "stdafx.h"


#include "GWTeamAppoint.h"
#include "Log.h"
#include "OnlineUser.h"
#include "ServerPlayer.h"
#include "WGTeamError.h"
#include "Team.h"
#include "ServerManager.h"

uint GWTeamAppointHandler::Execute( GWTeamAppoint* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	ServerPlayer* pSourServerPlayer = (ServerPlayer*)pPlayer;

	GUID_t sGuid = pPacket->GetSourGUID();
	GUID_t dGuid = pPacket->GetDestGUID();


	USER* pSourUser = g_pOnlineUser->FindUser( sGuid );
	if( pSourUser==NULL )
	{
		Log::SaveLog( WORLD_LOGFILE, "GWTeamAppointHandler...User Sour GUID=%X not find!", 
			sGuid );

		return PACKET_EXE_CONTINUE;
	}

	USER* pDestUser = g_pOnlineUser->FindUser( dGuid );
	if( pDestUser==NULL )
	{
		Log::SaveLog( WORLD_LOGFILE, "GWTeamAppointHandler...User Dest GUID=%X not find!", 
			dGuid );

		return PACKET_EXE_CONTINUE;
	}

	if( pSourUser->GetTeamID()==INVALID_ID )
	{//旧队长不在队伍中
		WGTeamError Msg;
		Msg.SetPlayerID( pSourUser->GetPlayerID() );
		Msg.SetGUID( pSourUser->GetGUID() );
		Msg.SetErrorCode(TEAM_ERROR_APPOINTSOURNOTEAM);
		pSourServerPlayer->SendPacket( &Msg );

		Log::SaveLog( WORLD_LOGFILE, "GWTeamAppointHandler...DestInTeam SourGUID=%X DestGUID=%X", 
			sGuid, dGuid );
	}
	else if( pDestUser->GetTeamID()==INVALID_ID )
	{//新队长不在队伍中
		WGTeamError Msg;
		Msg.SetPlayerID( pSourUser->GetPlayerID() );
		Msg.SetGUID( pSourUser->GetGUID() );
		Msg.SetErrorCode(TEAM_ERROR_APPOINTDESTNOTEAM);
		pSourServerPlayer->SendPacket( &Msg );

		Log::SaveLog( WORLD_LOGFILE, "GWTeamAppointHandler...DestInTeam SourGUID=%X DestGUID=%X", 
			sGuid, dGuid );
	}
	else if( pDestUser->GetTeamID()!=pSourUser->GetTeamID() )
	{//两个人不属于同一个队伍
		WGTeamError Msg;
		Msg.SetPlayerID( pSourUser->GetPlayerID() );
		Msg.SetGUID( pSourUser->GetGUID() );
		Msg.SetErrorCode(TEAM_ERROR_APPOINTNOTSAMETEAM);
		pSourServerPlayer->SendPacket( &Msg );

		Log::SaveLog( WORLD_LOGFILE, "GWTeamAppointHandler...DestInTeam SourGUID=%X DestGUID=%X", 
			sGuid, dGuid );
	}
	else
	{//队伍存在
		if( g_pOnlineUser->AppointTeamLeader(pSourUser, pDestUser) )
		{
			Log::SaveLog( WORLD_LOGFILE, "GWTeamAppointHandler...Success!  sGUID=%X dGUID=%X", 
				sGuid, dGuid );
		}
		else
		{
			Log::SaveLog( WORLD_LOGFILE, "GWTeamAppointHandler...Failed!  sGUID=%X dGUID=%X", 
				sGuid, dGuid );
		}
	}

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}
