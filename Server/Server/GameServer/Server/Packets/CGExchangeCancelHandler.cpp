#include "stdafx.h"
/*
客户端清除本地交易盒数据，关闭界面,发送消息，
服务器收到此消息，清除双方交易盒信息，发消息15给对方
*/

#include "CGExchangeCancel.h"
#include "GCExchangeCancel.h"
#include "GCExchangeError.h"
#include "GamePlayer.h"
#include "Obj_Human.h"
#include "Scene.h"
#include "Log.h"

uint CGExchangeCancelHandler::Execute( CGExchangeCancel* pPacket, Player* pPlayer )
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
	Obj_Human* pDestHuman = pScene->GetHumanManager()->GetHuman( pHuman->m_ExchangBox.m_ObjID );

	//清空状态
	pHuman->m_ExchangBox.CleanUp();
	pDestHuman->m_ExchangBox.CleanUp();

	g_pLog->FastSaveLog( LOG_FILE_1, "<交易> [%s] 单方面取消了交易",	pHuman->GetName() ) ;

	//发送取消消息给双方
	GCExchangeCancel MsgToMe,MsgToOt;
	pHuman->GetPlayer()->SendPacket(&MsgToMe);
	pDestHuman->GetPlayer()->SendPacket(&MsgToOt);

	return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
