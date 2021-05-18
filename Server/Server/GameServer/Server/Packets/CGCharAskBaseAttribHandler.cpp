#include "stdafx.h"


#include "CGCharAskBaseAttrib.h"
#include "Log.h"
#include "GamePlayer.h"
//#include "GCCharBaseAttrib.h"
#include "Scene.h"
#include "Obj_Monster.h"




uint CGCharAskBaseAttribHandler::Execute( CGCharAskBaseAttrib* pPacket, Player* pPlayer )
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

	ObjID_t ObjID = pPacket->getTargetID() ;
	if( ObjID == INVALID_ID )
	{
		Assert(FALSE) ;
		return PACKET_EXE_ERROR ;
	}
	
	Obj* pObj = pScene->GetObjManager()->GetObj( ObjID ) ;
	if( pObj==NULL )
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGCharAskBaseAttribHandler: not find obj=%d", ObjID ) ;
		return PACKET_EXE_CONTINUE ;
	}

	if( !IsCharacterObj( pObj->GetObjType() ) )
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGCharAskBaseAttribHandler: not character obj=%d", ObjID ) ;
		return PACKET_EXE_CONTINUE ;
	}

	Obj_Character* pCharacter = (Obj_Character*)pObj ;
	pCharacter->AskMsg_BaseAttrib( pHuman );

	//GCCharBaseAttrib Msg ;
	//BOOL bResult = pCharacter->FillAttribMsg( &Msg, TRUE );
	//if ( bResult )
	//{
	//	pGamePlayer->SendPacket( &Msg ) ;

	//	g_pLog->FastSaveLog( LOG_FILE_1, "CGCharAskBaseAttribHandler: ok obj=%d scene=%d", 
	//		ObjID, pScene->SceneID() ) ;
	//}
	//else
	//{
	//	g_pLog->FastSaveLog( LOG_FILE_1, "Error: CGCharAskBaseAttribHandler: obj=%d scene=%d", 
	//		ObjID, pScene->SceneID() ) ;
	//}
	g_pLog->FastSaveLog( LOG_FILE_1, "CGCharAskBaseAttribHandler: obj=%d", ObjID ) ;

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
