#include "stdafx.h"


#include "SSScenePlayerCount.h"
#include "Log.h"
#include "ServerManager.h"
#include "GamePlayer.h"
#include "SceneManager.h"



uint SSScenePlayerCountHandler::Execute( SSScenePlayerCount* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	Assert( pPlayer ) ;
	
	if( pPlayer->IsServerPlayer() )
	{
		INT count = pPacket->GetChangeCount() ;
		for( INT i=0; i<count; i++ )
		{
			SceneID_t sceneid = pPacket->GetChangeSceneID(i) ;
			INT playercount = pPacket->GetChangeScenePlayerCount(i) ;
			g_pSceneManager->SetScenePlayerCount(sceneid,playercount) ;
		}
		g_pSceneManager->SetTotalPlayerCount( pPacket->GetTotalPlayerCount() ) ;
	}

	g_pLog->FastSaveLog( LOG_FILE_1, "SSScenePlayerCountHandler ChangeSceneCount=%d TotalPlayer=%d OK ", 
		pPacket->GetChangeCount(), pPacket->GetTotalPlayerCount() ) ;


	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
