#include "stdafx.h"
#include "GCPickResult.h"

using namespace Packets;


BOOL GCPickResult::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_ObjID), sizeof(ObjID_t));
	iStream.Read( (CHAR*)(&m_ItemID), sizeof(_ITEM_GUID));
	iStream.Read( (CHAR*)(&m_Result), sizeof(BYTE));
	iStream.Read( (CHAR*)(&m_BagIndex), sizeof(BYTE));
	iStream.Read( (CHAR*)(&m_ItemTableIndex), sizeof(WORD));
	iStream.Read( (CHAR*)(&m_BagItemGUID),sizeof(_ITEM_GUID));

	return TRUE ;
	__LEAVE_FUNCTION
		return FALSE ;
}

BOOL GCPickResult::Write( SocketOutputStream& oStream ) const
{
	__ENTER_FUNCTION
		oStream.Write( (CHAR*)(&m_ObjID), sizeof(ObjID_t));
		oStream.Write( (CHAR*)(&m_ItemID), sizeof(_ITEM_GUID));
		oStream.Write( (CHAR*)(&m_Result), sizeof(BYTE));
		oStream.Write( (CHAR*)(&m_BagIndex), sizeof(BYTE));
		oStream.Write( (CHAR*)(&m_ItemTableIndex), sizeof(WORD));
		oStream.Write( (CHAR*)(&m_BagItemGUID),sizeof(_ITEM_GUID));

		return TRUE ;
	__LEAVE_FUNCTION
		return FALSE ;
}

UINT GCPickResult::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION
		return GCPickResultHandler::Execute( this, pPlayer ) ;
	__LEAVE_FUNCTION
		return FALSE ;
}