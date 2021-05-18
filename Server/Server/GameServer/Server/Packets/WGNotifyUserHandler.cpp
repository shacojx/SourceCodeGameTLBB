#include "stdafx.h"

#include "WGNotifyUser.h"
#include "GamePlayer.h"
#include "Obj_Human.h"
#include "GUIDManager.h"
#include "ServerManager.h"
#include "Scene.h"
#include "GWNotifyUser.h"
#include "PacketFactoryManager.h"


uint WGNotifyUserHandler::Execute( WGNotifyUser* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	GUID_t sGuid = pPacket->GetGUID() ;
	Obj_Human* pHuman = (Obj_Human*)(g_pGUIDManager->Get(sGuid)) ;
	if( pHuman==NULL )
	{
		GWNotifyUser* pMsg = (GWNotifyUser*)(g_pPacketFactoryManager->CreatePacket(PACKET_GW_NOTIFYUSER)) ;
		pMsg->SetGUID( sGuid ) ;
		pMsg->SetPlayerID( -1 ) ;
		pMsg->SetStatus( GWNotifyUser::NUS_NEED_WORLD_KICK ) ;
		g_pServerManager->SendPacket( pMsg, INVALID_ID ) ;

		g_pLog->FastSaveLog( LOG_FILE_1, "WGNotifyUserHandler: pHuman==NULL GUID=%X",
			sGuid ) ;
		return PACKET_EXE_CONTINUE ;
	}

	GamePlayer* pGamePlayer = (GamePlayer*)(pHuman->GetPlayer()) ;
	if( pGamePlayer==NULL )
	{
		GWNotifyUser* pMsg = (GWNotifyUser*)(g_pPacketFactoryManager->CreatePacket(PACKET_GW_NOTIFYUSER)) ;
		pMsg->SetGUID( sGuid ) ;
		pMsg->SetPlayerID( -1 ) ;
		pMsg->SetStatus( GWNotifyUser::NUS_NEED_WORLD_KICK ) ;
		g_pServerManager->SendPacket( pMsg, INVALID_ID ) ;

		g_pLog->FastSaveLog( LOG_FILE_1, "WGNotifyUserHandler: pGamePlayer==NULL GUID=%X",
			sGuid ) ;
		return PACKET_EXE_CONTINUE ;
	}

	Scene* pScene = pHuman->getScene() ;
	if( pScene==NULL )
	{
		GWNotifyUser* pMsg = (GWNotifyUser*)(g_pPacketFactoryManager->CreatePacket(PACKET_GW_NOTIFYUSER)) ;
		pMsg->SetGUID( sGuid ) ;
		pMsg->SetPlayerID( -1 ) ;
		pMsg->SetStatus( GWNotifyUser::NUS_NEED_WORLD_KICK ) ;
		g_pServerManager->SendPacket( pMsg, INVALID_ID ) ;

		g_pLog->FastSaveLog( LOG_FILE_1, "WGNotifyUserHandler: pScene==NULL GUID=%X",
			sGuid ) ;
		return PACKET_EXE_CONTINUE ;
	}

	if( pPlayer->IsServerPlayer() )
	{//服务器收到世界服务器发来的数据
		Assert( MyGetCurrentThreadID()==g_pServerManager->m_ThreadID ) ;

		PlayerID_t PlayerID = pHuman->GetPlayerID() ;
		pScene->SendPacket( pPacket, PlayerID ) ;

		g_pLog->FastSaveLog( LOG_FILE_1, "WGNotifyUserHandler: ServerPlayer GUID=%X",
			sGuid ) ;

		return PACKET_EXE_NOTREMOVE ;
	}
	else if( pPlayer->IsGamePlayer() )
	{//场景收到Cache里的消息
		Assert( MyGetCurrentThreadID()==pScene->m_ThreadID ) ;

		if( pPacket->GetStatus() == WGNotifyUser::NUS_REMOVE )
		{
			g_pLog->FastSaveLog( LOG_FILE_1, "WGNotifyUserHandler: GamePlayer GUID=%X kick!",
				sGuid ) ;
			return PACKET_EXE_ERROR ;
		}
	}

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
