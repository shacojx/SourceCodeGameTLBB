#include "stdafx.h"

#include "CLConnect.h"
#include "LCRetConnect.h"
#include "LoginPlayerManager.h"
#include "ServerManager.h"
#include "LoginPlayer.h"
#include "LCRetConnect.h"
#include "Log.h"

UINT CLConnectHandler::Execute(CLConnect* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		//检查线程执行资源是否正确
		if(MyGetCurrentThreadID()!= g_pLoginPlayerManager->m_ThreadID)
		{
			return PACKET_EXE_CONTINUE;
		}

		LoginPlayer* pLoginPlayer = static_cast<LoginPlayer*>(pPlayer);

		Assert(pLoginPlayer);
		
		//检查Login 服务器繁忙程度，返回一个相对负载小的IP 和Port
		
		_SERVER_DATA* pLoginInfo	= g_pServerManager->GetCurrentServerInfo();
		Assert(pLoginInfo);

		LCRetConnect	Msg;
		Msg.SetResult(LOGINCR_SUCCESS);
		Msg.SetLoginIP(pLoginInfo->m_IP0);
		Msg.SetLoginPort(pLoginInfo->m_Port0);
		pLoginPlayer->SendPacket(&Msg);
		Log::SaveLog( LOGIN_LOGFILE, "CLConnectHandler::Execute(...)...OK,IP = %s,Port = %d",Msg.GetLoginIP(),Msg.GetLoginPort()) ;

		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;

}