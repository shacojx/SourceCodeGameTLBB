// MissionDataMgr.cpp
//
//////////////////////////////////////////////////////

#include "StdAfx.h"
#include "MissionDataMgr.h"


CMissionDataMgr *CMissionDataMgr::s_pMissionDataMgr	= NULL;
CMissionDataMgr::CMissionDataMgr( VOID )
{
	m_pDataBase			= NULL;
	s_pMissionDataMgr		= this;
}

CMissionDataMgr::~CMissionDataMgr( VOID )
{
	s_pMissionDataMgr = NULL;
}

BOOL CMissionDataMgr::Init( const tDataBase *pDataBase )
{
	m_pDataBase = pDataBase;
	return TRUE;
}

SMissionData *CMissionDataMgr::GetMissionData( UINT dwID )
{
	SMissionData *pSet;
	pSet = (SMissionData*)(m_pDataBase->Search_Index_EQU( dwID ));
	return pSet;
}

const SMissionData *CMissionDataMgr::GetConstMissionData( UINT dwID )const
{
	return const_cast<CMissionDataMgr*>(this)->GetMissionData( dwID );
}
