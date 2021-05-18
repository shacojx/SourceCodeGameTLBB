#include "stdafx.h"


#include "GWMail.h"



BOOL GWMail::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	m_Mail.Read( iStream ) ;
	
	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

BOOL GWMail::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

	m_Mail.Write( oStream ) ;
	
	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

UINT GWMail::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return GWMailHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}


