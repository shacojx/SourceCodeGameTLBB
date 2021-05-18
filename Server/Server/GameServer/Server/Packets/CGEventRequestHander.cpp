#include "stdafx.h"
#include "CGEventRequest.h"
#include "OResultDef.h"
#include "Log.h"
#include "GamePlayer.h"
#include "Obj_Human.h"
#include "AI_Human.h"
#include "Scene.h"
#include "GCOperateResult.h"


using namespace Packets;

uint CGEventRequestHandler::Execute(CGEventRequest* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION
	GamePlayer* pGamePlayer = (GamePlayer*)pPlayer ;
	Assert( pGamePlayer != NULL ) ;
	if ( pGamePlayer == NULL )
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGEventRequestHandler: Error (pGamePlayer == NULL) " ) ;
		return PACKET_EXE_CONTINUE ;
	}

	Obj_Human* pHuman = pGamePlayer->GetHuman() ;
	Assert( pHuman != NULL ) ;
	if ( pHuman == NULL )
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGEventRequestHandler: Error (pHuman == NULL) " ) ;
		return PACKET_EXE_CONTINUE ;
	}

	Scene* pScene = pHuman->getScene() ;
	Assert( pScene != NULL ) ;
	if ( pScene == NULL )
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGEventRequestHandler: Error Obj = %d (pScene == NULL) ", pHuman->GetID() ) ;
		return PACKET_EXE_CONTINUE ;
	}

	//检查线程执行资源是否正确
	Assert( MyGetCurrentThreadID()==pScene->m_ThreadID ) ;
	if ( MyGetCurrentThreadID() != pScene->m_ThreadID )
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGEventRequestHandler: Error Obj = %d (MyGetCurrentThreadID() != pScene->m_ThreadID) ", pHuman->GetID() ) ;
		return PACKET_EXE_CONTINUE ;
	}

	ObjID_t idObj			= pHuman->GetID() ;
	ObjID_t idNPC			= pPacket->getNPCID();
	ScriptID_t idScript		= pPacket->getScriptID();
	INT exIndex				= pPacket->getExIndex();

	ORESULT oResult = pHuman->GetHumanAI()->PushCommand_EventRequest( idNPC, idScript,exIndex);
	if ( OR_FAILED( oResult ) )
	{
		pHuman->SendOperateResultMsg(oResult);

		g_pLog->FastSaveLog( LOG_FILE_1, "CGEventRequestHandler Faild: NPCID:%d ScriptID = %d ", 
			pPacket->getNPCID(), pPacket->getScriptID() ) ;
	}
	else
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGEventRequestHandler Succeed: NPCID:%d ScriptID = %d ", 
			pPacket->getNPCID(), pPacket->getScriptID() ) ;
	}

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;

}
