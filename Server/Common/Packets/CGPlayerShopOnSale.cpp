#include "stdafx.h"
// CGPlayerShopOnSale.cpp
// 
/////////////////////////////////////////////////////

#include "CGPlayerShopOnSale.h"

BOOL CGPlayerShopOnSale::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_ShopID), sizeof(_PLAYERSHOP_GUID));
	iStream.Read( (CHAR*)(&m_StallIndex), sizeof(BYTE));
	iStream.Read( (CHAR*)(&m_ItemGuid), sizeof(_ITEM_GUID));
	iStream.Read( (CHAR*)(&m_nSerial), sizeof(UINT));
	iStream.Read( (CHAR*)(&m_nPrice), sizeof(UINT));
	iStream.Read( (CHAR*)(&m_bIsOnSale), sizeof(BYTE));
	iStream.Read( (CHAR*)(&m_PetGuid), sizeof(PET_GUID_t));
	iStream.Read( (CHAR*)(&m_nShopSerial), sizeof(BYTE));

	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL CGPlayerShopOnSale::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_ShopID), sizeof(_PLAYERSHOP_GUID));
	oStream.Write( (CHAR*)(&m_StallIndex), sizeof(BYTE));
	oStream.Write( (CHAR*)(&m_ItemGuid), sizeof(_ITEM_GUID));
	oStream.Write( (CHAR*)(&m_nSerial), sizeof(UINT));
	oStream.Write( (CHAR*)(&m_nPrice), sizeof(UINT));
	oStream.Write( (CHAR*)(&m_bIsOnSale), sizeof(BYTE));
	oStream.Write( (CHAR*)(&m_PetGuid), sizeof(PET_GUID_t));
	oStream.Write( (CHAR*)(&m_nShopSerial), sizeof(BYTE));

	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT CGPlayerShopOnSale::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return CGPlayerShopOnSaleHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}

