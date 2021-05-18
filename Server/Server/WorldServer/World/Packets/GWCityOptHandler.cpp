#include "stdafx.h"


#include "GWCityOpt.h"
#include "Log.h"
#include "OnlineUser.h"
#include "ServerPlayer.h"
#include "ServerManager.h"
#include "WGCityError.h"
#include "CityManager.h"
#include "WGCityAttr.h"

uint GWCityOptHandler::Execute( GWCityOpt* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		ServerPlayer* pServerPlayer = (ServerPlayer*)pPlayer;
    	_CITY_GUID	CityGuid	= pPacket->GetCityGuid();
		BYTE		Opt			= pPacket->GetOpt();
		INT*		iParam		= pPacket->GetiParam();
		CHAR*		szParam		= pPacket->GetszParam();

		City* pCity = g_pCityManager->GetCityByGuid(CityGuid);
		if(!pCity)
		{
			//城市已经不存在
			Log::SaveLog( WORLD_LOGFILE, "GWCityOptHandler...ERROR: illegal city guid");
			Assert(FALSE);
			return PACKET_EXE_CONTINUE ;
		}

		switch(Opt)
		{
		case GWCityOpt::OPT_NONE:
			{
				return PACKET_EXE_CONTINUE ;
			}
			break;
		case GWCityOpt::OPT_CHANGE_BUILDING_DATA:
			{
				CityDB_t* pCityDB = pCity->GetCityDB();
				INT BuildIndex = 0;
				INT i = 0;
				for(i = 0; i<MAX_CITY_BUILDING_NUM;i++)
				{
					if(pCityDB->m_Builds[i].m_BuildingID == iParam[0])
					{
						pCityDB->m_Builds[i].m_BuildingLevel = iParam[1];
						BuildIndex = i;
						break;
					}
				}

				if(i == MAX_CITY_BUILDING_NUM)
				{
					//此城市,不存在这个建筑物.
					Assert(0);
					return PACKET_EXE_CONTINUE;
				}

				ID_t CityServerID = pCity->GetServerID();
				ServerPlayer* pCityServerPlayer = g_pServerManager->GetServerPlayer( CityServerID );

				WGCityAttr MsgToServer;
				MsgToServer.SetSceneID(pCity->GetSceneID());
				MsgToServer.SetUpdateAttrib(CITY_UPDATE_BUILDING, TRUE);
				MsgToServer.SetBuildingIndex(BuildIndex);
				MsgToServer.SetBuildingData(pCityDB->m_Builds[BuildIndex]);
				pCityServerPlayer->SendPacket(&MsgToServer);

				Log::SaveLog( WORLD_LOGFILE, "GWCityOptHandler... OPT_CHANGE_BUILDING_DATA SUCCESS");
			}
			break;
		default:
			Assert(0);
			break;

		}


		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
