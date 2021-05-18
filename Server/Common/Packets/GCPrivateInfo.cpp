#include "stdafx.h"
// GCPrivateInfo.cpp
// 
/////////////////////////////////////////////////////

#include "GCPrivateInfo.h"

BOOL GCPrivateInfo::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_CharGUID), sizeof(UINT));
	
	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL GCPrivateInfo::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_CharGUID), sizeof(UINT));

	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT GCPrivateInfo::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return GCPrivateInfoHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}

