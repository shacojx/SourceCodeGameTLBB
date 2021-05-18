#include "stdafx.h"

#include "ScanOperator_AESkill.h"
#include "Obj_Character.h"
#include "Obj_Monster.h"
#include "Skill_Core.h"
#include "SkillLogic.h"

using namespace Combat_Module::Skill_Module;

ScanOperator_AESkill::ScanOperator_AESkill( ):
	m_pTargets(NULL),
	m_fRadius(0.0f),
	m_nCount(0),
	m_pSkillInfo(NULL),
	m_pMe(NULL),
	m_bReady(FALSE)
{
	m_CentrePoint.CleanUp();
}

ScanOperator_AESkill::~ScanOperator_AESkill( )
{
}

BOOL ScanOperator_AESkill::Init( SCANOPERATOR_AESKILL_INIT* pInit )
{
__ENTER_FUNCTION
	if(pInit==NULL)
	{
		AssertEx(FALSE,"[ScanOperator_AESkill::Init]: NULL init struct pointer found!!") ;
		return FALSE ;
	}
	if(NULL==pInit->m_pTargets)
	{
		AssertEx(FALSE,"[ScanOperator_AESkill::Init]: NULL OBJLIST pointer found!!") ;
		return FALSE ;		
	}
	if(NULL==pInit->m_pMe)
	{
		AssertEx(FALSE,"[ScanOperator_AESkill::Init]: NULL init Me pointer found!!") ;
		return FALSE ;		
	}
	if(NULL==pInit->m_pSkillInfo)
	{
		AssertEx(FALSE,"[ScanOperator_AESkill::Init]: NULL Skill Info pointer found!!") ;
		return FALSE ;				
	}
	pInit->InitBase();
	m_pTargets = pInit->m_pTargets;
	m_fRadius = pInit->m_fRadius;
	m_nCount = pInit->m_nCount;
	m_pSkillInfo = pInit->m_pSkillInfo;
	m_pMe = pInit->m_pMe;
	m_CentrePoint.m_fX = pInit->m_CentrePoint.m_fX;
	m_CentrePoint.m_fZ = pInit->m_CentrePoint.m_fZ;
	if(TRUE==ScanOperator::Init(pInit))
	{
		m_bReady = TRUE;
		return TRUE;
	}
	m_bReady = FALSE;
__LEAVE_FUNCTION
	return FALSE;
}

UINT ScanOperator_AESkill::On_FindObj(Obj* pObj)
{
__ENTER_FUNCTION
	if(FALSE==m_bReady)
	{
		AssertEx(FALSE,"[ScanOperator_AESkill::On_FindObj]:Init first, use second! please~!!");
		return SCANRETURN_RETURN;
	}
	if(pObj==NULL)
	{
		AssertEx(FALSE,"[ScanOperator_AESkill::On_FindObj]: NULL Obj Pointer Found!!");
		return SCANRETURN_RETURN;
	}
	if(!IsCharacterObj(pObj->GetObjType()))
	{
		return SCANRETURN_CONTINUE;
	}
	FLOAT fDistSq = MyLengthSq(&m_CentrePoint, pObj->getWorldPos());
	if( fDistSq>(m_fRadius*m_fRadius))
	{
		//²»ÔÚAE·¶Î§
		return SCANRETURN_CONTINUE;
	}
	SkillLogic_T const* pSkillLogic = Skill_GetLogic(*m_pSkillInfo);
	if(FALSE == pSkillLogic->IsWantedTarget(*m_pMe, (Obj_Character&)(*pObj), *m_pSkillInfo))
	{
		return SCANRETURN_CONTINUE;
	}
	BYTE& rCount = m_pTargets->m_Count;
	if(rCount<m_nCount&&OBJLIST::MAX_OBJ_LIST_SIZE>rCount)
	{
		m_pTargets->m_aObj[rCount] = pObj;
		++rCount;
	}
__LEAVE_FUNCTION
	return SCANRETURN_CONTINUE;
}
