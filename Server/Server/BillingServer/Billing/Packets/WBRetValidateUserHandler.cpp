#include "stdafx.h"
#include "WBRetValidateUser.h"
#include "BLRetAuth.h"
#include "ServerPlayer.h"
#include "ServerManager.h"
#include "Log.h"

UINT WBRetValidateUserHandler::Execute( WBRetValidateUser* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION
		BLRetAuth Msg;
	Msg.SetAccount(pPacket->GetAccount());
	Msg.SetPlayerID(pPacket->GetPID());
	Msg.SetResult(pPacket->GetFettle());
	ServerPlayer* pLogin = g_pServerManager->GetServerPlayer(pPacket->GetLoginID());

	pLogin->SendPacket(&Msg);

	return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
