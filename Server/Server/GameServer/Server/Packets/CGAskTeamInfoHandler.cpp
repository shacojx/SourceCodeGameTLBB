#include "stdafx.h"
// CGAskTeamInfoHandler.cpp

#include "CGAskTeamInfo.h"
#include "GWAskTeamInfo.h"

#include "ServerManager.h"
#include "PacketFactoryManager.h"
#include "GamePlayer.h"
#include "Obj_Human.h"
#include "Scene.h"
#include "Log.h"
#include "Team.h"

uint CGAskTeamInfoHandler::Execute( CGAskTeamInfo* pPacket, Player* pPlayer )
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

	if( pHuman->GetID() != pPacket->GetObjID() )
	{ // 为以后留接口
		Assert(FALSE);
		return PACKET_EXE_ERROR;
	}

	Obj_Human *pDestHuman = pScene->GetHumanManager()->GetHuman( pPacket->GetObjID() );
	if( pDestHuman == NULL )
	{
		Assert(FALSE);
		return PACKET_EXE_CONTINUE;
	}

	GWAskTeamInfo* pMsg = (GWAskTeamInfo*)(g_pPacketFactoryManager->CreatePacket(PACKET_GW_ASKTEAMINFO));

	pMsg->SetPlayerID( pGamePlayer->PlayerID() );
	pMsg->SetGUID( pDestHuman->GetGUID() );

	g_pServerManager->SendPacket( pMsg, INVALID_ID );

	g_pLog->FastSaveLog( LOG_FILE_1, "CGAskTeamInfoHandler: AskManObjID=%X TargetManGUID=%X", 
		pGamePlayer->PlayerID(), pDestHuman->GetGUID() ) ;

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
