#include "stdafx.h"


#include "WGRetChangeScene.h"
#include "Log.h"
#include "ServerManager.h"
#include "GamePlayer.h"
#include "PlayerPool.h"
#include "Scene.h"
#include "Obj_Human.h"
#include "GCRetChangeScene.h"
#include "SceneManager.h"




uint WGRetChangeSceneHandler::Execute( WGRetChangeScene* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	PlayerID_t PlayerID = pPacket->GetPlayerID();
	GamePlayer* pGamePlayer = g_pPlayerPool->GetPlayer(PlayerID);
	if( pGamePlayer==NULL )
	{
		g_pLog->FastSaveLog( LOG_FILE_3, "WGRetChangeSceneHandler::Execute pGamePlayer==NULL" );
		return PACKET_EXE_CONTINUE;
	}

	if (pGamePlayer->m_HumanGUID != pPacket->GetGUID())
	{
		g_pLog->FastSaveLog( LOG_FILE_3, "WGRetChangeSceneHandler::Execute pGamePlayer->m_HumanGUID[%d] != pPacket->GetGUID()[%d]",pGamePlayer->m_HumanGUID, pPacket->GetGUID());
		return PACKET_EXE_CONTINUE;
	}

	Obj_Human* pHuman = pGamePlayer->GetHuman();
	Scene* pScene = pHuman->getScene();
	if( !pScene )
	{
		g_pLog->FastSaveLog( LOG_FILE_3, "WGRetChangeSceneHandler::Execute pHuman->getScene() == NULL" );
		return PACKET_EXE_CONTINUE;
	}

	if( pGamePlayer->GetPlayerStatus()==PS_SERVER_WAITING_FOR_CHANGESCENE )
	{//服务器收到世界服务器发来的数据
		//检查线程执行资源是否正确
		Assert( MyGetCurrentThreadID()==g_pServerManager->m_ThreadID );

		pGamePlayer->SetPlayerStatus( PS_SERVER_WAITING_FOR_CHANGESCENE_SCENE );

		pScene->SendPacket( pPacket, PlayerID );

		g_pLog->FastSaveLog( LOG_FILE_1, "WGRetChangeSceneHandler::Execute(ret=%d) GUID=%X ServerPlayer...OK ",
			pPacket->GetReturn(), pGamePlayer->m_HumanGUID );

		return PACKET_EXE_NOTREMOVE;
	}
	else if( pGamePlayer->GetPlayerStatus()==PS_SERVER_WAITING_FOR_CHANGESCENE_SCENE )
	{//场景收到Cache里的消息
		//检查线程执行资源是否正确
		Assert( MyGetCurrentThreadID()==pScene->m_ThreadID );

		GCRetChangeScene Msg;

		Msg.SetKey( pPacket->GetKey() ) ;

		switch( pPacket->GetReturn() )
		{
		case WGRetChangeScene::CSR_SUCCESS :
			{
				Msg.SetReturn( GCRetChangeScene::CSR_SUCCESS );
				pHuman->StopCharacterLogic(FALSE);
			}
			break;
		case WGRetChangeScene::CSR_SUCCESS_DIFFSERVER :
			{
				Msg.SetReturn( GCRetChangeScene::CSR_SUCCESS_DIFFSERVER );

				SceneID_t SceneID = pPacket->GetSceneID() ;
				Assert( SceneID!=INVALID_ID );
				ID_t ServerID = g_pSceneManager->GetSceneInfo(SceneID)->m_ServerID;

				_SERVER_DATA* pData = g_pServerManager->FindServerInfo( ServerID );

				if( pData )
				{
					Msg.SetIP( pData->m_IP0 );
					Msg.SetPort( pData->m_Port0 );

					//等待客户端断开连接
					pGamePlayer->SetDirty( TRUE );

					g_pLog->FastSaveLog( LOG_FILE_1, "WGRetChangeSceneHandler::Diff Server(GUID=%X SceneID=%d IP=%s Port=%d)",
						pGamePlayer->m_HumanGUID, SceneID, pData->m_IP0, pData->m_Port0 );
				}
				else
				{
					//没找到想去的那个服务器
					Msg.SetReturn( GCRetChangeScene::CSR_ERROR );
					pHuman->SetActiveFlag( TRUE );
					pHuman->SetChangeSceneFlag( FALSE );

					pGamePlayer->SetPlayerStatus( PS_SERVER_NORMAL );
				}
			}
			break;
		case WGRetChangeScene::CSR_ERROR :
			{
				Msg.SetReturn( GCRetChangeScene::CSR_ERROR );

				//ZoneID_t zid = pScene->CalcZoneID( pHuman->getWorldPos() );
				//Assert( zid!=INVALID_ID );
				//pScene->Register( pHuman, zid );
				pHuman->SetActiveFlag( TRUE );
				pHuman->SetChangeSceneFlag( FALSE );

				pGamePlayer->SetPlayerStatus( PS_SERVER_NORMAL );
			}
			break;
		default :
			{
				Assert(FALSE);
			}
			break;
		};

		pGamePlayer->SendPacket( &Msg );

		g_pLog->FastSaveLog( LOG_FILE_1, "WGRetChangeSceneHandler::Execute(GUID=%X player=%d,ret=%d) Scene..OK ",
			pGamePlayer->m_HumanGUID, PlayerID, pPacket->GetReturn() );

		return PACKET_EXE_CONTINUE;
	}
	else
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "WGRetChangeSceneHandler::Execute No Find Player (GUID=%X player=%d,ret=%d) Scene..OK ",
			pGamePlayer->m_HumanGUID, PlayerID, pPacket->GetReturn() );
	}

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}
