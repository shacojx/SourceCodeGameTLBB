#include "stdafx.h"


#include "GCChannelError.h"



BOOL GCChannelError::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

		iStream.Read( (CHAR*)(&m_Error), sizeof(BYTE) ) ;
	
	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

BOOL GCChannelError::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

		oStream.Write( (CHAR*)(&m_Error), sizeof(BYTE) ) ;
	
	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

UINT GCChannelError::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return GCChannelErrorHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}


