#include "stdafx.h"
#include "GCUnEquipResult.h"


BOOL GCUnEquipResult::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION


	iStream.Read( (CHAR*)(&m_EquipPoint), sizeof(BYTE));
	iStream.Read( (CHAR*)(&m_BagIndex), sizeof(BYTE));
	iStream.Read( (CHAR*)(&m_ItemTableIndex), sizeof(UINT));
	iStream.Read( (CHAR*)(&m_ItemId), sizeof(_ITEM_GUID));
	iStream.Read( (CHAR*)(&m_Result), sizeof(BYTE));


	return TRUE ;
	__LEAVE_FUNCTION
		return FALSE ;
}

BOOL GCUnEquipResult::Write( SocketOutputStream& oStream ) const
{
	__ENTER_FUNCTION
		oStream.Write( (CHAR*)(&m_EquipPoint), sizeof(BYTE));
		oStream.Write( (CHAR*)(&m_BagIndex), sizeof(BYTE));
		oStream.Write( (CHAR*)(&m_ItemTableIndex), sizeof(UINT));
		oStream.Write( (CHAR*)(&m_ItemId), sizeof(_ITEM_GUID));
		oStream.Write( (CHAR*)(&m_Result), sizeof(BYTE));

	return TRUE ;
	__LEAVE_FUNCTION
		return FALSE ;
}

UINT GCUnEquipResult::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION
		return GCUnEquipResultHandler::Execute( this, pPlayer ) ;
	__LEAVE_FUNCTION
		return FALSE ;
}