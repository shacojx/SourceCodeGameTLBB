#include "stdafx.h"
// GCPlayerShopOpenStall.cpp
// 
/////////////////////////////////////////////////////

#include "GCPlayerShopOpenStall.h"

BOOL GCPlayerShopOpenStall::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION
	
	iStream.Read( (CHAR*)(&m_ShopID), sizeof(_PLAYERSHOP_GUID));
	iStream.Read( (CHAR*)(&m_nStallIndex), sizeof(BYTE));
	iStream.Read( (CHAR*)(&m_bOpen), sizeof(BYTE));
	iStream.Read( (CHAR*)(&m_nShopSerial), sizeof(BYTE));


		return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL GCPlayerShopOpenStall::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION
	oStream.Write( (CHAR*)(&m_ShopID), sizeof(_PLAYERSHOP_GUID));
	oStream.Write( (CHAR*)(&m_nStallIndex), sizeof(BYTE));
	oStream.Write( (CHAR*)(&m_bOpen), sizeof(BYTE));
	oStream.Write( (CHAR*)(&m_nShopSerial), sizeof(BYTE));
		return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT GCPlayerShopOpenStall::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return GCPlayerShopOpenStallHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}

