#include "stdafx.h"


#include "GWTeamLeave.h"
#include "Log.h"
#include "OnlineUser.h"
#include "ServerPlayer.h"
#include "WGTeamError.h"
#include "Team.h"
#include "WGTeamResult.h"
#include "ServerManager.h"



uint GWTeamLeaveHandler::Execute( GWTeamLeave* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	ServerPlayer* pServerPlayer = (ServerPlayer*)pPlayer;

	GUID_t guid = pPacket->GetGUID();

	USER* pUser = g_pOnlineUser->FindUser( guid );
	if( pUser==NULL )
	{
		Log::SaveLog( WORLD_LOGFILE, "GWTeamLeaveHandler...User GUID=%X not find!", 
			guid );

		return PACKET_EXE_CONTINUE;
	}

	TeamID_t tid = pUser->GetTeamID();
	if (tid == INVALID_ID)
	{
		Log::SaveLog( WORLD_LOGFILE, "GWTeamLeaveHandler...User GUID=%X TeamID is ERROR Success!", 
			guid, tid );
	}

	if( g_pOnlineUser->UserLeaveTeam( pUser ) )
	{
		Log::SaveLog( WORLD_LOGFILE, "GWTeamLeaveHandler...User GUID=%X TeamID=%d Success!", 
			guid, tid );
	}
	else
	{
		Log::SaveLog( WORLD_LOGFILE, "GWTeamLeaveHandler...User GUID=%X TeamID=%d failed!", 
			guid, tid );
	}

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}
