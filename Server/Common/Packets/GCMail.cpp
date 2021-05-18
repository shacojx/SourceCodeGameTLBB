#include "stdafx.h"


#include "GCMail.h"



BOOL GCMail::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	m_MailList.Read( iStream ) ;

	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

BOOL GCMail::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

	m_MailList.Write( oStream ) ;
	
	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

UINT GCMail::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return GCMailHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}


