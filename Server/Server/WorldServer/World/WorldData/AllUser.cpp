#include "stdafx.h"

#include "AllUser.h"
#include "Config.h"
#include "Log.h"
#include "GuildManager.h"
#include "OnlineUser.h"

AllUser* g_pAllUser = NULL ;


AllUser::AllUser( )
{
__ENTER_FUNCTION

	m_pUsers = NULL ;
	m_uMaxUserCount = 0 ;
	m_uCurUserCount = 0 ;
	m_uSelectOffset = 0 ;

__LEAVE_FUNCTION
}

AllUser::~AllUser( )
{
__ENTER_FUNCTION

	SAFE_DELETE_ARRAY( m_pUsers ) ;
	m_uMaxUserCount = 0 ;
	m_uCurUserCount = 0 ;
	m_uSelectOffset = 0 ;

__LEAVE_FUNCTION
}

BOOL AllUser::Init( )
{
__ENTER_FUNCTION

	SAFE_DELETE_ARRAY( m_pUsers ) ;

	m_uMaxUserCount = g_Config.m_ConfigInfo.m_nMaxOfflineUserCount ;
	m_pUsers = new USER_SIMPLE_DATA[m_uMaxUserCount] ;
	if( m_pUsers==NULL )
		return FALSE ;

	m_IDTable.InitTable( m_uMaxUserCount ) ;


__LEAVE_FUNCTION

	return TRUE ;
}

VOID AllUser::CleanUp( )
{
__ENTER_FUNCTION

	m_uCurUserCount = 0 ;
	m_uSelectOffset = 0 ;

	UINT i; 
	for( i=0; i<m_uMaxUserCount; i++ )
	{
		m_pUsers[i].CleanUp() ;
	}
	m_IDTable.CleanUp() ;

__LEAVE_FUNCTION
}

//向数据库里增加一个上线玩家记录
BOOL AllUser::AddUser( const USER_SIMPLE_DATA* pSimpleUser )
{
__ENTER_FUNCTION

	if( pSimpleUser==NULL )
		return FALSE ;

	USER_SIMPLE_DATA* pUser = (USER_SIMPLE_DATA*)(m_IDTable.Get( pSimpleUser->m_GUID )) ;
	if( pUser )
	{//此用户已经存在数据库里了
		*pUser = *pSimpleUser; // 同步一下数据
		return TRUE ;
	}

	USER_SIMPLE_DATA* pNewUser = &(m_pUsers[m_uCurUserCount]) ;
	m_uCurUserCount ++ ;
	if( m_uCurUserCount>=m_uMaxUserCount )
	{
		m_uCurUserCount -- ;
		Log::SaveLog( WORLD_LOGFILE, "Warnning: AllUser record full!" ) ;
		return FALSE ;
	}

	*pNewUser = *pSimpleUser ;

	BOOL bRet = m_IDTable.Add( pNewUser->m_GUID, pNewUser ) ;
	if( bRet == FALSE )
	{
		m_uCurUserCount -- ;
		return FALSE ;
	}


__LEAVE_FUNCTION

	return TRUE ;
}

//根据guid搜索玩家
UINT AllUser::Select( GUID_t guid, RETURN_FINGER_PLAYERLIST* pPlayerList )
{
__ENTER_FUNCTION

	Assert( pPlayerList );

	USER_SIMPLE_DATA* pUserData = (USER_SIMPLE_DATA*)m_IDTable.Get( guid );
	if ( pUserData == NULL )
	{
		return 0;
	}

	pPlayerList->AddPlayerInfo( pUserData );
	FINGER_PLAYER_INFO* pPlayerInfo;
	pPlayerInfo = pPlayerList->GetLastPlayerInfo();
	Assert( pPlayerInfo );
	pPlayerInfo->SetOnlineFlag( FALSE );
	pPlayerInfo->SetGuildName( g_pGuildManager->GetGuildName(pPlayerInfo->GetGuildID()) );

	return pPlayerList->GetPlayerCount();

__LEAVE_FUNCTION

	return 0;
}

//根据名字搜索玩家
UINT AllUser::Select( const CHAR* szName, RETURN_FINGER_PLAYERLIST* pPlayerList, BOOL bPreciseFlag, INT nPosition )
{
__ENTER_FUNCTION

	Assert( szName );
	Assert( pPlayerList );

	USER* pUser = NULL;
	BOOL bOnlineFlag = FALSE;
	UINT i;

	if ( bPreciseFlag )
	{ // 精确查找只有一个
		for( i=0; i<m_uCurUserCount; ++i )
		{
			if ( strcmp( m_pUsers[i].m_Name, szName ) == 0 )
			{
				pUser = g_pOnlineUser->FindUser(szName);
				if ( pUser != NULL )
				{
					m_pUsers[i] = *(pUser->GetUserSimpleData());
					bOnlineFlag = TRUE;
				}

				pPlayerList->AddPlayerInfo( &m_pUsers[i] );
				FINGER_PLAYER_INFO* pPlayerInfo;
				pPlayerInfo = pPlayerList->GetLastPlayerInfo();
				Assert( pPlayerInfo );
				pPlayerInfo->SetOnlineFlag( bOnlineFlag );
				pPlayerInfo->SetGuildName( g_pGuildManager->GetGuildName(pPlayerInfo->GetGuildID()) );

				return pPlayerList->GetPlayerCount();
			}
		}
	}
	else
	{
		for( i=nPosition; i<m_uCurUserCount; ++i )
		{
			if ( strstr( m_pUsers[i].m_Name, szName ) != NULL )
			{
				pUser = g_pOnlineUser->FindUser(m_pUsers[i].m_Name);
				if ( pUser != NULL )
				{
					m_pUsers[i] = *(pUser->GetUserSimpleData());
					bOnlineFlag = TRUE;
				}
				else
				{
					bOnlineFlag = FALSE;
				}

				pPlayerList->AddPlayerInfo( &m_pUsers[i] );
				FINGER_PLAYER_INFO* pPlayerInfo;
				pPlayerInfo = pPlayerList->GetLastPlayerInfo();
				Assert( pPlayerInfo );
				pPlayerInfo->SetOnlineFlag( bOnlineFlag );
				pPlayerInfo->SetGuildName( g_pGuildManager->GetGuildName(pPlayerInfo->GetGuildID()) );
			}

			if ( pPlayerList->GetPlayerCount() >= FINGER_PLAYER_COUNT )
			{
				break;
			}
		}

		if ( i >= m_uCurUserCount )
		{ // 爆了
			pPlayerList->SetFingerFlag( FALSE );
		}
		else
		{
			pPlayerList->SetFingerFlag( TRUE );
			pPlayerList->SetPosition( i );
		}

		return pPlayerList->GetPlayerCount();
	}

__LEAVE_FUNCTION

	return 0;
}
