#include "stdafx.h"
#include "CGReqResetAttr.h"
#include "Type.h"
#include "Log.h"
#include "GamePlayer.h"
#include "Scene.h"
#include "Obj_Human.h"

#include "GCReqResetAttrResult.h"


uint	CGReqResetAttrHandler::Execute(CGReqResetAttr* pPacket,Player* pPlayer)
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
		
	//权限判断
	
	GCReqResetAttrResult	Msg;

	if(pHuman->GetLevel()>=DEFAULT_WASHPOINT_LEVEL)
	{
		//执行
		pHuman->WashPoints();
		Msg.SetAttrPoint(pHuman->Get_RemainPoints());
		Msg.SetResult(RATTR_SUCCESS);
	}
	else
	{
		Msg.SetResult(RATTR_NO_LEVEL);
	}
	


	//消息

	pGamePlayer->SendPacket(&Msg);



	g_pLog->FastSaveLog( LOG_FILE_1, "CGReqResetAttrHandler: Obj=%d Level=%d ", 
		pHuman->GetID(), pHuman->GetLevel() ) ;

	
	return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

	return	PACKET_EXE_ERROR;
}