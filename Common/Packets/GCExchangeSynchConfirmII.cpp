#include "stdafx.h"
// GCExchangeSynchConfirmII.cpp
// 
/////////////////////////////////////////////////////

#include "GCExchangeSynchConfirmII.h"

BOOL GCExchangeSynchConfirmII::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_IsEnable), sizeof(BYTE));

	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL GCExchangeSynchConfirmII::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_IsEnable), sizeof(BYTE));

	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT GCExchangeSynchConfirmII::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return GCExchangeSynchConfirmIIHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}

