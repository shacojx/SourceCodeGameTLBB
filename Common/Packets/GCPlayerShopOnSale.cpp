#include "stdafx.h"
// GCPlayerShopOnSale.cpp
// 
/////////////////////////////////////////////////////

#include "GCPlayerShopOnSale.h"

BOOL GCPlayerShopOnSale::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_ShopID), sizeof(_PLAYERSHOP_GUID));
	iStream.Read( (CHAR*)(&m_StallIndex), sizeof(BYTE));
	iStream.Read( (CHAR*)(&m_ItemGuid), sizeof(_ITEM_GUID));
	iStream.Read( (CHAR*)(&m_nSerial), sizeof(UINT));
	iStream.Read( (CHAR*)(&m_bIsOnSale), sizeof(BYTE));
	iStream.Read( (CHAR*)(&m_uPrice), sizeof(UINT));
	iStream.Read( (CHAR*)(&m_PetGuid), sizeof(PET_GUID_t));
	iStream.Read( (CHAR*)(&m_nShopSerial), sizeof(BYTE));
	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL GCPlayerShopOnSale::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_ShopID), sizeof(_PLAYERSHOP_GUID));
	oStream.Write( (CHAR*)(&m_StallIndex), sizeof(BYTE));
	oStream.Write( (CHAR*)(&m_ItemGuid), sizeof(_ITEM_GUID));
	oStream.Write( (CHAR*)(&m_nSerial), sizeof(UINT));
	oStream.Write( (CHAR*)(&m_bIsOnSale), sizeof(BYTE));
	oStream.Write( (CHAR*)(&m_uPrice), sizeof(UINT));
	oStream.Write( (CHAR*)(&m_PetGuid), sizeof(PET_GUID_t));
	oStream.Write( (CHAR*)(&m_nShopSerial), sizeof(BYTE));
	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT GCPlayerShopOnSale::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return GCPlayerShopOnSaleHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}

