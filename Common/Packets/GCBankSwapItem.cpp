#include "stdafx.h"
// GCBankSwapItem.cpp
// 
/////////////////////////////////////////////////////

#include "GCBankSwapItem.h"

BOOL GCBankSwapItem::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_ToType), sizeof(BYTE));
	iStream.Read( (CHAR*)(&m_FromType), sizeof(BYTE));
	iStream.Read( (CHAR*)(&m_indexFrom), sizeof(BYTE));
	iStream.Read( (CHAR*)(&m_indexTo), sizeof(BYTE));
	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL GCBankSwapItem::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_ToType), sizeof(BYTE));
	oStream.Write( (CHAR*)(&m_FromType), sizeof(BYTE));
	oStream.Write( (CHAR*)(&m_indexFrom), sizeof(BYTE));
	oStream.Write( (CHAR*)(&m_indexTo), sizeof(BYTE));
	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT GCBankSwapItem::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return GCBankSwapItemHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}

