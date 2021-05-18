#include "stdafx.h"
// CGPlayerShopFavorite.cpp
// 
/////////////////////////////////////////////////////

#include "CGPlayerShopFavorite.h"

BOOL CGPlayerShopFavorite::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION
	iStream.Read( (CHAR*)(&m_Opt), sizeof(BYTE));
	iStream.Read( (CHAR*)(&m_FavoriteID), sizeof(_PLAYERSHOP_GUID));

	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL CGPlayerShopFavorite::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_Opt), sizeof(BYTE));
	oStream.Write( (CHAR*)(&m_FavoriteID), sizeof(_PLAYERSHOP_GUID));

	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT CGPlayerShopFavorite::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return CGPlayerShopFavoriteHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}

