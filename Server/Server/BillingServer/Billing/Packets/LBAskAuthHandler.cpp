#include "stdafx.h"
#include "LBAskAuth.h"
#include "BLRetAuth.h"
#include "ServerPlayer.h"
#include "WebPlayer.h"
#include "UserDBManager.h"
#include "BWValidateUser.h"
#include "Log.h"

UINT LBAskAuthHandler::Execute(LBAskAuth* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION
		
		Assert(pPlayer);
		Assert(pPacket);

		//无认证
		BLRetAuth Msg;
		Msg.SetAccount(pPacket->GetAccount());
		Msg.SetPlayerID(pPacket->GetPlayerID());
		Msg.SetResult(LOGINR_SUCCESS);
		Msg.SetAge(20);
		((ServerPlayer*)pPlayer)->SendPacket(&Msg);
		//无认证

		//Web认证
		//BWValidateUser BWMsg;
		//BWMsg.SetUserName(pPacket->GetAccount());
		//BWMsg.SetPassword(pPacket->GetPassWord());
		//BWMsg.SetPID(pPacket->GetPlayerID());
		//BWMsg.SetLoginID(((ServerPlayer*)pPlayer)->PlayerManagerID());
		//Msg.SetAge(pPacket->getAge());
		//g_pWebPlayer->SendPacket(&BWMsg);
		//Web认证
		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;
}









//
//
//
//BLRetAuth	Msg;
//
//Msg.SetAccount(pPacket->GetAccount());
//Msg.SetPlayerID(pPacket->GetPlayerID());
//
//
//{
//	Msg.SetResult(LOGINR_SUCCESS);
//	Log::SaveLog("./Log/Billing.log","Account = %s Check Password SUCCESS!",pPacket->GetAccount());
//
//}
//		else
//		{
//			Msg.SetResult(LOGINR_AUTH_FAIL);
//			Log::SaveLog("./Log/Billing.log","Account = %s Check Password FAIL!",pPacket->GetAccount());
//		}
//
//
//		((ServerPlayer*)pPlayer)->SendPacket(&Msg);
