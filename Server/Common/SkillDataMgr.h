// SkillDataMgr.h
//
//////////////////////////////////////////////////////

#pragma once

#include "DB_Struct.h"
#include "DataBase\TLBB_DBC.h"

class DBC::DBCFile;
class CSkillDataMgr
{
protected:
	static CSkillDataMgr		*s_pSkillDataMgr;
public:
	static CSkillDataMgr *GetSkillDataMgr( VOID ){
		return s_pSkillDataMgr;
	}
public:
	CSkillDataMgr( VOID );
	~CSkillDataMgr( VOID );

	BOOL Init( const CHAR *lpszPathName );
	BOOL Init( const DBC::DBCFile *pDataBase );
	VOID Term( VOID );

	SSkillData *GetSkillData( UINT dwID );
	const SSkillData *GetConstSkillData( UINT dwID )const;

	INT	GetRandAnim(UINT dwSkillID, INT nActionIndex = -1, BOOL bSendAnim = TRUE) const;
	INT	GetAnimCount(UINT dwSkillID, BOOL bSendAnim = TRUE) const;

protected:
	const std::vector< INT > *GetAnimList(UINT uSkillID, BOOL bSendAnim = TRUE)const;

protected:
	DBC::DBCFile			*m_pDBCFileLoad;
	const DBC::DBCFile		*m_pDataBase;
};

inline CSkillDataMgr *GetSkillDataMgr( VOID ){
	return CSkillDataMgr::GetSkillDataMgr();
}
