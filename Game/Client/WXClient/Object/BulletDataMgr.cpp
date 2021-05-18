// BulletDataMgr.cpp
//
//////////////////////////////////////////////////////

#include "StdAfx.h"
#include "BulletDataMgr.h"


CBulletDataMgr *CBulletDataMgr::s_pBulletDataMgr	= NULL;
CBulletDataMgr::CBulletDataMgr( VOID )
{
	m_pDataBase			= NULL;
	s_pBulletDataMgr		= this;
}

CBulletDataMgr::~CBulletDataMgr( VOID )
{
	s_pBulletDataMgr = NULL;
}

BOOL CBulletDataMgr::Init( const tDataBase *pDataBase )
{
	m_pDataBase = pDataBase;
	return TRUE;
}

VOID CBulletDataMgr::Term( VOID )
{
}

SBulletData *CBulletDataMgr::GetBulletData( UINT dwID )
{
	SBulletData *pSet;
	pSet = (SBulletData*)(m_pDataBase->Search_Index_EQU( dwID ));
	return pSet;
}

const SBulletData *CBulletDataMgr::GetConstBulletData( UINT dwID )const
{
	return const_cast<CBulletDataMgr*>(this)->GetBulletData( dwID );
}
