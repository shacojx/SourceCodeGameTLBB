#include "stdafx.h"


#include "BWConnect.h"



BOOL BWConnect::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

		iStream.Read( (CHAR*)(&m_ServerID), sizeof(ID_t) ) ;
	iStream.Read( m_szKey, sizeof(CHAR)*KEY_SIZE ) ;

	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL BWConnect::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

		oStream.Write( (CHAR*)(&m_ServerID), sizeof(ID_t) ) ;
	oStream.Write( m_szKey, sizeof(CHAR)*KEY_SIZE ) ;

	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT BWConnect::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return BWConnectHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}


