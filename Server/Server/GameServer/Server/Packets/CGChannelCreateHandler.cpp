#include "stdafx.h"


#include "CGChannelCreate.h"
#include "Log.h"
#include "GamePlayer.h"
#include "Obj_Human.h"
#include "Scene.h"
#include "PacketFactoryManager.h"
#include "ServerManager.h"

#include "GWChannelCreate.h"



uint CGChannelCreateHandler::Execute( CGChannelCreate* pPacket, Player* pPlayer )
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


	GWChannelCreate* pMsg = (GWChannelCreate*)(g_pPacketFactoryManager->CreatePacket(PACKET_GW_CHANNELCREATE)) ;
	pMsg->SetGUID( pPacket->GetGUID() ) ;

	g_pServerManager->SendPacket( pMsg, INVALID_ID ) ;


	g_pLog->FastSaveLog( LOG_FILE_1, "CGChannelCreateHandler: GUID=%X", 
		pPacket->GetGUID() ) ;



	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
