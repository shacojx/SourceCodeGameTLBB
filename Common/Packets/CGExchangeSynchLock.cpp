#include "stdafx.h"
// CGExchangeSynchLock.cpp
// 
/////////////////////////////////////////////////////

#include "CGExchangeSynchLock.h"

BOOL CGExchangeSynchLock::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_LockMyself), sizeof(BYTE));
	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL CGExchangeSynchLock::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_LockMyself), sizeof(BYTE));
	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT CGExchangeSynchLock::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return CGExchangeSynchLockHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}

