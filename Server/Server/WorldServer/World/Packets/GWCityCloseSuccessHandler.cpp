#include "stdafx.h"
/*
根据CityGuid找到指定城市,解锁,回收,回消息17.WGCityCloseSuccess到Server.
*/

#include "GWCityCloseSuccess.h"
#include "Log.h"
#include "OnlineUser.h"
#include "ServerPlayer.h"
#include "ServerManager.h"
#include "WGCityError.h"
#include "CityManager.h"
#include "WGCityCloseSuccess.h"

uint GWCityCloseSuccessHandler::Execute( GWCityCloseSuccess* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		ServerPlayer* pServerPlayer = (ServerPlayer*)pPlayer;
		_CITY_GUID	CityGuid = pPacket->GetCityGuid();

		City* pDelCity = g_pCityManager->GetCityByGuid(CityGuid);
		if(!pDelCity->IsLock())
		{//已经有人在删除此城市了
			Assert(FALSE);
			return PACKET_EXE_CONTINUE ;
		}

		SceneID_t CitySceneID = pDelCity->GetSceneID();

		GUID_t PlayerGuid = pDelCity->GetApplicantGuid();
		g_pCityManager->DelCityByGuid(CityGuid);

		WGCityCloseSuccess MsgToServer;
		MsgToServer.SetPlayerGuid(PlayerGuid);
		MsgToServer.SetSceneID(CitySceneID);
		pServerPlayer->SendPacket(&MsgToServer);
		
		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
