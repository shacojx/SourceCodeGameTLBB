#include "stdafx.h"
#include "GGSceneNotify.h"
#include "Log.h"
#include "Scene.h"
#include "SceneManager.h"



uint GGSceneNotifyHandler::Execute( GGSceneNotify* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	Assert( pPlayer==NULL ) ;
	Scene* pSourScene = g_pSceneManager->GetScene( pPacket->GetSourSceneID() ) ;
	if( pSourScene )
	{
		pSourScene->OnSceneNotify( pPacket->GetDestSceneID() ) ;
	}


	g_pLog->FastSaveLog( LOG_FILE_1, "GGSceneNotifyHandler: Sour=%d Dest=%d Ready",
		pPacket->GetSourSceneID(), pPacket->GetDestSceneID() );

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
