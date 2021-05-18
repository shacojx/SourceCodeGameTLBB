#include "stdafx.h"

#include "GCNewItem.h"

BOOL GCNewItem::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_ObjID), sizeof(ObjID_t));
	iStream.Read( (CHAR*)(&m_uAmount), sizeof(UINT));
	iStream.Read( (CHAR*)(&m_idOwner), sizeof(ObjID_t));
	iStream.Read( (CHAR*)(&m_posWorld), sizeof(WORLD_POS));

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL GCNewItem::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_ObjID), sizeof(ObjID_t));
	oStream.Write( (CHAR*)(&m_uAmount), sizeof(UINT));
	oStream.Write( (CHAR*)(&m_idOwner), sizeof(ObjID_t));
	oStream.Write( (CHAR*)(&m_posWorld), sizeof(WORLD_POS));

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT GCNewItem::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return GCNewItemHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}

