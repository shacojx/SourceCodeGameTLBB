#include "stdafx.h"
// CGStallItemPrice.cpp
// 
/////////////////////////////////////////////////////

#include "CGStallItemPrice.h"

BOOL CGStallItemPrice::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_ItemGuid), sizeof(_ITEM_GUID));
	iStream.Read( (CHAR*)(&m_PetGuid), sizeof(PET_GUID_t));
	iStream.Read( (CHAR*)(&m_nPrice), sizeof(UINT));
	iStream.Read( (CHAR*)(&m_nSerial), sizeof(UINT));

	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL CGStallItemPrice::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_ItemGuid), sizeof(_ITEM_GUID));
	oStream.Write( (CHAR*)(&m_PetGuid), sizeof(PET_GUID_t));
	oStream.Write( (CHAR*)(&m_nPrice), sizeof(UINT));
	oStream.Write( (CHAR*)(&m_nSerial), sizeof(UINT));

	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT CGStallItemPrice::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return CGStallItemPriceHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}

