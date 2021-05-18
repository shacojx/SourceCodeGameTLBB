#include "stdafx.h"
// GCCityNotify.cpp
// 
/////////////////////////////////////////////////////

#include "GCCityNotify.h"

BOOL GCCityNotify::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_ID), sizeof(BYTE));
	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL GCCityNotify::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_ID), sizeof(BYTE));
	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT GCCityNotify::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return GCCityNotifyHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}

