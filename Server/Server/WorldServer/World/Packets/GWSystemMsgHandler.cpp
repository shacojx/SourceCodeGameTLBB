#include "stdafx.h"
#include "ServerManager.h"
#include "GWSystemMsg.h"
#include "WGSystemMsg.h"
#include "OnlineUser.h"
#include "PacketFactoryManager.h"

UINT GWSystemMsgHandler::Execute(GWSystemMsg* pPacket,Player *pPlayer) 
{
	__ENTER_FUNCTION

	ServerPlayer* pServerPlayer = (ServerPlayer*)pPlayer;

	GUID_t guid = pPacket->GetGUID();

	USER* pUser = g_pOnlineUser->FindUser( guid );
	if( pUser==NULL )
	{
		Log::SaveLog( WORLD_LOGFILE, "GWSystemMsgHandler...User GUID=%X not find!", 
			guid );

		return PACKET_EXE_CONTINUE;
	}

	BYTE nContexSize = pPacket->GetContexSize();
	if( nContexSize >= MAX_SYSTEM_MSG_SIZE ) nContexSize = MAX_SYSTEM_MSG_SIZE-1;

	WGSystemMsg Msg;
	Msg.SetGUID(guid);
	Msg.SetMessageType( pPacket->GetMessageType() );
	Msg.SetContexSize( nContexSize );
	Msg.SetContex( pPacket->GetContex() );
	g_pServerManager->BroadCastServer(&Msg);

	Log::SaveLog( WORLD_LOGFILE, "GWSystemMsgHandler...!  GUID=%X, MsgType=%d, ContexSize=%d, Contex=%s",
		guid, pPacket->GetMessageType(), nContexSize, pPacket->GetContex() );

	return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;


}
