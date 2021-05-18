#include "stdafx.h"

#include "LWAskDeleteChar.h"
#include "ServerPlayer.h"
#include "ServerManager.h"
#include "Log.h"


UINT LWAskDeleteCharHandler::Execute(LWAskDeleteChar* pPacket, Player* pPlayer )
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
			AssertEx(FALSE,"LWAskDeleteCharHandler 线程资源执行错误!");
		}
		Log::SaveLog(LOGIN_LOGFILE,"LWAskDeleteCharHandler::Execute() ....OK");

		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;
}