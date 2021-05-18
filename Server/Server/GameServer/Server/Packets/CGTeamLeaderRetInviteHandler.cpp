#include "stdafx.h"


#include "CGTeamLeaderRetInvite.h"
#include "Log.h"
#include "GamePlayer.h"
#include "Obj_Human.h"
#include "Scene.h"
#include "PacketFactoryManager.h"
#include "GWTeamLeaderRetInvite.h"
#include "ServerManager.h"



uint CGTeamLeaderRetInviteHandler::Execute( CGTeamLeaderRetInvite* pPacket, Player* pPlayer )
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




	GWTeamLeaderRetInvite* pMsg = (GWTeamLeaderRetInvite*)(g_pPacketFactoryManager->CreatePacket(PACKET_GW_TEAMLEADERRETINVITE)) ;
	pMsg->SetReturn( pPacket->GetReturn() ) ;
	pMsg->SetSourGUID( pPacket->GetSourGUID() ) ;
	pMsg->SetDestGUID( pPacket->GetDestGUID() ) ;
	pMsg->SetLeaderGUID( pGamePlayer->m_HumanGUID ) ;

	g_pServerManager->SendPacket( pMsg, INVALID_ID ) ;


	g_pLog->FastSaveLog( LOG_FILE_1, "CGTeamLeaderRetInviteHandler: Ret=%d sGUID=%X dGUID=%X lGUID=%X", 
		pPacket->GetReturn(), pPacket->GetSourGUID(),pPacket->GetDestGUID(),pGamePlayer->m_HumanGUID ) ;


	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
