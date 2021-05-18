#include "stdafx.h"

#include "WLRetDeleteChar.h"
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
#include "LCRetCharList.h"
#include "LCRetDeleteChar.h"
#include "DBDeleteCharOp.h"
#include "DBCharList.h"


UINT WLRetDeleteCharHandler::Execute(WLRetDeleteChar* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION
	
		TID CurrentThreadID = MyGetCurrentThreadID();

		if(CurrentThreadID == g_pServerManager->m_ThreadID)
		{
			if (g_pDBThreadManager->SendPacket(pPacket,pPacket->GetPlayerID()))
			{
				return PACKET_EXE_NOTREMOVE;
			}
			else
			{//DB 压力过大，让用户重新尝试
				LCRetDeleteChar* pMsg = (LCRetDeleteChar*)g_pPacketFactoryManager->CreatePacket(PACKET_LC_RETDELETECHAR);
				pMsg->SetAccount(pPacket->GetAccount());
				pMsg->SetResult(ASKDELETECHAR_SERVER_BUSY);
				g_pProcessManager->SendPacket(pMsg,pPacket->GetPlayerID());
				Log::SaveLog( LOGIN_LOGFILE, "WLRetDeleteCharHandler....Fails!, code = ASKDELETECHAR_SERVER_BUSY") ;
				return PACKET_EXE_CONTINUE;
			}
		}
		else if(g_pDBThreadManager->IsPoolTID(CurrentThreadID))
		{
			PlayerID_t	PlayerID	= pPacket->GetPlayerID();
			LoginPlayer* pLoginPlayer = static_cast<LoginPlayer*>(pPlayer);
			Assert(pLoginPlayer);

			ASKDELETECHAR_RESULT	Result;
			LCRetDeleteChar* pMsg = (LCRetDeleteChar*)g_pPacketFactoryManager->CreatePacket(PACKET_LC_RETDELETECHAR);
			if(!pMsg)
			{
				AssertEx(FALSE,"创建 LCRetDeleteChar 消息失败");
			}

			//检查玩家是否在线
			if( pPacket->GetResult() == ASKDELETECHAR_ONLINE )
			{
				pMsg->SetAccount(pPacket->GetAccount());
				pMsg->SetResult(ASKDELETECHAR_ONLINE);
				g_pProcessManager->SendPacket(pMsg,PlayerID);
				Log::SaveLog( LOGIN_LOGFILE, "WLRetDeleteCharHandler::Execute()....玩家正在线游戏!") ;
				return PACKET_EXE_CONTINUE;
			}

			//
			if(pLoginPlayer->GetDBOperating() == TRUE)
			{
				pMsg->SetAccount(pPacket->GetAccount());
				pMsg->SetResult(ASKDELETECHAR_SERVER_BUSY);
				g_pProcessManager->SendPacket(pMsg,PlayerID);
				Log::SaveLog( LOGIN_LOGFILE, "WLRetDeleteCharHandler::Execute()....数据库操作冲突!") ;
				return PACKET_EXE_NOTREMOVE;
			}

			//检查GUID 是否正确
			if(strcmp(pLoginPlayer->GetAccount(),pPacket->GetAccount())!= 0)
			{
				//应该是一次错误操作
				Log::SaveLog(LOGIN_LOGFILE, "ERROR: CLAskDeleteChar DBOperation Errors,acc = %s,Packet acc = %s",
					pLoginPlayer->GetAccount(),pPacket->GetAccount()) ;
				return PACKET_EXE_CONTINUE;
			}

			pLoginPlayer->SetDBOperating(TRUE);

			LCRetCharList* pRetListMsg = (LCRetCharList*)g_pPacketFactoryManager->CreatePacket(PACKET_LC_RETCHARLIST)	;
			if(!pRetListMsg)
			{

				AssertEx(FALSE,"创建 LCRetCharList 消息失败");
			}

			INT CharNumber = pLoginPlayer->GetCharNumber();
			if(CharNumber==0)
			{
				pRetListMsg->SetAccount(pPacket->GetAccount());
				Result = ASKDELETECHARR_EMPTY; //角色空了
			}
			else if(CharNumber ==-1)
			{
				pRetListMsg->SetAccount(pPacket->GetAccount());
				Result = ASKDELETECHAR_OP_ERROR;
			}
			else
			{

				ODBCInterface*	pInterface = g_pDBThreadManager->GetInterface(CurrentThreadID);
				Assert(pInterface);

				//删除纪录
				DBDeleteCharOp DeleteCharOp(pInterface);
				DeleteCharOp.SetAccount(pPacket->GetAccount());
				DeleteCharOp.SetCharGuid(pPacket->GetPlayerGUID());
				BOOL bRetDelete = 	DeleteCharOp.Delete();
				if(!bRetDelete)
				{
					Log::SaveLog(LOGIN_LOGFILE,"DeleteCharOp.Delete()....Get Errors: %s ",DeleteCharOp.GetErrorMessage());
					INT ErrorCode = DeleteCharOp.GetErrorCode();
					pMsg->SetAccount(pPacket->GetAccount());
					pMsg->SetResult(ASKDELETECHAR_INTERNAL_ERROR);
					g_pProcessManager->SendPacket(pMsg,PlayerID);
					pLoginPlayer->SetDBOperating(FALSE);
					Log::SaveLog( LOGIN_LOGFILE, "WLRetDeleteCharHandler::Execute()....Fails! code = ASKDELETECHAR_INTERNAL_ERROR \
													CharGUID = %d,Account = %s",pPacket->GetPlayerGUID(),pPacket->GetAccount()) ;
					return PACKET_EXE_NOTREMOVE;

				}
				DeleteCharOp.ParseResult(&Result);

				//返回新的角色列表
				DBCharList	CharListObject(pInterface);
				CharListObject.SetAccount(pPacket->GetAccount());
				BOOL bRetLoad = 	CharListObject.Load();
				if(bRetLoad)
				{

					CharListObject.ParseResult(pRetListMsg->GetCharBaseInfo());
					pRetListMsg->SetAccount(pPacket->GetAccount());
					pRetListMsg->SetResult(ASKCHARLIST_SUCCESS);
					pRetListMsg->SetCharNumber(CharListObject.GetCharNumber());

					if(CharListObject.GetCharNumber()<(UINT)CharNumber)
					{
						Result = ASKDELETECHAR_SUCCESS;
					}
					else
					{
						Result = ASKDELETECHAR_INTERNAL_ERROR;
					}
					pLoginPlayer->SetCharNumber(CharListObject.GetCharNumber());

					for(INT i=0;i<pLoginPlayer->GetCharNumber();i++)
					{
						pLoginPlayer->SetCharGUID(pRetListMsg->GetCharBaseInfo(i)->m_GUID,i);
					}
				}
				else //操作失败，可能是断开连接了
				{
					Log::SaveLog(LOGIN_LOGFILE,"CharListObject.Load()....Get Errors: %s ",
						CharListObject.GetErrorMessage());
					pRetListMsg->SetAccount(pPacket->GetAccount());
					pRetListMsg->SetResult(ASKCHARLIST_OP_FAILS);
					pRetListMsg->SetCharNumber(CharListObject.GetCharNumber());
				}

			}


			pMsg->SetAccount(pPacket->GetAccount());
			pMsg->SetResult(Result);
			g_pProcessManager->SendPacket(pMsg,PlayerID);
			g_pProcessManager->SendPacket(pRetListMsg,PlayerID);
			pLoginPlayer->SetDBOperating(FALSE);


			Log::SaveLog( LOGIN_LOGFILE, "WLRetDeleteCharHandler::Execute()....OK!CharGUID = %X,Account = %s",
				pPacket->GetPlayerGUID(),pPacket->GetAccount()) ;
			return PACKET_EXE_NOTREMOVE;
		}

		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;
}