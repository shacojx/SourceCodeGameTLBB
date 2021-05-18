#include "stdafx.h"

#include "ScanOperator_ActiveTeammates.h"
#include "Obj_Human.h"
#include "Scene.h"


ScanOperator_ActiveTeammates::ScanOperator_ActiveTeammates( )
{
	m_nActiveTeammateCount = 0 ;
	for( INT i=0; i<MAX_TEAM_MEMBER; i++ )
	{
		m_aActiveTeammate[i] = NULL ;
	}
}

ScanOperator_ActiveTeammates::~ScanOperator_ActiveTeammates( )
{
}

BOOL ScanOperator_ActiveTeammates::Init( SCANOPERATOR_ACTIVETEAMMATES_INIT* pInit )
{
__ENTER_FUNCTION
	
	if( pInit==NULL )
	{
		Assert( FALSE ) ;
		return FALSE ;
	}

	m_Position = pInit->m_Position ;
	m_TeamID = pInit->m_TeamID ;
	m_MemberGUID = pInit->m_MemberGUID ;
	m_fRadius = pInit->m_fRadius ;


	return 	ScanOperator::Init( pInit ) ;

__LEAVE_FUNCTION

	return TRUE ;
}

//BOOL ScanOperator_ActiveTeammates::On_BeforeScan( )
//{
//__ENTER_FUNCTION
//
//	if( m_TeamID == INVALID_ID )
//	{
//		Obj_Human* pHuman = (Obj_Human*)(GetScene()->GetObjManager()->GetObj(m_MemberObjID)) ;
//		if( pHuman )
//		{
//			m_aActiveTeammate[0] = pHuman ;
//			m_nActiveTeammateCount = 1 ;
//		}
//		return FALSE ;
//	}
//
//	return TRUE ;
//
//__LEAVE_FUNCTION
//
//	return FALSE ;
//}

UINT ScanOperator_ActiveTeammates::On_FindObj( Obj* pObj )
{
__ENTER_FUNCTION

	if( pObj==NULL )
	{
		Assert( FALSE ) ;
		return SCANRETURN_RETURN ;
	}

	if( pObj->GetObjType()!=Obj::OBJ_TYPE_HUMAN )
	{
		Assert(FALSE) ;
		return SCANRETURN_RETURN ;
	}

	Obj_Human* pHuman = (Obj_Human*)pObj ;

	if( m_TeamID==INVALID_ID )
	{
		if( pHuman->GetGUID()==m_MemberGUID )
		{
			m_aActiveTeammate[0] = pHuman ;
			m_nActiveTeammateCount = 1 ;

			return SCANRETURN_RETURN ;
		}
	}
	else
	{
		Assert(m_TeamID!=INVALID_ID ) ;


		if(pHuman->__GetTeamID() != m_TeamID)
		{
			return SCANRETURN_CONTINUE;
		}
		if( 1 )
		{
			//位置之间的距离
			FLOAT fDist = MySqrt(&m_Position, pHuman->getWorldPos() ) ;
			if( fDist<m_fRadius && m_nActiveTeammateCount<MAX_TEAM_MEMBER )
			{	
				m_aActiveTeammate[m_nActiveTeammateCount] = pHuman ;
				m_nActiveTeammateCount ++ ;
			}
		}
	}

__LEAVE_FUNCTION

	return SCANRETURN_CONTINUE ;
}




