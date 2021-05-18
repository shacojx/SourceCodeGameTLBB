#include "stdafx.h"

// BuffImpactMgr.cpp
//
//////////////////////////////////////////////////////

#include "DataBase\TLBB_DBC.h"

#include "BuffImpactMgr.h"


CBuffImpactMgr *CBuffImpactMgr::s_pBuffImpactMgr	= NULL;
CBuffImpactMgr::CBuffImpactMgr( VOID )
{
	m_pDBCFileLoad		= NULL;
	m_pDataBase			= NULL;
	s_pBuffImpactMgr	= this;
}

CBuffImpactMgr::~CBuffImpactMgr( VOID )
{
	s_pBuffImpactMgr = NULL;
}

BOOL CBuffImpactMgr::Init( const CHAR *lpszPathName )
{
	BOOL bResutl;

	m_pDBCFileLoad = new DBC::DBCFile( UINT_MAX );
	bResutl = m_pDBCFileLoad->OpenFromTXT( lpszPathName );
	if ( !bResutl )
		return FALSE;

	m_pDataBase = m_pDBCFileLoad;

	return TRUE;
}

BOOL CBuffImpactMgr::Init( const DBC::DBCFile *pDataBase )
{
	m_pDBCFileLoad = NULL;
	m_pDataBase = pDataBase;
	return TRUE;
}

VOID CBuffImpactMgr::Term( VOID )
{
	m_pDataBase = NULL;
	if ( m_pDBCFileLoad != NULL )
	{
		delete m_pDBCFileLoad;
		m_pDBCFileLoad = NULL;
	}
}

SBuffImpact *CBuffImpactMgr::GetBuffImpact( UINT dwID )
{
	SBuffImpact *pSet;
	pSet = (SBuffImpact*)(m_pDataBase->Search_Index_EQU( dwID ));
	return pSet;
}

const SBuffImpact *CBuffImpactMgr::GetConstBuffImpact( UINT dwID )const
{
	return const_cast<CBuffImpactMgr*>(this)->GetBuffImpact( dwID );
}
