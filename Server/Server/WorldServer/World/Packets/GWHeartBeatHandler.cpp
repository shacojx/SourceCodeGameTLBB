#include "stdafx.h"

#include "GWHeartBeat.h"
#include "Log.h"
#include "OnlineUser.h"
#include "ServerPlayer.h"
#include "ServerManager.h"
#include "WGNotifyUser.h"


uint GWHeartBeatHandler::Execute( GWHeartBeat* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

	ServerPlayer* pServerPlayer = (ServerPlayer*)pPlayer ;

	GUID_t sGUID = pPacket->GetGUID() ;

	USER* pUser = g_pOnlineUser->FindUser( sGUID ) ;
	if( pUser==NULL )
	{
		WGNotifyUser Msg ;
		Msg.SetGUID( sGUID ) ;
		Msg.SetStatus( WGNotifyUser::NUS_REMOVE ) ;

		pServerPlayer->SendPacket( &Msg ) ;
		
		Log::SaveLog( WORLD_LOGFILE, "GWHeartBeatHandler...User GUID=%X not find! Notify server!", 
			sGUID ) ;

		return PACKET_EXE_CONTINUE ;
	}

	pUser->ResetKickTime() ;

	Log::SaveLog( WORLD_LOGFILE, "GWHeartBeatHandler...User GUID=%X Status=%d!", 
		sGUID, pUser->UserStatus() ) ;
	
	return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
