#include "stdafx.h"


#include "CGTeamRetInvite.h"
#include "Log.h"
#include "GamePlayer.h"
#include "Obj_Human.h"
#include "Scene.h"
#include "PacketFactoryManager.h"
#include "GWTeamRetInvite.h"
#include "ServerManager.h"



uint CGTeamRetInviteHandler::Execute( CGTeamRetInvite* pPacket, Player* pPlayer )
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



	GWTeamRetInvite* pMsg = (GWTeamRetInvite*)(g_pPacketFactoryManager->CreatePacket(PACKET_GW_TEAMRETINVITE)) ;
	pMsg->SetReturn( pPacket->GetReturn() ) ;		//返回结果
	pMsg->SetSourGUID( pPacket->GetGUID() ) ;		//邀请人
	pMsg->SetDestGUID( pGamePlayer->m_HumanGUID ) ;	//被邀请人

	g_pServerManager->SendPacket( pMsg, INVALID_ID ) ;


	g_pLog->FastSaveLog( LOG_FILE_1, "CGTeamRetInviteHandler: Ret=%d sGUID=%X dGUID=%X PID=%d", 
		pPacket->GetReturn(), pPacket->GetGUID(), pGamePlayer->m_HumanGUID, pGamePlayer->PlayerID()) ;


	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
