#include "stdafx.h"
// GCBankRemoveItem.cpp
// 
/////////////////////////////////////////////////////

#include "GCBankRemoveItem.h"

BOOL GCBankRemoveItem::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_ToType), sizeof(BYTE));
	iStream.Read( (CHAR*)(&m_indexFrom), sizeof(BYTE));
	iStream.Read( (CHAR*)(&m_indexTo), sizeof(BYTE));
	iStream.Read( (CHAR*)(&m_OperateType), sizeof(BYTE));
	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL GCBankRemoveItem::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_ToType), sizeof(BYTE));
	oStream.Write( (CHAR*)(&m_indexFrom), sizeof(BYTE));
	oStream.Write( (CHAR*)(&m_indexTo), sizeof(BYTE));
	oStream.Write( (CHAR*)(&m_OperateType), sizeof(BYTE));
	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT GCBankRemoveItem::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return GCBankRemoveItemHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}

