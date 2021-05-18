#include "stdafx.h"
// CGShopSell.cpp
// 
/////////////////////////////////////////////////////

#include "CGShopSell.h"

BOOL CGShopSell::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_nBagIndex), sizeof(BYTE));
	iStream.Read( (CHAR*)(&m_UniqueID), sizeof(UINT));
	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL CGShopSell::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_nBagIndex), sizeof(BYTE));
	oStream.Write( (CHAR*)(&m_UniqueID), sizeof(UINT));

	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT CGShopSell::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return CGShopSellHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}

