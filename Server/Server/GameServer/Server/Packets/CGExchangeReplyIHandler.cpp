#include "stdafx.h"
/*
收到此消息证明被申请方已经接受了交易申请，可以向双方客户端发送建立交易连接的消息
*/

#include "CGExchangeReplyI.h"
#include "GCExchangeError.h"
#include "GCExchangeEstablishI.h"
#include "GamePlayer.h"
#include "Obj_Human.h"
#include "Scene.h"
#include "Log.h"

uint CGExchangeReplyIHandler::Execute( CGExchangeReplyI* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION
	GamePlayer* pGamePlayer = (GamePlayer*)pPlayer ;
	Assert( pGamePlayer ) ;

	Obj_Human* pHuman = pGamePlayer->GetHuman() ;
	Assert( pHuman ) ;

	Scene* pScene = pHuman->getScene() ;
	if( pScene==NULL )
	{
		Assert(FALSE) ;
		return PACKET_EXE_ERROR ;
	}

	//检查线程执行资源是否正确
	Assert( MyGetCurrentThreadID()==pScene->m_ThreadID ) ;

	ObjID_t	SourceID = pPacket->GetObjID();
	Obj_Human* pDestHuman= pHuman;//自己被申请方
	Obj_Human* pSourceHuman = pScene->GetHumanManager()->GetHuman( SourceID );//交易发起者

	//验证
	if( pSourceHuman == NULL )
	{//发起者掉线
		Assert(FALSE);
		return PACKET_EXE_CONTINUE;
	}

	if(	pSourceHuman->m_ExchangBox.m_Status >= ServerExchangeBox::EXCHANGE_SYNCH_DATA )
	{
		if( pSourceHuman->m_ExchangBox.m_ObjID == pDestHuman->GetID() )
		{//正在和你交易别捣乱了
			return PACKET_EXE_CONTINUE;
		}
		else
		{//告诉自己对方已经在跟别人交易了,所有状态清空
			GCExchangeError Msg;
			Msg.SetID(EXCHANGE_MSG::ERR_TARGET_IN_EXCHANGE);
			pHuman->GetPlayer()->SendPacket(&Msg);
			g_pLog->FastSaveLog( LOG_FILE_1, "<交易> [%s] 已经在交易中， 不能接受[%s]的申请",	pSourceHuman->GetName(), pDestHuman->GetName() ) ;
			return PACKET_EXE_CONTINUE;
		}
	}

	if(pDestHuman->m_ExchangBox.m_Status >= ServerExchangeBox::EXCHANGE_SYNCH_DATA)
	{
		if(	pDestHuman->m_ExchangBox.m_ObjID == pSourceHuman->GetID())
		{//正是当前交易
			return PACKET_EXE_CONTINUE;
		}
		else
		{//告诉自己自己已经在跟别人交易了,所有状态清空
			GCExchangeError Msg;
			Msg.SetID(EXCHANGE_MSG::ERR_SELF_IN_EXCHANGE);
			pHuman->GetPlayer()->SendPacket(&Msg);
			g_pLog->FastSaveLog( LOG_FILE_1, "<交易> [%s] 已经在交易中， 不能接受[%s]的申请",	pDestHuman->GetName(), pSourceHuman->GetName() ) ;
			return PACKET_EXE_CONTINUE;
		}
	}

	//距离判定
	if(MySqrt( pSourceHuman->getWorldPos(), pDestHuman->getWorldPos() ) > 20 )
	{
		GCExchangeError Msg;
		Msg.SetID(EXCHANGE_MSG::ERR_TOO_FAR);
		pHuman->GetPlayer()->SendPacket(&Msg);
		g_pLog->FastSaveLog( LOG_FILE_1, "<交易> [%s] 超过交易范围[%s]]",	pDestHuman->GetName(), pSourceHuman->GetName() ) ;
		return PACKET_EXE_CONTINUE;
	}

	//验证通过，开始初始化双方交易盒,进入步骤2
	pSourceHuman->m_ExchangBox.CleanUp();
	pSourceHuman->m_ExchangBox.m_Status = ServerExchangeBox::EXCHANGE_SYNCH_DATA;
	pSourceHuman->m_ExchangBox.m_Money = 0;
	pSourceHuman->m_ExchangBox.m_IsLocked = FALSE;
	pSourceHuman->m_ExchangBox.m_CanConform = FALSE;
	pSourceHuman->m_ExchangBox.m_ObjID = pDestHuman->GetID();
	//pSourceHuman->m_ExchangBox.

	pDestHuman->m_ExchangBox.CleanUp();
	pDestHuman->m_ExchangBox.m_Status = ServerExchangeBox::EXCHANGE_SYNCH_DATA;
	pDestHuman->m_ExchangBox.m_Money = 0;
	pDestHuman->m_ExchangBox.m_IsLocked = FALSE;
	pDestHuman->m_ExchangBox.m_CanConform = FALSE;
	pDestHuman->m_ExchangBox.m_ObjID = pSourceHuman->GetID();
	//pDestHuman->m_ExchangBox.
	g_pLog->FastSaveLog( LOG_FILE_1, "<交易> [%s] 与 [%s] 建立交易连接",	pSourceHuman->GetName(), pDestHuman->GetName() ) ;

	GCExchangeEstablishI MsgToSource;
	MsgToSource.SetObjID(pDestHuman->GetID());
	pSourceHuman->GetPlayer()->SendPacket(&MsgToSource);

	GCExchangeEstablishI MsgToDest;
	MsgToDest.SetObjID(pSourceHuman->GetID());
	pDestHuman->GetPlayer()->SendPacket(&MsgToDest);


	return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
