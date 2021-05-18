#include "stdafx.h"


#include "WGMail.h"



BOOL WGMail::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

 	iStream.Read( (CHAR*)(&m_PlayerID), sizeof(PlayerID_t) ) ;
	m_MailList.Read( iStream ) ;
	iStream.Read( (CHAR*)(&m_GUID), sizeof(m_GUID) ) ;
	
	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

BOOL WGMail::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

 	oStream.Write( (CHAR*)(&m_PlayerID), sizeof(PlayerID_t) ) ;
	m_MailList.Write( oStream ) ;
	oStream.Write( (CHAR*)(&m_GUID), sizeof(m_GUID) ) ;
	
	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

UINT WGMail::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return WGMailHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}


