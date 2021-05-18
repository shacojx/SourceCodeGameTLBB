#include "stdafx.h"
// GCStallRemoveItem.cpp
// 
/////////////////////////////////////////////////////

#include "GCStallRemoveItem.h"

BOOL GCStallRemoveItem::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION
	iStream.Read( (CHAR*)(&m_ItemGuid), sizeof(_ITEM_GUID));
	iStream.Read( (CHAR*)(&m_PetGuid), sizeof(PET_GUID_t));
	iStream.Read( (CHAR*)(&m_Serial), sizeof(UINT));
	iStream.Read( (CHAR*)(&m_ToType), sizeof(BYTE));
	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL GCStallRemoveItem::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_ItemGuid), sizeof(_ITEM_GUID));
	oStream.Write( (CHAR*)(&m_PetGuid), sizeof(PET_GUID_t));
	oStream.Write( (CHAR*)(&m_Serial), sizeof(UINT));
	oStream.Write( (CHAR*)(&m_ToType), sizeof(BYTE));

	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT GCStallRemoveItem::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return GCStallRemoveItemHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}

