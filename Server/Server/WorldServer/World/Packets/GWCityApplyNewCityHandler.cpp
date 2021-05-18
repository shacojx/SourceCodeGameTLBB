#include "stdafx.h"

#include "GWCityApplyNewCity.h"
#include "Log.h"
#include "OnlineUser.h"
#include "ServerPlayer.h"
#include "ServerManager.h"
#include "WGCityError.h"
#include "CityManager.h"
#include "GuildManager.h"
#include "WGCityApplyNewCity.h"

uint GWCityApplyNewCityHandler::Execute( GWCityApplyNewCity* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		ServerPlayer* pServerPlayer = (ServerPlayer*)pPlayer;
		//入口场景ID
		SceneID_t	PortSceneID		=	pPacket->GetSceneID();
		//申请人GUID
		GUID_t		ApplicantGuid	=	pPacket->GetPlayerGuid();

		WGCityError MsgError;

		USER* pUser = g_pOnlineUser->FindUser( ApplicantGuid ) ;
		if( pUser==NULL )
		{
			Log::SaveLog( WORLD_LOGFILE, "GWCityApplyNewCityHandler...User GUID=%X not find!", 
				ApplicantGuid ) ;

			return PACKET_EXE_CONTINUE ;
		}

		//这里要判定当前任务是否有权利建立城市，当前工会是否有资格申请城市
		GuildID_t guildID = pUser->GetGuildID();
		Guild* pGuild = g_pGuildManager->GetGuild( guildID );

		//if(!pGuild)
		//{
		//	Log::SaveLog( WORLD_LOGFILE, "GWCityApplyNewCityHandler...User Guild not find! guildID = %d", 
		//		guildID ) ;
		//	return PACKET_EXE_CONTINUE ;
		//}

		//合法
		//通知Server去load城市场景，返回城市场景号
		WGCityApplyNewCity MsgToServer;
		MsgToServer.SetSceneID(PortSceneID);
		MsgToServer.SetPlayerGuid(ApplicantGuid);
		pServerPlayer->SendPacket( &MsgToServer );

		Log::SaveLog( WORLD_LOGFILE, "GWCityApplyNewCityHandler...User GUID=%X Finished", 
			ApplicantGuid ) ;

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
