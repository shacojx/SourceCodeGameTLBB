#include "stdafx.h"
// GCPlayerShopError.cpp
// 
/////////////////////////////////////////////////////

#include "GCPlayerShopError.h"

BOOL GCPlayerShopError::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_ID), sizeof(BYTE));
	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL GCPlayerShopError::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_ID), sizeof(BYTE));
	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT GCPlayerShopError::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return GCPlayerShopErrorHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}

