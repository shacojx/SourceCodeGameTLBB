#include "stdafx.h"
// CGAskPrivateInfo.cpp
// 
/////////////////////////////////////////////////////

#include "CGAskPrivateInfo.h"

BOOL CGAskPrivateInfo::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_PlayerID), sizeof(ObjID_t) ) ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL CGAskPrivateInfo::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_PlayerID), sizeof(ObjID_t) ) ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT CGAskPrivateInfo::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return CGAskPrivateInfoHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}

