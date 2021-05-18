#include "stdafx.h"


#include "GWCityAskInitInfo.h"
#include "Log.h"
#include "OnlineUser.h"
#include "ServerPlayer.h"
#include "ServerManager.h"
#include "WGCityError.h"
#include "CityManager.h"
#include "GuildManager.h"
#include "Config.h"
#include "WGCityInitInfo.h"


uint GWCityAskInitInfoHandler::Execute( GWCityAskInitInfo* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		ServerPlayer* pServerPlayer = (ServerPlayer*)pPlayer;
		ID_t	ServerID = pServerPlayer->GetServerData()->m_ServerID;
		for(UINT i = 0; i<g_Config.m_SceneInfo.m_SceneCount; i++)
		{
			if(g_Config.m_SceneInfo.m_pScene[i].m_ServerID == ServerID)
			{//本服务器的场景
				for(INT j =0; j<MAX_CITY_PER_WORLD; j++ )
				{
					City* pCity = g_pCityManager->GetCityByIndex(j);
					if(pCity)
					{
						if(pCity->GetPortSceneID() == g_Config.m_SceneInfo.m_pScene[i].m_SceneID)
						{//当前城市是挂在这个场景上的
							/*WGCityInitInfo*/

						}
						
					}
				}
					

			}


		}
		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
