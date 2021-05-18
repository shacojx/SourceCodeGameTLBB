#include "stdafx.h"
#include "CGPickBoxItem.h"

using namespace Packets;


BOOL CGPickBoxItem::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

		iStream.Read( (CHAR*)(&m_ObjID), sizeof(ObjID_t));
		iStream.Read( (CHAR*)(&m_ItemId), sizeof(_ITEM_GUID));
		return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL CGPickBoxItem::Write( SocketOutputStream& oStream ) const
{
	__ENTER_FUNCTION
		oStream.Write( (CHAR*)(&m_ObjID), sizeof(ObjID_t));
		oStream.Write( (CHAR*)(&m_ItemId), sizeof(_ITEM_GUID));
		return TRUE ;
	__LEAVE_FUNCTION
		return FALSE ;
}

UINT	CGPickBoxItem::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION
		return CGPickBoxItemHandler::Execute( this, pPlayer ) ;
	__LEAVE_FUNCTION
		return FALSE ;
}