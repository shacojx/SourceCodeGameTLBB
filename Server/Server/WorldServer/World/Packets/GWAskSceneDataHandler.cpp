#include "stdafx.h"


#include "GWAskSceneData.h"
#include "Log.h"
#include "WGRetSceneData.h"
#include "ServerPlayer.h"
#include "CityManager.h"

uint GWAskSceneDataHandler::Execute( GWAskSceneData* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	//world收到服务器对场景信息的请求
	//根据不同类型，回应不同数据
	ServerPlayer* pServerPlayer = (ServerPlayer*)pPlayer ;

	SceneID_t	SceneID		=	pPacket->GetSceneID();
	BYTE		bType		=	pPacket->GetType();
	BYTE		ExtraDataL	=	pPacket->GetExtraDataLength();
	CHAR*		ExtraData	=	pPacket->GetExtraData();

	WGRetSceneData Msg ;

	switch(bType)
	{
		case GWAskSceneData::TYPE_COPY:
			{
				Msg.SetSceneID( SceneID ) ;
				pServerPlayer->SendPacket( &Msg ) ;
			}
			break;

		case GWAskSceneData::TYPE_CITY:
			{
				//收到服务器端城市场景load成功的消息
				//world可以分配自己真实的城市结构了
				GWAskSceneData::CITY_DATA CityData;
				memcpy(&CityData, ExtraData, ExtraDataL);
				SceneID_t	PortSceneID = CityData.m_PortSceneID;
				GuildID_t	nGuildID	= CityData.m_GuildID;

				_SERVER_DATA* pServerData = pServerPlayer->GetServerData();
				City* pCity = g_pCityManager->NewCity();
				pCity->SetSceneID(SceneID);						//城市自己的ID
				pCity->SetPortSceneID(PortSceneID);				//城市的入口场景ID
				pCity->SetGuildID(nGuildID);					//城市的帮会ID
				pCity->SetServerID(pServerData->m_ServerID);	//城市所在服务器
				pCity->LoadBuildingInfo(SceneID);				//读入场景建筑信息

				SCENE_INIT_DATA CityInitData;
				
				CityInitData.m_nDataType	= SCENE_TYPE_CIT;
				CityInitData.m_CityData		= *pCity->GetCityDB();

				Msg.SetSceneID(SceneID);
				Msg.SetSceneInitData(&CityInitData);
				pServerPlayer->SendPacket( &Msg ) ;
			}
			break;

		case GWAskSceneData::TYPE_BATTLE:
			{

			}
			break;
		default:
			break;
	}

	Log::SaveLog( WORLD_LOGFILE, "GWAskSceneDataHandler SceneID=%d", 
		pPacket->GetSceneID() ) ;

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
