#include "stdafx.h"
// CGBankAcquireList.cpp
// 
/////////////////////////////////////////////////////

#include "CGBankAcquireList.h"

BOOL CGBankAcquireList::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

		iStream.Read( (CHAR*)(&m_BankID), sizeof(BYTE));
	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL CGBankAcquireList::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

		oStream.Write( (CHAR*)(&m_BankID), sizeof(BYTE));
	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT CGBankAcquireList::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return CGBankAcquireListHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}

