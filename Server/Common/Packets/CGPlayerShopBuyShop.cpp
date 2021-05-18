#include "stdafx.h"
// CGPlayerShopBuyShop.cpp
// 
/////////////////////////////////////////////////////

#include "CGPlayerShopBuyShop.h"

BOOL CGPlayerShopBuyShop::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_ShopID), sizeof(_PLAYERSHOP_GUID));
	iStream.Read( (CHAR*)(&m_Serial), sizeof(BYTE));


	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL CGPlayerShopBuyShop::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_ShopID), sizeof(_PLAYERSHOP_GUID));
	oStream.Write( (CHAR*)(&m_Serial), sizeof(BYTE));

	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT CGPlayerShopBuyShop::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return CGPlayerShopBuyShopHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}

