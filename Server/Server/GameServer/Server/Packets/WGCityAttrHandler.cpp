#include "stdafx.h"
/*
world上对城市属性的修改通过此消息刷新到服务器
*/

#include "WGCityAttr.h"
#include "Log.h"
#include "Scene.h"
#include "Obj_Human.h"
#include "GamePlayer.h"
#include "SceneManager.h"
#include "Scene.h"
#include "ServerManager.h"
#include "PlayerPool.h"
#include "GCCityAttr.h"

uint WGCityAttrHandler::Execute( WGCityAttr* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		SceneID_t SceneID = pPacket->GetSceneID();
		Scene* pScene = g_pSceneManager->GetScene( SceneID ) ;
		if( pScene==NULL )
			return PACKET_EXE_ERROR;
		if(pScene->m_SceneInitData.m_nDataType != SCENE_TYPE_CIT)
		{
			Assert(0);
			return PACKET_EXE_ERROR;
		}

		if( pPlayer )
		{//serverplayer线程
			Assert( MyGetCurrentThreadID()==g_pServerManager->m_ThreadID );
			pScene->SendPacket(pPacket, INVALID_ID_EX);
			return PACKET_EXE_NOTREMOVE ;
		}
		else
		{//场景线程
			Assert( MyGetCurrentThreadID()==pScene->m_ThreadID ) ;

			//取出这个场景中所有玩家
			ScenePlayerManager* pCurScenePlayerManager = pScene->GetScenePlayerManager();
			PlayerID_t PlayerInTheCity[MAX_PLAYER];
			INT PlayerNum = 0;
			for(UINT i =0 ;i<pCurScenePlayerManager->GetPlayerNumber();i++ )
			{
				PlayerInTheCity[PlayerNum++] = pCurScenePlayerManager->GetPlayers()[i];
			}
			
			if(pPacket->IsUpdateAttrib(CITY_UPDATE_CITY_LEVEL))
			{
				return PACKET_EXE_CONTINUE;
			}
			if(pPacket->IsUpdateAttrib(CITY_UPDATE_CITY_NAME))
			{
				return PACKET_EXE_CONTINUE;
			}
			if(pPacket->IsUpdateAttrib(CITY_UPDATE_BUILDING))
			{
				//1.更改本地城市属性
				pScene->m_SceneInitData.m_CityData.m_Builds[pPacket->GetBuildingIndex()] = pPacket->GetBuildingData();
								
				//2.通知场景中所有玩家，建筑物发生变化
				for(INT i =0 ;i<PlayerNum;i++ )
				{
					GamePlayer* pGamePlayer = g_pPlayerPool->GetPlayer(PlayerInTheCity[i]);
					if( pGamePlayer==NULL )
					{
						Assert(FALSE);
						return PACKET_EXE_ERROR;
					}

					Obj_Human* pHuman = pGamePlayer->GetHuman();
					GCCityAttr MsgToClient;
					MsgToClient.SetUpdateAttrib(CITY_UPDATE_BUILDING, TRUE);
					MsgToClient.SetBuildingIndex(pPacket->GetBuildingIndex());
					MsgToClient.SetBuildingData(pPacket->GetBuildingData());
					pGamePlayer->SendPacket(&MsgToClient);
				}
				return PACKET_EXE_CONTINUE;
			}
		}

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
