#include "stdafx.h"


#include "GWChannelCreate.h"



BOOL GWChannelCreate::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

		iStream.Read( (CHAR*)&m_GUID, sizeof(GUID_t) );
	
	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

BOOL GWChannelCreate::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

		oStream.Write( (CHAR*)&m_GUID, sizeof(GUID_t) );

	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

UINT GWChannelCreate::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return GWChannelCreateHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}


