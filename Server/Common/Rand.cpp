#include "stdafx.h"

#include "Rand.h"


UINT MyRand::m_uKey = 0 ;
CHAR MyRand::m_pKey[MAX_KEY_SIZE] ;
MyLock MyRand::m_Lock ;
RandGen	g_RandGen	= RandGen((unsigned)time(NULL));

MyRand::MyRand( )
{
}

MyRand::~MyRand( )
{
}

VOID MyRand::SetRand( UINT uKey )
{
	m_Lock.Lock() ;

	m_uKey = uKey%MAX_KEY_SIZE ;
	
	m_Lock.Unlock() ;
}

UINT MyRand::Rand( )
{
	m_Lock.Lock() ;

	if( m_uKey == 0 )
	{
		srand( (unsigned)time( NULL ) ) ;
		for( INT i=0; i<MAX_KEY_SIZE; i++ )
		{
			m_pKey[i] = rand()%255 ;
		}
	}

	m_uKey ++ ;
	if( m_uKey >= MAX_KEY_SIZE-sizeof(UINT) )
		m_uKey = 1 ;

	UINT nRet = *((UINT*)(m_pKey+m_uKey)) ;

	m_Lock.Unlock() ;

	return nRet ;
}

UINT RandGen::GetRand(INT nStart, INT nEnd)
{
	return (INT)((nEnd-nStart)*g_RandGen.RandDouble()) + nStart;
}
