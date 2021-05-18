#include "stdafx.h"


#include "WGNotifyMail.h"



BOOL WGNotifyMail::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	 	iStream.Read( (CHAR*)(&m_PlayerID), sizeof(PlayerID_t) ) ;
		iStream.Read( (CHAR*)(&m_MailCount), sizeof(BYTE) ) ;
		iStream.Read( (CHAR*)(&m_GUID), sizeof(m_GUID) ) ;
		
	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

BOOL WGNotifyMail::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

	 	oStream.Write( (CHAR*)(&m_PlayerID), sizeof(PlayerID_t) ) ;
		oStream.Write( (CHAR*)(&m_MailCount), sizeof(BYTE) ) ;
		oStream.Write( (CHAR*)(&m_GUID), sizeof(m_GUID) ) ;
	
	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

UINT WGNotifyMail::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return WGNotifyMailHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}


