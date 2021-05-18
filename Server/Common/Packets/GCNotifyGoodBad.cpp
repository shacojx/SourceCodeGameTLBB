#include "stdafx.h"

#include "GCNotifyGoodBad.h"

BOOL GCNotifyRMBMoney::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read((CHAR*)(&m_nValue), sizeof(m_nValue));

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

BOOL GCNotifyRMBMoney::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	oStream.Write((CHAR*)(&m_nValue), sizeof(m_nValue));

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

UINT GCNotifyRMBMoney::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return GCNotifyRMBMoneyHandler::Execute( this, pPlayer );

__LEAVE_FUNCTION

	return FALSE;
}
