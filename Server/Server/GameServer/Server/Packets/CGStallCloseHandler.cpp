#include "stdafx.h"
/*
该客户端收摊
*/

#include "CGStallClose.h"
#include "OResultDef.h"
#include "GCOperateResult.h"

#include "Type.h"
#include "Log.h"
#include "GamePlayer.h"
#include "Scene.h"
#include "Obj_Human.h"
#include "AI_Human.h"
#include "GCStallClose.h"

uint CGStallCloseHandler::Execute( CGStallClose* pPacket, Player* pPlayer )
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

	//验证是否有资格摆摊
	if(pHuman->m_StallBox.GetStallStatus() != ServerStallBox::STALL_OPEN)
	{//丢弃
		g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGStallCloseHandler::ObjID=%d, != ServerStallBox::STALL_OPEN"
			,pHuman->GetID()) ;
		return PACKET_EXE_CONTINUE ;
	}


	pHuman->m_StallBox.GetBBS()->SaveTitle();

	//验证完毕,设置摊位盒
	pHuman->m_StallBox.CleanUp();

	ORESULT oResult = pHuman->GetHumanAI()->PushCommand_Idle( );
	if ( OR_FAILED( oResult ) )
	{
		pHuman->SendOperateResultMsg(oResult);
	}

	GCStallClose MsgClose;
	pGamePlayer->SendPacket(&MsgClose);
	g_pLog->FastSaveLog( LOG_FILE_1, "CGStallCloseHandler::ObjID=%d"
		,pHuman->GetID()) ;
	return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
