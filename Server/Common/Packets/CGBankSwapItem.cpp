#include "stdafx.h"
// CGBankSwapItem.cpp
// 
/////////////////////////////////////////////////////

#include "CGBankSwapItem.h"

BOOL CGBankSwapItem::Read( SocketInputStream& iStream ) 
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

BOOL CGBankSwapItem::Write( SocketOutputStream& oStream )const
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

UINT CGBankSwapItem::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return CGBankSwapItemHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}

