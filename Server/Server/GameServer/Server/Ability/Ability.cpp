#include "stdafx.h"
// Ability.cpp

#include "Ability.h"
#include "GameTable.h"
#include "Scene.h"
#include "Obj_Static.h"
#include "Obj_Platform.h"
#include "HumanItemLogic.h"
#include "GCAbilityAction.h"
#include "GCAbilityResult.h"
#include "AI_Human.h"

Ability::Ability()
{
	CleanUp();
}

VOID Ability::CleanUp()
{
	m_AbilityID = 0;
	m_AbilityName = NULL;
	m_LevelDemand = 0;
	m_AbilityLevelLimit = 0;
	m_OperationTime = 0;
	m_OperationToolID = INVALID_ID;
	m_PlatformID = INVALID_ID;
	m_PlatformDistance = .0;
	m_OperationActionID = 0;
}

ORESULT Ability::CanUseAbility(Obj_Human* pHuman)
{
__ENTER_FUNCTION

	Assert( pHuman );

	AbilityOpera* pAbilityOpera;

	pAbilityOpera = pHuman->GetAbilityOpera();
	Assert( pAbilityOpera );

	// 判断玩家是否已经学会这项技能
	if( pHuman->__IsAbilityHaveLearned(m_AbilityID) == FALSE )
	{
		return OR_WARNING;
	}

	// 判断玩家是否有技能所需要的工具
	if( m_OperationToolID!=INVALID_ID && HumanItemLogic::CalcEquipItemCount(pHuman,m_OperationToolID)<1 )
	{
		return OR_NO_TOOL;
	}

	Obj* pObj;

	pObj = pHuman->getScene()->GetObjManager()->GetObj( pAbilityOpera->m_Obj );
	if( pObj != NULL )
	{
		if( pHuman->IsInValidRadius( pObj, m_PlatformDistance ) == FALSE )
		{ // 判断 pObj 是否在有效距离内
			return OR_OUT_PLATFORM;
		}

		// 让玩家面对操作台(对象)
		pHuman->FaceTo( pObj );
	}

	// 判断玩家是否符合技能的要求（在操作台旁边）
	if( (m_PlatformID >= 0) )
	{
		if( pObj == NULL )
		{
			return OR_NO_PLATFORM;
		}

		// 这里需要判断 pPlatformObj 是否就是 m_PlatformID 指定的东西
		if( pObj->GetObjType() != Obj::OBJ_TYPE_PLATFORM
		 || ((Obj_Platform*)pObj)->GetPlatformClass() != PlatformID()
		 )
		{
			return OR_NO_PLATFORM;
		}
	}

	return OR_OK;

__LEAVE_FUNCTION

	return OR_ERROR;
}

VOID Ability::OnProcInterrupt(Obj_Human* pHuman)
{
__ENTER_FUNCTION

	Assert( pHuman );

	pHuman->GetHumanAI()->PushCommand_Idle();

	AbilityOpera* pAbilityOpera;

	pAbilityOpera = pHuman->GetAbilityOpera();
	Assert( pAbilityOpera );

	GCAbilityAction StopMsg;
	StopMsg.setObjID( pHuman->GetID() );
	StopMsg.setAction( pAbilityOpera->m_AbilityID );
	StopMsg.setLogicCount( pHuman->GetLogicCount() );
	StopMsg.setTargetID( pAbilityOpera->m_Obj );
	StopMsg.setPrescriptionID( pAbilityOpera->m_PresID );
	StopMsg.setBeginOrEnd( GCAbilityAction::ABILITY_END );
	pHuman->getScene()->BroadCast( &StopMsg, pHuman, FALSE );

	GCAbilityResult Msg;
	Msg.SetAbilityID( pAbilityOpera->m_AbilityID );
	Msg.SetPrescriptionID( pAbilityOpera->m_PresID );
	Msg.setResult( OR_FAILURE );
	pHuman->GetPlayer()->SendPacket( &Msg );

	pHuman->StopCharacterLogic();

__LEAVE_FUNCTION
}
