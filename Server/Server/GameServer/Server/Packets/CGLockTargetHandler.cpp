#include "stdafx.h"

#include "Log.h"
#include "GamePlayer.h"
#include "CGLockTarget.h"
#include "Obj_Monster.h"
#include "Scene.h"

uint CGLockTargetHandler::Execute( CGLockTarget* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

	GamePlayer* pGamePlayer = (GamePlayer*)pPlayer ;
	Assert( pGamePlayer ) ;

	Obj_Human* pHuman = pGamePlayer->GetHuman() ;

	if(NULL==pHuman)
	{
		AssertEx(FALSE,"[CGLockTargetHandler::Execute]: NULL Obj_Human pointer found!!");
		return PACKET_EXE_ERROR ;
	}
	Scene* pScene = pHuman->getScene() ;
	if( pScene==NULL )
	{
		Assert(FALSE) ;
		return PACKET_EXE_ERROR ;
	}

	//检查线程执行资源是否正确
	Assert( MyGetCurrentThreadID()==pScene->m_ThreadID ) ;

	ObjID_t nMeID = pHuman->GetID();
	if( nMeID == INVALID_ID )
	{
		Assert(FALSE) ;
		return PACKET_EXE_ERROR ;
	}
	//update target
	if(NULL==pPacket)
	{
		AssertEx(FALSE,"[CGLockTargetHandler::Execute]: NULL packet pointer found!!");
		return PACKET_EXE_ERROR ;
	}

	pHuman->SetLockedTarget(pPacket->getTargetID());
	// 当锁定的目标是NPC时将会触发NPC的泡泡说话
	Obj* pTarget = pScene->GetObjManager()->GetObj(pPacket->getTargetID());
	if (pTarget->GetObjType() == Obj::OBJ_TYPE_MONSTER)
	{
		((Obj_Monster*)pTarget)->ProcessPaoPao();
	}
	
	g_pLog->FastSaveLog( LOG_FILE_1, "CGLockTargetHandler: ok obj=%d scene=%d", 
		nMeID, pScene->SceneID() ) ;

	return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}


