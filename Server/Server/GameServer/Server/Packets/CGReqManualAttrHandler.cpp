#include "stdafx.h"
#include "CGReqManualAttr.h"
#include "Type.h"
#include "Log.h"
#include "GamePlayer.h"
#include "Scene.h"
#include "Obj_Human.h"

#include "GCManualAttrResult.h"


uint CGReqManualAttrHandler::Execute(CGReqManualAttr* pPacket,Player* pPlayer)
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
	
	
	GCManualAttrResult	Msg;
	INT nStr = pPacket->GetStr();
	INT nSpr = pPacket->GetSpr();
	INT nCon = pPacket->GetCon();
	INT nInt = pPacket->GetInt();
	INT nDex = pPacket->GetDex();
	if(FALSE == pHuman->ManualAttrPoint(nStr, nSpr, nCon, nInt, nDex))
	{
		Msg.SetResult(ATTR_RESULT_NOT_ENOUGH_REMAIN_POINT);
	}
	else
	{
		Msg.SetResult(ATTR_RESUlT_SUCCESS);
	}
	pGamePlayer->SendPacket(&Msg);

	g_pLog->FastSaveLog( LOG_FILE_1, "CGReqManualAttrHandler: Obj=%d: +Str: %d, +Spr, +Con: %d, +Int: %d, +Dex: %d, ", 
		pHuman->GetID(), nStr, nSpr, nCon, nInt, nDex);
	// force update attr here
	// ...need modify in future
	return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}
