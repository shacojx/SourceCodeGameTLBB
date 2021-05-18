#include "stdafx.h"
// GCBankMoney.cpp
// 
/////////////////////////////////////////////////////

#include "GCBankMoney.h"

BOOL GCBankMoney::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_Save), sizeof(BYTE));
	iStream.Read( (CHAR*)(&m_Amount), sizeof(INT));
	iStream.Read( (CHAR*)(&m_AmountRMB), sizeof(INT));
	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL GCBankMoney::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_Save), sizeof(BYTE));
	oStream.Write( (CHAR*)(&m_Amount), sizeof(INT));
	oStream.Write( (CHAR*)(&m_AmountRMB), sizeof(INT));
	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT GCBankMoney::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return GCBankMoneyHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}

