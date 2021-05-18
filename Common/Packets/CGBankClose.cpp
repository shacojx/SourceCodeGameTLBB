#include "stdafx.h"
// CGBankClose.cpp
// 
/////////////////////////////////////////////////////

#include "CGBankClose.h"

BOOL CGBankClose::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

		iStream.Read( (CHAR*)(&m_BankID), sizeof(BYTE));
	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL CGBankClose::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

		oStream.Write( (CHAR*)(&m_BankID), sizeof(BYTE));
	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT CGBankClose::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return CGBankCloseHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}

