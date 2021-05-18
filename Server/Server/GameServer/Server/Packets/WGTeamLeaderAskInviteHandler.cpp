#include "stdafx.h"


#include "WGTeamLeaderAskInvite.h"
#include "Log.h"
#include "ServerManager.h"
#include "GamePlayer.h"
#include "PlayerPool.h"
#include "Scene.h"
#include "Obj_Human.h"
#include "SceneManager.h"
#include "GCTeamLeaderAskInvite.h"



uint WGTeamLeaderAskInviteHandler::Execute( WGTeamLeaderAskInvite* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	PlayerID_t PlayerID = pPacket->GetPlayerID();
	GamePlayer* pGamePlayer = g_pPlayerPool->GetPlayer(PlayerID);
	if( pGamePlayer==NULL )
	{
		g_pLog->FastSaveLog( LOG_FILE_3, "WGTeamLeaderAskInviteHandler::Execute pGamePlayer==NULL" );
		return PACKET_EXE_CONTINUE;
	}

	if (pGamePlayer->m_HumanGUID != pPacket->GetGUID())
	{
		g_pLog->FastSaveLog( LOG_FILE_3, "WGTeamLeaderAskInviteHandler::Execute pGamePlayer->m_HumanGUID[%d] != pPacket->GetGUID()[%d]",pGamePlayer->m_HumanGUID, pPacket->GetGUID());
		return PACKET_EXE_CONTINUE;
	}

	Obj_Human* pHuman = pGamePlayer->GetHuman();
	Assert( pHuman );
	Scene* pScene = pHuman->getScene();
	if( !pScene )
	{
		g_pLog->FastSaveLog( LOG_FILE_3, "WGTeamLeaderAskInviteHandler::Execute pHuman->getScene() == NULL" );
		return PACKET_EXE_CONTINUE;
	}

	if( pPlayer->IsServerPlayer() )
	{//服务器收到世界服务器发来的数据
		Assert( MyGetCurrentThreadID()==g_pServerManager->m_ThreadID ) ;

		pScene->SendPacket( pPacket, PlayerID ) ;

		g_pLog->FastSaveLog( LOG_FILE_1, "WGTeamAskInviteHandler: ServerPlayer sGUID=%X, dGUID=%X ",
			pPacket->GetSourGUID(), pPacket->GetDestGUID() ) ;

		return PACKET_EXE_NOTREMOVE ;
	}
	else if( pPlayer->IsGamePlayer() )
	{//场景收到Cache里的消息
		Assert( MyGetCurrentThreadID()==pScene->m_ThreadID ) ;

		GCTeamLeaderAskInvite tMsg ;
		tMsg.SetSourGUID( pPacket->GetSourGUID() );
		tMsg.SetDestGUID( pPacket->GetDestGUID() );
		tMsg.SetSourName( pPacket->GetSourName() );
		tMsg.SetDestName( pPacket->GetDestName() );

		pGamePlayer->SendPacket( &tMsg ) ;

		g_pLog->FastSaveLog( LOG_FILE_1, "WGTeamLeaderAskInviteHandler: GamePlayer sGUID=%X,dGUID=%X ",
			pPacket->GetSourGUID(),pPacket->GetDestGUID() ) ;
	}
	else
	{
		Assert(FALSE) ;
	}


	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
