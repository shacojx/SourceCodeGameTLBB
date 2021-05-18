#include "stdafx.h"
#include "Log.h"
#include "GamePlayer.h"
#include "Scene.h"
#include "HumanDB.h"
#include "CGDoEvent.h"

using namespace Packets;

uint CGDoEventHandler::Execute( CGDoEvent* pPacket, Player* pPlayer )
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

	ObjID_t ObjID = pHuman->GetID();
	if( ObjID == INVALID_ID )
	{
		Assert(FALSE) ;
		return PACKET_EXE_ERROR ;
	}

	Obj_Character *pTarget = (Obj_Character*)(pScene->GetObjManager()->GetObj(pPacket->getTargetID()));
	if( pTarget==NULL )
	{
		Assert( pTarget ) ;
		return PACKET_EXE_ERROR ;
	}

	//// 权限判断
	//// to be continue...
	//if ( pTarget->GetObjType() != Obj::OBJ_TYPE_HUMAN
	//	&& pTarget->GetObjType() != Obj::OBJ_TYPE_PET )
	//{
	//	return PACKET_EXE_ERROR;
	//}

	//GCSkillClass	Msg;

	//Msg.setObjID( pTarget->GetID() );

	//_XINFA_LIST *pXinFaList = pTarget->GetXinfaList( );
	//Msg.setXinFa( (WORD)(pXinFaList->m_Count), pXinFaList->m_aXinFa );

	//pGamePlayer->SendPacket( &Msg ) ;

	//g_pLog->FastSaveLog( LOG_FILE_1, "CGDoEventHandler: ok obj=%d scene=%d", 
	//	ObjID, pScene->SceneID() ) ;

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
