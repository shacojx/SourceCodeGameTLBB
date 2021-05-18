#include "stdafx.h"
// CGPlayerShopMoney.cpp
// 
/////////////////////////////////////////////////////

#include "CGPlayerShopMoney.h"

BOOL CGPlayerShopMoney::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_ShopID), sizeof(_PLAYERSHOP_GUID));
	iStream.Read( (CHAR*)(&m_Opt), sizeof(BYTE));
	iStream.Read( (CHAR*)(&m_Type), sizeof(BYTE));
	iStream.Read( (CHAR*)(&m_Amount), sizeof(UINT));
	iStream.Read( (CHAR*)(&m_nSerial), sizeof(BYTE));
	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL CGPlayerShopMoney::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_ShopID), sizeof(_PLAYERSHOP_GUID));
	oStream.Write( (CHAR*)(&m_Opt), sizeof(BYTE));
	oStream.Write( (CHAR*)(&m_Type), sizeof(BYTE));
	oStream.Write( (CHAR*)(&m_Amount), sizeof(UINT));
	oStream.Write( (CHAR*)(&m_nSerial), sizeof(BYTE));

	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT CGPlayerShopMoney::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return CGPlayerShopMoneyHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}

