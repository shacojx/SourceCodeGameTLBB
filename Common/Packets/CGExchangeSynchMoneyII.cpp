#include "stdafx.h"
// CGExchangeSynchMoneyII.cpp
// 
/////////////////////////////////////////////////////

#include "CGExchangeSynchMoneyII.h"

BOOL CGExchangeSynchMoneyII::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_Money), sizeof(UINT));
	
	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL CGExchangeSynchMoneyII::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_Money), sizeof(UINT));
	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT CGExchangeSynchMoneyII::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return CGExchangeSynchMoneyIIHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}

