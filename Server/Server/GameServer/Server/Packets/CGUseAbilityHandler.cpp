#include "stdafx.h"
#include "CGUseAbility.h"
#include "Type.h"
#include "Log.h"
#include "GamePlayer.h"
#include "Scene.h"
#include "Obj_Human.h"
#include "AI_Human.h"
#include "Ability.h"
#include "AbilityManager.h"

#include "OResultDef.h"
#include "GCAbilityResult.h"
#include "GCAbilityAction.h"
#include "ActionDelegator.h"

using namespace Action_Module;

uint CGUseAbilityHandler::Execute(CGUseAbility* pPacket,Player* pPlayer)
{
	__ENTER_FUNCTION

	GamePlayer* pGamePlayer = (GamePlayer*)pPlayer;
	Assert( pGamePlayer );

	Obj_Human* pHuman = pGamePlayer->GetHuman();
	Assert( pHuman );

	Scene* pScene = pHuman->getScene();
	if( pScene==NULL )
	{
		Assert(FALSE);
		return PACKET_EXE_ERROR;
	}

	// 检查线程执行资源是否正确
	Assert( MyGetCurrentThreadID()==pScene->m_ThreadID );

	// 玩家处于不可行动状态
	if( !pHuman->IsAlive() )
	{ // 返给玩家一条错误信息
		GCAbilityResult Msg ;
		Msg.SetAbilityID( pPacket->GetAbilityID() );
		Msg.SetPrescriptionID( pPacket->GetPrescriptionID() );
		Msg.setResult( OR_DIE ) ;
		pGamePlayer->SendPacket( &Msg ) ;

		return PACKET_EXE_CONTINUE ;
	}
	//玩家有没有别昏迷或沉默？
	if(FALSE==pHuman->CanActionFlag1() || FALSE==pHuman->CanActionFlag2())
	{
		GCAbilityResult Msg ;
		Msg.SetAbilityID( pPacket->GetAbilityID() );
		Msg.SetPrescriptionID( pPacket->GetPrescriptionID() );
		Msg.setResult(OR_U_CANNT_DO_THIS_RIGHT_NOW) ;
		pGamePlayer->SendPacket( &Msg );
		return PACKET_EXE_CONTINUE;
	}
	//玩家有没有在做别的事？
	if(FALSE==GetGlobalActionDelegator().CanDoNextAction(*pHuman))
	{
		GCAbilityResult Msg ;
		Msg.SetAbilityID( pPacket->GetAbilityID() );
		Msg.SetPrescriptionID( pPacket->GetPrescriptionID() );
		Msg.setResult(OR_BUSY) ;
		pGamePlayer->SendPacket( &Msg ) ;
		return PACKET_EXE_CONTINUE;
	}


	Ability* pAbility;

	pAbility = g_pAbilityManager->GetAbility(pPacket->GetAbilityID());
	if( pAbility == NULL )
	{ // 不存在该技能
		return PACKET_EXE_CONTINUE;
	}

	pHuman->reset_AbilityOpera();

	AbilityOpera* pAbilityOpera;
	pAbilityOpera = pHuman->GetAbilityOpera();
	pAbilityOpera->m_AbilityID = pPacket->GetAbilityID();
	pAbilityOpera->m_PresID = pPacket->GetPrescriptionID();
	pAbilityOpera->m_Obj = pPacket->GetPlatformGUID();
	pAbilityOpera->m_nMaxTime = pAbility->OperationTime();

	ORESULT res = pHuman->GetHumanAI()->PushCommand_UseAbility();

	if( res == OR_OK )
	{
		//GCAbilityAction Msg;
		//Msg.setObjID( pHuman->GetID() );
		//Msg.setAction( pAbility->AbilityID() );
		//Msg.setTargetID( pAbilityOpera->m_Obj );
		//Msg.setBeginOrEnd( GCAbilityAction::ABILITY_BEGIN );
		//pScene->BroadCast( &Msg, pHuman, FALSE );
	}

	GCAbilityResult Msg;
	Msg.SetAbilityID( pAbilityOpera->m_AbilityID );
	Msg.SetPrescriptionID( pAbilityOpera->m_PresID );
	Msg.setResult( res );
	pGamePlayer->SendPacket( &Msg );

	g_pLog->FastSaveLog( LOG_FILE_1, "CGUseAbilityHandler AbilityID=%d PrescriptionID=%d PlatformGUID=%d Result=%d",
		pPacket->GetAbilityID(), pPacket->GetPrescriptionID(), pPacket->GetPlatformGUID(), res ) ;

	return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

	return	PACKET_EXE_ERROR;
}

