#include "stdafx.h"
// MissionManager.cpp

#include "Scene.h"
#include "LuaInterface.h"
#include "SceneManager.h"

#include "MissionManager.h"

/////////////////////////////////////////////////////
// Mission
Mission::Mission( )
{
	m_idMission	= INVALID_ID;
	m_idScript	= INVALID_ID;
}

Mission::~Mission( )
{
}

BOOL Mission::Init( MissionID_t idMission, ScriptID_t idScript, SceneID_t idScene )
{
__ENTER_FUNCTION

	Scene *pScene = g_pSceneManager->GetScene( idScene );
	if ( pScene == NULL )
	{
		Assert( pScene != NULL && "Mission::Init" );
		return FALSE;
	}

	LuaInterface *pLuaInterface = pScene->GetLuaInterface();
	pLuaInterface->ExeScript( idScript, "GetDetailInfo" );

	m_DetailInfo = *(pScene->m_pMissionData);

	m_idMission	= idMission;
	m_idScript	= idScript;

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

VOID Mission::CleanUp( )
{
	m_idMission	= INVALID_ID;
	m_idScript	= INVALID_ID;
}

///////////////////////////////////////////////////
// MissionManager
MissionManager::MissionManager( )
{
	m_uMissionLength	= 0;
	m_papMission		= NULL;
}

MissionManager::~MissionManager( )
{
	Assert( m_papMission == NULL && "MissionManager::~MissionManager" );
}

BOOL MissionManager::Init( UINT uMaxLength )
{
__ENTER_FUNCTION

	if ( uMaxLength == 0 )
	{
		Assert( uMaxLength > 0 && "MissionManager::Init" );
		return FALSE;
	}

	Assert( m_papMission == NULL && "MissionManager::Init" );

	m_papMission = new Mission* [uMaxLength];
	memset( m_papMission, 0, sizeof( Mission* ) * uMaxLength );
	m_uMissionLength = uMaxLength;

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

VOID MissionManager::CleanUp( )
{
	if ( m_papMission != NULL )
	{
		UINT i;
		for ( i = 0; i < m_uMissionLength; i++ )
		{
			if ( m_papMission[i] != NULL )
			{
				m_papMission[i]->CleanUp();
				SAFE_DELETE( m_papMission[i] );
			}
		}
		SAFE_DELETE_ARRAY( m_papMission );
	}
	m_uMissionLength = 0;
}

const Mission *MissionManager::GetMission( MissionID_t idMission, ScriptID_t idScript, SceneID_t idScene )
{
__ENTER_FUNCTION

	if ( idMission < 0 || idMission >= (INT)m_uMissionLength )
	{
		Assert( (idMission >= 0 && idMission <= (INT)m_uMissionLength) && "MissionManager::GetMission" );
		return NULL;
	}
	if ( m_papMission[idMission] == NULL )
	{
		m_papMission[idMission] = new Mission;
		BOOL bResult = m_papMission[idMission]->Init( idMission, idScript, idScene );
		if ( !bResult )
		{
			Assert( FALSE && "MissionManager::GetMission   m_papMission[idMission]->Init" );
			m_papMission[idMission]->CleanUp();
			SAFE_DELETE( m_papMission[idMission] );
			return NULL;
		}
	}
	return m_papMission[idMission];
__LEAVE_FUNCTION
	return NULL ;
}
