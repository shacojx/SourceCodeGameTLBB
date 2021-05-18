// ActionSetMgr.cpp
//
//////////////////////////////////////////////////////

#include "StdAfx.h"
#include "../DBC/GMDataBase.h"
#include "TLBB_DBC.h"
#include "ActionSetMgr.h"

#pragma warning(disable:4996)

CActionSetMgr *CActionSetMgr::s_pActionSetMgr	= NULL;
CActionSetMgr::CActionSetMgr( VOID )
{
	s_pActionSetMgr	= this;
}

CActionSetMgr::~CActionSetMgr( VOID )
{
	s_pActionSetMgr = NULL;
}

BOOL CActionSetMgr::Init( STRING strResPath )
{
	m_strResPath = strResPath;
	return TRUE;
}

VOID CActionSetMgr::CleanUp( VOID )
{
	CActionSetMap::iterator itCur, itEnd;
	DBC::DBCFile *pDBFile;
	itEnd = m_mapActionSet.end();
	for ( itCur = m_mapActionSet.begin(); itCur != itEnd; itCur++ )
	{
		pDBFile = (itCur->second);
		delete pDBFile;
	}
	m_mapActionSet.erase( m_mapActionSet.begin(), m_mapActionSet.end() );
}

const _DBC_CHARACTER_ACTION_SET *CActionSetMgr::GetActionSet( std::string strFileName, UINT dwID )
{
	CActionSetMap::const_iterator itFind;
	itFind = m_mapActionSet.find( strFileName );
	if ( itFind == m_mapActionSet.end() )
	{
		DBC::DBCFile *pDBCFile = new DBC::DBCFile( -1 );
		STRING strPathName = m_strResPath + strFileName;
		const CHAR *pszPathName = strPathName.c_str();
		BOOL bResult = CDataBase::Util_OpenDBCFromTxt(pszPathName, pDBCFile);
		if ( bResult )
		{
			itFind = m_mapActionSet.insert( m_mapActionSet.end(), CActionSetMap::value_type( strFileName, pDBCFile ) );
		}
		else
		{
			delete pDBCFile;
			pDBCFile = NULL;
		}
	}

	if ( itFind == m_mapActionSet.end() )
		return NULL;

	const DBC::DBCFile *pDBCFile = itFind->second;
	const _DBC_CHARACTER_ACTION_SET *pActionSet = (const _DBC_CHARACTER_ACTION_SET*)(pDBCFile->Search_Index_EQU( dwID ));
	return pActionSet;
}

const DBC::DBCFile *CActionSetMgr::GetActionSetFile(std::string strFileName)
{
	CActionSetMap::const_iterator itFind;
	itFind = m_mapActionSet.find( strFileName );
	if ( itFind == m_mapActionSet.end() )
	{
		DBC::DBCFile *pDBCFile = new DBC::DBCFile( -1 );
		STRING strPathName = m_strResPath + strFileName;
		const CHAR *pszPathName = strPathName.c_str();
		BOOL bResult = CDataBase::Util_OpenDBCFromTxt(pszPathName, pDBCFile);
		if ( bResult )
		{
			itFind = m_mapActionSet.insert( m_mapActionSet.end(), CActionSetMap::value_type( strFileName, pDBCFile ) );
		}
		else
		{
			delete pDBCFile;
			pDBCFile = NULL;
		}
	}

	if ( itFind == m_mapActionSet.end() )
		return NULL;

	return itFind->second;
}