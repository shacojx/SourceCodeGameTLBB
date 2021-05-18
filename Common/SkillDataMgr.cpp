#include "stdafx.h"

// SkillDataMgr.cpp
//
//////////////////////////////////////////////////////

#include "DataBase\TLBB_DBC.h"

#include "SkillDataMgr.h"
#include <map>
#include <assert.h>

CSkillDataMgr *CSkillDataMgr::s_pSkillDataMgr	= NULL;
CSkillDataMgr::CSkillDataMgr( VOID )
{
	m_pDBCFileLoad		= NULL;
	m_pDataBase			= NULL;
	s_pSkillDataMgr		= this;
}

CSkillDataMgr::~CSkillDataMgr( VOID )
{
	s_pSkillDataMgr = NULL;
}

BOOL CSkillDataMgr::Init( const CHAR *lpszPathName )
{
	BOOL bResutl;

	m_pDBCFileLoad = new DBC::DBCFile( UINT_MAX );
	bResutl = m_pDBCFileLoad->OpenFromTXT( lpszPathName );
	if ( !bResutl )
		return FALSE;

	m_pDataBase = m_pDBCFileLoad;

	return TRUE;
}

BOOL CSkillDataMgr::Init( const DBC::DBCFile *pDataBase )
{
	m_pDBCFileLoad = NULL;
	m_pDataBase = pDataBase;
	return TRUE;
}

VOID CSkillDataMgr::Term( VOID )
{
	m_pDataBase = NULL;
	if ( m_pDBCFileLoad != NULL )
	{
		delete m_pDBCFileLoad;
		m_pDBCFileLoad = NULL;
	}
}

SSkillData *CSkillDataMgr::GetSkillData( UINT dwID )
{
	SSkillData *pSet;
	pSet = (SSkillData*)(m_pDataBase->Search_Index_EQU( dwID ));
	return pSet;
}

const SSkillData *CSkillDataMgr::GetConstSkillData( UINT dwID )const
{
	return const_cast<CSkillDataMgr*>(this)->GetSkillData( dwID );
}

INT CSkillDataMgr::GetRandAnim(UINT dwSkillID, INT nActionIndex/* = -1*/, BOOL bSendAnim/* = TRUE*/) const
{
	const std::vector< INT > *pvTheAnimID = GetAnimList( dwSkillID, bSendAnim );
	if ( pvTheAnimID == NULL )
	{
		return 0;
	}

	const std::vector< INT > &vTheAnimID = *pvTheAnimID;

	INT nSize = (INT)vTheAnimID.size();
	if ( nSize <= 0 )
		return 0;

	INT nRandIndex;
	if ( nActionIndex <= 0 )
		nRandIndex = 0;
	else
		nRandIndex = nActionIndex%(INT)nSize;
		
	return vTheAnimID[nRandIndex];
}

INT	CSkillDataMgr::GetAnimCount(UINT dwSkillID, BOOL bSendAnim/* = TRUE*/) const
{
	const std::vector< INT > *pvTheAnimID = GetAnimList( dwSkillID );
	if ( pvTheAnimID == NULL )
	{
		return 0;
	}
		
	return (INT)(pvTheAnimID->size());
}

const std::vector< INT > * CSkillDataMgr::GetAnimList(UINT uSkillID, BOOL bSendAnim/* = TRUE*/)const
{
	typedef std::map< UINT, std::vector< INT > > MAP_SKILLANIM;

	static MAP_SKILLANIM s_mapSkillLeadAnim, s_mapSkillSendAnim;
	MAP_SKILLANIM *pSkillAnimMap;
	if ( bSendAnim )
	{
		pSkillAnimMap = &s_mapSkillSendAnim;
	}
	else
	{
		pSkillAnimMap = &s_mapSkillLeadAnim;
	}

	MAP_SKILLANIM::const_iterator it = pSkillAnimMap->find(uSkillID);
	if(it == pSkillAnimMap->end())
	{
		static std::vector< INT > s_vEmpty;

		const SSkillData *pSet = (const SSkillData*)(m_pDataBase->Search_Index_EQU( uSkillID ));
		if(!pSet)
		{
			assert(false && "CSkillDataMgr::GetAnimList Invalid skill id");
			return NULL;
		}

		std::vector< std::string > vAnim;
		INT nNumber;
		if ( bSendAnim )
		{
			nNumber = DBC::DBCFile::_ConvertStringToVector(pSet->m_lpszSendActionSetID, vAnim, ";", TRUE, TRUE);
		}
		else
		{
			nNumber = DBC::DBCFile::_ConvertStringToVector(pSet->m_lpszGatherLeadActionSetID, vAnim, ";", TRUE, TRUE);
		}

		if(0 == nNumber)
		{
			assert(false && "CSkillDataMgr::GetAnimList Invalid skill anim set");
			return NULL;
		}

		std::vector< INT > vAnimID;
		for(INT i=0; i<(INT)vAnim.size(); i++)
		{
			vAnimID.push_back(atoi(vAnim[i].c_str()));
		}

		(*pSkillAnimMap)[uSkillID] = vAnimID;
		it = pSkillAnimMap->find(uSkillID);
	}

	if ( it == pSkillAnimMap->end() )
		return NULL;

	return &(it->second);
}
