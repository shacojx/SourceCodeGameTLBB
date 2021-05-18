#include "stdafx.h"
// GCMenpaiInfo.cpp
// 
/////////////////////////////////////////////////////

#include "GCMenpaiInfo.h"

BOOL GCMenpaiInfo::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION
	iStream.Read( (CHAR*)(&m_idMenpai), sizeof(short));

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL GCMenpaiInfo::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_idMenpai), sizeof(short));

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT GCMenpaiInfo::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return GCMenpaiInfoHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}

