#include "stdafx.h"
// GCJoinMenpai.cpp
// 
/////////////////////////////////////////////////////

#include "GCJoinMenpai.h"

BOOL GCJoinMenpai::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_SucceedJoin), sizeof(short) ) ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL GCJoinMenpai::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_SucceedJoin), sizeof(short) ) ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT GCJoinMenpai::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return GCJoinMenpaiHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}

