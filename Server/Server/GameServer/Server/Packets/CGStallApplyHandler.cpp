#include "stdafx.h"
/*
客户端申请摆摊
*/

#include "CGStallApply.h"

#include "Type.h"
#include "Log.h"
#include "GamePlayer.h"
#include "Scene.h"
#include "Obj_Human.h"

#include "GCStallApply.h"

uint CGStallApplyHandler::Execute( CGStallApply* pPacket, Player* pPlayer )
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
	//获得本地税率
	UINT	PosTax		= 0;
	BYTE	TradeTax	= 0;
	GCStallApply	Msg;

	//验证是否有资格摆摊
	if(pHuman->GetLevel()<30)
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGStallApplyHandler::ObjID=%d level = %d ", pHuman->GetID(), pHuman->GetLevel()) ;
		return PACKET_EXE_CONTINUE ;
	}

	//摆摊地点判断
	StallInfoManager* pStallInfoMgr = pScene->GetStallInfoManager();
	if(pStallInfoMgr)
	{
		UINT PosX = (UINT)pHuman->getWorldPos()->m_fX;
		UINT PosZ = (UINT)pHuman->getWorldPos()->m_fZ;
		BOOL bCanStall = pStallInfoMgr->CanStall(PosX, PosZ);
		if(bCanStall == FALSE)
		{
			Msg.SetIsCanStall(FALSE);
		}
		else
		{
			PosTax		= pStallInfoMgr->StallPosPayment(PosX, PosZ);
			TradeTax	= pStallInfoMgr->StallExchangeTax(PosX, PosZ);

			pHuman->m_StallBox.CleanUp();
			pHuman->m_StallBox.SetStallStatus(ServerStallBox::STALL_READY);
			pHuman->m_StallBox.GetBBS()->LoadTitle();

			Msg.SetIsCanStall(TRUE);
			Msg.SetPosTax(PosTax);
			Msg.SetTradeTax(TradeTax);
		}
	}

	//验证完毕,通知客户端确定
	pGamePlayer->SendPacket(&Msg);

	g_pLog->FastSaveLog( LOG_FILE_1, "CGStallApplyHandler::ObjID=%d ", pHuman->GetID()) ;
	return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
