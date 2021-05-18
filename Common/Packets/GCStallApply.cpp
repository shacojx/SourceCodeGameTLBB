#include "stdafx.h"
// GCStallApply.cpp
// 
/////////////////////////////////////////////////////

#include "GCStallApply.h"

BOOL GCStallApply::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION
	iStream.Read( (CHAR*)(&m_IsCanStall), sizeof(BYTE));
	iStream.Read( (CHAR*)(&m_PosTax), sizeof(UINT));
	iStream.Read( (CHAR*)(&m_TradeTax), sizeof(BYTE));
	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL GCStallApply::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_IsCanStall), sizeof(BYTE));
	oStream.Write( (CHAR*)(&m_PosTax), sizeof(UINT));
	oStream.Write( (CHAR*)(&m_TradeTax), sizeof(BYTE));
	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT GCStallApply::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return GCStallApplyHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}

