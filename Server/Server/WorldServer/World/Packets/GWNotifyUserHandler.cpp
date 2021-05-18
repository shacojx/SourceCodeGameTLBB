#include "stdafx.h"


#include "GWNotifyUser.h"
#include "Log.h"
#include "OnlineUser.h"
#include "ServerPlayer.h"




uint GWNotifyUserHandler::Execute( GWNotifyUser* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	ServerPlayer* pServerPlayer = (ServerPlayer*)pPlayer ;

	USER* pUser = g_pOnlineUser->FindUser( pPacket->GetGUID() ) ;
	if( pUser==NULL )
	{
		Log::SaveLog( WORLD_LOGFILE, "GWNotifyUserHandler...User GUID=%X not find!", 
			pPacket->GetGUID(), pPacket->GetStatus() ) ;

		return PACKET_EXE_CONTINUE ;
	}

	if( pPacket->GetStatus()==GWNotifyUser::NUS_NEED_WORLD_KICK )
	{
		g_pOnlineUser->OnUserRemove( pUser );
		g_pOnlineUser->DelUser( pUser ) ;
		Log::SaveLog( WORLD_LOGFILE, "GWNotifyUserHandler...KICK User GUID=%X Status=%d SP=%d DP=%d", 
			pPacket->GetGUID(), pPacket->GetStatus(), pUser->GetPlayerID(), pPacket->GetPlayerID() ) ;
	}

	if( pUser->GetPlayerID() != pPacket->GetPlayerID() )
	{
		Log::SaveLog( WORLD_LOGFILE, "ERROR: GWNotifyUserHandler...User GUID=%X Status=%d SP=%d DP=%d", 
			pPacket->GetGUID(), pPacket->GetStatus(), pUser->GetPlayerID(), pPacket->GetPlayerID() ) ;

		return PACKET_EXE_CONTINUE ;
	}

	if( pPacket->GetStatus()==GWNotifyUser::NUS_DISCONNECT )
	{
		pUser->SetUserStatus( US_OFFLINE );
		pUser->ResetKickTime();
		g_pOnlineUser->OnUserOffLine( pUser );
		Log::SaveLog( WORLD_LOGFILE, "GWNotifyUserHandler...User Is Disconnect GUID=%X Status=%d SP=%d DP=%d", 
			pPacket->GetGUID(), pPacket->GetStatus(), pUser->GetPlayerID(), pPacket->GetPlayerID() ) ;
	}
	else if( pPacket->GetStatus()==GWNotifyUser::NUS_REMOVE )
	{
		g_pOnlineUser->OnUserRemove( pUser );
		g_pOnlineUser->DelUser( pUser ) ;
		Log::SaveLog( WORLD_LOGFILE, "GWNotifyUserHandler...User Is Remove GUID=%X Status=%d SP=%d DP=%d", 
			pPacket->GetGUID(), pPacket->GetStatus(), pUser->GetPlayerID(), pPacket->GetPlayerID() ) ;
	}

	Log::SaveLog( WORLD_LOGFILE, "GWNotifyUserHandler...User GUID=%X Status=%d", 
		pPacket->GetGUID(), pPacket->GetStatus() ) ;

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
