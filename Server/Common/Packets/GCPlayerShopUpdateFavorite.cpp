#include "stdafx.h"
// GCPlayerShopUpdateFavorite.cpp
// 
/////////////////////////////////////////////////////

#include "GCPlayerShopUpdateFavorite.h"

BOOL GCPlayerShopUpdateFavorite::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_ShopID), sizeof(_PLAYERSHOP_GUID));
	iStream.Read( (CHAR*)(&m_bIsInFavorite), sizeof(BYTE));
	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL GCPlayerShopUpdateFavorite::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_ShopID), sizeof(_PLAYERSHOP_GUID));
	oStream.Write( (CHAR*)(&m_bIsInFavorite), sizeof(BYTE));
	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT GCPlayerShopUpdateFavorite::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return GCPlayerShopUpdateFavoriteHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}

