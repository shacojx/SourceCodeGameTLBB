#include "stdafx.h"

#include "GCCharFirstLogin.h"
 
BOOL GCCharFirstLogin::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_bFirstLogin), sizeof(m_bFirstLogin) ) ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL GCCharFirstLogin::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_bFirstLogin), sizeof(m_bFirstLogin) ) ;
	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT GCCharFirstLogin::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return GCCharFirstLoginHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}

