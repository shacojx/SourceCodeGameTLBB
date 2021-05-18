#include "stdafx.h"
#include "LWAskDeleteChar.h"
#include "OnlineUser.h"
#include "ServerPlayer.h"
#include "Log.h"
#include "ServerManager.h"
#include "WLRetDeleteChar.h"

UINT LWAskDeleteCharHandler::Execute(LWAskDeleteChar* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION
	
	Assert(pPacket);
	
	ServerPlayer* pServerPlayer = (ServerPlayer*)pPlayer ;
	Assert(pServerPlayer);

	WLRetDeleteChar Msg;
	Msg.SetAccount( pPacket->GetAccount() );
	Msg.SetPlayerGUID( pPacket->GetPlayerGUID() );
	Msg.SetPlayerID( pPacket->GetPlayerID() );

	USER* pUser = g_pOnlineUser->FindUser( pPacket->GetPlayerGUID() );
	if( pUser )
	{
		Msg.SetResult( ASKDELETECHAR_ONLINE );
		Log::SaveLog( WORLD_LOGFILE, "LWAskDeleteCharHandler...Cannot Delete User, CharGUID = %X",pPacket->GetPlayerGUID()) ;
	}
	else
	{
		Msg.SetResult( ASKDELETECHAR_SUCCESS );
		Log::SaveLog( WORLD_LOGFILE, "LWAskDeleteCharHandler...Can Delete User, CharGUID = %X",pPacket->GetPlayerGUID()) ;
	}

	pServerPlayer->SendPacket(&Msg);


	return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}