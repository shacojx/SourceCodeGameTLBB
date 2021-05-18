#include "stdafx.h"
// GCAddLockObj.cpp
// 
/////////////////////////////////////////////////////

#include "GCAddLockObj.h"

BOOL GCAddLockObj::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_bLockObj),		sizeof(BYTE		) ) ;
	iStream.Read( (CHAR*)(&m_bResult),	sizeof(BYTE		) ) ;
	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL GCAddLockObj::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_bLockObj),	sizeof(BYTE		) ) ;
	oStream.Write( (CHAR*)(&m_bResult),	sizeof(BYTE		) ) ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT GCAddLockObj::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return TRUE;
	//return GCAddLockObjHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}

