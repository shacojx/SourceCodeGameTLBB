#include "stdafx.h"
// GCPlayerShopBuyShop.cpp
// 
/////////////////////////////////////////////////////

#include "GCPlayerShopBuyShop.h"

BOOL GCPlayerShopBuyShop::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_ShopID), sizeof(_PLAYERSHOP_GUID));

	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL GCPlayerShopBuyShop::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_ShopID), sizeof(_PLAYERSHOP_GUID));

	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT GCPlayerShopBuyShop::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return GCPlayerShopBuyShopHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}

