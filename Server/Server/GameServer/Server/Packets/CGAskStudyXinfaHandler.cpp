#include "stdafx.h"
//请求升级技能
#include "DataRecords.h"
#include "GameTable.h"
#include "GamePlayer.h"
#include "Scene.h"

#include "CGAskStudyXinfa.h"
#include "GCStudyXinfa.h"
#include "ScriptLogic.h"

using namespace Script_Module;
uint CGAskStudyXinfaHandler::Execute( CGAskStudyXinfa* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	GamePlayer* pGamePlayer = (GamePlayer*)pPlayer ;
	Assert( pGamePlayer ) ;

	Obj_Human* pHuman = pGamePlayer->GetHuman() ;
	Assert( pHuman ) ;
	INT skillid = pPacket->GetUplevelInfoInfo().m_idXinfa;
	INT skilllevel	= pPacket->GetUplevelInfoInfo().m_NowLevel;
	INT NPCId = pPacket->GetUplevelInfoInfo().m_idMenpai;

	Scene* pScene = pHuman->getScene() ;
	Obj* pNPC = (Obj*) (pScene->GetObjManager()->GetObj(NPCId));
	if ( pNPC == NULL )
	{
		Log::SaveLog( SERVER_LOGFILE, "ERROR: CGAskStudyXinfaHandler::pScene->GetObjManager()->GetObj()!" ) ;
		return 0;
	}
	ScriptID_t nScript = pNPC->GetScriptID();
	INT skillidandlevel = skillid*100+skilllevel;
	g_ScriptLogic.Item_CallSkillStudyEvent(* pHuman,nScript,NPCId,skillidandlevel);
//#if 0
//	// 改变服务器数据
//	BYTE XinfaLevel = pHuman->GetDB()->GetXinfaLevel(pPacket->GetUplevelInfoInfo().m_idXinfa) + 1;
//	if( XinfaLevel >= MAX_PLAYER_EXP_LEVEL )
//	{
//		return PACKET_EXE_CONTINUE;
//	}
//	if( XinfaLevel > pHuman->GetLevel() + 5 )
//	{
//		return PACKET_EXE_CONTINUE;
//	}
//	
//	UINT nNeedMoney	= g_XinfaUpLevelSpendTbl.GetMoney( pPacket->GetUplevelInfoInfo().m_idXinfa,
//													   pPacket->GetUplevelInfoInfo().m_NowLevel+1);
//
//	UINT nNeedExp	= g_XinfaUpLevelSpendTbl.GetExp(   pPacket->GetUplevelInfoInfo().m_idXinfa,
//													   pPacket->GetUplevelInfoInfo().m_NowLevel+1);
//
//	if( pHuman->GetDB()->GetMoney() >= nNeedMoney && pHuman->GetDB()->GetExp() >= nNeedExp )
//	{
//		pHuman->GetDB()->SetXinfaUplevel( pPacket->GetUplevelInfoInfo().m_idXinfa, XinfaLevel );
//
//		UINT uSpareExp   = pHuman->GetDB()->GetExp() - nNeedExp;
//		UINT uSpareMoney = pHuman->GetDB()->GetMoney() - nNeedMoney;
//		pHuman->GetDB()->SetExp(uSpareExp );
//		pHuman->GetDB()->SetMoney(uSpareMoney );
//
//		MONEY_LOG_PARAM	MoneyLogParam;
//		MoneyLogParam.CharGUID	=	pHuman->GetGUID();
//		MoneyLogParam.OPType	=	MONEY_LEARN_XINFA_COST;
//		MoneyLogParam.Count		=	nNeedMoney;
//		MoneyLogParam.SceneID	=	pHuman->getScene()->SceneID();
//		MoneyLogParam.XPos		=	pHuman->getWorldPos()->m_fX;
//		MoneyLogParam.ZPos		=	pHuman->getWorldPos()->m_fZ;
//		SaveMoneyLog(&MoneyLogParam);
//
//		GCStudyXinfa::_STUDERESULT_INFO StudeResult;
//		StudeResult.m_uSpareExperience	= uSpareExp;
//		StudeResult.m_uSpareMoney		= uSpareMoney;
//		StudeResult.m_idXinfa			= pPacket->GetUplevelInfoInfo().m_idXinfa;
//		StudeResult.m_StudedLevel		= XinfaLevel;
//
//		GCStudyXinfa msg;
//		msg.SetStudeResultInfo(&StudeResult);
//
//		Player* pPlayer = pHuman->GetPlayer();
//		pPlayer->SendPacket( &msg ) ;
//
//	}
//#endif
	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
