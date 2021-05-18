// BulletDataMgr.h
//
//////////////////////////////////////////////////////

#pragma once

#include "..\DBC\GMDataBase.h"
#include "DB_Struct.h"

typedef _DBC_BULLET_DATA SBulletData;

class tDataBase;
class CBulletDataMgr
{
protected:
	static CBulletDataMgr		*s_pBulletDataMgr;
public:
	static CBulletDataMgr *GetBulletDataMgr( VOID ){
		return s_pBulletDataMgr;
	}
public:
	CBulletDataMgr( VOID );
	~CBulletDataMgr( VOID );

	BOOL Init( const tDataBase *pDataBase );
	VOID Term( VOID );

	SBulletData *GetBulletData( UINT dwID );
	const SBulletData *GetConstBulletData( UINT dwID )const;

protected:
	const tDataBase		*m_pDataBase;
};

inline CBulletDataMgr *GetBulletDataMgr( VOID ){
	return CBulletDataMgr::GetBulletDataMgr();
}
