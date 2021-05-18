#include "stdafx.h"

#include "LBAskAuth.h"
#include "ServerManager.h"
#include "ServerPlayer.h"
#include "Log.h"

UINT	LBAskAuthHandler::Execute(LBAskAuth* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION
		
		TID CurrentThreadID = MyGetCurrentThreadID();
		
		if(CurrentThreadID == g_pServerManager->m_ThreadID)
		{
			Assert(pPacket);

			ServerPlayer* pServerPlayer  = (ServerPlayer*)pPlayer;

			Assert(pServerPlayer);

			pServerPlayer->SendPacket(pPacket);
		}
		else
		{
			AssertEx(FALSE,"LBAskAuthHandler 线程资源执行错误!");
		}
		
		Log::SaveLog(LOGIN_LOGFILE,"LBAskAuthHandler::Execute() ....OK");

		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;
}