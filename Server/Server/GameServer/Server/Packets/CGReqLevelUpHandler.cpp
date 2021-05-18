#include "stdafx.h"
#include "CGReqLevelUp.h"

#include "Type.h"
#include "Log.h"
#include "GamePlayer.h"
#include "Scene.h"
#include "Obj_Human.h"
#include "GameTable.h"
#include "GCLevelUpResult.h"
#include "GCOperateResult.h"
#include "GCLevelUp.h"
#include "ServerManager.h"
#include "PacketFactoryManager.h"
#include "GWLevelUp.h"
#include "MenPais.h"

using namespace Packets;
using namespace MenPai_Module;

uint CGReqLevelUpHandler::Execute(CGReqLevelUp* pPacket,Player* pPlayer)
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

	if(!pHuman->IsAlive())
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGReqLevelUpHandler: GUID=%X Obj=%d dead", 
			pHuman->GetGUID(), pHuman->GetID() ) ;
		return PACKET_EXE_CONTINUE;
	}

	UINT	CurrentExp =	pHuman->GetExp();

	uint	iLevel		=	pHuman->GetLevel();
	
	if(iLevel>=MAX_PLAYER_EXP_LEVEL)
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGReqLevelUpHandler: GUID=%X Obj=%d max level", 
			pHuman->GetGUID(), pHuman->GetID() ) ;
		return PACKET_EXE_CONTINUE;
	}

	UINT	LevelExp =		g_LevelExpTbl.Get(iLevel-1);	
	
	if((INVALID_ID==pHuman->GetMenPai()|| MenPai_T::ID_WUMENPAI==pHuman->GetMenPai()) && 10<=pHuman->GetLevel())
	{	//继续升级前，需要加入门派
		pHuman->SendOperateResultMsg(OR_NEED_MENPAI_FOR_LEVELUP);
	}
	else if(CurrentExp>=LevelExp)
	{
		//可以升级
		CurrentExp-=LevelExp;
		pHuman->SetExp(CurrentExp);
		pHuman->Levelup();

		pHuman->SetHP( pHuman->GetMaxHP() ) ;
		pHuman->SetMP( pHuman->GetMaxMP() ) ;

		GCLevelUpResult ResultMsg;
		ResultMsg.SetRemainExp(CurrentExp);
		ResultMsg.SetLevelUpResult(LEVELUP_SUCCESS);

		pGamePlayer->SendPacket(&ResultMsg);
		
		GCLevelUp	otherMsg;
		otherMsg.setObjId(pHuman->GetID());
		otherMsg.setLevel(pHuman->GetLevel());

		pScene->BroadCast(&otherMsg,pHuman,TRUE);
	   
		GWLevelUp* pWorldMsg = (GWLevelUp*)(g_pPacketFactoryManager->CreatePacket(PACKET_GW_LEVEUP));
		pWorldMsg->SetGUID( pHuman->GetGUID() );
		pWorldMsg->SetLevel( pHuman->GetLevel() );

		g_pServerManager->SendPacket( pWorldMsg, INVALID_ID );
	}	   
	else
	{
		GCLevelUpResult ResultMsg;
		ResultMsg.SetLevelUpResult(LEVELUP_NO_EXP);
		pGamePlayer->SendPacket(&ResultMsg);
	}

	g_pLog->FastSaveLog( LOG_FILE_1, "CGReqLevelUpHandler: GUID=%X Obj=%d CurrentExp=%d iLevel=%d", 
		pHuman->GetGUID(), pHuman->GetID(), CurrentExp, iLevel ) ;


	return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

	return	PACKET_EXE_ERROR;
}
