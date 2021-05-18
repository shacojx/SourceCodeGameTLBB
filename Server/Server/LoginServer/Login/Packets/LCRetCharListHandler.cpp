#include "stdafx.h"

#include "LCRetCharList.h"
#include "LoginPlayer.h"
#include "ProcessPlayerManager.h"
#include "Log.h"

UINT LCRetCharListHandler::Execute(LCRetCharList* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

	if(MyGetCurrentThreadID() == g_pProcessPlayerManager->m_ThreadID)
	{
		LoginPlayer*	pLoginPlayer = (LoginPlayer*)pPlayer ;
		if( pLoginPlayer==NULL )
		{
			Assert(FALSE) ;
			return PACKET_EXE_CONTINUE ;
		}

		//检查GUID 是否正确
		if(strcmp(pLoginPlayer->GetAccount(),pPacket->GetAccount())!= 0)
		{
			Log::SaveLog(LOGIN_LOGFILE,"LCRetCharListHandler::Execute() ....Fails,packacc=%s,acc=%s",
				pPacket->GetAccount(),pLoginPlayer->GetAccount());
			return PACKET_EXE_CONTINUE;
		}

		pLoginPlayer->SendPacket(pPacket);
	}
	else
	{
		AssertEx(FALSE,"LCRetCharListHandler 线程资源执行错误!");
	}
		
	Log::SaveLog(LOGIN_LOGFILE,"LCRetCharListHandler::Execute() ....OK");

	return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}