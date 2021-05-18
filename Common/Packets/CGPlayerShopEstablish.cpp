#include "stdafx.h"
// CGPlayerShopEstablish.cpp
// 
/////////////////////////////////////////////////////

#include "CGPlayerShopEstablish.h"

BOOL CGPlayerShopEstablish::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_Type), sizeof(BYTE));
	iStream.Read( (CHAR*)(&m_ShopNameSize), sizeof(BYTE));
	if(m_ShopNameSize<MAX_SHOP_NAME)
	{
		iStream.Read( (CHAR*)(m_ShopName), sizeof(CHAR)*m_ShopNameSize ) ;
	}
	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL CGPlayerShopEstablish::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_Type), sizeof(BYTE));
	oStream.Write( (CHAR*)(&m_ShopNameSize), sizeof(BYTE));
	if(m_ShopNameSize<MAX_SHOP_NAME)
	{
		oStream.Write( (CHAR*)(m_ShopName), sizeof(CHAR)*m_ShopNameSize ) ;
	}

	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT CGPlayerShopEstablish::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return CGPlayerShopEstablishHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}

