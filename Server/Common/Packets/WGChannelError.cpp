#include "stdafx.h"


#include "WGChannelError.h"



BOOL WGChannelError::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

		iStream.Read( (CHAR*)(&m_PlayerID), sizeof(PlayerID_t) ) ;
		iStream.Read( (CHAR*)(&m_Error), sizeof(BYTE) ) ;
		iStream.Read( (CHAR*)(&m_GUID), sizeof(m_GUID) ) ;
	
	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

BOOL WGChannelError::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

		oStream.Write( (CHAR*)(&m_PlayerID), sizeof(PlayerID_t) ) ;
		oStream.Write( (CHAR*)(&m_Error), sizeof(BYTE) ) ;
		oStream.Write( (CHAR*)(&m_GUID), sizeof(m_GUID) ) ;
	
	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

UINT WGChannelError::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return WGChannelErrorHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}


