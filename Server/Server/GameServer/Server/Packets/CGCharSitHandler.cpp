#include "stdafx.h"


#include "CGCharSit.h"
#include "OResultDef.h"
#include "GamePlayer.h"
#include "Scene.h"
#include "Obj_Human.h"
#include "AI_Human.h"
#include "Log.h"
#include "GCOperateResult.h"


uint CGCharSitHandler::Execute( CGCharSit* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION
	//GamePlayer* pGamePlayer = (GamePlayer*)pPlayer ;
	//Assert( pGamePlayer != NULL ) ;
	//if ( pGamePlayer == NULL )
	//{
	//	g_pLog->FastSaveLog( LOG_FILE_1, "CGCharSitHandler: Error (pGamePlayer == NULL) " ) ;
	//	return PACKET_EXE_CONTINUE ;
	//}

	//Obj_Human* pHuman = pGamePlayer->GetHuman() ;
	//Assert( pHuman != NULL ) ;
	//if ( pHuman == NULL )
	//{
	//	g_pLog->FastSaveLog( LOG_FILE_1, "CGCharSitHandler: Error (pHuman == NULL) " ) ;
	//	return PACKET_EXE_CONTINUE ;
	//}

	//Scene* pScene = pHuman->getScene() ;
	//Assert( pScene != NULL ) ;
	//if ( pScene == NULL )
	//{
	//	g_pLog->FastSaveLog( LOG_FILE_1, "CGCharSitHandler: Error Obj = %d (pScene == NULL) ", pHuman->GetID() ) ;
	//	return PACKET_EXE_CONTINUE ;
	//}

	////检查线程执行资源是否正确
	//Assert( MyGetCurrentThreadID()==pScene->m_ThreadID ) ;
	//if ( MyGetCurrentThreadID() != pScene->m_ThreadID )
	//{
	//	g_pLog->FastSaveLog( LOG_FILE_1, "CGCharSitHandler: Error Obj = %d (MyGetCurrentThreadID() != pScene->m_ThreadID) ", pHuman->GetID() ) ;
	//	return PACKET_EXE_CONTINUE ;
	//}

	//ORESULT oResult = pHuman->GetHumanAI()->PushCommand_Sit(pPacket->IsSit());
	//if ( OR_FAILED( oResult ) )
	//{
	//	pHuman->SendOperateResultMsg(oResult);
	//}

	//g_pLog->FastSaveLog( LOG_FILE_1, "CGCharSitHandler: ok obj=%d guid=%X scene=%d", 
	//	pHuman->GetID(), pHuman->GetGUID(), pScene->SceneID() ) ;

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
