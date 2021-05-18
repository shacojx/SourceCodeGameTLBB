#include "stdafx.h"


#include "WGCityCloseSuccess.h"
#include "PlayerPool.h"
#include "Log.h"
#include "Scene.h"
#include "Obj_Human.h"
#include "GamePlayer.h"
#include "SceneManager.h"
#include "Scene.h"
#include "ServerManager.h"
#include "GUIDManager.h"
#include "GCCityNotify.h"

uint WGCityCloseSuccessHandler::Execute( WGCityCloseSuccess* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		GUID_t		PlayerGuid	=	pPacket->GetPlayerGuid();
		SceneID_t	nSceneID	=	pPacket->GetSceneID();

		if( pPlayer->IsServerPlayer() )
		{//服务器收到世界服务器发来的数据
			Assert( MyGetCurrentThreadID()==g_pServerManager->m_ThreadID );
			Obj_Human* pHuman = (Obj_Human*)(g_pGUIDManager->Get(PlayerGuid)) ;
			Scene* pScene = pHuman->getScene();
			pScene->SendPacket(pPacket, pHuman->GetPlayerID());//玩家所在场景
			return PACKET_EXE_NOTREMOVE ;
		}
		else if( pPlayer->IsGamePlayer() )
		{
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

			Assert( MyGetCurrentThreadID()==pScene->m_ThreadID );

			Scene*	pCityScene	= g_pSceneManager->GetScene( nSceneID ) ;//城市场景
			if(!pCityScene)
			{
				Assert(FALSE) ;
				return PACKET_EXE_ERROR ;
			}

			//启动城市关闭逻辑
			pCityScene->m_CityData.m_bClose = TRUE;

			//通知申请人，已经成功关闭城市
			GCCityNotify MsgNotifySuccess;
			pGamePlayer->SendPacket(&MsgNotifySuccess);

		}

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
