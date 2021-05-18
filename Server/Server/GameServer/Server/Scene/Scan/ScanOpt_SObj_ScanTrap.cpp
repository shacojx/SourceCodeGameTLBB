#include "stdafx.h"

#include "ScanOpt_SObj_ScanTrap.h"
#include "Obj_Special.h"


using namespace Combat_Module;
using namespace Combat_Module::Special_Obj_Module;

ScanOpt_SObj_ScanTrap::ScanOpt_SObj_ScanTrap( ):
	m_pTargets(NULL),
	m_fRadius(0.0f),
	m_nCount(0),
	m_pLogic(NULL),
	m_pMe(NULL),
	m_bReady(FALSE)
{
	m_CentrePoint.CleanUp();
}

ScanOpt_SObj_ScanTrap::~ScanOpt_SObj_ScanTrap( )
{
}

BOOL ScanOpt_SObj_ScanTrap::Init( SCANOPT_SOBJ_SCANTRAP_INIT* pInit )
{
__ENTER_FUNCTION
	if(pInit==NULL)
	{
		AssertEx(FALSE,"[ScanOpt_SObj_ScanTrap::Init]: NULL init struct pointer found!!") ;
		return FALSE ;
	}
	if(NULL==pInit->m_pTargets)
	{
		AssertEx(FALSE,"[ScanOpt_SObj_ScanTrap::Init]: NULL Target LIST pointer found!!") ;
		return FALSE ;		
	}
	if(NULL==pInit->m_pMe)
	{
		AssertEx(FALSE,"[ScanOpt_SObj_ScanTrap::Init]: NULL me pointer found!!") ;
		return FALSE ;				
	}
	pInit->InitBase();
	m_pTargets = pInit->m_pTargets;
	m_fRadius = pInit->m_fRadius;
	m_nCount = pInit->m_nCount;
	m_pMe = pInit->m_pMe;
	m_pLogic = ObjSpecial_GetLogic(*m_pMe);
	if(NULL==m_pLogic)
	{
		AssertEx(FALSE,"[ScanOpt_SObj_ScanTrap::Init]: Can not find the logic for this special obj!!") ;
		return FALSE ;				
	}
	
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

UINT ScanOpt_SObj_ScanTrap::On_FindObj(Obj* pObj)
{
__ENTER_FUNCTION
	if(FALSE==m_bReady)
	{
		AssertEx(FALSE,"[ScanOpt_SObj_ScanTrap::On_FindObj]:Init first, use second! please~!!");
		return SCANRETURN_RETURN;
	}
	if(pObj==NULL)
	{
		AssertEx(FALSE,"[ScanOpt_SObj_ScanTrap::On_FindObj]: NULL Obj Pointer Found!!");
		return SCANRETURN_RETURN;
	}
	if(!IsSpecialObj(pObj->GetObjType()))
	{
		return SCANRETURN_CONTINUE;
	}
	FLOAT fDistSq = MyLengthSq(&m_CentrePoint, pObj->getWorldPos());
	if( fDistSq>(m_fRadius*m_fRadius))
	{
		//不在有效范围
		return SCANRETURN_CONTINUE;
	}
	if(FALSE == m_pLogic->IsScanedTargetValid(*m_pMe, (Obj_Special&)(*pObj)))
	{
		return SCANRETURN_CONTINUE;
	}
	BYTE& rCount = m_pTargets->m_Count;
	if(rCount<m_nCount&&m_pTargets->MAX_OBJ_LIST_SIZE>rCount)
	{
		m_pTargets->m_aObj[rCount] = (Obj_Special*)pObj;
		++rCount;
	}
__LEAVE_FUNCTION
	return SCANRETURN_CONTINUE;
}
