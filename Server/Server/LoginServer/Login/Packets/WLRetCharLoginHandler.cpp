#include "stdafx.h"

#include "WLRetCharLogin.h"
#include "ProcessManager.h"
#include "ProcessPlayerManager.h"
#include "ServerManager.h"
#include "LoginPlayer.h"
#include "LCRetCharLogin.h"
#include "PlayerPool.h"
#include "TimeManager.h"
#include "DBLogicManager.h"
#include "DBCharFullData.h"
#include "DB_Struct.h"
#include "LWAskCharLogin.h"
#include "PacketFactoryManager.h"
#include "Log.h"
#include "DBThreadManager.h"


UINT WLRetCharLoginHandler::Execute(WLRetCharLogin* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION
	
		TID CurrentThreadID = MyGetCurrentThreadID();

		if(CurrentThreadID == g_pServerManager->m_ThreadID)
		{
			g_pProcessManager->SendPacket(pPacket,pPacket->GetPlayerID());

			return PACKET_EXE_NOTREMOVE;

		}
		else if(CurrentThreadID == g_pProcessPlayerManager->m_ThreadID) 
		{
			
			PlayerID_t		PlayerID   = pPacket->GetPlayerID();

			LoginPlayer* pLoginPlayer = g_pPlayerPool->GetPlayer(PlayerID);
			Assert(pLoginPlayer);

			if(strcmp(pLoginPlayer->GetAccount(),pPacket->GetAccount())!= 0)
			{
				Log::SaveLog(LOGIN_LOGFILE,"WLRetCharLoginHandler::Execute()....acc error,acc=%s,packacc=%s",
				pPacket->GetAccount(),pPacket->GetAccount());
				return PACKET_EXE_CONTINUE;
			}
			if(pPacket->GetHoldStatus() == TRUE) //用户在线
			{
				LCRetCharLogin Msg;
				Msg.SetResult(pPacket->GetResult());


				if(pPacket->GetResult() == ASKCHARLOGIN_SERVER_STOP)
				{
					pLoginPlayer->SendPacket(&Msg);

					Log::SaveLog(LOGIN_LOGFILE,"WLRetCharLoginHandler::Execute()....User is Online ,Server is CrashDown\
											   Account = %s,GUID = %d",pPacket->GetAccount(),pPacket->GetPlayerGUID());
					return PACKET_EXE_CONTINUE;
				}

				if(pPacket->GetResult() == ASKCHARLOGIN_SUCCESS)
				{
					//计算角色所在服务器信息
					ID_t	ServerID = pPacket->GetPlayerServerID();
					INT index = g_Config.m_ServerInfo.m_HashServer[ServerID] ;
					_SERVER_DATA* pServerData = 	&(g_Config.m_ServerInfo.m_pServer[index]);
					Assert(pServerData);
					Msg.SetServerIP(pServerData->m_IP0);
					Msg.SetServerPort(pServerData->m_Port0);
					//取得用户登陆Key值
					Msg.SetUserKey(pLoginPlayer->GetUserKey());
					pLoginPlayer->SendPacket(&Msg);
					pLoginPlayer->SetPlayerStatus(PS_LOGIN_SERVER_READY);
					Log::SaveLog(LOGIN_LOGFILE,"WLRetCharLoginHandler::Execute()...Online ,Acc=%s,GUID=%X,IP=%s,Port=%d,SID=%d",
						pPacket->GetAccount(),pPacket->GetPlayerGUID(),
						pServerData->m_IP0,pServerData->m_Port0,
						ServerID);
					return PACKET_EXE_CONTINUE;
				}
			}
			else //用户不在线 ,需要Load 数据
			{
				//对数据库操作频繁度判断
				UINT uTime = g_pTimeManager->CurrentTime();
				if(uTime<pLoginPlayer->m_LastDBOpTime+DB_OPERATION_TIME)
				{
					//用户操作过于频繁
					LCRetCharLogin Msg;
					Msg.SetResult(ASKCHARLOGIN_OP_TIMES);
					pLoginPlayer->SendPacket(&Msg);
					
					Log::SaveLog(LOGIN_LOGFILE,"WLRetCharLoginHandler::Execute()....User Need Load From DB  Account = %s,GUID = %X",pPacket->GetAccount(),pPacket->GetPlayerGUID());

					return PACKET_EXE_CONTINUE;
				}
				//
				if(pPacket->GetResult() == ASKCHARLIST_WORLD_FULL 
					|| pPacket->GetResult() == ASKCHARLOGIN_SERVER_STOP)
				{
					LCRetCharLogin Msg;
					Msg.SetResult(pPacket->GetResult());
					pLoginPlayer->SendPacket(&Msg);
					Log::SaveLog(LOGIN_LOGFILE,"WLRetCharLoginHandler::Execute()....User Can't Login  Account = %s,GUID = %X ,Result = %d",pPacket->GetAccount(),pPacket->GetPlayerGUID(),pPacket->GetResult());
					return PACKET_EXE_CONTINUE;

				}
				
				if (pPacket->GetResult() == ASKCHARLOGIN_LOADDB_ERROR)
				{
					if(g_pDBThreadManager->SendPacket(pPacket,pLoginPlayer->PlayerID()))
					{//加入成功，将消息发送到DB处理
						pLoginPlayer->m_LastDBOpTime = uTime;
						return PACKET_EXE_NOTREMOVE;	
					}
					else
					{
						//DB 压力过大，让用户重新尝试
						LCRetCharLogin Msg;
						Msg.SetResult(ASKCHARLOGIN_SERVER_BUSY);
						pLoginPlayer->SendPacket(&Msg);
						pLoginPlayer->m_LastDBOpTime = uTime;

						Log::SaveLog(LOGIN_LOGFILE,"WLRetCharLoginHandler::Execute()....Server Busy  Account = %s,GUID = %X",pPacket->GetAccount(),pPacket->GetPlayerGUID());

						return PACKET_EXE_CONTINUE;
					}
				}
			}

		}
		else if(g_pDBThreadManager->IsPoolTID(CurrentThreadID))
		{

			PlayerID_t	PlayerID			= pPacket->GetPlayerID();
			GUID_t	    PlayerCharGUID		= pPacket->GetPlayerGUID();

			LoginPlayer* pLoginPlayer = static_cast<LoginPlayer*>(pPlayer);
			Assert(pLoginPlayer);
			if(pLoginPlayer->GetDBOperating() == TRUE||!g_pDBThreadManager->GetInterface(CurrentThreadID)->IsConnected())
			{
				LCRetCharLogin* pRetCharLoginMsg = 
					(LCRetCharLogin*)g_pPacketFactoryManager->CreatePacket(PACKET_LC_RETCHARLOGIN);
				if(!pRetCharLoginMsg)
				{
					AssertEx(FALSE,"创建 LCRetCharLogin 消息失败");
				}
				pRetCharLoginMsg->SetResult(ASKCHARLOGIN_SERVER_BUSY);
				g_pProcessManager->SendPacket(pRetCharLoginMsg,PlayerID);
				Log::SaveLog( LOGIN_LOGFILE, "WLRetCharLoginHandler::Execute()....数据库操作冲突!") ;
				return PACKET_EXE_CONTINUE;
			}

			pLoginPlayer->SetDBOperating(TRUE);

			ODBCInterface*	pInterface = g_pDBThreadManager->GetInterface(CurrentThreadID);
			Assert(pInterface);

			DBCharFullData	CharFullDataObject(pInterface);
			CharFullDataObject.SetCharGuid(PlayerCharGUID);
			
			BOOL bRetLoad	 = CharFullDataObject.Load();
			INT	 ResultCount = CharFullDataObject.GetResultCount();
			if(!bRetLoad)
			{
				Log::SaveLog(LOGIN_LOGFILE,"CharFullDataObject.Load()....Get Errors: %s",CharFullDataObject.GetErrorMessage());
				
				LCRetCharLogin* pRetCharLoginMsg = 
					(LCRetCharLogin*)g_pPacketFactoryManager->CreatePacket(PACKET_LC_RETCHARLOGIN);

				if(!pRetCharLoginMsg)
				{
					AssertEx(FALSE,"创建 LCRetCharLogin 消息失败");
				}

				pRetCharLoginMsg->SetResult(ASKCHARLOGIN_LOADDB_ERROR);
				g_pProcessManager->SendPacket(pRetCharLoginMsg,PlayerID);
				pLoginPlayer->SetDBOperating(FALSE);


				return PACKET_EXE_NOTREMOVE;
                
			}

			if(ResultCount == 0)
			{
				Assert(FALSE);
				LCRetCharLogin* pRetCharLoginMsg = 
					(LCRetCharLogin*)g_pPacketFactoryManager->CreatePacket(PACKET_LC_RETCHARLOGIN);

				pRetCharLoginMsg->SetResult(ASKCHARLOGIN_LOADDB_ERROR);
				g_pProcessManager->SendPacket(pRetCharLoginMsg,PlayerID);
				pLoginPlayer->SetDBOperating(FALSE);
				return PACKET_EXE_NOTREMOVE;
			}

			LWAskCharLogin*		pMsg	=	
				(LWAskCharLogin*)g_pPacketFactoryManager->CreatePacket(PACKET_LW_ASKCHARLOGIN);
			if(!pMsg)
			{
				AssertEx(FALSE,"创建 LWAskCharLogin 消息失败");
			}

			UINT res1, res2, res3, res4;
			INT nRet = CharFullDataObject.ParseResult(pMsg->GetUserData(),res1, res2, res3, res4);
			if( nRet == 2 || nRet == 3 )
				Log::SaveLog(LOGIN_LOGFILE,"WLRetCharLoginHandler::Execute()....Get Errors:(Ret=%d) %d,%d,%d,%d", nRet, res1, res2, res3, res4);
			pMsg->GetUserData()->m_Human.m_LastLoginTime	= g_pTimeManager->CurrentDate();
			pMsg->SetAskStatus(ALS_SENDDATA);
			pMsg->SetAccount(pLoginPlayer->GetAccount());
			pMsg->SetPlayerID(PlayerID);
			pMsg->SetPlayerGUID(PlayerCharGUID);
			pMsg->SetUserKey(pLoginPlayer->GetUserKey());
			pMsg->SetUserAge(pLoginPlayer->GetPlayerAge());

			g_pServerManager->SendPacket(pMsg,WORLD_PLAYER_ID);
			pLoginPlayer->SetDBOperating(FALSE);

		}
		else 
		{
			AssertEx(FALSE,"WLRetCharLoginHandler 线程资源执行错误!");
		}

		Log::SaveLog(LOGIN_LOGFILE,"WLRetCharLoginHandler::Execute()....OK!");
	
		
		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;
}