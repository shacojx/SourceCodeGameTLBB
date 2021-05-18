#include "stdafx.h"

#include "LCRetCreateChar.h"
#include "LoginPlayer.h"
#include "ProcessPlayerManager.h"
#include "Log.h"

UINT	LCRetCreateCharHandler::Execute(LCRetCreateChar* pPacket, Player* pPlayer )
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

			//检查账号是否正确
			if(strcmp(pLoginPlayer->GetAccount(),pPacket->GetAccount()) != 0)
			{
				
				Log::SaveLog(LOGIN_LOGFILE,"LCRetCreateCharHandler::Execute() ....OK,acc=%s,packacc",
					pLoginPlayer->GetAccount(),pPacket->GetAccount());
				return PACKET_EXE_CONTINUE;
			}

			pLoginPlayer->SendPacket(pPacket);

			Log::SaveLog(LOGIN_LOGFILE,"LCRetCreateCharHandler::Execute() ....OK");
		}
		else
		{
			AssertEx(FALSE,"LCRetCreateCharHandler 线程资源执行错误!");
		}

		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;
}