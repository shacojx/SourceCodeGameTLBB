#include "stdafx.h"
// CGBankRemoveItem.cpp
// 
/////////////////////////////////////////////////////

#include "CGBankRemoveItem.h"

BOOL CGBankRemoveItem::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_ToType), sizeof(BYTE));
	iStream.Read( (CHAR*)(&m_indexFrom), sizeof(BYTE));
	iStream.Read( (CHAR*)(&m_indexTo), sizeof(BYTE));
	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL CGBankRemoveItem::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_ToType), sizeof(BYTE));
	oStream.Write( (CHAR*)(&m_indexFrom), sizeof(BYTE));
	oStream.Write( (CHAR*)(&m_indexTo), sizeof(BYTE));

	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT CGBankRemoveItem::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return CGBankRemoveItemHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}

