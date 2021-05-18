#include "stdafx.h"

#include "GWChangeMenpai.h"
#include "Log.h"
#include "OnlineUser.h"

using namespace Packets;

uint GWChangeMenpaiHandler::Execute(GWChangeMenpai* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	USER* pUser = g_pOnlineUser->FindUser( pPacket->GetGUID() );
	if( pUser==NULL )
	{//没有发现在“在线用户列表”里对应此GUID的用户
		Log::SaveLog( WORLD_LOGFILE, "GWChangeMenpaiHandler...User GUID=%X not find!",
			pPacket->GetGUID() );

		Assert( FALSE && "找不到此人" );
		return PACKET_EXE_CONTINUE;
	}

	pUser->SetMenpai( pPacket->GetMenpai() );

	Log::SaveLog( WORLD_LOGFILE, "GWChangeMenpaiHandler...User GUID=%X Changed Menpai to Menpai=%d", 
		pPacket->GetGUID(), pPacket->GetMenpai() );

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}
