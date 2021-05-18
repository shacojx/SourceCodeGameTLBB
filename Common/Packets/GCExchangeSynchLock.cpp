#include "stdafx.h"
// GCExchangeSynchLock.cpp
// 
/////////////////////////////////////////////////////

#include "GCExchangeSynchLock.h"

BOOL GCExchangeSynchLock::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_IsBoth), sizeof(BYTE));
	iStream.Read( (CHAR*)(&m_IsMyself), sizeof(BYTE));
	iStream.Read( (CHAR*)(&m_IsLocked), sizeof(BYTE));

	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL GCExchangeSynchLock::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_IsBoth), sizeof(BYTE));
	oStream.Write( (CHAR*)(&m_IsMyself), sizeof(BYTE));
	oStream.Write( (CHAR*)(&m_IsLocked), sizeof(BYTE));

	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT GCExchangeSynchLock::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return GCExchangeSynchLockHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}

