// DetailAttrib_Player.cpp

#include "StdAfx.h"
#include "TDTimeSystem.h"
#include "..\Procedure\GameProcedure.h"
#include "SkillDataMgr.h"
#include "DetailAttrib_Player.h"
#include "..\Event\GMEventSystem.h"
#include "..\DBC\GMDataBase.h"
#include "TDException.h"
#include "..\Action\GMActionSystem.h"
#include "TDUISystem.h"
#include "..\Sound\GMSoundSystem.h"

// temp code {
// 公式模拟
UINT RunFormula( UINT dwFormulaID, UINT dwParam1, UINT dwParam2, UINT dwParam3 )
{
	return (dwParam3 - dwParam2)/((dwParam1)?(dwParam1):(1)) + dwParam2;
}

INT RunFormula( UINT dwFormulaID, UINT dwParam1, INT nParam2, INT nParam3 )
{
	return (nParam3 - nParam2)/((dwParam1)?(dwParam1):(1)) + nParam2;
}

FLOAT RunFormula( UINT dwFormulaID, UINT dwParam1, FLOAT fParam2, FLOAT fParam3 )
{
	return (fParam3 - fParam2)/((dwParam1)?(dwParam1):(1)) + fParam2;
}

CDetailAttrib_Player::CDetailAttrib_Player( VOID )
{
	m_dwCanPickMissionItemIDNum				= 0;
	UINT i;
	for ( i = 0; i < MAX_CHAR_CAN_PICK_MISSION_ITEM_NUM; i++ )
		m_adwCanPickMissionItemList[i]		= UINT_MAX;
/*
	for ( i = 0; i < COOLDOWN_LIST_SIZE; i++ )
	{
		m_anCoolDownTime[i]			= -1;
		m_anCoolDownTotalTime[i]	= -1;
	}
*/
}

CDetailAttrib_Player::~CDetailAttrib_Player( VOID )
{

}

BOOL CDetailAttrib_Player::Init( VOID )
{
	return TRUE;
}

VOID CDetailAttrib_Player::Term( VOID )
{
	m_dwCanPickMissionItemIDNum				= 0;
	UINT i;
	for ( i = 0; i < MAX_CHAR_CAN_PICK_MISSION_ITEM_NUM; i++ )
		m_adwCanPickMissionItemList[i]		= UINT_MAX;

/*
	for ( i = 0; i < COOLDOWN_LIST_SIZE; i++ )
	{
		m_anCoolDownTime[i]			= -1;
		m_anCoolDownTotalTime[i]	= -1;
	}
*/
}

VOID CDetailAttrib_Player::Tick( VOID )
{

}

VOID CDetailAttrib_Player::AddMission( const _OWN_MISSION *pMission )
{
	if ( m_listMission.m_Count == MAX_CHAR_MISSION_NUM )
		return ;

	UINT dwIndex = GetMissionIndexByID( pMission->m_idMission );
	if ( dwIndex != UINT_MAX )
		return ;

	dwIndex = UINT_MAX;
	UINT i;
	for ( i = 0; i < MAX_CHAR_MISSION_NUM; i++ )
	{
		if ( m_listMission.m_aMission[i].m_idMission == INVALID_ID )
		{
			dwIndex = i;
			break;
		}
	}

	if ( dwIndex == UINT_MAX )
		return ;

	m_listMission.m_aMission[dwIndex] = *pMission;
	m_listMission.m_Count++;
//任务增加，播放声音。
	CSoundSystemFMod::_PlayUISoundFunc(70);

	if(CGameProcedure::s_pUISystem && CGameProcedure::s_pUISystem->IsWindowShow("QuestLog"))
		CEventSystem::GetMe()->PushEvent(GE_UPDATE_MISSION);
}

VOID CDetailAttrib_Player::ModifyMission( const _OWN_MISSION *pMission )
{
	UINT dwIndex = GetMissionIndexByID( pMission->m_idMission );
	if ( dwIndex == UINT_MAX )
		return ;

	m_listMission.m_aMission[dwIndex] = *pMission;
	if(CGameProcedure::s_pUISystem && CGameProcedure::s_pUISystem->IsWindowShow("QuestLog"))
		CEventSystem::GetMe()->PushEvent(GE_UPDATE_MISSION);
}

VOID CDetailAttrib_Player::ModifyMissionData( const INT* pMissionData )
{

	memcpy((void*)m_listMission.m_aMissionData,(void*)pMissionData,sizeof(m_listMission.m_aMissionData));

	if(CGameProcedure::s_pUISystem && CGameProcedure::s_pUISystem->IsWindowShow("QuestLog"))
		CEventSystem::GetMe()->PushEvent(GE_UPDATE_MISSION);
}

extern void RemoveMissionInfo(INT nIndex);

VOID CDetailAttrib_Player::RemoveMission( MissionID_t idMission )
{
	UINT dwIndex = GetMissionIndexByID( idMission );
	if ( dwIndex != UINT_MAX && m_listMission.m_aMission[dwIndex].m_idMission != INVALID_ID )
	{
		RemoveMissionInfo(dwIndex);
		m_listMission.m_aMission[dwIndex].Cleanup();
		m_listMission.m_Count--;

		if(CGameProcedure::s_pUISystem && CGameProcedure::s_pUISystem->IsWindowShow("QuestLog"))
			CEventSystem::GetMe()->PushEvent(GE_UPDATE_MISSION);
	}
}

VOID CDetailAttrib_Player::UpdateCanPickMissionItemList( UINT dwItemCount, const UINT *paItemList )
{
	m_dwCanPickMissionItemIDNum = dwItemCount;
	memcpy( m_adwCanPickMissionItemList, paItemList, sizeof( UINT ) * m_dwCanPickMissionItemIDNum );
}

VOID CDetailAttrib_Player::AddCanPickMissionItem( UINT dwItemDataID )
{
	if ( m_dwCanPickMissionItemIDNum < MAX_CHAR_CAN_PICK_MISSION_ITEM_NUM )
	{
		m_adwCanPickMissionItemList[m_dwCanPickMissionItemIDNum++] = dwItemDataID;
	}
}

VOID CDetailAttrib_Player::RemoveCanPickMissionItem( UINT dwItemDataID )
{
	UINT i;
	for ( i= 0; i < m_dwCanPickMissionItemIDNum; i++ )
	{
		if ( m_adwCanPickMissionItemList[i] == dwItemDataID )
		{
			m_dwCanPickMissionItemIDNum--;
			m_adwCanPickMissionItemList[i] = m_adwCanPickMissionItemList[m_dwCanPickMissionItemIDNum];
			m_adwCanPickMissionItemList[m_dwCanPickMissionItemIDNum]	= UINT_MAX;
			return ;
		}
	}
}


UINT CDetailAttrib_Player::GetMissionIndexByID( MissionID_t idMission )const
{
	UINT i;
	for ( i = 0; i < MAX_CHAR_MISSION_NUM; i++ )
	{
		if ( m_listMission.m_aMission[i].m_idMission == idMission )
		{
			return i;
		}
	}
	return UINT_MAX;
}


VOID CDetailAttrib_Player::OnMissionChanged( UINT dwIndex )
{
}
