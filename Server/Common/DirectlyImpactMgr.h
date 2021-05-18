// DirectlyImpactMgr.h
//
//////////////////////////////////////////////////////

#pragma once

#include "DB_Struct.h"
class DBC::DBCFile;
class CDirectlyImpactMgr
{
protected:
	static CDirectlyImpactMgr		*s_pDirectlyImpactMgr;
public:
	static CDirectlyImpactMgr *GetDirectlyImpactMgr( VOID ){
		return s_pDirectlyImpactMgr;
	}
public:
	CDirectlyImpactMgr( VOID );
	~CDirectlyImpactMgr( VOID );

	BOOL Init( const CHAR *lpszPathName );
	BOOL Init( const DBC::DBCFile *pDataBase );
	VOID Term( VOID );

	SDirectImpact *GetDirectlyImpact( UINT dwID );
	const SDirectImpact *GetConstDirectlyImpact( UINT dwID )const;

protected:
	DBC::DBCFile		*m_pDBCFileLoad;

	const DBC::DBCFile	*m_pDataBase;
};

inline CDirectlyImpactMgr *GetDirectlyImpactMgr( VOID ){
	return CDirectlyImpactMgr::GetDirectlyImpactMgr();
}
