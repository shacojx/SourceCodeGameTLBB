#include "stdafx.h"
// GCBankAddItem.cpp
// 
/////////////////////////////////////////////////////

#include "GCBankAddItem.h"

BOOL GCBankAddItem::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_FromType), sizeof(BYTE));
	iStream.Read( (CHAR*)(&m_indexFrom), sizeof(BYTE));
	iStream.Read( (CHAR*)(&m_indexTo), sizeof(BYTE));
	iStream.Read( (CHAR*)(&m_OperateType), sizeof(BYTE));

	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL GCBankAddItem::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_FromType), sizeof(BYTE));
	oStream.Write( (CHAR*)(&m_indexFrom), sizeof(BYTE));
	oStream.Write( (CHAR*)(&m_indexTo), sizeof(BYTE));
	oStream.Write( (CHAR*)(&m_OperateType), sizeof(BYTE));
	
	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT GCBankAddItem::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return GCBankAddItemHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}

