#include "stdafx.h"

#include "Log.h"
#include "GamePlayer.h"
#include "GCMissionList.h"
#include "Scene.h"
#include "CGAskMissionList.h"
#include "GCMissionModify.h"



using namespace Packets;


uint CGAskMissionListHandler::Execute( CGAskMissionList* pPacket, Player* pPlayer )
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


	GCMissionList	Msg;
	Msg.SetObjID( pHuman->GetID() );

	UINT i;
	const _OWN_MISSION *pMission;
	for ( i = 0; i < MAX_CHAR_MISSION_NUM; i++ )
	{
		pMission = pHuman->GetMission( i );
		if ( pMission->m_idMission != INVALID_ID )
		{
			Msg.AddMission( (BYTE)i, pMission );
		}
	}
	pGamePlayer->SendPacket( &Msg ) ;

	GCMissionModify msgMissionModify;
	msgMissionModify.SetFlag(1) ;
	msgMissionModify.SetMissionData( pHuman->GetConstMissionList()->m_aMissionData );
	pGamePlayer->SendPacket( &msgMissionModify );

	g_pLog->FastSaveLog( LOG_FILE_1, "CGAskMissionListHandler: ok GUID=%X scene=%d PID=%d", 
		pGamePlayer->m_HumanGUID, pScene->SceneID(), pGamePlayer->PlayerID() ) ;

	return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
