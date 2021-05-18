#include "stdafx.h"

#include "ScanOperator.h"
#include "Scene.h"

ScanOperator::ScanOperator( )
{
__ENTER_FUNCTION

	m_pScene = NULL ;
	m_ZoneID = INVALID_ID ;
	m_nZoneRadius = 0 ;
	m_bScanHuman = FALSE;
__LEAVE_FUNCTION
}

ScanOperator::~ScanOperator( )
{
__ENTER_FUNCTION

	m_pScene = NULL ;
	m_ZoneID = INVALID_ID ;
	m_nZoneRadius = 0 ;
	m_bScanHuman = FALSE;

__LEAVE_FUNCTION
}

BOOL ScanOperator::Init( SCANOPERATOR_INIT* pInit )
{
__ENTER_FUNCTION
	
	if( pInit==NULL )
	{
		Assert( FALSE ) ;
		return FALSE ;
	}

	m_pScene = pInit->m_pScene ;
	m_ZoneID = pInit->m_ZoneID ;
	m_nZoneRadius = pInit->m_nZoneRadius ;
	m_bScanHuman = pInit->m_bScanHuman;
__LEAVE_FUNCTION

	return TRUE ;
}



