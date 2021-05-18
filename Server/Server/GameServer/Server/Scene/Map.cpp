#include "stdafx.h"

#include "Map.h"
#include "File.h"
#include "Ini.h"

#ifndef __PATHFINDER_H__
#include "PathFinder.h"
#endif

Map::Map( )
{
__ENTER_FUNCTION

	m_CX = 0 ;
	m_CZ = 0 ;

	m_pPathFinder = NULL;

__LEAVE_FUNCTION
}

Map::~Map( )
{
__ENTER_FUNCTION

	SAFE_DELETE(m_pPathFinder);

__LEAVE_FUNCTION
}

VOID Map::CleanUp( )
{
__ENTER_FUNCTION



__LEAVE_FUNCTION
}

BOOL Map::Load( CHAR* filename )
{
__ENTER_FUNCTION

	if( m_pPathFinder==NULL )
	{
		m_pPathFinder = new PathFinder(this,filename,m_CX,m_CZ);
		if( m_pPathFinder==NULL )
		{
			Assert(FALSE) ;
			return FALSE ;
		}
	}

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL Map::IsCanGo(const WORLD_POS& pos, const INT nLevel)
{
__ENTER_FUNCTION

	if(m_pPathFinder)
		return m_pPathFinder->IsCanGo(pos,nLevel);

	Assert(m_pPathFinder);
	return FALSE;

__LEAVE_FUNCTION

	return FALSE;
}

BOOL Map::FindEmptyPos( WORLD_POS* pos, const INT nLevel )
{
__ENTER_FUNCTION

#define MAX_FIND_SIZE 64

	if( IsCanGo( *pos,nLevel ) )
		return TRUE ;

	for( INT i=0; i<MAX_FIND_SIZE; i++ )
	{
		WORLD_POS pTest ;

		pTest.m_fX = pos->m_fX+i ;
		pTest.m_fZ = pos->m_fZ ;
		VerifyPos( &pTest ) ;
		if( IsCanGo( pTest,nLevel ) )
		{
			*pos = pTest ;
			return TRUE ;
		}
		pTest.m_fX = pos->m_fX ;
		pTest.m_fZ = pos->m_fZ+i ;
		VerifyPos( &pTest ) ;
		if( IsCanGo( pTest,nLevel ) )
		{
			*pos = pTest ;
			return TRUE ;
		}

		pTest.m_fX = pos->m_fX-i ;
		pTest.m_fZ = pos->m_fZ ;
		VerifyPos( &pTest ) ;
		if( IsCanGo( pTest,nLevel ) )
		{
			*pos = pTest ;
			return TRUE ;
		}
		pTest.m_fX = pos->m_fX ;
		pTest.m_fZ = pos->m_fZ-i ;
		VerifyPos( &pTest ) ;
		if( IsCanGo( pTest,nLevel ) )
		{
			*pos = pTest ;
			return TRUE ;
		}

		pTest.m_fX = pos->m_fX+i ;
		pTest.m_fZ = pos->m_fZ+i ;
		VerifyPos( &pTest ) ;
		if( IsCanGo( pTest,nLevel ) )
		{
			*pos = pTest ;
			return TRUE ;
		}
		pTest.m_fX = pos->m_fX-i ;
		pTest.m_fZ = pos->m_fZ-i ;
		VerifyPos( &pTest ) ;
		if( IsCanGo( pTest,nLevel ) )
		{
			*pos = pTest ;
			return TRUE ;
		}

		pTest.m_fX = pos->m_fX+i ;
		pTest.m_fZ = pos->m_fZ-i ;
		VerifyPos( &pTest ) ;
		if( IsCanGo( pTest,nLevel ) )
		{
			*pos = pTest ;
			return TRUE ;
		}
		pTest.m_fX = pos->m_fX-i ;
		pTest.m_fZ = pos->m_fZ+i ;
		VerifyPos( &pTest ) ;
		if( IsCanGo( pTest,nLevel ) )
		{
			*pos = pTest ;
			return TRUE ;
		}
	}

	return FALSE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL Map::IsPosLogicValid( const WORLD_POS* pos )
{//判断此位置是否在地图最外面VALID_SIZE范围的圈内
	__ENTER_FUNCTION

#define VALID_SIZE 10

		if(	   pos->m_fX < VALID_SIZE 
			|| (pos->m_fX+VALID_SIZE) > CX()
			|| pos->m_fZ < VALID_SIZE 
			|| (pos->m_fZ+VALID_SIZE) > CZ() )
		{
			return FALSE ;
		}

		return TRUE ;

		__LEAVE_FUNCTION

			return FALSE ;
}


