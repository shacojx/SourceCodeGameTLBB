#include "stdafx.h"

#include "GWAskChangeScene.h"
#include "Log.h"
#include "OnlineUser.h"

#include "ServerPlayer.h"
#include "WGRetChangeScene.h"

uint GWAskChangeSceneHandler::Execute( GWAskChangeScene* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	ServerPlayer* pServerPlayer = (ServerPlayer*)pPlayer ;

	GUID_t guid = pPacket->GetGUID() ;
	USER* pUser = g_pOnlineUser->FindUser( guid ) ;
	if( pUser )
	{
		//设置转移场景验证码
		pUser->SetKey( pPacket->GetKey() );

		WGRetChangeScene Msg ;

		Msg.SetKey(pPacket->GetKey()) ;
		Msg.SetPlayerID( pPacket->GetPlayerID() ) ;
		Msg.SetGUID( guid ) ;
		if( pPacket->GetStatus() == GWAskChangeScene::CSS_SAMESCENE )
		{//当前场景和目的场景处于相同的游戏服务器内
			SceneID_t OldSceneID = pUser->GetSceneID() ;
			pUser->SetSceneID( pPacket->GetDestSceneID() );

			g_pOnlineUser->OnUserChangeScene( pUser, OldSceneID, pPacket->GetDestSceneID() ) ;

			Msg.SetReturn( WGRetChangeScene::CSR_SUCCESS ) ;
		}
		else if( pPacket->GetStatus() == GWAskChangeScene::CSS_DIFFSERVER )
		{//当前场景和目的场景处于不同的游戏服务器内
			FULLUSERDATA* pFullUserData = g_pOnlineUser->NewData() ;
			if( pFullUserData==NULL )
			{
				Msg.SetReturn( WGRetChangeScene::CSR_ERROR ) ;
			}
			else if ( g_Config.m_SceneInfo.m_HashScene[pPacket->GetSourSceneID()] == INVALID_ID )
			{
				Msg.SetReturn( WGRetChangeScene::CSR_ERROR ) ;
			}
			else
			{
				memcpy( pFullUserData, pPacket->GetUserData(), sizeof(FULLUSERDATA) ) ;
				pUser->SetFullUserData( pFullUserData );
				pUser->SetUserStatus( US_CHANGESCENE );
				pUser->ResetKickTime();

				//修改用户数据资料
				SceneID_t OldSceneID = pUser->GetSceneID() ;
				pUser->GetFullUserData()->m_Human.m_StartScene = pPacket->GetDestSceneID() ;
				pUser->SetSceneID( pPacket->GetDestSceneID() );
				pUser->SetOldSceneID( pPacket->GetSourSceneID() );
				g_pOnlineUser->OnUserChangeScene( pUser, OldSceneID, pPacket->GetDestSceneID() ) ;

				Msg.SetSceneID( pPacket->GetDestSceneID() ) ;
				Msg.SetReturn( WGRetChangeScene::CSR_SUCCESS_DIFFSERVER ) ;
			}
		}
		else 
		{
			Assert(FALSE) ;
		}

		pServerPlayer->SendPacket( &Msg ) ;
	}
	else
	{
		Log::SaveLog( WORLD_LOGFILE, "ERROR:GWAskChangeSceneHandler...GUID=%X,S=%d,D=%d!", 
			guid, pPacket->GetSourSceneID(), pPacket->GetDestSceneID() ) ;
		return PACKET_EXE_CONTINUE ;
	}

	Log::SaveLog( WORLD_LOGFILE, "GWAskChangeSceneHandler...GUID=%X,S=%d,D=%d!", 
		guid, pPacket->GetSourSceneID(), pPacket->GetDestSceneID() ) ;

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
