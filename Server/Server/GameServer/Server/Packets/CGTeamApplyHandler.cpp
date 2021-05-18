#include "stdafx.h"


#include "CGTeamApply.h"
#include "Log.h"
#include "GamePlayer.h"
#include "Obj_Human.h"
#include "Scene.h"
#include "PacketFactoryManager.h"
#include "GWTeamApply.h"
#include "ServerManager.h"




uint CGTeamApplyHandler::Execute( CGTeamApply* pPacket, Player* pPlayer )
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



	GWTeamApply* pMsg = (GWTeamApply*)(g_pPacketFactoryManager->CreatePacket(PACKET_GW_TEAMAPPLY)) ;
	pMsg->SetSourGUID( pPacket->GetSourGUID() ) ;//申请人
	pMsg->SetDestName( pPacket->GetDestName() ) ;//被申请人

	g_pServerManager->SendPacket( pMsg, INVALID_ID ) ;


	g_pLog->FastSaveLog( LOG_FILE_1, "CGTeamApplyHandler: sGUID=%X dName=%s", 
		pPacket->GetSourGUID(), pPacket->GetDestName() ) ;




	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
