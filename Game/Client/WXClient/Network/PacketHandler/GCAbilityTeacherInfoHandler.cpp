
#include "StdAfx.h"
#include "GCAbilityTeacherInfo.h"

#include "..\..\DataPool\GMDataPool.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\Event\GMEventSystem.h"
#include "..\..\Object\Object.h"
#include "..\..\Object\ObjectManager.h"


uint GCAbilityTeacherInfoHandler::Execute( GCAbilityTeacherInfo* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	// 打开生活技能学习对话筐
	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		GCAbilityTeacherInfo::_AbilityTeacherInfo StudyInfo = pPacket->GetAbilityTeacherInfo();

		CDataPool::GetMe()->StudyAbility_SetID(StudyInfo.m_nAbilityID); 
		CDataPool::GetMe()->StudyAbility_SetNeedMoney(StudyInfo.m_uSpendMoney);
		CDataPool::GetMe()->StudyAbility_SetNeedExp(StudyInfo.m_uSpendExp); 
		CDataPool::GetMe()->StudyAbility_SetSkillExp(StudyInfo.m_uSkillExp); 
		CDataPool::GetMe()->StudyAbility_SetLevelLimit(StudyInfo.m_nLevelLimit);
		CDataPool::GetMe()->StudyAbility_SetScriptId(StudyInfo.m_uScriptId);
		CDataPool::GetMe()->StudyAbility_SetNpcId(StudyInfo.m_uTeacherId);

		INT nObjId = (INT)StudyInfo.m_uTeacherId;
		CObject* pObj = (CObject*)CObjectManager::GetMe()->FindServerObject(nObjId);
		if(!pObj)
			return PACKET_EXE_ERROR;

		CEventSystem::GetMe()->PushEvent(GE_TOGLE_ABILITY_STUDY);
	}
	
	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
