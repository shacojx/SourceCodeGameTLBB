#include "stdafx.h"


#include "CGStallEstablish.h"
#include "OResultDef.h"
#include "GCOperateResult.h"

#include "Type.h"
#include "Log.h"
#include "GamePlayer.h"
#include "Scene.h"
#include "Obj_Human.h"
#include "AI_Human.h"
#include "GCStallEstablish.h"

uint CGStallEstablishHandler::Execute( CGStallEstablish* pPacket, Player* pPlayer )
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

	// 通过AI判断当前状态下能否摆摊
	AI_Human* pAIHuman = (AI_Human*)(pHuman->GetAIObj());
	if (!pAIHuman) {
		return PACKET_EXE_ERROR;
	}

	ORESULT oResult = pAIHuman->PushCommand_Stall();
	if (OR_OK != oResult) {
		pHuman->SendOperateResultMsg(oResult);
		return PACKET_EXE_ERROR;
	}
	//验证是否有资格摆摊
	if(pHuman->m_StallBox.GetStallStatus() != ServerStallBox::STALL_READY)
	{
		GCStallError	Msg;
		Msg.SetID(STALL_MSG::ERR_ILLEGAL);
		pHuman->m_StallBox.CleanUp();
		pGamePlayer->SendPacket(&Msg);
		g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGStallEstablishHandler::ObjID=%d, ERR_ILLEGAL:!= ServerStallBox::STALL_READY"
			,pHuman->GetID()) ;
		return PACKET_EXE_CONTINUE ;
	}
	
	UINT	PosTax		= 0;
	BYTE	TradeTax	= 0;
	StallInfoManager* pStallInfoMgr = pScene->GetStallInfoManager();
	if(pStallInfoMgr)
	{
		UINT PosX = (UINT)pHuman->getWorldPos()->m_fX;
		UINT PosZ = (UINT)pHuman->getWorldPos()->m_fZ;
		PosTax	  = pStallInfoMgr->StallPosPayment(PosX, PosZ);
		TradeTax  = pStallInfoMgr->StallExchangeTax(PosX, PosZ);

		if(pHuman->GetMoney() < PosTax)
		{
			GCStallError	Msg;
			Msg.SetID(STALL_MSG::ERR_NOT_ENOUGH_MONEY_TO_OPEN);
			pHuman->m_StallBox.CleanUp();
			pGamePlayer->SendPacket(&Msg);
			g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGStallEstablishHandler::ObjID=%d, ERR_NOT_ENOUGH_MONEY_TO_OPEN"
				,pHuman->GetID()) ;
			return PACKET_EXE_CONTINUE ;
		}
		else
		{
			pHuman->SetMoney(pHuman->GetMoney() - PosTax);
			MONEY_LOG_PARAM	MoneyLogParam;
			MoneyLogParam.CharGUID	=	pHuman->GetGUID();
			MoneyLogParam.OPType	=	MONEY_EXCHANGE_STALL_TAX;	
			MoneyLogParam.Count		=	PosTax;
			MoneyLogParam.SceneID	=	pHuman->getScene()->SceneID();
			MoneyLogParam.XPos		=	pHuman->getWorldPos()->m_fX;
			MoneyLogParam.ZPos		=	pHuman->getWorldPos()->m_fZ;
			SaveMoneyLog(&MoneyLogParam);

		}

		//验证完毕,设置摊位盒
		pHuman->m_StallBox.SetStallStatus(ServerStallBox::STALL_OPEN);
		pHuman->m_StallBox.SetStallIsOpen(TRUE);//这个语句会引起客户端的显示更新
		pHuman->m_StallBox.SetPosTax(PosTax);
		pHuman->m_StallBox.SetTradeTax(TradeTax);
		//pHuman->m_StallBox.SetFirstPage(0);

		//占了这格，不准别人再摆摊了
		pStallInfoMgr->SetCanStall(PosX, PosZ, FALSE);
		pHuman->m_StallBox.SetStallPos(PosX, PosZ);

		//通知自己初始化自己的摊位盒
		GCStallEstablish Msg;
		pGamePlayer->SendPacket(&Msg);
		g_pLog->FastSaveLog( LOG_FILE_1, "CGStallEstablishHandler::ObjID=%d"
			,pHuman->GetID()) ;
	}
	else
	{
		Assert(0);
	}

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
