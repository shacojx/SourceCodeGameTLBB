#include "stdafx.h"

// GCAskActiveTimeUpdate.cpp
// 
/////////////////////////////////////////////////////

#include "CGAskActiveTimeUpdate.h"

BOOL CGAskActiveTimeUpdate::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION
	iStream.Read( (CHAR*)(&m_nActiveTime), sizeof(m_nActiveTime));
	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL CGAskActiveTimeUpdate::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_nActiveTime), sizeof(SHORT));

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT CGAskActiveTimeUpdate::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return CGAskActiveTimeUpdateHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}

