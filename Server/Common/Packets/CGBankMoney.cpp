#include "stdafx.h"
// CGBankMoney.cpp
// 
/////////////////////////////////////////////////////

#include "CGBankMoney.h"

BOOL CGBankMoney::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_Save), sizeof(BYTE));
	iStream.Read( (CHAR*)(&m_AmountMoney), sizeof(INT));
	iStream.Read( (CHAR*)(&m_AmountRMB), sizeof(INT));

	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL CGBankMoney::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_Save), sizeof(BYTE));
	oStream.Write( (CHAR*)(&m_AmountMoney), sizeof(INT));
	oStream.Write( (CHAR*)(&m_AmountRMB), sizeof(INT));
	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT CGBankMoney::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return CGBankMoneyHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}

