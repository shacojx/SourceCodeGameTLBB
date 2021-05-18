#include "stdafx.h"


#include "GCNotifyMail.h"



BOOL GCNotifyMail::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	 	iStream.Read( (CHAR*)(&m_MailCount), sizeof(BYTE) ) ;
	
	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

BOOL GCNotifyMail::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

	 	oStream.Write( (CHAR*)(&m_MailCount), sizeof(BYTE) ) ;
	
	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

UINT GCNotifyMail::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return GCNotifyMailHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}


