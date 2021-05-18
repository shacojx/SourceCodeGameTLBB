#include "stdafx.h"
// GCShopBuy.cpp
// 
/////////////////////////////////////////////////////

#include "GCShopBuy.h"

BOOL GCShopBuy::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

		iStream.Read( (CHAR*)(&m_IsBuyOk), sizeof(BYTE));
		iStream.Read( (CHAR*)(&m_ItemIndex), sizeof(UINT));
		iStream.Read( (CHAR*)(&m_ItemNum), sizeof(BYTE));

	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL GCShopBuy::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_IsBuyOk), sizeof(BYTE));
	oStream.Write( (CHAR*)(&m_ItemIndex), sizeof(UINT));
	oStream.Write( (CHAR*)(&m_ItemNum), sizeof(BYTE));

	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT GCShopBuy::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return GCShopBuyHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}

