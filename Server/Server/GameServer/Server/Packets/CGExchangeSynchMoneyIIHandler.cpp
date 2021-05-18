#include "stdafx.h"
/*
客户端的金钱变化通知服务器
*/

#include "CGExchangeSynchMoneyII.h"
#include "GCExchangeSynchII.h"
#include "GCExchangeError.h"
#include "GamePlayer.h"
#include "Obj_Human.h"
#include "Scene.h"
#include "Log.h"

uint CGExchangeSynchMoneyIIHandler::Execute( CGExchangeSynchMoneyII* pPacket, Player* pPlayer )
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

	//验证
	EXCHANGE_CERTIFY_EACH_OTHER(pHuman)
	EXCHANGE_CERTIFY_ISLOCK(pHuman)

	ObjID_t	DestID = pHuman->m_ExchangBox.m_ObjID;
	Obj_Human* pDestHuman = pScene->GetHumanManager()->GetHuman( DestID );

	EXCHANGE_CERTIFY_STATUS(pHuman, EXCHANGE_SYNCH_DATA)
	EXCHANGE_CERTIFY_STATUS(pDestHuman, EXCHANGE_SYNCH_DATA)
	

	//操作
	UINT	Money	 = pPacket->GetMoney();		

	if( Money<= pHuman->GetMoney() )
	{
		pHuman->m_ExchangBox.m_Money = Money;
		g_pLog->FastSaveLog( LOG_FILE_1, "<交易> [%s] 设置交易金钱 [%d]",	pHuman->GetName(), Money ) ;

		//金钱改变不发给自己了，省条消息
		GCExchangeSynchII MsgToTarget;
		MsgToTarget.SetIsMyself(FALSE);
		MsgToTarget.SetOpt(EXCHANGE_MSG::OPT_MONEY);
		MsgToTarget.SetMoney(Money);
		pDestHuman->GetPlayer()->SendPacket(&MsgToTarget);
		return PACKET_EXE_CONTINUE;
	}
	else
	{
		GCExchangeError Msg;
		Msg.SetID(EXCHANGE_MSG::ERR_ILLEGAL);
		pHuman->GetPlayer()->SendPacket(&Msg);
		pHuman->m_ExchangBox.CleanUp();
		return PACKET_EXE_CONTINUE;
	}
	
	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
