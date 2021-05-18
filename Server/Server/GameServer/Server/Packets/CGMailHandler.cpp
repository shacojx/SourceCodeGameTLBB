#include "stdafx.h"


#include "CGMail.h"
#include "Log.h"
#include "GamePlayer.h"
#include "Obj_Human.h"
#include "Scene.h"
#include "PacketFactoryManager.h"
#include "ServerManager.h"

#include "GWMail.h"


uint CGMailHandler::Execute( CGMail* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	GamePlayer* pGamePlayer = (GamePlayer*)pPlayer ;
	Assert( pGamePlayer != NULL ) ;
	if ( pGamePlayer == NULL )
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGMailHandler: Error (pGamePlayer == NULL) " ) ;
		return PACKET_EXE_CONTINUE ;
	}

	Obj_Human* pHuman = pGamePlayer->GetHuman() ;
	Assert( pHuman != NULL ) ;
	if ( pHuman == NULL )
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGMailHandler: Error (pHuman == NULL) " ) ;
		return PACKET_EXE_CONTINUE ;
	}

	Scene* pScene = pHuman->getScene() ;
	Assert( pScene != NULL ) ;
	if ( pScene == NULL )
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGMailHandler: Error Obj = %d (pScene == NULL) ", pHuman->GetID() ) ;
		return PACKET_EXE_CONTINUE ;
	}

	//检查线程执行资源是否正确
	Assert( MyGetCurrentThreadID()==pScene->m_ThreadID ) ;

	GWMail* pMsg = (GWMail*)(g_pPacketFactoryManager->CreatePacket(PACKET_GW_MAIL)) ;
	pMsg->SetMail( pPacket->GetMail() ) ;
	pMsg->GetMail()->m_Camp = pHuman->GetCampData()->m_nCampID;

	g_pServerManager->SendPacket( pMsg, INVALID_ID ) ;

	g_pLog->FastSaveLog( LOG_FILE_1, "CGMailHandler: sName=%s dName=%s", 
		pPacket->GetMail()->m_szSourName, pPacket->GetMail()->m_szDestName ) ;

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
