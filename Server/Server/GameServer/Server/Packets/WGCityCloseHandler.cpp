#include "stdafx.h"
/*
Server首先将执行权转到场景线程，
场景线程强行将此场景中所有玩家移到此城市的入口场景中，（暂时不考虑那么多，强行移动）
清空入口场景的CityPortList中指向自己这项
清空自己的SCENE_INT_DATA,
设置m_DynamicSceneQuitTimer,等待关闭,逻辑会自动把场景设置成睡眠状态
返回消息16, GWCityCloseSuccess给World

*/

#include "WGCityClose.h"
#include "PlayerPool.h"
#include "Log.h"
#include "Scene.h"
#include "Obj_Human.h"
#include "GamePlayer.h"
#include "SceneManager.h"
#include "Scene.h"
#include "ServerManager.h"
#include "GWCityCloseSuccess.h"
#include "PacketFactoryManager.h"

uint WGCityCloseHandler::Execute( WGCityClose* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		PlayerID_t	PlayerID	= pPacket->GetPlayerID();
		SceneID_t	SceneID		= pPacket->GetSceneID();
		GamePlayer* pGamePlayer = g_pPlayerPool->GetPlayer(PlayerID);
		if( pGamePlayer==NULL ) 
		{
			Assert(FALSE);
			return PACKET_EXE_CONTINUE;
		}
		Obj_Human* pHuman = pGamePlayer->GetHuman();
		Assert( pHuman );

		Scene*	pScene		= pHuman->getScene();
		Scene*	pCityScene	= g_pSceneManager->GetScene( SceneID ) ;

		if( pScene==NULL ) return PACKET_EXE_CONTINUE;

		if( pPlayer->IsServerPlayer() )
		{//服务器收到世界服务器发来的数据
			Assert( MyGetCurrentThreadID()==g_pServerManager->m_ThreadID );
			pCityScene->SendPacket(pPacket,PlayerID);
			return PACKET_EXE_NOTREMOVE ;
		}
		else if( pPlayer->IsGamePlayer() )
		{
			Assert( MyGetCurrentThreadID()==pCityScene->m_ThreadID ) ;

			//1.场景中所有玩家移走
			ScenePlayerManager* pCurScenePlayerManager = pCityScene->GetScenePlayerManager();
			PlayerID_t PlayerInTheCity[MAX_PLAYER];
			UINT PlayerNum = 0;
			for(UINT i =0 ;i<pCurScenePlayerManager->GetPlayerNumber();i++ )
			{
				PlayerInTheCity[PlayerNum++] = pCurScenePlayerManager->GetPlayers()[i];
			}

			SceneID_t	PortSceneID = pCityScene->m_SceneInitData.m_CityData.m_PortSceneID;
			Scene*		pPortScene	= g_pSceneManager->GetScene( PortSceneID ) ;
			WORLD_POS pos(150,100);
			for(UINT i =0 ;i<PlayerNum;i++ )
			{
				GamePlayer* pGamePlayer = g_pPlayerPool->GetPlayer(PlayerInTheCity[i]);
				if( pGamePlayer==NULL )
				{
					Assert(FALSE);
					return PACKET_EXE_CONTINUE;
				}

				Obj_Human* pHuman = pGamePlayer->GetHuman();

				pHuman->ChangeScene( SceneID, PortSceneID, pos, 9 ) ;
			}

			//2.清空入口场景的CityPortList中指向自己这项
			if(!pPortScene->m_CityData.RemovePort(SceneID))
			{
				Assert(FALSE);
				return PACKET_EXE_CONTINUE;
			}

			//3.清空自己的SCENE_INT_DATA
			_CITY_GUID CityGuid = pCityScene->m_SceneInitData.m_CityData.m_Guid;
			pCityScene->m_SceneInitData.m_CityData.CleanUp(); 

			//通知world场景已经删除完毕
			GWCityCloseSuccess* pMsgToWorld = (GWCityCloseSuccess*)g_pPacketFactoryManager->CreatePacket(PACKET_GW_CITYCLOSESUCCESS);
			pMsgToWorld->SetCityGuid(CityGuid);
			g_pServerManager->SendPacket( pMsgToWorld, INVALID_ID ) ;
		}


		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
