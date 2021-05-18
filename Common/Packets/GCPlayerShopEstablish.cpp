#include "stdafx.h"
// GCPlayerShopEstablish.cpp
// 
/////////////////////////////////////////////////////

#include "GCPlayerShopEstablish.h"

BOOL GCPlayerShopEstablish::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_szShopName), MAX_SHOP_NAME_SIZE);
	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL GCPlayerShopEstablish::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_szShopName), MAX_SHOP_NAME_SIZE);
	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT GCPlayerShopEstablish::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return GCPlayerShopEstablishHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}

