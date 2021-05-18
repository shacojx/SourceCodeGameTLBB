#include "stdafx.h"

#include "CLAskDeleteChar.h"
#include "LCRetDeleteChar.h"
#include "DBDeleteCharOp.h"
#include "ProcessPlayerManager.h"
#include "LoginPlayer.h"
#include "TimeManager.h"
#include "DBLogicManager.h"
#include "DBCharList.h"
#include "PacketFactoryManager.h"
#include "LCRetCharList.h"
#include "ProcessManager.h"
#include "Log.h"
#include "DBThreadManager.h"
#include "LWAskDeleteChar.h"
#include "ServerManager.h"

UINT CLAskDeleteCharHandler::Execute(CLAskDeleteChar* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

	TID CurrentThreadID = MyGetCurrentThreadID();

	if(CurrentThreadID == g_pProcessPlayerManager->m_ThreadID) 
	{
		LoginPlayer* pLoginPlayer = static_cast<LoginPlayer*>(pPlayer);
		Assert(pLoginPlayer);

		if( !StrSafeCheck( pPacket->GetAccount(), MAX_ACCOUNT ) )
		{
			Log::SaveLog(LOGIN_LOGFILE, "ERROR: CLAskDeleteCharHandler Get IllegalString,acc = %s",
				pLoginPlayer->GetAccount()) ;
			return PACKET_EXE_ERROR;
		}

		if(pLoginPlayer->GetPlayerStatus()!=PS_LOGIN_NORMAL)
		{
			//没有权限，什么也不做
			return PACKET_EXE_CONTINUE;
		}

		GUID_t	TryLoginGuid = pPacket->GetCharGuid();

		if(!pLoginPlayer->IsGUIDOwner(TryLoginGuid))
		{
			//用户操作非法,用户不是请求角色的所有者
			LCRetDeleteChar Msg;
			Msg.SetResult(ASKDELETECHAR_NOT_OWNER);
			pLoginPlayer->SendPacket(&Msg);
			Log::SaveLog( LOGIN_LOGFILE, "CLAskDeleteCharHandler::Execute()....Fails!,code = ASKDELETECHAR_NOT_OWNER") ;
			return PACKET_EXE_CONTINUE;
		}

		UINT uTime = g_pTimeManager->CurrentTime();
		if(uTime<pLoginPlayer->m_LastDBOpTime+DB_OPERATION_TIME)
		{
			LCRetDeleteChar Msg;
			Msg.SetResult(ASKDELETECHAR_OP_TIMES);
			//发送DB操作频繁消息
			pLoginPlayer->SendPacket(&Msg);
			Log::SaveLog(LOGIN_LOGFILE, "CLAskDeleteCharHandler::Execute()....database busy,acc=%s,tryguid=%X",
			pLoginPlayer->GetAccount(),TryLoginGuid) ;
			return PACKET_EXE_CONTINUE;
		}

		//________________________________________
		//pPacket->SetPlayerID(pLoginPlayer->PlayerID());
		//pPacket->SetAccount(pLoginPlayer->GetAccount());

		////将玩家操作加入DB 队列
		////并且设置最后操作时间
		//if(g_pDBThreadManager->SendPacket(pPacket,pLoginPlayer->PlayerID()))
		//{//加入成功，将消息发送到DB处理
		//	pLoginPlayer->m_LastDBOpTime = uTime;	
		//	return PACKET_EXE_NOTREMOVE;	
		//}
		//else
		//{//DB 压力过大，让用户重新尝试
		//	LCRetDeleteChar Msg;
		//	Msg.SetResult(ASKDELETECHAR_SERVER_BUSY);
		//	pLoginPlayer->SendPacket(&Msg);
		//	return PACKET_EXE_CONTINUE;
		//}
		//________________________________________

		LWAskDeleteChar	MsgDeleteChar;
		MsgDeleteChar.SetAccount( pLoginPlayer->GetAccount() );
		MsgDeleteChar.SetPlayerID( pLoginPlayer->PlayerID() );
		MsgDeleteChar.SetPlayerGUID( pPacket->GetCharGuid() );
		g_pServerManager->GetServerPlayer(WORLD_PLAYER_ID)->SendPacket(&MsgDeleteChar);

		return PACKET_EXE_CONTINUE;

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

		if(pLoginPlayer->GetDBOperating() == TRUE)
		{
			pMsg->SetAccount(pPacket->GetAccount());
			pMsg->SetResult(ASKDELETECHAR_SERVER_BUSY);
			g_pProcessManager->SendPacket(pMsg,PlayerID);
			Log::SaveLog( LOGIN_LOGFILE, "CLAskDeleteCharHandler::Execute()....数据库操作冲突!") ;
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
			DeleteCharOp.SetCharGuid(pPacket->GetCharGuid());
			BOOL bRetDelete = 	DeleteCharOp.Delete();
			if(!bRetDelete)
			{
				Log::SaveLog(LOGIN_LOGFILE,"DeleteCharOp.Delete()....Get Errors: %s ",DeleteCharOp.GetErrorMessage());
				INT ErrorCode = DeleteCharOp.GetErrorCode();
				pMsg->SetAccount(pPacket->GetAccount());
				pMsg->SetResult(ASKDELETECHAR_INTERNAL_ERROR);
				g_pProcessManager->SendPacket(pMsg,PlayerID);
				pLoginPlayer->SetDBOperating(FALSE);
				Log::SaveLog( LOGIN_LOGFILE, "CLAskDeleteCharHandler::Execute()....Fails! code = ASKDELETECHAR_INTERNAL_ERROR \
				 CharGUID = %d,Account = %s",pPacket->GetCharGuid(),pPacket->GetAccount()) ;
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
		

		Log::SaveLog( LOGIN_LOGFILE, "CLAskDeleteCharHandler::Execute()....OK!CharGUID = %X,Account = %s",
			pPacket->GetCharGuid(),pPacket->GetAccount()) ;
		return PACKET_EXE_NOTREMOVE;
	}

	return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;
}