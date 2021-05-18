#include "stdafx.h"

#include "Log.h"
#include "OnlineUser.h"
#include "ServerPlayer.h"
#include "ServerManager.h"
#include "WGCallOfHuman.h"

#include "GWCallOfHuman.h"

uint GWCallOfHumanHandler::Execute( GWCallOfHuman* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	USER* pUser = g_pOnlineUser->FindUser( pPacket->GetCallOfTargetGUID() ) ;
	if( pUser==NULL )
	{
		Assert(FALSE) ;
		return PACKET_EXE_CONTINUE ;
	}

	ID_t ServerID = pUser->GetServerID() ;
	ServerPlayer* pServerPlayer = g_pServerManager->GetServerPlayer( ServerID ) ;
	if( pServerPlayer==NULL )
	{
		Assert(FALSE) ;
		return PACKET_EXE_CONTINUE ;
	}

	WGCallOfHuman msgCallOfHuman;
	msgCallOfHuman.SetCallOfTargetPlayerID(pUser->GetPlayerID());
	msgCallOfHuman.SetCallOfInfo(pPacket->GetCallOfInfo());
	msgCallOfHuman.SetGUID(pPacket->GetCallOfTargetGUID());
	pServerPlayer->SendPacket(&msgCallOfHuman);

	Log::SaveLog( WORLD_LOGFILE, "GWCallOfHumanHandler" );

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
