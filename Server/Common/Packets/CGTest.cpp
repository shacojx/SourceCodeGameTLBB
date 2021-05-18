#include "stdafx.h"



#include "CGTest.h"

BOOL CGTest::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	UINT r ;
	r = iStream.Read( (CHAR*)(&m_Type), sizeof(UINT) ) ;
	Assert( r!=0 ) ;
	r = iStream.Read( (CHAR*)(&m_Test), sizeof(_PCGTEST) ) ;
	Assert( r!=0 ) ;


	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL CGTest::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	UINT w ;
	w = oStream.Write( (CHAR*)(&m_Type), sizeof(UINT) ) ;
	Assert( w!=0 ) ;
	w = oStream.Write( (CHAR*)(&m_Test), sizeof(_PCGTEST) ) ;
	Assert( w!=0 ) ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT CGTest::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return CGTestHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}


