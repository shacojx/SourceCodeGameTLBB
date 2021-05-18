// BuffImpactMgr.h
//
//////////////////////////////////////////////////////

#pragma once

#include "DB_Struct.h"

namespace DBC
{
	class DBCFile;
}

class CBuffImpactMgr
{
protected:
	static CBuffImpactMgr		*s_pBuffImpactMgr;
public:
	static CBuffImpactMgr *GetBuffImpactMgr( VOID ){
		return s_pBuffImpactMgr;
	}
public:
	CBuffImpactMgr( VOID );
	~CBuffImpactMgr( VOID );

	BOOL Init( const CHAR *lpszPathName );
	BOOL Init( const DBC::DBCFile *pDataBase );
	VOID Term( VOID );

	SBuffImpact *GetBuffImpact( UINT dwID );
	const SBuffImpact *GetConstBuffImpact( UINT dwID )const;

protected:
	DBC::DBCFile		*m_pDBCFileLoad;

	const DBC::DBCFile	*m_pDataBase;
};

inline CBuffImpactMgr *GetBuffImpactMgr( VOID ){
	return CBuffImpactMgr::GetBuffImpactMgr();
}
