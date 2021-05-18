#include "stdafx.h"

#include "GWUpdateTitle.h"
#include "Log.h"
#include "OnlineUser.h"

using namespace Packets;

uint GWUpdateTitleHandler::Execute(GWUpdateTitle* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	USER* pUser = g_pOnlineUser->FindUser( pPacket->GetGUID() );
	if( pUser==NULL )
	{//没有发现在“在线用户列表”里对应此GUID的用户
		Log::SaveLog( WORLD_LOGFILE, "GWUpdateTitleHandler...User GUID=%X not find!",
			pPacket->GetGUID() );

		Assert( FALSE && "找不到此人" );
		return PACKET_EXE_CONTINUE;
	}

	pUser->SetTitle( pPacket->GetTitle() );

	Log::SaveLog( WORLD_LOGFILE, "GWUpdateTitleHandler...User GUID=%X Changed Title to Title=%s", 
		pPacket->GetGUID(), pPacket->GetTitle() );

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}
