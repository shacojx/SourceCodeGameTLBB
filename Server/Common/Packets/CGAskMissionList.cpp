#include "stdafx.h"
// CGAskMissionList.cpp
// 
/////////////////////////////////////////////////////

#include "CGAskMissionList.h"

using namespace Packets;

BOOL CGAskMissionList::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_ObjID), sizeof(ObjID_t) ) ;
	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL CGAskMissionList::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_ObjID), sizeof(ObjID_t) ) ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT CGAskMissionList::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return CGAskMissionListHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}

