#include "stdafx.h"

#include "BLRetAuth.h"
#include "LoginPlayer.h"
#include "ServerPlayer.h"
#include "PlayerPool.h"
#include "LoginPlayerManager.h"
#include "LCRetLogin.h"
#include "ServerManager.h"
#include "ProcessManager.h"
#include "ProcessPlayerManager.h"
#include "TurnPlayerQueue.h"
#include "Log.h"
#include "TimeManager.h"


#define MAX_WRONGPW_TIMES 3

UINT BLRetAuthHandler::Execute(BLRetAuth* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

	PlayerID_t PlayerID ;
	LoginPlayer* pLoginPlayer = NULL ;
	Assert( pPlayer ) ;
	if( pPlayer->IsServerPlayer() )
	{
		PlayerID = pPacket->GetPlayerID();

		pLoginPlayer = g_pPlayerPool->GetPlayer(PlayerID) ;
		if( pLoginPlayer==NULL )
		{
			Assert(FALSE) ;
			return PACKET_EXE_CONTINUE ;
		}
	}
	else if( pPlayer->IsLoginPlayer() )
	{
		PlayerID = pPlayer->PlayerID() ;
		pLoginPlayer = (LoginPlayer*)pPlayer ;
		
		if( pLoginPlayer==NULL )
		{
			Assert(FALSE) ;
			return PACKET_EXE_CONTINUE ;
		}
	}
	else
	{
		Assert(FALSE) ;
		return PACKET_EXE_CONTINUE ;
	}

	TID CurrentThreadID = MyGetCurrentThreadID();
	
	if(CurrentThreadID == g_pServerManager->m_ThreadID)
	{
		if(pLoginPlayer->GetPlayerStatus() == PS_LOGIN_WAIT_AUTH)
		{   //ServerManager 中执行
			g_pLoginPlayerManager->SendPacket(pPacket,PlayerID);
			return PACKET_EXE_NOTREMOVE; 
		}
		return PACKET_EXE_CONTINUE;
	}
	else if ( CurrentThreadID == g_pLoginPlayerManager->m_ThreadID)
	{

		//检查GUID 是否正确
		if(strcmp(pLoginPlayer->GetAccount(),pPacket->GetAccount())!= 0)
		{
			//应该是一次错误操作
			Log::SaveLog(LOGIN_LOGFILE, "ERROR: BLRetAuthHandler::Execute Errors,acc = %s,Packet acc = %s",
				pLoginPlayer->GetAccount(),pPacket->GetAccount()) ;
			return PACKET_EXE_CONTINUE;
		}

		if (pLoginPlayer->GetPlayerStatus() == PS_LOGIN_WAIT_AUTH)
		{
			if(pPacket->GetResult() == LOGINR_SUCCESS)
			{
				pLoginPlayer->SetAccount(pPacket->GetAccount());
				pLoginPlayer->SetPlayerStatus(PS_LOGIN_AUTHED);
				pLoginPlayer->SetPlayerAge( (BYTE)pPacket->GetAge() );
				//把客户端从当前LoginPlayerManager 中释放
				g_pLoginPlayerManager->DelPlayer(pLoginPlayer->PlayerID());
				//并且向ProcessManager 发送消息
				g_pProcessManager->SendPacket(pPacket,pLoginPlayer->PlayerID());
				//修改客户端状态为 PS_LOGIN_WAIT_PROCESS_TURN
				pLoginPlayer->SetPlayerStatus(PS_LOGIN_WAIT_PROCESS_TURN);
				return PACKET_EXE_NOTREMOVE ;
			}
			else
			{
				pLoginPlayer->SetAccount(pPacket->GetAccount());
				pLoginPlayer->SetWrongPWCount(pLoginPlayer->GetWrongPWCount()+1);
				LCRetLogin Msg;
				Msg.SetAccount(pLoginPlayer->GetAccount());
				Msg.SetResult(LOGINR_AUTH_FAIL);
				pLoginPlayer->SendPacket(&Msg);

				if(pLoginPlayer->GetWrongPWCount()>=MAX_WRONGPW_TIMES)
				{
					Log::SaveLog(LOGIN_LOGFILE, "ERROR: BLRetAuthHandler Reach Max Password Wrong Times acc=%s",
						pLoginPlayer->GetAccount()) ;
					return PACKET_EXE_ERROR;
				}
				else
					return PACKET_EXE_CONTINUE;
			}
			
		}
	}
	else if (CurrentThreadID == g_pProcessPlayerManager->m_ThreadID )
	{
		//检查GUID 是否正确
		if(strcmp(pLoginPlayer->GetAccount(),pPacket->GetAccount())!= 0)
		{
			//应该是一次错误操作
			Log::SaveLog(LOGIN_LOGFILE, "ERROR: BLRetAuthHandler::Execute Process Errors,acc = %s,Packet acc = %s",
				pLoginPlayer->GetAccount(),pPacket->GetAccount()) ;
			return PACKET_EXE_CONTINUE;
		}

		//加入到g_pProcessPlayerManager 中，能够处理对应的HeartBeat
		g_pProcessPlayerManager->AddPlayer(pLoginPlayer);
		//先处理login 结果
		LCRetLogin Msg;
		Msg.SetAccount(pLoginPlayer->GetAccount());
		Msg.SetResult(LOGINR_SUCCESS);
		pLoginPlayer->SendPacket(&Msg);
		UINT QueuePos;
		//加入到g_pProcessPlayerQueue 中，能够对客户端排队
		if(g_pProcessPlayerQueue->AddInPlayer(pLoginPlayer->PlayerID(),
			pLoginPlayer->GetAccount(),QueuePos))
		{
			pLoginPlayer->SetQueuePos(QueuePos);
			//设置当前玩家状态
			pLoginPlayer->SetPlayerStatus(PS_LOGIN_PROCESS_TURN);
			pLoginPlayer->SetLastSendTurnTime(g_pTimeManager->CurrentTime());	
			//发送开始排队消息(HeartBeat 中执行）
		}
		else
		{	//排队玩家都超过MAX_TURN_PLAYER个
			//所以只能断开此玩家的网络连接
			BOOL boo = pLoginPlayer->FreeOwn() ;
			Log::SaveLog( LOGIN_LOGFILE, "ERROR: BLRetAuthHandler::FreeOwn " ) ;
			Assert( boo ) ;
			return PACKET_EXE_ERROR ;

		}
	}
	
	Log::SaveLog( LOGIN_LOGFILE, "BLRetAuthHandler::Execute()....OK! " ) ;

	return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}