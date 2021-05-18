#include "stdafx.h"


#include "GWChannelDismiss.h"
#include "Log.h"
#include "OnlineUser.h"
#include "ServerPlayer.h"
#include "ServerManager.h"

#include "ChatCenter.h"
#include "WGChannelError.h"
#include "WGChannelResult.h"




uint GWChannelDismissHandler::Execute( GWChannelDismiss* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	ServerPlayer* pServerPlayer = (ServerPlayer*)pPlayer ;

	GUID_t Guid = pPacket->GetGUID() ;

	USER* pUser = g_pOnlineUser->FindUser( Guid ) ;
	if( pUser==NULL )
	{
		Log::SaveLog( WORLD_LOGFILE, "GWChannelDismissHandler...User GUID=%X not find!", 
			Guid ) ;

		return PACKET_EXE_CONTINUE ;
	}

	if( pUser->GetChannelID() == INVALID_ID )
	{
		Log::SaveLog( WORLD_LOGFILE, "GWChannelDismissHandler...User GUID=%X not channel!", 
			Guid ) ;

		return PACKET_EXE_CONTINUE ;
	}

	ChatChannel* pChannel = g_pChatCenter->GetChatChannel( pUser->GetChannelID() ) ;
	if( pChannel==NULL )
	{
		Log::SaveLog( WORLD_LOGFILE, "GWChannelDismissHandler...User GUID=%X cannot find channel!", 
			Guid ) ;

		return PACKET_EXE_CONTINUE ;
	}

	WGChannelResult Msg ;
	Msg.SetReturn( CHANNEL_RESULT_DISMISS ) ;
	Msg.SetChannelID( pUser->GetChannelID() ) ;

    //通知所有频道内的玩家
	for( int i=0; i<pChannel->MemberCount(); i++ )
	{
		GUID_t guid = pChannel->Member( i ) ;
		if( guid==INVALID_ID )
		{
			Assert(FALSE) ;
			continue ;
		}

		USER* pUser = g_pOnlineUser->FindUser( guid ) ;
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
	}

	//清除自建聊天频道信息
	g_pChatCenter->DestoryChatChannel( pUser->GetChannelID() ) ;

	//清除用户数据
	pUser->SetChannelID( INVALID_ID );

	Log::SaveLog( WORLD_LOGFILE, "GWChannelDismissHandler...User GUID=%X ChatChannel Dismiss!", 
		Guid ) ;

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
