#include "stdafx.h"
// CGAskDetailAbilityInfoHandler.cpp

#include "Log.h"
#include "GamePlayer.h"
#include "GCDetailAbilityInfo.h"
#include "Scene.h"
#include "HumanDB.h"
#include "AbilityManager.h"

#include "CGAskDetailAbilityInfo.h"

uint CGAskDetailAbilityInfoHandler::Execute( CGAskDetailAbilityInfo* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	GamePlayer* pGamePlayer = (GamePlayer*)pPlayer;
	Assert( pGamePlayer );

	Obj_Human* pHuman = pGamePlayer->GetHuman();
	Assert( pHuman );

	Scene* pScene = pHuman->getScene();
	if( pScene==NULL )
	{
		Assert(FALSE);
		return PACKET_EXE_ERROR;
	}

	//检查线程执行资源是否正确
	Assert( MyGetCurrentThreadID()==pScene->m_ThreadID );

	ObjID_t ObjID = pHuman->GetID();
	if( ObjID == INVALID_ID )
	{
		Assert(FALSE);
		return PACKET_EXE_ERROR;
	}

	Obj_Human *pTarget = (Obj_Human*)(pScene->GetObjManager()->GetObj(pPacket->getTargetID()));
	if( pTarget==NULL )
	{
		Assert( pTarget );
		return PACKET_EXE_ERROR;
	}

	// 权限判断
	// to be continue...
	if ( pTarget->GetObjType() != Obj::OBJ_TYPE_HUMAN )
	{
		return PACKET_EXE_ERROR;
	}

	GCDetailAbilityInfo Msg;

	Msg.setObjID( pTarget->GetID() );

	const _ABILITY_LIST *pAbilityList = pTarget->GetAbilityList();
	Msg.setAbilityList( g_pAbilityManager->MaxAbilityNum(), pAbilityList->m_aABility, pAbilityList->m_aPrescr );

	pGamePlayer->SendPacket( &Msg );

	g_pLog->FastSaveLog( LOG_FILE_1, "CGAskDetailAbilityInfoHandler: ok obj=%d scene=%d", 
		ObjID, pScene->SceneID() );

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}
