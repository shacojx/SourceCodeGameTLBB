#include "stdafx.h"


#include "CGHeartBeat.h"
#include "Log.h"
#include "GamePlayer.h"
#include "Scene.h"
#include "Obj_Human.h"
#include "GWHeartBeat.h"
#include "PacketFactoryManager.h"
#include "ServerManager.h"



uint CGHeartBeatHandler::Execute( CGHeartBeat* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	GamePlayer* pGamePlayer = (GamePlayer*)pPlayer;
	Assert( pGamePlayer );

	Obj_Human* pHuman = pGamePlayer->GetHuman();
	Assert( pHuman );

	Scene* pScene = pHuman->getScene();
	if( pScene==NULL )
	{
		Assert(FALSE);
		return PACKET_EXE_ERROR;
	}

	// 检查线程执行资源是否正确
	Assert( MyGetCurrentThreadID()==pScene->m_ThreadID );

	GWHeartBeat* pMsg = (GWHeartBeat*)(g_pPacketFactoryManager->CreatePacket(PACKET_GW_HEARTBEAT)) ;
	pMsg->SetGUID( pGamePlayer->m_HumanGUID ) ;

	g_pServerManager->SendPacket( pMsg, INVALID_ID ) ;

	g_pLog->FastSaveLog( LOG_FILE_1, "CGHeartBeatHandler::SOCKET=%d GUID=%X OK!",
		pPlayer->GetSocket()->getSOCKET(), pGamePlayer->m_HumanGUID ) ;

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
