#include "stdafx.h"
//请求升级心法
#include "Log.h"

#include "DataRecords.h"
#include "GameTable.h"
#include "GamePlayer.h"
#include "Scene.h"


#include "CGSetMoodToHead.h"


uint CGSetMoodToHeadHandler::Execute( CGSetMoodToHead* pPacket, Player* pPlayer )
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
	Assert( MyGetCurrentThreadID()==pScene->m_ThreadID );

	INT bView = pPacket->GetView();

	if(bView == 1)
	{
		//显示心情
		pHuman->SetTitleType(_TITLE::MOOD_TITLE);
		pHuman->SetTitle(pHuman->GetHumanRelation()->GetMood());
		
	}
	else
	{
		pHuman->SetTitleType(_TITLE::NO_TITLE);
		pHuman->SetTitle("");
	}

	g_pLog->FastSaveLog( LOG_FILE_1, "CGSetMoodToHeadHandler::ObjName=%s"
		,pHuman->GetName());

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
