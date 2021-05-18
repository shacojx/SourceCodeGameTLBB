#include "stdafx.h"


#include "CGTeamLeave.h"
#include "Log.h"
#include "GamePlayer.h"
#include "Obj_Human.h"
#include "Scene.h"
#include "PacketFactoryManager.h"
#include "GWTeamLeave.h"
#include "ServerManager.h"



uint CGTeamLeaveHandler::Execute( CGTeamLeave* pPacket, Player* pPlayer )
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



	GWTeamLeave* pMsg = (GWTeamLeave*)(g_pPacketFactoryManager->CreatePacket(PACKET_GW_TEAMLEAVE)) ;
	pMsg->SetGUID( pPacket->GetGUID() ) ;//离开队伍的人

	g_pServerManager->SendPacket( pMsg, INVALID_ID ) ;


	g_pLog->FastSaveLog( LOG_FILE_1, "CGTeamLeaveHandler: GUID=%X", 
		pPacket->GetGUID() ) ;



	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
