#include "stdafx.h"
// CGPlayerShopSize.cpp
// 
/////////////////////////////////////////////////////

#include "CGPlayerShopSize.h"

BOOL CGPlayerShopSize::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_ShopID), sizeof(_PLAYERSHOP_GUID));
	iStream.Read( (CHAR*)(&m_Opt), sizeof(BYTE));
	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL CGPlayerShopSize::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_ShopID), sizeof(_PLAYERSHOP_GUID));
	oStream.Write( (CHAR*)(&m_Opt), sizeof(BYTE));
	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT CGPlayerShopSize::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return CGPlayerShopSizeHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}
