#include "stdafx.h"
#include "SSConnect.h"
#include "ServerPlayer.h"
#include "ServerManager.h"
#include "Log.h"



uint SSConnectHandler::Execute( SSConnect* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	Log::SaveLog( SERVER_LOGFILE, "SSConnectHandler::Execute(...) ServerID:%d ...OK ", pPacket->GetServerID() ) ;

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
