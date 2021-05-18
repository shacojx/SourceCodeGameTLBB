// MissionDataMgr.h
//
//////////////////////////////////////////////////////

#pragma once

#include "..\DBC\GMDataBase.h"

struct SMissionData
{
public:
	// 任务ID
	// 任务名称
	// 任务等级
	// 任务城市
	// 任务简介
	// 条件字符串
	UINT		m_dwID;
	UINT		m_dwNameTextID;
	UINT		m_dwLevel;
	UINT		m_dwCityID;
	UINT		m_dwInfoTextID;
	UINT		m_dwDemandTextID;

public:
	SMissionData( VOID ){
		Reset();
	}

	VOID Reset( VOID ){
		m_dwID				= INVALID_UID;
		m_dwNameTextID		= INVALID_UID;
		m_dwLevel			= INVALID_UID;
		m_dwCityID			= INVALID_UID;
		m_dwInfoTextID		= INVALID_UID;
		m_dwDemandTextID	= INVALID_UID;
	}
};

class tDataBase;
class CMissionDataMgr
{
protected:
	static CMissionDataMgr		*s_pMissionDataMgr;
public:
	static CMissionDataMgr *GetMissionDataMgr( VOID ){
		return s_pMissionDataMgr;
	}
public:
	CMissionDataMgr( VOID );
	~CMissionDataMgr( VOID );

	BOOL Init( const tDataBase *pDataBase );

	SMissionData *GetMissionData( UINT dwID );
	const SMissionData *GetConstMissionData( UINT dwID )const;

protected:
	const tDataBase		*m_pDataBase;
};

inline CMissionDataMgr *GetMissionDataMgr( VOID ){
	return CMissionDataMgr::GetMissionDataMgr();
}
