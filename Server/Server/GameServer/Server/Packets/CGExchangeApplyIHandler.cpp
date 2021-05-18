#include "stdafx.h"
/*
验证申请的合法性，并转发
*/

#include "CGExchangeApplyI.h"
#include "GCExchangeApplyI.h"
#include "GCExchangeError.h"
#include "GamePlayer.h"
#include "Obj_Human.h"
#include "Scene.h"
#include "Log.h"

uint CGExchangeApplyIHandler::Execute( CGExchangeApplyI* pPacket, Player* pPlayer )
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

	ObjID_t		TargetID = pPacket->GetObjID();
	Obj_Human* pSourceHuman = pHuman;//交易发起者
	Obj_Human* pDestHuman = pScene->GetHumanManager()->GetHuman( TargetID );//交易对象

	//验证
	if( pDestHuman == NULL )
	{
		Assert(FALSE);
		return PACKET_EXE_CONTINUE;
	}
	// 不同阵营，不让查看
	if( pSourceHuman->IsEnemy( pDestHuman ) )
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGExchangeApplyIHandler: %s cann't ask %s's DetailAttr ", pSourceHuman->GetName(), pDestHuman->GetName() ) ;
		return	PACKET_EXE_CONTINUE;
	}

	INT iSettingData = pDestHuman->GetDB()->GetSetting(SETTING_TYPE_GAME)->m_SettingData;
	if(SETTINGFLAGISTRUE(iSettingData, GSF_REFUSE_TRADE))
	{
		GCExchangeError Msg;
		Msg.SetID(EXCHANGE_MSG::ERR_REFUSE_TRADE);
		pGamePlayer->SendPacket(&Msg);
		g_pLog->FastSaveLog( LOG_FILE_1, "CGExchangeApplyIHandler: ID = %d ERR_REFUSE_TRADE", pSourceHuman->GetID()) ;
		return PACKET_EXE_CONTINUE;
	}
	if(pSourceHuman->m_ExchangBox.m_Status >= ServerExchangeBox::EXCHANGE_SYNCH_DATA)
	{//发起者正在交易中
		GCExchangeError Msg;
		Msg.SetID(EXCHANGE_MSG::ERR_SELF_IN_EXCHANGE);
		pGamePlayer->SendPacket(&Msg);
		g_pLog->FastSaveLog( LOG_FILE_1, "CGExchangeApplyIHandler: ID = %d status = %d", pSourceHuman->GetID(), pSourceHuman->m_ExchangBox.m_Status) ;
		return PACKET_EXE_CONTINUE;
	}
	if(pDestHuman->m_ExchangBox.m_Status >= ServerExchangeBox::EXCHANGE_SYNCH_DATA)
	{//目标正在交易中
		GCExchangeError Msg;
		Msg.SetID(EXCHANGE_MSG::ERR_TARGET_IN_EXCHANGE);
		pGamePlayer->SendPacket(&Msg);
		g_pLog->FastSaveLog( LOG_FILE_1, "CGExchangeApplyIHandler: ID = %d status = %d", pDestHuman->GetID(),	pDestHuman->m_ExchangBox.m_Status ) ;
		return PACKET_EXE_CONTINUE;
	}

	//操作
	//发送消息向目标申请
	GCExchangeApplyI Msg;
	Msg.SetObjID(pSourceHuman->GetID());
	pDestHuman->GetPlayer()->SendPacket(&Msg);
	g_pLog->FastSaveLog( LOG_FILE_1, "CGExchangeApplyIHandler: SOURCEID = %d DESTID = %d", pSourceHuman->GetID(), pDestHuman->GetID() ) ;
		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
