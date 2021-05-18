#include "stdafx.h"

#include "GUIDManager.h"
#include "Log.h"


GUIDManager* g_pGUIDManager=NULL ;

GUIDManager::GUIDManager( )
{
__ENTER_FUNCTION

	CleanUp( ) ;

__LEAVE_FUNCTION
}

GUIDManager::~GUIDManager( )
{
__ENTER_FUNCTION

//	Assert( m_nCount == 0 ) ;

__LEAVE_FUNCTION
}

VOID GUIDManager::CleanUp( )
{
__ENTER_FUNCTION

	m_nCount = 0 ;
	for( INT i=0; i<MAX_GUID_SIZE; i++ )
	{
		m_pGuidItem[i].m_GUID = INVALID_ID ;
		m_pGuidItem[i].m_pPtr = NULL ;
		m_pGuidItem[i].m_Status = GUIDTS_EMPTY ;
	}

	m_TempGUID = 1 ;

__LEAVE_FUNCTION
}

BOOL GUIDManager::Init( )
{
__ENTER_FUNCTION

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL GUIDManager::Add( GUID_t gid, VOID* pPtr )
{
__ENTER_FUNCTION

	if( gid==INVALID_ID ) return FALSE ;

	m_Lock.Lock() ;

	uint c = ((gid&0xfffff)*2)%MAX_GUID_SIZE ;
	for( uint i=0; i<MAX_GUID_SIZE; i++ )
	{
		if( m_pGuidItem[c].m_Status == GUIDTS_SET )
		{
			c ++ ;
			if( c>= MAX_GUID_SIZE ) c = 0 ;

			if( m_pGuidItem[c].m_GUID==gid )
			{//异常，管理器里面有重复的数据
				g_pLog->FastSaveLog( LOG_FILE_3, "ERROR: GUIDManager::Add GUID=%X rep!", gid ) ;
				break ;
			}

			continue ;
		}
		
		m_pGuidItem[c].m_GUID = gid ;
		m_pGuidItem[c].m_pPtr = pPtr ;
		m_pGuidItem[c].m_Status = GUIDTS_SET ;

		m_nCount ++ ;

		m_Lock.Unlock() ;
		return TRUE ;
	}

__LEAVE_FUNCTION

	m_Lock.Unlock() ;
	return FALSE ;
}

VOID* GUIDManager::Get( GUID_t gid )
{
__ENTER_FUNCTION

	if( gid==INVALID_ID ) return NULL ;

	m_Lock.Lock() ;


	uint c = ((gid&0xfffff)*2)%MAX_GUID_SIZE ;
	for( uint i=0; i<MAX_GUID_SIZE; i++ )
	{
		if( m_pGuidItem[c].m_Status == GUIDTS_EMPTY )
		{
			m_Lock.Unlock() ;
			return NULL ;
		}
		else if( m_pGuidItem[c].m_Status == GUIDTS_USE )
		{
			c++ ;
			if( c>=MAX_GUID_SIZE ) c = 0 ;
			continue ;
		}

		if( m_pGuidItem[c].m_GUID == gid )
		{
			VOID* p = m_pGuidItem[c].m_pPtr ;
			m_Lock.Unlock() ;
			return p ;
		}

		c++ ;
		if( c>=MAX_GUID_SIZE ) c = 0 ;
	}

__LEAVE_FUNCTION

	m_Lock.Unlock() ;
	return NULL ;
}

BOOL GUIDManager::Del( GUID_t gid, VOID* pPtr )
{
__ENTER_FUNCTION

	if( gid==INVALID_ID ) return FALSE ;

	m_Lock.Lock() ;

	uint c = ((gid&0xfffff)*2)%MAX_GUID_SIZE ;
	for( uint i=0; i<MAX_GUID_SIZE; i++ )
	{
		if( m_pGuidItem[c].m_Status == GUIDTS_EMPTY )
		{
			m_Lock.Unlock() ;
			return FALSE ;
		}
		else if( m_pGuidItem[c].m_Status == GUIDTS_USE )
		{
			c++ ;
			if( c>=MAX_GUID_SIZE ) c = 0 ;
			continue ;
		}

		if( m_pGuidItem[c].m_GUID == gid && m_pGuidItem[c].m_pPtr==pPtr )
		{
			m_pGuidItem[c].m_GUID = INVALID_ID ;
			m_pGuidItem[c].m_pPtr = NULL;
			m_pGuidItem[c].m_Status = GUIDTS_USE ;

			m_nCount -- ;

			m_Lock.Unlock() ;
			return TRUE ;
		}

		c++ ;
		if( c>=MAX_GUID_SIZE ) c = 0 ;
	}


__LEAVE_FUNCTION

	m_Lock.Unlock() ;
	return FALSE ;
}


