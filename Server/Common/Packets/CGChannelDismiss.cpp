#include "stdafx.h"


#include "CGChannelDismiss.h"



BOOL CGChannelDismiss::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

		iStream.Read( (CHAR*)&m_GUID, sizeof(GUID_t) );
	
	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

BOOL CGChannelDismiss::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

		oStream.Write( (CHAR*)&m_GUID, sizeof(GUID_t) );
	
	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

UINT CGChannelDismiss::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return CGChannelDismissHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}


