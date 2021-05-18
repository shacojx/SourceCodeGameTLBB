#include "stdafx.h"
// GCShopSell.cpp
// 
/////////////////////////////////////////////////////

#include "GCShopSell.h"

BOOL GCShopSell::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

		iStream.Read( (CHAR*)(&m_IsSellOk), sizeof(BYTE));
	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL GCShopSell::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

		oStream.Write( (CHAR*)(&m_IsSellOk), sizeof(BYTE));
	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT GCShopSell::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return GCShopSellHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}

