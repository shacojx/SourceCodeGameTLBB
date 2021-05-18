#include "stdafx.h"


#include "CGChannelCreate.h"



BOOL CGChannelCreate::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

		iStream.Read( (CHAR*)&m_GUID, sizeof(GUID_t) );
	
	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

BOOL CGChannelCreate::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

		oStream.Write( (CHAR*)&m_GUID, sizeof(GUID_t) );
	
	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

UINT CGChannelCreate::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return CGChannelCreateHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}


