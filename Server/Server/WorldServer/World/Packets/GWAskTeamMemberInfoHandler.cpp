#include "stdafx.h"
// GWAskTeamMemberInfoHandler.cpp

#include "GWAskTeamMemberInfo.h"
#include "WGTeamMemberInfo.h"

#include "ServerPlayer.h"
#include "OnlineUser.h"
#include "Log.h"

uint GWAskTeamMemberInfoHandler::Execute( GWAskTeamMemberInfo* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	ServerPlayer* pServerPlayer = (ServerPlayer*)pPlayer;
	GUID_t guid = pPacket->GetGUID(); // 被请求信息的玩家的 GUID
	WGTeamMemberInfo Msg;

	Msg.SetPlayerID( pPacket->GetObjID() );
	Msg.SetGUID( guid );

	USER* pUser = g_pOnlineUser->FindUser( guid );
	if( pUser == NULL )
	{ // 离线状态
		Log::SaveLog( WORLD_LOGFILE, "GWAskTeamInfoHandler...User GUID=%X not find!", guid );
		return PACKET_EXE_CONTINUE;
	}

	Msg.SetFamily( pUser->GetMenpai() );
	Msg.SetLevel( pUser->GetLevel() );

	if( pUser->UserStatus() != US_NORMAL )
	{
		Msg.SetDeadLinkFlag( TRUE );
	}
	else
	{
		Msg.SetDeadLinkFlag( FALSE );
	}

	pServerPlayer->SendPacket( &Msg );
	Log::SaveLog( WORLD_LOGFILE, "GWAskTeamInfoHandler...User GUID=%X was find!", guid );

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}
