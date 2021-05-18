#include "stdafx.h"

/*
合法，锁住该城市，关闭者PlayerGuid放到临时结构中保存
发消息15.WGCityClose,到指定Server
非法，发消息6.WGCityError通知Server

*/
#include "GWCityClose.h"
#include "Log.h"
#include "OnlineUser.h"
#include "ServerPlayer.h"
#include "ServerManager.h"
#include "WGCityError.h"
#include "CityManager.h"
#include "WGCityClose.h"
uint GWCityCloseHandler::Execute( GWCityClose* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		ServerPlayer* pServerPlayer = (ServerPlayer*)pPlayer;

		_CITY_GUID	  CityGuid		= pPacket->GetCityGuid();
		GUID_t		  PlayerGuid	= pPacket->GetPlayerGuid();
		WGCityError MsgError;

		USER* pUser = g_pOnlineUser->FindUser( PlayerGuid ) ;
		if( pUser==NULL )
		{
			Log::SaveLog( WORLD_LOGFILE, "GWCityCloseHandler...User GUID=%X not find!", 
				PlayerGuid) ;
			return PACKET_EXE_CONTINUE ;
		}

		City* pDelCity = g_pCityManager->GetCityByGuid(CityGuid);
		if(!pDelCity)
		{
			Log::SaveLog( WORLD_LOGFILE, "GWCityCloseHandler...User GUID world=%d poolpos=%d  not find!", 
				CityGuid.m_World, CityGuid.m_PoolPos) ;
			return PACKET_EXE_CONTINUE ;
		}

		if(pDelCity->IsLock())
		{//已经有人删除此城市了
			Log::SaveLog( WORLD_LOGFILE, "GWCityCloseHandler...User GUID world=%d poolpos=%d  not find!", 
				CityGuid.m_World, CityGuid.m_PoolPos) ;
			return PACKET_EXE_CONTINUE ;
		}
		
		pDelCity->SetApplicantGuid(PlayerGuid);
		pDelCity->SetLock(TRUE);
		
		WGCityClose MsgToServer;
		MsgToServer.SetSceneID(pDelCity->GetSceneID());
		MsgToServer.SetPlayerID(pUser->GetPlayerID());
		pServerPlayer->SendPacket(&MsgToServer);
		
		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
