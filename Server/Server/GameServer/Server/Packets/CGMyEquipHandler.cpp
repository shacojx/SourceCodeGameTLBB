#include "stdafx.h"



#include "CGMyEquip.h"
#include "GamePlayer.h"
#include "Scene.h"
#include "Human.h"
#include "GCMyEquip.h"
#include "Log.h"


uint CGMyEquipHandler::Execute( CGMyEquip* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	GamePlayer* pGamePlayer = (GamePlayer*)pPlayer ;
	Assert( pGamePlayer ) ;

	Human* pHuman = pGamePlayer->GetHuman() ;
	Assert( pHuman ) ;

	Scene* pScene = pHuman->getScene() ;
	Assert( pScene ) ;

	//检查线程执行资源是否正确
	Assert( MyGetCurrentThreadID()==pScene->m_ThreadID ) ;

	ObjID_t ObjID = pHuman->GetID() ;
	if( ObjID != pPacket->getObjID() )
	{
		Assert(FALSE) ;
		return PACKET_EXE_ERROR ;
	}

	GCMyEquip Msg ;
	Msg.setObjID( ObjID ) ;
	Msg.setName( pHuman->GetName() ) ;
	Msg.setDirectory( pHuman->getDir() ) ;
//其他信息设置

	pGamePlayer->SendPacket( &Msg ) ;

	g_pLog->FastSaveLog( LOG_FILE_1, "CGMyEquipHandler::ObjID=%d\r\n", ObjID ) ;


	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
