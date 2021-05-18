#include "stdafx.h"
#include "CGMissionCheck.h"
#include "Type.h"
#include "Log.h"
#include "GamePlayer.h"
#include "Scene.h"
#include "Obj_Human.h"
#include "AI_Human.h"
#include "GCOperateResult.h"

using namespace Packets;
uint CGMissionCheckHandler::Execute(CGMissionCheck* pPacket,Player* pPlayer)
{
	__ENTER_FUNCTION
	

	GamePlayer* pGamePlayer = (GamePlayer*)pPlayer ;
	Assert( pGamePlayer ) ;

	Obj_Human* pHuman = pGamePlayer->GetHuman() ;
	Assert( pHuman ) ;

	Scene* pScene = pHuman->getScene() ;
	if( pScene==NULL )
	{
		Assert(FALSE) ;
		return PACKET_EXE_ERROR ;
	}
	//检查线程执行资源是否正确
	Assert( MyGetCurrentThreadID()==pScene->m_ThreadID ) ;

	ObjID_t idObj			= pHuman->GetID() ;
	ObjID_t idNPC			= pPacket->getNPCID();
	ScriptID_t idScript		= pPacket->getScriptID();
	BYTE	PetIndex		= pPacket->getPetIndex();

	ORESULT oResult = pHuman->GetHumanAI()->PushCommand_MissionCheck( idNPC, idScript, pPacket->getItemIndexList()[0], pPacket->getItemIndexList()[1], pPacket->getItemIndexList()[2], PetIndex );
	if ( OR_FAILED( oResult ) )
	{
		pHuman->SendOperateResultMsg(oResult);
	}

	g_pLog->FastSaveLog( LOG_FILE_1, "CGMissionContinueHandler: NPCID:%d ScriptID = %d ", 
		pPacket->getNPCID(), pPacket->getScriptID() ) ;

	

	return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

	return	PACKET_EXE_ERROR;
}

