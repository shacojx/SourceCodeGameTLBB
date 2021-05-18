#include "stdafx.h"


#include "CGEnvRequest.h"
#include "SceneManager.h"
#include "GamePlayer.h"
#include "Obj_Human.h"
#include "PlayerPool.h"
#include "Log.h"
#include "GCWorldTime.h"



uint CGEnvRequestHandler::Execute( CGEnvRequest* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	GamePlayer* pGamePlayer = (GamePlayer*)pPlayer ;
	Assert( pGamePlayer ) ;

	Obj_Human* pHuman = pGamePlayer->GetHuman() ;
	Assert( pHuman ) ;

	Scene* pScene = pHuman->getScene() ;
	if( pScene==NULL ) return PACKET_EXE_CONTINUE ;

	//检查线程执行资源是否正确
	Assert( MyGetCurrentThreadID()==pScene->m_ThreadID ) ;


	////将在加入玩家位置周围的玩家信息、怪物信息、物品信息等取出来
	////然后发送给玩家

	//BOOL ret = pScene->ScanEnv( pHuman ) ;
	//Assert( ret ) ;

	//Assert( pGamePlayer->GetPlayerStatus()==PS_SERVER_WAITTING_FOR_ENVREQUEST ||
	//		pGamePlayer->GetPlayerStatus()==PS_SERVER_NORMAL ) ;
	//pGamePlayer->SetPlayerStatus( PS_SERVER_NORMAL ) ;


	GCWorldTime	Msg;
	Msg.SetWorldTime(g_WorldTime);
	pGamePlayer->SendPacket(&Msg);
    
	g_pLog->FastSaveLog( LOG_FILE_1, "CGEnvRequest::GUID=%X OK",
		pHuman->GetGUID() ) ;

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
