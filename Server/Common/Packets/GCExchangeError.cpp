#include "stdafx.h"
// GCExchangeError.cpp
// 
/////////////////////////////////////////////////////

#include "GCExchangeError.h"

BOOL GCExchangeError::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_ID), sizeof(BYTE));
	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL GCExchangeError::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_ID), sizeof(BYTE));
	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT GCExchangeError::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return GCExchangeErrorHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}

