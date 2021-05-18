#include "stdafx.h"


#include "IDTable.h"


IDTable::IDTable( )
{
__ENTER_FUNCTION

	m_pTable = NULL ;
	m_Count = 0 ;

__LEAVE_FUNCTION
}

IDTable::~IDTable( )
{
__ENTER_FUNCTION
	
	SAFE_DELETE_ARRAY(m_pTable) ;
	m_Count = 0 ;

__LEAVE_FUNCTION
}

VOID IDTable::InitTable( uint MaxItem )
{
__ENTER_FUNCTION
	
	m_Count = MaxItem ;
	m_pTable = new _TABLEITEM[m_Count] ;
	Assert( m_pTable ) ;

	for( uint i=0; i<m_Count; i++ )
	{
		m_pTable[i].m_ID = INVALID_ID ;
		m_pTable[i].m_pPtr = NULL ;
		m_pTable[i].m_Status = IDTS_EMPTY ;
	}

__LEAVE_FUNCTION
}

BOOL IDTable::Add( uint id, VOID* pPtr )
{
__ENTER_FUNCTION
	
 	uint c = toIndex(id) ;
	for( uint i=0; i<m_Count; i++ )
	{
		if( m_pTable[c].m_Status == IDTS_SET )
		{
			c ++ ;
			if( c>= m_Count ) c = 0 ;

			Assert( m_pTable[c].m_ID!=id ) ;

			continue ;
		}
		
		m_pTable[c].m_ID = id ;
		m_pTable[c].m_pPtr = pPtr ;
		m_pTable[c].m_Status = IDTS_SET ;

		return TRUE ;
	}

	return FALSE ;

__LEAVE_FUNCTION

	return FALSE ;
}

VOID* IDTable::Get( uint id )
{
__ENTER_FUNCTION
	
	uint c = toIndex(id) ;
	for( uint i=0; i<m_Count; i++ )
	{
		if( m_pTable[c].m_Status == IDTS_EMPTY )
		{
			return NULL ;
		}
		else if( m_pTable[c].m_Status == IDTS_USE )
		{
			c++ ;
			if( c>=m_Count ) c = 0 ;
			continue ;
		}

		if( m_pTable[c].m_ID == id )
		{
			return m_pTable[c].m_pPtr ;
		}

		c++ ;
		if( c>=m_Count ) c = 0 ;
	}

__LEAVE_FUNCTION

	return NULL ;
}

VOID IDTable::Remove( uint id )
{
__ENTER_FUNCTION
	
	uint c = toIndex(id) ;
	for( uint i=0; i<m_Count; i++ )
	{
		if( m_pTable[c].m_Status == IDTS_EMPTY )
		{
			return ;
		}
		else if( m_pTable[c].m_Status == IDTS_USE )
		{
			c++ ;
			if( c>=m_Count ) c = 0 ;
			continue ;
		}

		if( m_pTable[c].m_ID == id )
		{
			m_pTable[c].m_ID = INVALID_ID ;
			m_pTable[c].m_pPtr = NULL;
			m_pTable[c].m_Status = IDTS_USE ;

			return ;
		}

		c++ ;
		if( c>=m_Count ) c = 0 ;
	}

__LEAVE_FUNCTION
}

VOID IDTable::CleanUp( )
{
__ENTER_FUNCTION

	if( m_pTable == NULL )
		return ;

	for( uint i=0; i<m_Count; i++ )
	{
		m_pTable[i].m_ID = INVALID_ID ;
		m_pTable[i].m_pPtr = NULL ;
		m_pTable[i].m_Status = IDTS_EMPTY ;
	}

__LEAVE_FUNCTION
}

