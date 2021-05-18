#include "stdafx.h"


#include "CGMail.h"



BOOL CGMail::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	m_Mail.Read( iStream ) ;
	
	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

BOOL CGMail::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

	m_Mail.Write( oStream ) ;
	
	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

UINT CGMail::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return CGMailHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}


