#include "stdafx.h"
// CGPlayerShopSaleOut.cpp
// 
/////////////////////////////////////////////////////

#include "CGPlayerShopSaleOut.h"

BOOL CGPlayerShopSaleOut::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_ShopID), sizeof(_PLAYERSHOP_GUID));
	iStream.Read( (CHAR*)(&m_bSaleOut), sizeof(BYTE));
	iStream.Read( (CHAR*)(&m_Price), sizeof(UINT));
	iStream.Read( (CHAR*)(&m_Serial), sizeof(BYTE));

	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL CGPlayerShopSaleOut::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_ShopID), sizeof(_PLAYERSHOP_GUID));
	oStream.Write( (CHAR*)(&m_bSaleOut), sizeof(BYTE));
	oStream.Write( (CHAR*)(&m_Price), sizeof(UINT));
	oStream.Write( (CHAR*)(&m_Serial), sizeof(BYTE));

	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT CGPlayerShopSaleOut::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return CGPlayerShopSaleOutHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}

