#include "stdafx.h"

// CampDataMgr.cpp
//
//////////////////////////////////////////////////////
//
//#include "DataBase\TLBB_DBC.h"

#include "CampDataMgr.h"
//
//
//CCampDataMgr *CCampDataMgr::s_pCampDataMgr	= NULL;
//CCampDataMgr::CCampDataMgr( VOID )
//{
//	m_pDBCFileLoad		= NULL;
//	m_pDataBase			= NULL;
//	s_pCampDataMgr	= this;
//}
//
//CCampDataMgr::~CCampDataMgr( VOID )
//{
//	s_pCampDataMgr = NULL;
//}
//
//BOOL CCampDataMgr::Init( const CHAR *lpszPathName )
//{
//	BOOL bResutl;
//
//	m_pDBCFileLoad = new DBC::DBCFile( UINT_MAX );
//	bResutl = m_pDBCFileLoad->OpenFromTXT( lpszPathName );
//	if ( !bResutl )
//		return FALSE;
//
//	m_pDataBase = m_pDBCFileLoad;
//
//	return TRUE;
//}
//
//BOOL CCampDataMgr::Init( const DBC::DBCFile *pDataBase )
//{
//	m_pDBCFileLoad = NULL;
//	m_pDataBase = pDataBase;
//	return TRUE;
//}
//
//VOID CCampDataMgr::Term( VOID )
//{
//	m_pDataBase = NULL;
//	if ( m_pDBCFileLoad != NULL )
//	{
//		delete m_pDBCFileLoad;
//		m_pDBCFileLoad = NULL;
//	}
//}
//
//SCampData *CCampDataMgr::GetCampData( UINT dwID )
//{
//	SCampData *pSet;
//	pSet = (SCampData*)(m_pDataBase->Search_Index_EQU( dwID ));
//	return pSet;
//}
//
//const SCampData *CCampDataMgr::GetConstCampData( UINT dwID )const
//{
//	return const_cast<CCampDataMgr*>(this)->GetCampData( dwID );
//}
