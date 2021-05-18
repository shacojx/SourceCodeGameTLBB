#include "stdafx.h"

#include "CLAskCharList.h"
#include "LoginPlayer.h"
#include "ProcessPlayerManager.h"
#include "DBLogicManager.h"
#include "TimeManager.h"
#include "LCRetCharList.h"
#include "DBCharList.h"
#include "PacketFactoryManager.h"
#include "ProcessManager.h"
#include "Log.h"
#include "DBThreadManager.h"


UINT	CLAskCharListHandler::Execute(CLAskCharList* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION
	
	TID CurrentThreadID = MyGetCurrentThreadID();
	
	//检查线程执行资源是否正确
	if(CurrentThreadID == g_pProcessPlayerManager->m_ThreadID) 
	{	
		LoginPlayer* pLoginPlayer = static_cast<LoginPlayer*>(pPlayer);
		Assert(pLoginPlayer);

		if( !StrSafeCheck( pPacket->GetAccount(), MAX_ACCOUNT ) )
		{
			Log::SaveLog(LOGIN_LOGFILE, "ERROR: CLAskCharListHandler Get IllegalString,acc = %s",
				pLoginPlayer->GetAccount()) ;
			return PACKET_EXE_ERROR;
		}

		//检查GUID 是否正确
		if(strcmp(pLoginPlayer->GetAccount(),pPacket->GetAccount())!= 0)
		{
			Log::SaveLog(LOGIN_LOGFILE, "ERROR: CLAskCharListHandler Get Guid Errors,acc = %s,Packet acc = %s",
				pLoginPlayer->GetAccount(),pPacket->GetAccount()) ;
			return PACKET_EXE_ERROR;
		}

		UINT uTime = g_pTimeManager->CurrentTime();

		if(uTime<pLoginPlayer->m_LastDBOpTime+DB_OPERATION_TIME)
		{
			LCRetCharList Msg;
			Msg.SetResult(ASKCHARLIST_OP_TIMES);
			pLoginPlayer->SendPacket(&Msg);
			return PACKET_EXE_CONTINUE;
		}

		if(pLoginPlayer->GetPlayerStatus()!=PS_LOGIN_NORMAL)
		{
			return PACKET_EXE_CONTINUE;
		}

		pPacket->SetPlayerID(pLoginPlayer->PlayerID());
		//将玩家操作加入DB 队列
		//并且设置最后操作时间

		if(g_pDBThreadManager->SendPacket(pPacket,pLoginPlayer->PlayerID()))
		{//加入成功，将消息发送到DB处理
			pLoginPlayer->m_LastDBOpTime = uTime;
			return PACKET_EXE_NOTREMOVE;	
		}
		else
		{//DB 压力过大，让用户重新尝试
			LCRetCharList Msg;
			Msg.SetResult(ASKCHARLIST_SERVER_BUSY);
			pLoginPlayer->SendPacket(&Msg);
			pLoginPlayer->m_LastDBOpTime = uTime;
			return PACKET_EXE_CONTINUE;
		}
		
	}
	else if(g_pDBThreadManager->IsPoolTID(CurrentThreadID))
	{


		PlayerID_t	PlayerID	= pPacket->GetPlayerID();
		LoginPlayer* pLoginPlayer = static_cast<LoginPlayer*>(pPlayer);
		Assert(pLoginPlayer);
		LCRetCharList* pMsg = (LCRetCharList*)g_pPacketFactoryManager->CreatePacket(PACKET_LC_RETCHARLIST)	;
		if(!pMsg)
		{   //不能进行操作
			AssertEx(FALSE,"创建 LCRetCharList 消息失败");

		}
		if(pLoginPlayer->GetDBOperating() == TRUE||!g_pDBThreadManager->GetInterface(CurrentThreadID)->IsConnected())
		{
			pMsg->SetAccount(pPacket->GetAccount());
			pMsg->SetResult(ASKCHARLIST_SERVER_BUSY);
			pMsg->SetCharNumber(0);
			g_pProcessManager->SendPacket(pMsg,PlayerID);
			Log::SaveLog(LOGIN_LOGFILE, "CLAskCharListHandler::Execute()....数据库操作冲突 ,Acc = %s",
				pLoginPlayer->GetAccount()) ;
			return PACKET_EXE_CONTINUE;
		}

		//检查GUID 是否正确
		if(strcmp(pLoginPlayer->GetAccount(),pPacket->GetAccount())!= 0)
		{
			//应该是一次错误操作
			Log::SaveLog(LOGIN_LOGFILE, "ERROR: CLAskCharListHandler DBOperation Errors,acc = %s,Packet acc = %s",
				pLoginPlayer->GetAccount(),pPacket->GetAccount()) ;
			return PACKET_EXE_CONTINUE;
		}


		pLoginPlayer->SetDBOperating(TRUE);

		ODBCInterface*	pInterface = g_pDBThreadManager->GetInterface(CurrentThreadID);
		Assert(pInterface);
		DBCharList	CharListObject(pInterface);
		CharListObject.SetAccount(pPacket->GetAccount());
		
		BOOL bLoadRet = CharListObject.Load();
		if(!bLoadRet)
		{
			Log::SaveLog(LOGIN_LOGFILE,"CharListObject.Load() ... Get Errors: %s",
				CharListObject.GetErrorMessage());
			pMsg->SetAccount(pPacket->GetAccount());
			pMsg->SetResult(ASKCHARLIST_OP_FAILS);
			pMsg->SetCharNumber(0);
			g_pProcessManager->SendPacket(pMsg,PlayerID);
			Log::SaveLog( LOGIN_LOGFILE, "CLAskCharListHandler::Execute()....Fails!,code = ASKCHARLIST_OP_FAILS" ) ;
			
			pLoginPlayer->SetDBOperating(FALSE);
			return PACKET_EXE_CONTINUE;
		}
		//取结果
		CharListObject.ParseResult(pMsg->GetCharBaseInfo()); 

		//判断是否没有进入过场景
		INT iCharNumber = CharListObject.GetCharNumber();
	
		//把参数设置到pMsg
		pMsg->SetAccount(pPacket->GetAccount());
		pMsg->SetResult(ASKCHARLIST_SUCCESS);
		pMsg->SetCharNumber(iCharNumber);
		pLoginPlayer->SetCharNumber(iCharNumber);
		for(INT i=0;i<pLoginPlayer->GetCharNumber();i++)
		{
			pLoginPlayer->SetCharGUID(pMsg->GetCharBaseInfo(i)->m_GUID,i);
			pLoginPlayer->SetPlayerCamp( pMsg->GetCharBaseInfo(i)->m_Camp );
		}
		g_pProcessManager->SendPacket(pMsg,PlayerID);
		
		pLoginPlayer->SetDBOperating(FALSE);

		Log::SaveLog( LOGIN_LOGFILE, "CLAskCharListHandler::Execute()....OK! Acc=%s CharNum=%d",
			pPacket->GetAccount(), pLoginPlayer->GetCharNumber() ) ;
	}


	
	
	return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}