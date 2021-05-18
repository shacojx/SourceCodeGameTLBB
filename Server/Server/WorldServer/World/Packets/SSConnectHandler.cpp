#include "stdafx.h"


#include "SSConnect.h"
#include "Log.h"
#include "ServerPlayer.h"
#include "ServerManager.h"
#include "WGWorldTime.h"
#include "WorldTimeInfo.h"
#include "OnlineUser.h"



uint SSConnectHandler::Execute( SSConnect* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	Assert( pPlayer ) ;

	ServerPlayer* pServerPlayer = (ServerPlayer*)pPlayer ;
	
	ID_t ServerID = pPacket->GetServerID() ;
	Assert( ServerID != INVALID_ID ) ;

	int index = g_Config.m_ServerInfo.m_HashServer[ServerID] ;
	pServerPlayer->SetServerData( &(g_Config.m_ServerInfo.m_pServer[index]) ) ;

	g_pServerManager->m_aServerHash[ServerID] = pServerPlayer->PlayerID() ;

	pServerPlayer->SetPlayerStatus( PS_WORLD_NORMAL ) ;

	g_pOnlineUser->RemoveUser( ServerID, US_WAIT_SHUTDOWN );
	
	if(pServerPlayer->GetServerData()->m_Type == SERVER_GAME)
	{
		WGWorldTime	Msg;
		Msg.SetWorldTime(g_pWorldTimeInfo->GetWorldTime());
		pServerPlayer->SendPacket(&Msg);
	}
	Log::SaveLog( WORLD_LOGFILE, "SSConnectHandler...ServerID=%d", 
		ServerID ) ;

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
