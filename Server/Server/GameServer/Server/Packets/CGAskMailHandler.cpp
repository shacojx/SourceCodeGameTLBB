#include "stdafx.h"


#include "CGAskMail.h"
#include "Log.h"
#include "GamePlayer.h"
#include "Obj_Human.h"
#include "Scene.h"
#include "PacketFactoryManager.h"
#include "ServerManager.h"

#include "GWAskMail.h"




uint CGAskMailHandler::Execute( CGAskMail* pPacket, Player* pPlayer )
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

	if( pPacket->GetAskType() == ASK_TYPE_LOGIN )
	{
		pHuman->OnHumanLogin( ) ;
	}

	GWAskMail* pMsg = (GWAskMail*)(g_pPacketFactoryManager->CreatePacket(PACKET_GW_ASKMAIL)) ;
	pMsg->SetGUID( pGamePlayer->m_HumanGUID ) ;
	pMsg->SetAskType( pPacket->GetAskType() );

	g_pServerManager->SendPacket( pMsg, INVALID_ID ) ;


	g_pLog->FastSaveLog( LOG_FILE_1, "GWAskMailHandler: GUID=%X PID=%d", 
		pGamePlayer->m_HumanGUID, pGamePlayer->PlayerID() ) ;


	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
