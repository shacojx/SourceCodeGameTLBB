#include "stdafx.h"


#include "GWChannelDismiss.h"



BOOL GWChannelDismiss::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

		iStream.Read( (CHAR*)&m_GUID, sizeof(GUID_t) );
	
	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

BOOL GWChannelDismiss::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

		oStream.Write( (CHAR*)&m_GUID, sizeof(GUID_t) );
	
	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

UINT GWChannelDismiss::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return GWChannelDismissHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}


