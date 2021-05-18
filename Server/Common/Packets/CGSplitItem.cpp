#include "stdafx.h"
// CGSplitItem.cpp
// 
/////////////////////////////////////////////////////

#include "CGSplitItem.h"

BOOL CGSplitItem::Read(SocketInputStream& iStream) 
{
	__ENTER_FUNCTION

		iStream.Read( (CHAR*)(&m_nContainer), sizeof(INT));
		iStream.Read( (CHAR*)(&m_nPosition), sizeof(INT));
		iStream.Read( (CHAR*)(&m_nNumber), sizeof(INT));
	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL CGSplitItem::Write(SocketOutputStream& oStream)const
{
	__ENTER_FUNCTION

		oStream.Write( (CHAR*)(&m_nContainer), sizeof(INT));
		oStream.Write( (CHAR*)(&m_nPosition), sizeof(INT));
		oStream.Write( (CHAR*)(&m_nNumber), sizeof(INT));

		return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT CGSplitItem::Execute(Player* pPlayer)
{
	__ENTER_FUNCTION

		return CGSplitItemHandler::Execute(this, pPlayer) ;

	__LEAVE_FUNCTION

		return FALSE ;
}

