#include "stdafx.h"

// DirectlyImpactMgr.cpp
//
//////////////////////////////////////////////////////

#include "DataBase\TLBB_DBC.h"

#include "DirectlyImpactMgr.h"


CDirectlyImpactMgr *CDirectlyImpactMgr::s_pDirectlyImpactMgr	= NULL;
CDirectlyImpactMgr::CDirectlyImpactMgr( VOID )
{
	m_pDBCFileLoad		= NULL;
	m_pDataBase			= NULL;
	s_pDirectlyImpactMgr	= this;
}

CDirectlyImpactMgr::~CDirectlyImpactMgr( VOID )
{
	Term();//gaoqi+
	s_pDirectlyImpactMgr = NULL;
}

BOOL CDirectlyImpactMgr::Init( const CHAR *lpszPathName )
{
	BOOL bResutl;

	m_pDBCFileLoad = new DBC::DBCFile( UINT_MAX );
	bResutl = m_pDBCFileLoad->OpenFromTXT( lpszPathName );
	if ( !bResutl )
		return FALSE;

	m_pDataBase = m_pDBCFileLoad;

	return TRUE;
}

BOOL CDirectlyImpactMgr::Init( const DBC::DBCFile *pDataBase )
{
	m_pDBCFileLoad = NULL;
	m_pDataBase = pDataBase;
	return TRUE;
}

VOID CDirectlyImpactMgr::Term( VOID )
{
	m_pDataBase = NULL;
	if ( m_pDBCFileLoad != NULL )
	{
		delete m_pDBCFileLoad;
		m_pDBCFileLoad = NULL;
	}
}

SDirectImpact *CDirectlyImpactMgr::GetDirectlyImpact( UINT dwID )
{
	SDirectImpact *pSet;
	pSet = (SDirectImpact*)(m_pDataBase->Search_Index_EQU( dwID ));
	return pSet;
}

const SDirectImpact *CDirectlyImpactMgr::GetConstDirectlyImpact( UINT dwID )const
{
	return const_cast<CDirectlyImpactMgr*>(this)->GetDirectlyImpact( dwID );
}
