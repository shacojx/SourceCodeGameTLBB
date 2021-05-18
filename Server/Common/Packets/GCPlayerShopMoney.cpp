#include "stdafx.h"
// GCPlayerShopMoney.cpp
// 
/////////////////////////////////////////////////////

#include "GCPlayerShopMoney.h"

BOOL GCPlayerShopMoney::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_ShopID), sizeof(_PLAYERSHOP_GUID));
	iStream.Read( (CHAR*)(&m_Type), sizeof(BYTE));
	iStream.Read( (CHAR*)(&m_Amount), sizeof(UINT));
	iStream.Read( (CHAR*)(&m_nShopSerial), sizeof(BYTE));

	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL GCPlayerShopMoney::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_ShopID), sizeof(_PLAYERSHOP_GUID));
	oStream.Write( (CHAR*)(&m_Type), sizeof(BYTE));
	oStream.Write( (CHAR*)(&m_Amount), sizeof(UINT));
	oStream.Write( (CHAR*)(&m_nShopSerial), sizeof(BYTE));

	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT GCPlayerShopMoney::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return GCPlayerShopMoneyHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}

