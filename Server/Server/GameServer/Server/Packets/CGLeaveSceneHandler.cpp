#include "stdafx.h"


#include "CGLeaveScene.h"
#include "Log.h"
#include "GamePlayer.h"
#include "Scene.h"
#include "Obj_Human.h"



uint CGLeaveSceneHandler::Execute( CGLeaveScene* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	GamePlayer* pGamePlayer = (GamePlayer*)pPlayer ;
	Assert( pGamePlayer ) ;

	Obj_Human* pHuman = pGamePlayer->GetHuman() ;
	Assert( pHuman ) ;

	Scene* pScene = pHuman->getScene() ;
	if( pScene==NULL ) return PACKET_EXE_CONTINUE ;

	//检查线程执行资源是否正确
	Assert( MyGetCurrentThreadID()==pScene->m_ThreadID ) ;





	g_pLog->FastSaveLog( LOG_FILE_1, "CGLeaveSceneHandler: " ) ;

	return PACKET_EXE_ERROR ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
