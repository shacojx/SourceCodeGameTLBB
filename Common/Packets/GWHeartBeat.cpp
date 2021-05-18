#include "stdafx.h"


#include "GWHeartBeat.h"



BOOL GWHeartBeat::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_GUID), sizeof(GUID_t) ) ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL GWHeartBeat::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_GUID), sizeof(GUID_t) ) ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT GWHeartBeat::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return GWHeartBeatHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}


