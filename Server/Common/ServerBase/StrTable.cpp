#include "stdafx.h"


#include "StrTable.h"


StrTable::StrTable( )
{
__ENTER_FUNCTION

	m_pTable = NULL ;
	m_Count = 0 ;
	m_StringSize = 0 ;

__LEAVE_FUNCTION
}

StrTable::~StrTable( )
{
__ENTER_FUNCTION
	
	SAFE_DELETE_ARRAY(m_pTable) ;
	m_Count = 0 ;
	m_StringSize = 0 ;

__LEAVE_FUNCTION
}

VOID StrTable::InitTable( UINT uMaxItem, UINT uSize )
{
__ENTER_FUNCTION
	
	m_Count = uMaxItem ;
	m_StringSize = uSize ;
	m_pTable = new _TABLEITEM_STR[m_Count] ;
	Assert( m_pTable ) ;

	for( uint i=0; i<m_Count; i++ )
	{
		m_pTable[i].m_pString = new CHAR[m_StringSize] ;
		memset( m_pTable[i].m_pString, 0, sizeof(CHAR)*m_StringSize ) ;
		//m_pTable[i].m_pString = NULL ;

		m_pTable[i].m_pPtr = NULL ;
		m_pTable[i].m_Status = STRTS_EMPTY ;
	}

__LEAVE_FUNCTION
}

BOOL StrTable::Add( const CHAR* szString, VOID* pPtr )
{
__ENTER_FUNCTION

	UINT id = MyCRC( szString ) ;
	
	uint c = (id/4)%m_Count ;
	for( uint i=0; i<m_Count; i++ )
	{
		if( m_pTable[c].m_Status == STRTS_SET )
		{
			c ++ ;
			if( c>= m_Count ) c = 0 ;

			continue ;
		}

		if( m_pTable[c].m_pString == NULL )
		{
			m_pTable[c].m_pString = new CHAR[m_StringSize] ;
		}

		memset( m_pTable[c].m_pString, 0, sizeof(CHAR)*m_StringSize ) ;
		strncpy( m_pTable[c].m_pString, szString, m_StringSize-1 ) ;

		m_pTable[c].m_pPtr = pPtr ;
		m_pTable[c].m_Status = STRTS_SET ;

		return TRUE ;
	}

	return FALSE ;

__LEAVE_FUNCTION

	return FALSE ;
}

VOID* StrTable::Get( const CHAR* szString )
{
__ENTER_FUNCTION

	if( szString==NULL || szString[0]==0 )
	{
		Assert( FALSE ) ;
		return NULL ;
	}

	UINT id = MyCRC( szString ) ;
	
	uint c = (id/4)%m_Count ;
	for( uint i=0; i<m_Count; i++ )
	{
		if( m_pTable[c].m_Status == STRTS_EMPTY )
		{
			return NULL ;
		}
		else if( m_pTable[c].m_Status == STRTS_USE )
		{
			c++ ;
			if( c>=m_Count ) c = 0 ;
			continue ;
		}

		if( strcmp( m_pTable[c].m_pString, szString )==0 )
		{
			return m_pTable[c].m_pPtr ;
		}

		c++ ;
		if( c>=m_Count ) c = 0 ;
	}

__LEAVE_FUNCTION

	return NULL ;
}

VOID StrTable::Remove( const CHAR* szString )
{
__ENTER_FUNCTION

	UINT id = MyCRC( szString ) ;
	
	uint c = (id/4)%m_Count ;
	for( uint i=0; i<m_Count; i++ )
	{
		if( m_pTable[c].m_Status == STRTS_EMPTY )
		{
			return ;
		}
		else if( m_pTable[c].m_Status == STRTS_USE )
		{
			c++ ;
			if( c>=m_Count ) c = 0 ;
			continue ;
		}

		if( strcmp( m_pTable[c].m_pString, szString )==0 )
		{
			memset( m_pTable[c].m_pString, 0, sizeof(CHAR)*m_StringSize ) ;
			m_pTable[c].m_pPtr = NULL;
			m_pTable[c].m_Status = STRTS_USE ;

			return ;
		}

		c++ ;
		if( c>=m_Count ) c = 0 ;
	}

__LEAVE_FUNCTION
}

VOID StrTable::RemoveWithData( const CHAR* szString, VOID* pPrt ) 
{
__ENTER_FUNCTION

	UINT id = MyCRC( szString ) ;
	
	uint c = (id/4)%m_Count ;
	for( uint i=0; i<m_Count; i++ )
	{
		if( m_pTable[c].m_Status == STRTS_EMPTY )
		{
			return ;
		}
		else if( m_pTable[c].m_pPtr != pPrt )
		{
			c++ ;
			if( c>=m_Count ) c = 0 ;
			continue ;
		}

		if( strcmp( m_pTable[c].m_pString, szString )==0 )
		{
			uint cplus = c+1;
			if( cplus >= m_Count ) cplus = 0;
			uint cplusplus = cplus+1;
			if( cplusplus >= m_Count ) cplusplus = 0;

			if( m_pTable[cplus].m_Status == STRTS_SET && strcmp( m_pTable[cplus].m_pString, szString )==0 )
			{
				memset( m_pTable[cplus].m_pString, 0, sizeof(CHAR)*m_StringSize ) ;
				m_pTable[c].m_pPtr = m_pTable[cplus].m_pPtr;
				m_pTable[cplus].m_pPtr = NULL;
				m_pTable[cplus].m_Status = STRTS_EMPTY ;
			}
			else if( m_pTable[cplusplus].m_Status == STRTS_SET && strcmp( m_pTable[cplusplus].m_pString, szString )==0 )
			{
				memset( m_pTable[cplusplus].m_pString, 0, sizeof(CHAR)*m_StringSize ) ;
				m_pTable[c].m_pPtr = m_pTable[cplusplus].m_pPtr;
				m_pTable[cplusplus].m_pPtr = NULL;
				m_pTable[cplusplus].m_Status = STRTS_EMPTY ;
			}
			else
			{
				memset( m_pTable[c].m_pString, 0, sizeof(CHAR)*m_StringSize ) ;
				m_pTable[c].m_pPtr = NULL;
				m_pTable[c].m_Status = STRTS_EMPTY ;
			}

			return ;
		}

		c++ ;
		if( c>=m_Count ) c = 0 ;
	}

__LEAVE_FUNCTION
}

VOID StrTable::CleanUp( )
{
__ENTER_FUNCTION

	if( m_pTable == NULL )
		return ;

	for( uint i=0; i<m_Count; i++ )
	{
		memset( m_pTable[i].m_pString, 0, sizeof(CHAR)*m_StringSize ) ;
		m_pTable[i].m_pPtr = NULL ;
		m_pTable[i].m_Status = STRTS_EMPTY ;
	}

__LEAVE_FUNCTION
}

